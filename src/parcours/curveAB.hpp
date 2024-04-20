#ifndef CURVE_AB_H
#define CURVE_AB_H

#include "l_curve.hpp"

class CurveAB{
public:
    GpsPoint m_pointA;
    GpsPoint m_pointB;
    
    double m_largeur = 10;
    double m_offset = 0;
   
    std::string m_name;
    
    std::vector<GpsPoint_ptr> m_listAB;
    
    int m_i_min = 0;
    int m_i_max = 0;
    int m_i_current = 0;
    
    void computeCurve(Curve_ptr);
    void verify(int i);
    Curve_ptr getCurrentLine();
    Curve_ptr getCurrentLineRel(int i);
    
    std::map<int, Curve_ptr> m_curves;
    
    void clearAll();
    void clearWithoutAB();
    
    void addLine(int i);
    void setCurrent(int i);
    
    double distance(double x, double y, double lg);
    void addPoint(GpsPoint_ptr p);
    
    void savePointB();
    
    
    
    void calculProjete(GpsPoint_ptr p, double deplacement_x, double deplacement_y, bool change_line);
    double getOffsetAB(GpsPoint_ptr p);

    double calculRearWheelPosition(double x_pont, double y_pont, double x, double y, double deplacement_x, double deplacement_y, double vitesse, double L, double KTH, double KE);

    double m_d_k;
    double m_d_angle;
    double m_d_e;
    double m_d_xk;
    double m_d_xe;
    double m_d_xthe;
    double m_d_res;
    double m_d_delta;
};

#endif // LINE_AB_H
