#pragma once

#include <string>
#include <map>
#include <vector>
#include <utility>
#include <atomic>

class QPixmap;

// Fond carto Esri World Imagery (satellite) avec cache disque pour usage offline.
// Tuiles 256px en Web Mercator (z/x/y). Telechargement dynamique autour de la
// position GPS, en parallele (pool de threads).
class MapTiles {
public:
    bool m_enable = true;

    void init();                                // dossier cache + creation
    QPixmap * getTile(int x, int y);            // depuis cache memoire/disque (NULL si absent)
    void ensureArea(double lat, double lon);    // (re)telecharge autour de la position si besoin

    int zoom() const { return m_zoom; }

    // maths tuiles (Web Mercator)
    static void lonlatToTileFrac(double lon, double lat, int z, double & xt, double & yt);
    static void tileToLonLat(int x, int y, int z, double & lon, double & lat); // coin Nord-Ouest

    // etat telechargement (affichage)
    std::atomic<int> m_dl_done{0};
    std::atomic<int> m_dl_total{0};
    std::atomic<bool> m_downloading{false};

private:
    void coordinator(double lat, double lon);
    void worker();
    static double distanceKm(double lat1, double lon1, double lat2, double lon2);

    int m_zoom = 18;          // ~0.6 m/px
    double m_half_km = 5.0;   // demi-cote du buffer -> 10x10 km
    std::string m_dir;

    double m_last_dl_lat = 0, m_last_dl_lon = 0;
    bool m_has_dl = false;

    // partage coordinator -> workers (un seul download a la fois via m_downloading)
    std::vector<std::pair<int,int>> m_todo;
    std::string m_base;
    std::atomic<size_t> m_next{0};

    std::map<long long, QPixmap*> m_cache;
};
