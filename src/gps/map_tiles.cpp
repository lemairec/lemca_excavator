#include "map_tiles.hpp"

#include "../util/log.hpp"
#include "../util/directory_manager.hpp"

#include <QPixmap>
#include <QImage>
#include <QDir>
#include <QFile>
#include <QString>
#include <QByteArray>
#include <QEventLoop>
#include <QTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <cmath>
#include <thread>
#include <vector>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static const int NB_WORKERS = 8; // telechargements paralleles

static long long tileKey(int x, int y){
    return ((long long)x << 32) | (unsigned int)y;
}

void MapTiles::lonlatToTileFrac(double lon, double lat, int z, double & xt, double & yt){
    double n = std::pow(2.0, z);
    xt = (lon + 180.0) / 360.0 * n;
    double lr = lat * M_PI / 180.0;
    yt = (1.0 - std::asinh(std::tan(lr)) / M_PI) / 2.0 * n;
}

void MapTiles::tileToLonLat(int x, int y, int z, double & lon, double & lat){
    double n = std::pow(2.0, z);
    lon = (double)x / n * 360.0 - 180.0;
    double k = M_PI * (1.0 - 2.0 * (double)y / n);
    lat = std::atan(std::sinh(k)) * 180.0 / M_PI;
}

double MapTiles::distanceKm(double lat1, double lon1, double lat2, double lon2){
    double x = (lon2 - lon1) * std::cos((lat1 + lat2) * 0.5 * M_PI / 180.0);
    double y = (lat2 - lat1);
    return std::sqrt(x * x + y * y) * 111.32;
}

void MapTiles::init(){
    m_dir = DirectoryManager::instance().getDataDirectory() + "/map_cache";
    QDir().mkpath(QString::fromStdString(m_dir));
    INFO("map_cache " << m_dir);
}

QPixmap * MapTiles::getTile(int x, int y){
    long long key = tileKey(x, y);
    auto it = m_cache.find(key);
    if(it != m_cache.end()){
        return it->second;
    }
    QString path = QString::fromStdString(m_dir) + "/" + QString::number(m_zoom)
                 + "/" + QString::number(x) + "_" + QString::number(y) + ".jpg";
    if(!QFile::exists(path)){
        return nullptr; // pas (encore) telecharge
    }
    QPixmap * pix = new QPixmap();
    if(!pix->load(path) || pix->isNull()){
        delete pix;
        return nullptr; // fichier partiel/corrompu -> reessai au prochain frame
    }
    m_cache[key] = pix;
    return pix;
}

void MapTiles::ensureArea(double lat, double lon){
    if(!m_enable){
        return;
    }
    if(m_downloading){
        return; // un download en cours
    }
    // pas encore telecharge, ou la position a assez bouge -> (re)prefetch
    if(m_has_dl && distanceKm(lat, lon, m_last_dl_lat, m_last_dl_lon) < m_half_km * 0.5){
        return;
    }
    m_has_dl = true;
    m_last_dl_lat = lat;
    m_last_dl_lon = lon;
    std::thread([this, lat, lon](){ this->coordinator(lat, lon); }).detach();
}

void MapTiles::coordinator(double lat, double lon){
    m_downloading = true;

    double res = 156543.03392 * std::cos(lat * M_PI / 180.0) / std::pow(2.0, m_zoom); // m/px
    double tile_m = res * 256.0;
    int rad = (int)std::ceil(m_half_km * 1000.0 / tile_m);

    double xtf, ytf;
    lonlatToTileFrac(lon, lat, m_zoom, xtf, ytf);
    int xc = (int)std::floor(xtf);
    int yc = (int)std::floor(ytf);

    m_base = m_dir + "/" + std::to_string(m_zoom);
    QDir().mkpath(QString::fromStdString(m_base));

    m_todo.clear();
    for(int dx = -rad; dx <= rad; ++dx){
        for(int dy = -rad; dy <= rad; ++dy){
            int x = xc + dx, y = yc + dy;
            QString path = QString::fromStdString(m_base) + "/" + QString::number(x) + "_" + QString::number(y) + ".jpg";
            if(!QFile::exists(path)){
                m_todo.push_back({x, y});
            }
        }
    }
    // telecharger du centre vers l'exterieur (tuiles visibles d'abord)
    std::sort(m_todo.begin(), m_todo.end(), [xc, yc](const std::pair<int,int> & a, const std::pair<int,int> & b){
        long da = (long)(a.first-xc)*(a.first-xc) + (long)(a.second-yc)*(a.second-yc);
        long db = (long)(b.first-xc)*(b.first-xc) + (long)(b.second-yc)*(b.second-yc);
        return da < db;
    });

    m_dl_total = (int)m_todo.size();
    m_dl_done = 0;
    m_next = 0;
    INFO("map prefetch z" << m_zoom << " rad " << rad << " a telecharger " << (int)m_todo.size() << " (x" << NB_WORKERS << ")");

    std::vector<std::thread> pool;
    for(int i = 0; i < NB_WORKERS; ++i){
        pool.emplace_back([this](){ this->worker(); });
    }
    for(auto & t : pool){
        t.join();
    }

    m_downloading = false;
    INFO("map prefetch fini " << (int)m_dl_done << "/" << (int)m_dl_total);
}

void MapTiles::worker(){
    QNetworkAccessManager mgr; // un manager par thread

    while(true){
        size_t idx = m_next.fetch_add(1);
        if(idx >= m_todo.size()){
            break;
        }
        int x = m_todo[idx].first;
        int y = m_todo[idx].second;

        QString url = QString("https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/%1/%2/%3")
                          .arg(m_zoom).arg(y).arg(x); // Esri = z/y/x
        QNetworkRequest req((QUrl(url)));
        req.setHeader(QNetworkRequest::UserAgentHeader, "lemca_excavator/1.0");

        QNetworkReply * reply = mgr.get(req);
        QEventLoop loop;
        QTimer timer; timer.setSingleShot(true);
        QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        timer.start(15000);
        loop.exec();

        if(reply->error() == QNetworkReply::NoError){
            QByteArray data = reply->readAll();
            if(data.size() > 0){
                QString path = QString::fromStdString(m_base) + "/" + QString::number(x) + "_" + QString::number(y) + ".jpg";
                QFile f(path);
                if(f.open(QIODevice::WriteOnly)){
                    f.write(data);
                    f.close();
                }
            }
        }
        reply->deleteLater();

        m_dl_done.fetch_add(1);
    }
}
