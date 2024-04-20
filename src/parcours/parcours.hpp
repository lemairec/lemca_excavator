#ifndef PARCOURS_H
#define PARCOURS_H

#include "curveAB.hpp"
#include "lineAB.hpp"

class DemiTour : public Curve {
public:
    DemiTour(): Curve(0){
    }
    bool m_isInit = false;
    int m_i_source = 0;
    int m_i_dest = 0;
    GpsPoint_ptr P0;
    GpsPoint_ptr P0_c;
    GpsPoint_ptr P1_c1;
    GpsPoint_ptr P1;
    GpsPoint_ptr P1_c2;
    GpsPoint_ptr P2_c;
    GpsPoint_ptr P2;
    GpsPoint_ptr P3;

    void point_bezier_1(double t, GpsPoint_ptr res);
    void point_bezier_2(double t, GpsPoint_ptr res);
    void computeBezier();
};


#endif // LINE_AB_H
