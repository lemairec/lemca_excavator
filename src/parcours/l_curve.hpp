#ifndef CURVE_H
#define CURVE_H

#include "l_line_curve.hpp"

class Curve : public LineOrCurve {
public:
    Curve(int i){
        m_i = i;
    }
    
    virtual ~Curve(){
    }
    
    int m_i;
    int m_curve_i_min = 0;
    int m_curve_i_min2 = 0;
    std::vector<GpsPoint_ptr> m_points;
    std::vector<GpsPoint_ptr> m_points_simpl;
    std::vector<GpsPoint_ptr> m_points_to_draw;
    
    void calculProjete2(double x, double y, double deplacement_x, double deplacement_y);
    
    double m_curb_x, m_curb_y, m_curb_l;
    double m_curb_c_x, m_curb_c_y;
    double calculCurbature(size_t i);
    
    double anglePurePursuit(double x, double y, double deplacement_x, double deplacement_y, double lookhead, double wheelbase);
};

typedef std::shared_ptr<Curve> Curve_ptr;

#endif
