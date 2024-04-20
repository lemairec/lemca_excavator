#include "parcours.hpp"
#include "../util/log.hpp"
void combinaison_lineaire(GpsPoint_ptr A, GpsPoint_ptr B, double u, double v, GpsPoint_ptr res){
    res->m_x = A->m_x*u + B->m_x*v;
    res->m_y = A->m_y*u + B->m_y*v;
    //return [A[0]*u+B[0]*v,A[1]*u+B[1]*v]
}


void DemiTour::point_bezier_1(double t, GpsPoint_ptr res){
    double x=(1-t)*(1-t);
    double y=t*t;
    GpsPoint_ptr A = GpsPoint_ptr(new GpsPoint());
    GpsPoint_ptr B = GpsPoint_ptr(new GpsPoint());
    INFO(t);
    combinaison_lineaire(P0,P0_c,(1-t)*x,3*t*x, A);
    combinaison_lineaire(P1_c1,P1,3*y*(1-t),y*t, B);
    res->m_x = A->m_x+B->m_x;
    res->m_y = A->m_y+B->m_y;
}

void DemiTour::point_bezier_2(double t, GpsPoint_ptr res){
    double x=(1-t)*(1-t);
    double y=t*t;
    GpsPoint_ptr A = GpsPoint_ptr(new GpsPoint());
    GpsPoint_ptr B = GpsPoint_ptr(new GpsPoint());
    INFO(t);
    combinaison_lineaire(P1,P1_c2,(1-t)*x,3*t*x, A);
    combinaison_lineaire(P2_c,P2,3*y*(1-t),y*t, B);
    res->m_x = A->m_x+B->m_x;
    res->m_y = A->m_y+B->m_y;
}

void DemiTour::computeBezier(){
    m_points.clear();
    double dt = 1.0/50.0;
    double t = dt;
    m_points.push_back(P0);
    while(t < 1.0) {
        GpsPoint_ptr p = GpsPoint_ptr(new GpsPoint());
        point_bezier_1(t, p);
        m_points.push_back(p);
        t += dt;
    }
    
    
    t = dt;
    m_points.push_back(P1);
    while(t < 1.0) {
        GpsPoint_ptr p = GpsPoint_ptr(new GpsPoint());
        point_bezier_2(t, p);
        m_points.push_back(p);
        t += dt;
    }
    
    t = dt;
    m_points.push_back(P2);
    while(t < 1.0) {
        GpsPoint_ptr p = GpsPoint_ptr(new GpsPoint());
        p->m_x = t*P2->m_x + (1-t)*P3->m_x;
        p->m_y = t*P2->m_y + (1-t)*P3->m_y;
        m_points.push_back(p);
        t += dt;
    }
    
    m_points.push_back(P3);
    //m_points.push_back(P4);
}
