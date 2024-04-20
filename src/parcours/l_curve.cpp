#include "l_curve.hpp"

#include <cmath>
#include "../util/util.hpp"


void Curve::calculProjete2(double x, double y, double deplacement_x, double deplacement_y){
    if(this->m_points.size() == 0){
        return;
    }
    this->m_curve_i_min = 0;
    this->m_curve_i_min2 = 0;
    double dist_min = 10000;
    
    for(int i = 0; i < (int)this->m_points.size(); ++i){
        double d = this->m_points[i]->distanceCarre(x, y);
        if(d < dist_min){
            dist_min = d;
            this->m_curve_i_min = i;
        }
    }
    
    if(this->m_curve_i_min == 0){
        this->m_curve_i_min2 = 1;
    } else if(this->m_curve_i_min == ((int)this->m_points.size())-1){
        this->m_curve_i_min2 = this->m_points.size()-2;
    } else {
        
        double d1 = this->m_points[this->m_curve_i_min-1]->distanceCarre(x, y);
        double d2 = this->m_points[this->m_curve_i_min+1]->distanceCarre(x, y);
        
        if(d1 < d2){
            this->m_curve_i_min = this->m_curve_i_min-1;
        }
        this->m_curve_i_min2 = this->m_curve_i_min+1;
    }
    
    double x_a = this->m_points[this->m_curve_i_min]->m_x;
    double y_a = this->m_points[this->m_curve_i_min]->m_y;
    double x_b = this->m_points[this->m_curve_i_min2]->m_x;
    double y_b = this->m_points[this->m_curve_i_min2]->m_y;

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

double Curve::anglePurePursuit(double x, double y, double deplacement_x, double deplacement_y, double lookhead, double wheelbase){
    if(this->m_points.size() == 0){
        return 0.0;
    }
    m_fc_x = x;
    m_fc_y = y;
    
    this->calculProjete2(x, y, deplacement_x, deplacement_y);
    m_fc_xh = this->m_proj_x;
    m_fc_yh = this->m_proj_y;
    
    //Follow carrot PT LOOKHEAD
    double d = this->m_proj_prod_vect;
    double x_v2 = 0;
    double y_v2 = 0;
    
    if(d>0){
        double d = lookhead;
        
        size_t i_min2 = this->m_curve_i_min2;
        while(d > 0 && i_min2 < this->m_points.size()-1){
            double dh = sqrt(this->m_points[i_min2]->distanceCarre(*this->m_points[i_min2+1]));
            d = d - dh;
            i_min2++;
        }
        
        auto pt1 = this->m_points[i_min2];

        x_v2 = pt1->m_x-m_fc_xh;
        y_v2 = pt1->m_y-m_fc_yh;
    } else {
        double d = lookhead;
        
        size_t i_min2 = this->m_curve_i_min2;
        while(d > 0 && i_min2 > 1){
            double dh = sqrt(this->m_points[i_min2]->distanceCarre(*this->m_points[i_min2-1]));
            d = d - dh;
            i_min2--;
        }
        
        auto pt1 = this->m_points[i_min2];

        x_v2 = pt1->m_x-m_fc_xh;
        y_v2 = pt1->m_y-m_fc_yh;
    }
    
    double d_v2 = sqrt(x_v2*x_v2 + y_v2*y_v2);
    x_v2 = x_v2/d_v2;
    y_v2 = y_v2/d_v2;
    double d2 = deplacement_x*x_v2+deplacement_y*y_v2;
    double x_lk;
    double y_lk;
    if(d2 >0){
        x_lk = x + lookhead*x_v2;
        y_lk = y + lookhead*y_v2;
    } else {
        x_lk = x - lookhead*x_v2;
        y_lk = y - lookhead*y_v2;
    }
    
    calculProjete2(x_lk, y_lk, deplacement_x, deplacement_y);
    m_fc_lh_x = this->m_proj_x;
    m_fc_lh_y = this->m_proj_y;
    
    //Follow carrot CALCUL ANGLE
    double x_segment = m_fc_lh_x - m_fc_x;
    double y_segment = m_fc_lh_y - m_fc_y;

    double alpha = my_angle(x_segment, y_segment, deplacement_x, deplacement_y);
    
    m_r = lookhead/(2*sin(alpha));

    double delta = atan2(2.0 * wheelbase *sin(alpha) / lookhead, 1.0);

    return delta;
}


double Curve::calculCurbature(size_t i){
    double tol = 0.9999999;
    size_t size_cur = 4;
    if(this->m_points.size() < size_cur){
        m_curb_l = 0;
        return 0.0;
    }
    if(i < size_cur){
        m_curb_l = 0;
        return 0.0;
    }
    if(i > this->m_points.size()-size_cur-1){
        m_curb_l = 0;
        return 0.0;
    }
    auto pointB = this->m_points[i];
    auto pointA = this->m_points[i-size_cur];
    auto pointC = this->m_points[i+size_cur];
    
    double a = sqrt(pointA->distanceCarre(*pointB));
    double b = sqrt(pointB->distanceCarre(*pointC));
    double c = sqrt(pointC->distanceCarre(*pointA));
    
    double temp = (a + b)/c;
    
    if(temp > tol && temp < (1/tol)) {
        return 0;
    }
    double p = (a+b+c)/2;
    double curbature_p = (4*sqrt(p*(p-a)*(p-b)*(p-c)))/(a*b*c);
    
    double x_ab = pointB->m_x - pointA->m_x;
    double y_ab = pointB->m_y - pointA->m_y;
    double x_bc = pointC->m_x - pointB->m_x;;
    double y_bc = pointC->m_y - pointB->m_y;
    
    
    double det = x_ab*y_bc-y_ab*x_bc;
    
    double curbature = curbature_p;
    if(det < 0){
        curbature = -curbature;
    }
    curbature = -curbature;
    
    double x_ca = pointC->m_x - pointA->m_x;
    double y_ca = pointC->m_y - pointA->m_y;
    double d_ca = sqrt(x_ca*x_ca+y_ca*y_ca);
    double x_v = x_ca/d_ca;
    double y_v = y_ca/d_ca;
    
    if(curbature>0){
        m_curb_x = (pointC->m_x+ pointA->m_x)/2+y_v/curbature_p;
        m_curb_y = (pointC->m_y+ pointA->m_y)/2-x_v/curbature_p;
    } else {
        m_curb_x = (pointC->m_x+ pointA->m_x)/2-y_v/curbature_p;
        m_curb_y = (pointC->m_y+ pointA->m_y)/2+x_v/curbature_p;
    }
    m_curb_l = 1/curbature_p;
    m_curb_c_x = pointB->m_x;
    m_curb_c_y = pointB->m_y;
    
    return curbature;
}

