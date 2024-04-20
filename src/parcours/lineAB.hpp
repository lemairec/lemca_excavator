#ifndef LINE_AB_H
#define LINE_AB_H

#include "l_line.hpp"
#include "curveAB.hpp"

class LineAB{
public:
    GpsPoint m_pointA;
    GpsPoint m_pointB;
    
    double m_largeur = 10.003;
    double m_offset = 0;

    std::string m_name;
    
    //spec
    double m_x_ab , m_y_ab;
    
    int m_i_min = 0;
    int m_i_max = 0;
    int m_i_current = 0;
    
    void computeCurve(GpsLine_ptr);
    void verify(int i);
    GpsLine_ptr getCurrentLine();
    GpsLine_ptr getCurrentLineRel(int i);
    GpsLine_ptr getCurrentLineI(int i);
    
    std::map<int, GpsLine_ptr> m_curves;
    
    void clearAll();
    void clearWithoutAB();
    
    void addLine(int i);
    void setCurrent(int i);
    
    void savePointB();
    
    
    double m_curb_x, m_curb_y, m_curb_l;
    double m_curb_c_x, m_curb_c_y;
    double calculCurbature(GpsLine_ptr line, size_t i);
    
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

class LineCurves {
public:
    std::string m_line_selected;
    std::vector<std::string> m_lines_curves;
    void load();
    void loadCurveOrLine(std::string name);
    void remove(std::string name);
    void add(LineAB & p);
    void add(CurveAB & p);
    void save();
};

#endif // LINE_AB_H
