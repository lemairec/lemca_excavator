#include "l_line.hpp"

#include <cmath>
#include "../util/util.hpp"

void GpsLine::calculProjete2(double x, double y){
    double x_a = m_pointA.m_x;
    double y_a = m_pointA.m_y;
    double x_b = m_pointB.m_x;
    double y_b = m_pointB.m_y;
    
    double x_v = x_b-x_a;
    double y_v = y_b-y_a;
    double d_v = sqrt(x_v*x_v + y_v*y_v);
    x_v = x_v/d_v;
    y_v = y_v/d_v;

    double bh = (x-x_a)*x_v+(y-y_a)*y_v;
    m_proj_x = x_a + bh*x_v;
    m_proj_y = y_a + bh*y_v;
}


void GpsLine::calculProjete2(double x, double y, double deplacement_x, double deplacement_y){
    double x_a = m_pointA.m_x;
    double y_a = m_pointA.m_y;
    double x_b = m_pointB.m_x;
    double y_b = m_pointB.m_y;

    double x_v = x_b-x_a;
    double y_v = y_b-y_a;
    m_proj_x_segment = x_v;
    m_proj_y_segment = y_v;
    double d_v = sqrt(x_v*x_v + y_v*y_v);
    x_v = x_v/d_v;
    y_v = y_v/d_v;
    
    double bh = (x-x_a)*x_v+(y-y_a)*y_v;
    m_proj_x = x_a + bh*x_v;
    m_proj_y = y_a + bh*y_v;
    
    
    double ah = sqrt((m_proj_x-x)*(m_proj_x-x) + (m_proj_y-y)*(m_proj_y-y));
    m_proj_distance = ah;
    m_proj_prod_vect = deplacement_x*x_v+deplacement_y*y_v;
    
    double det = (m_proj_x-x)*deplacement_y-(m_proj_y-y)*deplacement_x;
    if(det < 0){
        m_proj_distance = -m_proj_distance;
    }
    if(m_proj_prod_vect > 0){
        m_sens_AB = true;
    } else {
        m_sens_AB = false;
    }
}


double GpsLine::anglePurePursuit(double x, double y, double deplacement_x, double deplacement_y, double lookhead, double wheelbase){
    m_fc_x = x;
    m_fc_y = y;
    
    calculProjete2(x, y, deplacement_x, deplacement_y);
    m_fc_xh = this->m_proj_x;
    m_fc_yh = this->m_proj_y;
    
    // Follow carrot PT LOOKHEAD
    double x_v2 = m_pointB.m_x - m_pointA.m_x;
    double y_v2 = m_pointB.m_y - m_pointA.m_y;
    
    double d_v2 = sqrt(x_v2*x_v2 + y_v2*y_v2);
    x_v2 = x_v2/d_v2;
    y_v2 = y_v2/d_v2;
    double d2 = deplacement_x*x_v2+deplacement_y*y_v2;
    if(d2 >0){
        m_fc_lh_x = m_fc_xh + lookhead*x_v2;
        m_fc_lh_y = m_fc_yh + lookhead*y_v2;
    } else {
        m_fc_lh_x = m_fc_xh - lookhead*x_v2;
        m_fc_lh_y = m_fc_yh - lookhead*y_v2;
    }
    
    // Follow carrot CALCUL ANGLE
    double x_segment = m_fc_lh_x - m_fc_x;
    double y_segment = m_fc_lh_y - m_fc_y;

    double alpha = my_angle(x_segment, y_segment, deplacement_x, deplacement_y);
    
    m_r = lookhead/(2*sin(alpha));
    
    double delta = std::atan2(2.0 * wheelbase *sin(alpha) / lookhead, 1.0);

    return delta;
}

