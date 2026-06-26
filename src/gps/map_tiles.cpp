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

void MapTiles::ensurePrefetch(double lat, double lon){
    if(m_started || !m_enable){
        return;
    }
    m_started = true;
    std::thread([this, lat, lon](){ this->downloadThread(lat, lon); }).detach();
}

void MapTiles::downloadThread(double lat, double lon){
    m_downloading = true;

    // etendue tuiles pour un carre 2*m_half_km de cote
    double res = 156543.03392 * std::cos(lat * M_PI / 180.0) / std::pow(2.0, m_zoom); // m/px
    double tile_m = res * 256.0;
    int rad = (int)std::ceil(m_half_km * 1000.0 / tile_m);

    double xtf, ytf;
    lonlatToTileFrac(lon, lat, m_zoom, xtf, ytf);
    int xc = (int)std::floor(xtf);
    int yc = (int)std::floor(ytf);

    // liste des tuiles manquantes
    std::vector<std::pair<int,int>> todo;
    QString base = QString::fromStdString(m_dir) + "/" + QString::number(m_zoom);
    QDir().mkpath(base);
    for(int dx = -rad; dx <= rad; ++dx){
        for(int dy = -rad; dy <= rad; ++dy){
            int x = xc + dx, y = yc + dy;
            QString path = base + "/" + QString::number(x) + "_" + QString::number(y) + ".jpg";
            if(!QFile::exists(path)){
                todo.push_back({x, y});
            }
        }
    }
    // telecharger du centre vers l'exterieur (les tuiles visibles d'abord)
    std::sort(todo.begin(), todo.end(), [xc, yc](const std::pair<int,int> & a, const std::pair<int,int> & b){
        long da = (long)(a.first-xc)*(a.first-xc) + (long)(a.second-yc)*(a.second-yc);
        long db = (long)(b.first-xc)*(b.first-xc) + (long)(b.second-yc)*(b.second-yc);
        return da < db;
    });

    m_dl_total = (int)todo.size();
    m_dl_done = 0;
    INFO("map prefetch z" << m_zoom << " rad " << rad << " a telecharger " << (int)todo.size());

    QNetworkAccessManager mgr;
    for(auto & t : todo){
        int x = t.first, y = t.second;
        QString url = QString("https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/%1/%2/%3")
                          .arg(m_zoom).arg(y).arg(x); // Esri = z/y/x
        QNetworkRequest req((QUrl(url)));
        req.setHeader(QNetworkRequest::UserAgentHeader, "lemca_excavator/1.0");

        QNetworkReply * reply = mgr.get(req);
        QEventLoop loop;
        QTimer timer; timer.setSingleShot(true);
        QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        timer.start(15000); // timeout 15s
        loop.exec();

        if(reply->error() == QNetworkReply::NoError){
            QByteArray data = reply->readAll();
            if(data.size() > 0){
                QString path = base + "/" + QString::number(x) + "_" + QString::number(y) + ".jpg";
                QFile f(path);
                if(f.open(QIODevice::WriteOnly)){
                    f.write(data);
                    f.close();
                }
            }
        } else {
            WARN("map tile fail " << x << "," << y << " : " << reply->errorString().toStdString());
        }
        reply->deleteLater();

        m_dl_done = m_dl_done + 1;
        if(m_dl_done % 100 == 0){
            INFO("map prefetch " << (int)m_dl_done << "/" << (int)m_dl_total);
        }
    }

    m_downloading = false;
    INFO("map prefetch fini " << (int)m_dl_done << "/" << (int)m_dl_total);
}
