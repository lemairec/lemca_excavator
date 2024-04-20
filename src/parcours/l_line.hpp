#ifndef LINE_H
#define LINE_H

#include "l_line_curve.hpp"

class GpsLine : public LineOrCurve{
public:
    GpsLine(int i){
        m_i = i;
    }
   
    virtual ~GpsLine(){
    }
    
    int m_i;
    
    GpsPoint m_pointA;
    GpsPoint m_pointB;
    
    void calculProjete2(double x, double y);
    void calculProjete2(double x, double y, double deplacement_x, double deplacement_y);
    
    double anglePurePursuit(double x_pont, double y_pont, double deplacement_x, double deplacement_y, double lookhead, double wheelbase);
};

typedef std::shared_ptr<GpsLine> GpsLine_ptr;

#endif
