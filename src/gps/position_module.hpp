#ifndef POSITION_MODULE_H
#define POSITION_MODULE_H

#include "nmea_parser.hpp"
#include "../config/config.hpp"
#include "tracteur.hpp"
#include <string>
#include <memory>
#include <list>


#include "ekf_module.hpp"
#include "custom_cap_nbr.hpp"
#include "projection_module.hpp"



class PointPosition {
public :
    double m_altitude;
    double m_latitude;
    double m_longitude;
    
    double m_x = 0.0;
    double m_y = 0.0;
    
    double m_time = 0.0;
    double m_timeHour = 0.0;
    
    double m_cap_rad = 0.0;
    double m_vitesse_km_h = 0.0;
    
    double distanceCarre(GpsPoint & p2){
        return (p2.m_x - m_x)*(p2.m_x - m_x) + (p2.m_y - m_y)*(p2.m_y - m_y);
    }
    
    double distanceCarre(double x, double y){
        return (x - m_x)*(x - m_x) + (y - m_y)*(y - m_y);
    }
};

class PositionModule {
public:
    EkfModule m_ekf_module;
    ProjectionModule m_projection_module;
    CustomCapNbr m_custom_cap_nbr;
    
    void initOrLoad(Config & config);

    bool m_reset = true;
    
    
    
    double m_v_x;
    double m_v_y;
    
    double m_final_x;
    double m_final_y;

    double m_cap_deg;
    double m_cap_rad;
    double m_v_m_s;
    
    int m_size_max = 100;
    
    std::list<GpsPoint_ptr> m_list_before_ekf;
    std::list<GpsPoint_ptr> m_list_after_ekf;
    
    void onNewImuAcc(double a_x, double a_y, double a_z);
    void onNewImuAccAng(double a_roll_x_deg, double a_pitch_y_deg, double a_yaw_z_deg);
    void onNewImuAng(double roll_x_deg, double pitch_y_deg, double yaw_z_deg);
    
    void setXY(GpsPoint & point);
    void SetLatLong(GpsPoint & point);
    
    void reset();
    
    GGAFrame_ptr m_last_gga;
    RMCFrame_ptr m_last_rmc;
    VTGFrame_ptr m_last_vtg;
    ATTFrame_ptr m_last_att;
    void onGGAFrame(GGAFrame_ptr frame);
    void onRMCFrame(RMCFrame_ptr frame);
    void onVTGFrame(VTGFrame_ptr frame);
    void onATTFrame(ATTFrame_ptr frame);
    
    void onAutoPath(double latitude, double longitude, double altitude);
    
};

#endif //POSITION_MODULE_H
