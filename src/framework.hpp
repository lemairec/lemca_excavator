#ifndef GPS_FRAMEWORK_H
#define GPS_FRAMEWORK_H

#include "pilot_module/pilot_translator_module.hpp"
#include "gps/position_module.hpp"
#include "gps/nmea_parser.hpp"
#include "gui/qt/my_qt_serial_port.hpp"
#include "gui/qt/my_qt_file.hpp"
#include "gui/qt/my_qt_timer.hpp"
#include "gui/qt/my_qt_media_player.hpp"
#include "config/config.hpp"
#include "parcours/parcours.hpp"
#include "parcelle/synchro.hpp"
#include "job_module.hpp"
#include "auto_path.hpp"

#include <chrono>
#include <QThread>
#include <time.h>
#include <math.h>
#include "util/log.hpp"
#include "gps/tracteur.hpp"
#include "balise/balises.hpp"
#include "util/stat.hpp"
#include "parser/hexa_parser.hpp"

#include "gui/qt/my_qt_network.hpp"

class IGpsObserver {
public:
    virtual void onNewPoint() = 0;
};

struct OutilPosition {
    GpsPoint_ptr m_point_right;
    GpsPoint_ptr m_point_center;
    GpsPoint_ptr m_point_left;
};

typedef std::shared_ptr<OutilPosition> OutilPosition_ptr;

enum ParcellePoint {
    ParcellePointMiddle = 0,
    ParcellePointLeft = 1,
    ParcellePointRight = 2
};

enum EtatAB {
    EtatAB_Line = 0,
    EtatAB_Curve = 1
};

class SurfaceToDraw{
public:
    bool m_isClose = false;
    std::list<OutilPosition_ptr> m_points;
    OutilPosition_ptr m_lastPoint = nullptr;
};

typedef std::shared_ptr<SurfaceToDraw> SurfaceToDraw_ptr;

class Framework {
    Framework();
    
    double m_resolution_draw;
    
    
    JobModule m_job_module;
public:
    static Framework & instance();
    ~Framework();
    void initOrLoadConfig();
    
    GpsPoint_ptr m_lastPoint;
    GpsPoint_ptr m_lastImportantPoint;
    
    Synchro m_synchro;
    Balises m_balises;
    
    std::string m_debug_str_1 = "";
    std::string m_debug_str_2 = "";
    std::string m_debug_str_3 = "";
    std::string m_debug_str_4 = "";
    
    GpsPointCap_ptr m_point_current;
    GpsPointCap_ptr m_point_save;
    double m_point_dist;
    
    double m_hauteur_current;
    double m_hauteur_save;
    double m_hauteur_diff;
    
    double m_profondeur_mm;
    
    double m_last_soil_temp;
    double m_last_soil_hum;
    double m_last_soil_cond;
    double m_last_soil_ph;
    double m_last_soil_n;
    double m_last_soil_p;
    double m_last_soil_k;
    
    
    std::string m_messages_errors;
    void addError(std::string s);
    
    int m_on_new_time_ms = 0;
    void onNewPoint(GpsPointCap_ptr point);
    int m_on_new_important_time_ms = 0;
    void onNewImportantPoint(GpsPoint_ptr point);
    
    void processPilot(double deplacementX, double deplacementY
        , double essieu_avant_x, double essieu_avant_y
        , double essieu_arriere_x, double essieu_arriere_y);
    
    void onFrame(const std::string & frame);
    void setRef(double latitude, double longitude);
    
    void setDistance(double distance, bool led);
    double getOffsetAB();
    
    void savePointA();
    void savePointB();
    
    void test();
    
    Config m_config;
    
    int m_date;
    std::string m_datetime;
    
    bool m_gga = false;
    
    EtatAB m_etat_ab = EtatAB_Line;
private:
    Etat m_etat = Etat_Reset;
public:
    void setEtat(Etat etat);
    Etat getEtat();
    
    LineOrCurve * getCurrentLineOrCurve();
    LineAB m_lineAB;
    CurveAB m_curveAB;
    LineCurves m_lines;
    
    int m_demitour_current_i = 0;
    int m_demitour_next_i = 8888;
    
    ParcellePoint m_parcellePoint = ParcellePointMiddle;
    void setAB();
    
    //deplacement
    double m_deplacementX;
    double m_deplacementY;
    double m_cap_rad;
    double m_cap_deg;
    
    double m_distance_last_point;
    double m_time_last_point;
    double m_vitesse;
    void calculDeplacement(GpsPoint_ptr p);
    
    double m_distanceAB = 0.0;
    double m_distanceABRound = 0.0;
    int m_ledAB = 0;
    
    bool m_sens_AB = true;
    
    double m_angle_ab_rad = 0.0;
    double m_angle_ab_deg = 0.0;
    
    bool m_rtk = false; //todo remove
    
    std::string m_last_csq;

    //surface
    double m_surface = 0;
    double m_surface_h = 0;
    
    //contour_exterieur;
    double m_surface_exterieur = 0;
    double m_surface_exterieur_h = 0;
    void calculContourExterieur();
    
    void clearSurface();
    void calculSurface();
    
    void calculDraw(GpsPoint_ptr);
    
    
    std::list<SurfaceToDraw_ptr> m_listSurfaceToDraw;
    std::string m_pilot_last_error;
    
    TimeObserver m_gpgn_time;
    TimeObserver m_gps_time;
    TimeObserver m_draw_time;
    
    ValueStat m_stat_distance_cm;
    ValueStat m_stat_angle_AB_deg;
    ValueStat m_stat_angle_correction_roue_deg;
    
    
    ValueStat m_stat_roue_deg;
    ValueStat m_stat_cap_deg;
    ValueStat m_stat_cap_jd_deg;
    ValueStat m_stat_cap_rmc_deg;
    ValueStat m_stat_cap_rmc2_deg;
    ValueStat m_stat_cap_calc_deg;
    ValueStat m_stat_cap_calc2_deg;
    ValueStat m_stat_cap_vtg_mag_deg;
    ValueStat m_stat_cap_vtg_real_deg;
    ValueStat m_stat_cap_att_deg;
    ValueStat m_stat_cap_custom_d_deg;
    ValueStat m_stat_cap_custom_p_deg;
    
    ValueStat m_stat_pilot_desired;
    ValueStat m_stat_pilot_mesured;
    
    bool isGpsConnected();
    bool isPilotConnected();
    bool isTranslateurConnected();
    void exportHtml();
    
    void loadParcelle(const std::string & name, int flags_i, bool line, bool demi_outil);
     
    void changeDraw();
    void changeDrawBool(bool b);
    bool m_pauseDraw = false;
         
    void setVolantEngaged(bool value);
    bool getVolantEngaged();
    
    
    std::list<ErrorOrWarning> m_errors;
    void setErrorWarning();
    
    void setDistanceTranslator();
    
    void addOffset(double value_m, bool relatif);
    
    void setAntennaH();
    void getAntennaH();
    
    double m_angle_roue_by_yawl_deg = 0;
     
    Tracteur m_tracteur;
    NmeaParser m_nmea_parser_gps;
    NmeaParser m_nmea_parser_mcu;
    HexaParser m_hexa_parser;
    MyQTSerialPorts m_serialModule;
    MyQTFile m_fileModule;
    PilotTranslatorModule m_pilot_translator_module;
    PositionModule m_position_module;
    MyQTNetwork m_qt_network;
    AutoPathModule m_auto_path_module;
    MyQtMediaPlayer m_qt_media_player;
    MyQTTimer m_qt_timer;
    
    std::list<std::string> m_listLog;
private:
    void readFile();
};

#endif // GPS_FRAMEWORK_H
