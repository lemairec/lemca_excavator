#ifndef EKF_MODULE_H
#define EKF_MODULE_H

class EkfModule {
public:
    double m_h = 1.85;
    double m_vitesse_min_kmh = 0.8;

    double m_lissage_ekf_xy = 0.8;
    
    double m_lissage_ekf_cap =  0.8;
    double m_lissage_ekf_a_cap = 0.8;
    
    double m_lissage_ekf_v = 0.8;
    double m_lissage_ekf_a_v = 0.8;
    
    double m_lissage_ekf_roll = 0.8;

    
    double m_old_x = 0;
    double m_old_y = 0;
    double m_old_v = 0;
    double m_old_cap_deg = 0;
    double m_old_a_v = 0;
    double m_old_a_cap = 0;
    double m_old_roll_deg = 0;

    double m_new_x = 0;
    double m_new_y = 0;
    double m_new_v = 0;
    double m_new_cap_deg = 0;
    double m_new_a_v = 0;
    double m_new_a_cap = 0;
    double m_new_roll_deg = 0;

    double m_calc_v = 0;
    double m_calc_a_v = 0;
    double m_calc_cap_deg = 0;
    double m_calc2_cap_deg = 0;
    double m_calc_a_cap = 0;

    double m_lat_ref = 0;
    double m_lon_ref = 0;

    void reset(){
        m_old_x = 0;
        m_old_y = 0;
        m_new_x = 0;
        m_new_y = 0;
    }

    void normalizeDeg(double & d){
        if(d < -180){
            d = d + 2*180;
        }
        if(d > 180){
            d = d - 2*180;
        }
        if(d < -180){
            d = d + 2*180;
        }
        if(d > 180){
            d = d - 2*180;
        }
    }

    double m_imu_a_x = 0;
    double m_imu_a_y = 0;
    double m_imu_a_z = 0;
    void onNewImuAcc(double a_x, double a_y, double a_z){
        m_imu_a_x = a_x;
        m_imu_a_y = a_y;
        m_imu_a_z = a_z;
    };

    double m_imu_ang_acc_x_deg_s = 0;
    double m_imu_ang_acc_y_deg_s = 0;
    double m_imu_ang_acc_z_deg_s = 0;
    void onNewImuAccAng(double ang_acc_x_deg_s, double ang_acc_y_deg_s, double ang_acc_z_deg_s){
        m_imu_ang_acc_x_deg_s = ang_acc_x_deg_s;
        m_imu_ang_acc_y_deg_s = ang_acc_y_deg_s;
        m_imu_ang_acc_z_deg_s = ang_acc_z_deg_s;
    };

    double m_imu_ang_x_deg = 0;
    double m_imu_ang_y_deg = 0;
    double m_imu_ang_z_deg = 0;
    void onNewImuAng(double ang_x_deg, double ang_y_deg, double ang_z_deg){
        m_imu_ang_x_deg = ang_x_deg;
        m_imu_ang_y_deg = ang_y_deg;
        m_imu_ang_z_deg = ang_z_deg;
    };


    void workRMC(double lat, double lon, double z, double vitesse_kmh, double angle_deg){
        
        if(abs(lat-m_lat_ref) > 0.1){
            reset();
            m_lat_ref = lat;
            m_lon_ref = lon;
        }

        double a = 6378249.2;
        double latitude_ref = m_lat_ref;
        double longitude_ref = m_lon_ref;
        double acosLat_ref = a * cos(0.01745329251*latitude_ref);

        double lat1 = 0.01745329251*(lat - latitude_ref);
        double lon1 = 0.01745329251*(lon - longitude_ref);
        double x1 = acosLat_ref * lon1;
        double y1 = a * lat1;

        work(x1, y1, z, vitesse_kmh, angle_deg);
    }

    void work(double x, double y, double z, double vitesse_kmh, double angle_deg){
        double d_t = 0.1;
        m_new_x = x;
        m_new_y = y;

        if(vitesse_kmh < m_vitesse_min_kmh){
            m_new_cap_deg = m_old_cap_deg;
            m_new_v = 0;
        } else {
            m_new_cap_deg = angle_deg;
            m_new_v = vitesse_kmh*1000/3600;
        }
        
        m_new_a_cap = -m_imu_ang_acc_z_deg_s;
        m_new_a_v = m_imu_a_y;
        m_new_roll_deg = m_imu_ang_x_deg;
        //m_new_z = z;

        if(m_old_x == 0){
            m_old_x = m_new_x;
            m_old_y = m_new_y;
            m_old_v = m_new_v;
            m_old_cap_deg = m_new_cap_deg;
        }

        double cap = m_old_cap_deg/180*M_PI;
        double roll = m_old_roll_deg/180*M_PI;
        //INFO(sin(pitch)*m_h);

        m_new_x += cos(cap)*sin(roll)*m_h;
        m_new_y += -sin(cap)*sin(roll)*m_h;
        m_calc_v = (sqrt((m_old_x-m_new_x)*(m_old_x-m_new_x)+(m_old_y-m_new_y)*(m_old_y-m_new_y)))/d_t;
        //m_calc_cap_deg = (atan2(-(m_old_x-m_new_x),-(m_old_y-m_new_y)))*180/M_PI;
        m_calc_a_v = (m_new_v-m_old_v)/d_t;
        m_calc_a_cap = (m_new_cap_deg-m_old_cap_deg)/d_t;
        
        m_old_x = m_old_x + sin(cap)*m_old_v*d_t;
        m_old_y = m_old_y + cos(cap)*m_old_v*d_t;
        m_old_v = m_old_v + m_old_a_v*d_t;
        //m_calc2_cap_deg =  m_old_cap_deg + m_old_a_cap*d_t;
        m_old_cap_deg = m_old_cap_deg + m_old_a_cap*d_t;
        normalizeDeg(m_old_cap_deg);


        m_old_x = m_lissage_ekf_xy*m_old_x+(1.0-m_lissage_ekf_xy)*m_new_x;
        m_old_y = m_lissage_ekf_xy*m_old_y+(1.0-m_lissage_ekf_xy)*m_new_y;

        m_old_v = m_lissage_ekf_v*m_old_v+(1.0-m_lissage_ekf_v)*m_new_v;

        double diff = m_new_cap_deg-m_old_cap_deg;
        normalizeDeg(diff);
        if(diff < -45 || diff > 45){
            m_old_cap_deg = m_new_cap_deg;
        } else {
            m_old_cap_deg = m_old_cap_deg+(1.0-m_lissage_ekf_cap)*(diff);
        }

        normalizeDeg(m_old_cap_deg);

        m_old_a_v = m_lissage_ekf_a_v*m_old_a_v+(1.0-m_lissage_ekf_a_v)*m_new_a_v;
        m_old_a_cap = m_lissage_ekf_a_cap*m_old_a_cap+(1.0-m_lissage_ekf_a_cap)*m_new_a_cap;

        m_old_roll_deg = m_lissage_ekf_roll*m_old_roll_deg+(1.0-m_lissage_ekf_roll)*m_new_roll_deg;
    }
};

#endif
