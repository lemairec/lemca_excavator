#pragma once

#include <string>
#include <map>
#include <atomic>

class QPixmap;

// Fond carto Esri World Imagery (satellite) avec cache disque pour usage offline.
// Tuiles 256px en Web Mercator (z/x/y, schema Google/OSM).
// Esri tile url : .../World_Imagery/MapServer/tile/{z}/{y}/{x}
class MapTiles {
public:
    bool m_enable = true;

    void init();                                // dossier cache + creation
    QPixmap * getTile(int x, int y);            // depuis cache memoire/disque (NULL si absent)
    void ensurePrefetch(double lat, double lon);// lance le download offline une seule fois

    int zoom() const { return m_zoom; }

    // maths tuiles (Web Mercator)
    static void lonlatToTileFrac(double lon, double lat, int z, double & xt, double & yt);
    static void tileToLonLat(int x, int y, int z, double & lon, double & lat); // coin Nord-Ouest

    // etat telechargement (pour affichage progression)
    std::atomic<int> m_dl_done{0};
    std::atomic<int> m_dl_total{0};
    std::atomic<bool> m_downloading{false};

private:
    void downloadThread(double lat, double lon);

    int m_zoom = 18;          // ~0.6 m/px
    double m_half_km = 5.0;   // demi-cote du buffer -> 10x10 km
    std::string m_dir;
    bool m_started = false;
    std::map<long long, QPixmap*> m_cache;
};
