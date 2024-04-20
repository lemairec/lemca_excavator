#include "position_module.hpp"
#include "../framework.hpp"

#include "../util/log.hpp"
#include <math.h>




void PositionModule::initOrLoad(Config & config){
    if(config.m_correction_devers){
        m_ekf_module.m_h = config.m_tracteur_hauteur;
    } else {
        m_ekf_module.m_h = 0;
    }
}

double lissageAngle(double old_value, double new_value, double coeff){
    double new_value2 = new_value;
    double old_value2 = old_value;
    
    if(old_value2 - new_value2 > 3.14){
        old_value2 = old_value2-2*3.14;
    }
    if(old_value2 - new_value2 < -3.14){
        old_value2 = old_value2+2*3.14;
    }
    if(old_value2 - new_value2 > 3.14){
        INFO("problem");
    }
    if(old_value2 - new_value2 < -3.14){
        INFO("problem");
    }
    return (1.0-coeff)*new_value+coeff*old_value;
}

void PositionModule::onNewImuAcc(double ax, double ay, double az){
    m_ekf_module.onNewImuAcc(ax, ay, az);
}

void PositionModule::onNewImuAccAng(double ax, double ay, double az){
    m_ekf_module.onNewImuAccAng(ax, ay, az);
}

void PositionModule::onNewImuAng(double roll_x_deg, double pitch_y_deg, double yaw_z_deg){
    m_ekf_module.onNewImuAng(roll_x_deg, pitch_y_deg, yaw_z_deg);
}


void PositionModule::reset(){
    m_list_after_ekf.clear();
    m_list_before_ekf.clear();
    
    m_reset = true;
}

void PositionModule::setXY(GpsPoint & point){
    m_projection_module.setXY(point.m_latitude, point.m_longitude, point.m_x, point.m_y);
}
void PositionModule::SetLatLong(GpsPoint & point){
    m_projection_module.setLatLon(point.m_x, point.m_y, point.m_latitude, point.m_longitude);
}


void PositionModule::onGGAFrame(GGAFrame_ptr gga){
    //Framework & f = Framework::instance();
    m_last_gga = gga;
    //INFO("gga");
}

void PositionModule::onAutoPath(double latitude, double longitude, double altitude){
    double x, y;
    m_projection_module.setXY(latitude, longitude, x, y);
    
    GpsPoint_ptr p(new GpsPoint());
    p->m_latitude = latitude;
    p->m_longitude =  longitude;
    p->m_altitude = altitude;
    p->m_x = x;
    p->m_y = y;
    
    Framework & f = Framework::instance();
    f.m_auto_path_module.m_paths.push_back(p);
}


std::string print2(int i){
    return std::to_string(i/10)+std::to_string(i%10);
}

void PositionModule::onRMCFrame(RMCFrame_ptr rmc){
    m_last_rmc = rmc;
    Framework & f = Framework::instance();

    f.m_date = rmc->m_date;
    int h = rmc->m_time/10000;
    int min = (int)(rmc->m_time/100) - h*100;
    int sec = rmc->m_time - h*10000 - min*100;
    int ms = round((rmc->m_time - h*10000 - min*100 - sec)*100);
    int yy = f.m_date/10000;
    int m = (int)(f.m_date/100) - yy*100;
    int d = f.m_date - yy*10000 - m*100;
    std::string s = "20"+print2(yy) + "-" + print2(m) + "-" + print2(d) + "T" + print2(h) + ":" + print2(min) + ":" + print2(sec) + "." + print2(ms);
    f.m_datetime = s;
    
    
    double x, y;
    m_projection_module.setXY(rmc->m_latitude, rmc->m_longitude, x, y);
    m_custom_cap_nbr.workXY(x, y);
    m_ekf_module.work(x, y, 0, rmc->m_vitesse_kmh, m_custom_cap_nbr.m_custom_cap_deg);
    //INFO(m_custom_cap_nbr.m_v_km_h);
    
    m_final_x = m_ekf_module.m_old_x;
    m_final_y = m_ekf_module.m_old_y;
    
    m_cap_deg = m_custom_cap_nbr.m_custom_cap_deg;
    m_cap_rad = m_cap_deg/180*M_PI;
    m_v_m_s = rmc->m_vitesse_kmh*1000/3600;
    m_v_x = sin(m_cap_rad)*m_v_m_s;
    m_v_y = cos(m_cap_rad)*m_v_m_s;
    
    
    GpsPointCap_ptr p(new GpsPointCap());
    p->m_x = m_final_x;
    p->m_y = m_final_y;
    p->m_vitesse_m_s = m_v_m_s;
    p->m_cap_deg = m_cap_deg;
    p->m_timeHour = rmc->m_timeHour;
    m_list_after_ekf.push_front(p);
    if(m_list_after_ekf.size()>100){
        m_list_after_ekf.pop_back();
    };
    
    GpsPoint_ptr before(new GpsPoint());
    before->m_latitude = rmc->m_latitude;
    before->m_longitude = rmc->m_longitude;
    setXY(*before);
    m_list_before_ekf.push_front(before);
    if(m_size_max != 0 && m_list_before_ekf.size()>m_size_max){
        m_list_before_ekf.pop_back();
    };
    
    if(p->m_timeHour == 0){
        return;
    }
    
   
    
    
    p->m_latitude = rmc->m_latitude;
    p->m_longitude = rmc->m_longitude;
    if(m_last_gga){
        p->m_altitude = m_last_gga->m_altitude;
    }
    f.onNewPoint(p);
}

void PositionModule::onVTGFrame(VTGFrame_ptr vtg){
    m_last_vtg = vtg;
    Framework & f = Framework::instance();
    f.m_stat_cap_vtg_mag_deg.addNewValueDeg(vtg->m_cap_mag_deg);
    f.m_stat_cap_vtg_real_deg.addNewValueDeg(vtg->m_cap_real_deg);
}

void PositionModule::onATTFrame(ATTFrame_ptr att){
    m_last_att = att;
    Framework & f = Framework::instance();
    f.m_stat_cap_att_deg.addNewValueDeg(att->m_head);
}

/*
 
 */
