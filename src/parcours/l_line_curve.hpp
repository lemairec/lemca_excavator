#ifndef LINE_OR_CURVE_H
#define LINE_OR_CURVE_H

#include "../gps/position_module.hpp"
#include "../util/util.hpp"
#include <list>
#include <vector>
#include <map>

class LineOrCurve{
public:
    void calculProjete2(double x, double y);
    
    double m_proj_x_segment;
    double m_proj_y_segment;
    
    double m_proj_distance = 0;
    bool m_sens_AB = true;
    
    double m_proj_prod_vect = 0;
    double m_proj_x = 0, m_proj_y = 0;
    virtual void calculProjete2(double x, double y, double deplacement_x, double deplacement_y){
    }
    virtual void calculProjete2P(GpsPoint_ptr p, double deplacement_x, double deplacement_y){
        this->calculProjete2(p->m_x, p->m_y, deplacement_x, deplacement_y);
    }
    
    double m_fc_x = 0, m_fc_y = 0;
    double m_fc_lh_x = 0, m_fc_lh_y = 0;
    double m_fc_xh = 0, m_fc_yh = 0;
    double m_r;
    virtual double anglePurePursuit(double x_pont, double y_pont, double deplacement_x, double deplacement_y, double lookhead, double wheelbase){
        return 0.0;
    }
    
    virtual double getAngle(double deplacement_x, double deplacement_y){
        double x_segment = m_proj_x_segment;
        double y_segment = m_proj_y_segment;
        double angle = my_angle(deplacement_x, deplacement_y, x_segment, y_segment);
        angle = angleBetweenMDemiPiDemiPi(angle);
        return angle;
    }
    
    
    ~LineOrCurve(){};
};

#endif
