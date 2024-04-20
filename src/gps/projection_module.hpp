#ifndef PROJECTION_MODULE_H
#define PROJECTION_MODULE_H

//#include "../util/log.hpp"

const double to_rad = 0.01745329251;
const double to_deg = 57.2957795131;

#define DEG2RAD(a)   ((a)*to_rad)
#define RAD2DEG(a)   ((a)*to_deg)

class ProjectionModule {
   
    /**
     * TRANSFORME LAMBERT
     */

    /*
    //Algo 01
    double __YGLatitudeISOFromLatitude(double lat, double e)
    {
        return log(tan(M_PI_4+lat/2)*pow((1-e*sin(lat))/(1+e*sin(lat)),e/2));
    }

    double __YGLatitudeISO(double lat, double e)
    {
        return log(tan(M_PI_4 + lat/2)*pow((1-e*sin(lat))/(1+e*sin(lat)),e/2));
    }

    void __YGCoordinatesTransform(double lat_d, double lon_d, double & x, double & y, double e, double n, double c, double lambda_c, double x_s, double y_s)
    {
        double lon_r = DEG2RAD(lon_d);
        double lat_r = DEG2RAD(lat_d);
        double latiso = __YGLatitudeISO(lat_r,e);
        x = x_s + c*exp(-n*latiso)*sin(n*(lon_r-lambda_c));
        y = y_s - c*exp(-n*latiso)*cos(n*(lon_r-lambda_c));
    }

    static double lambert_n[6] = {0.7604059656, 0.7289686274, 0.6959127966, 0.6712679322, 0.7289686274, 0.7256077650};
    static double lambert_c[6] = {11603796.98, 11745793.39, 11947992.52, 12136281.99, 11745793.39, 11754255.426};
    static double lambert_xs[6]= {600000.0, 600000.0, 600000.0, 234.358, 600000.0, 700000.0};
    static double lambert_ys[6]= {5657616.674, 6199695.768, 6791905.085, 7239161.542, 8199695.768, 12655612.050};
    #define E_CLARK_IGN 0.08248325676
    #define LON_MERID_GREENWICH 0.04079234433

    #define LAMBERT_I 0
    #define LAMBERT_II 1
    #define LAMBERT_III 2
    #define LAMBERT_IV 3
    #define LAMBERT_II_E 4
    #define LAMBERT_93 5
    #define LAMBERT LAMBERT_I*/


    /**
     * Cordonnees spheriques
     *
     * http://geodesie.ign.fr/contenu/fichiers/documentation/pedagogiques/TransformationsCoordonneesGeodesiques.pdf
     **/

    double a = 6378249.2;
    //double b = 6356515;
    //double e_2 = (a*a-b*b)/(a*a);

    double m_lat_ref_d;
    double m_lon_ref_d;
    double m_a_cos_lat_ref;
    
    void setXYSpherique(double lat_d, double lon_d, double & x, double & y){
        double lat = DEG2RAD(lat_d - m_lat_ref_d);
        double lon = DEG2RAD(lon_d - m_lon_ref_d);
        
        x = m_a_cos_lat_ref * lon;
        y = a * lat;
    }

    void setLatLonSpherique(double x, double y, double & lat_d, double & lon_d){
        double lat_rel = y/a;
        double lon_rel = x/m_a_cos_lat_ref;
        
        lat_d = m_lat_ref_d + RAD2DEG(lat_rel);
        lon_d = m_lon_ref_d + RAD2DEG(lon_rel);
    }
    
    void setRef(double lat_d, double lon_d){
        m_lat_ref_d = lat_d;
        m_lon_ref_d = lon_d;
        m_a_cos_lat_ref = a * cos(DEG2RAD(lat_d));
    }
    
    /**
     * Mercator
     *
     * http://wiki.openstreetmap.org/wiki/Mercator
     */
    /*#define EARTH_RADIUS 6378137
    double lat2y_m(double lat) { return log(tan( DEG2RAD(lat) / 2 + M_PI/4 )) * EARTH_RADIUS; }
    double lon2x_m(double lon) { return          DEG2RAD(lon)                 * EARTH_RADIUS; }

    void __MercatorCoordinatesTransform(GpsPoint & gpsPoint)
    {
        gpsPoint.m_x = lon2x_m(gpsPoint.m_longitude);
        gpsPoint.m_y = lat2y_m(gpsPoint.m_latitude);
        
    }*/



public:
    ProjectionModule(){
        setRef(49, 4);
    }

    void setXY(double lat_d, double lon_d, double & x, double & y){
        //__YGCoordinatesTransform(gpsPoint, E_CLARK_IGN, lambert_n[LAMBERT], lambert_c[LAMBERT], LON_MERID_GREENWICH, 0.0, 0.0);
        setXYSpherique(lat_d, lon_d, x, y);
        //SetLatLongSpherique(gpsPoint);
    }

    void setLatLon(double x, double y, double & lat_d, double & lon_d){
        setLatLonSpherique(x,y,lat_d, lon_d);
    }

};

#endif // GPS_FRAMEWORK_H
