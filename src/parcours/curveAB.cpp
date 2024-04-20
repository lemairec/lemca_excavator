#include "curveAB.hpp"
#include "../util/log.hpp"

#include "environnement.hpp"
#include "../util/util.hpp"

#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>


const double longeur = 2;


void CurveAB::clearAll(){
    m_curves.clear();
    m_listAB.clear();
}

void CurveAB::clearWithoutAB(){
    m_curves.clear();
    m_i_min = 0;
    m_i_max = 0;
    m_i_current = 0;
    savePointB();
}

double CurveAB::distance(double x, double y, double lg){
    return 0.0;
    
}

Curve_ptr CurveAB::getCurrentLine(){
    return m_curves[m_i_current];
}

Curve_ptr CurveAB::getCurrentLineRel(int i){
    int res = m_i_current+i;
    verify(res);
    return m_curves[res];
}

void CurveAB::verify(int i){
    if(i<m_i_min){
        for(int j = m_i_min-1; j >= i-10; --j){
            addLine(j);
        }
    }
    if(i>m_i_max){
        for(int j = m_i_max+1; j <= i+10; ++j){
            addLine(j);
        }
    }
}
             
void CurveAB::addPoint(GpsPoint_ptr p){
    m_listAB.push_back(p);
}

void clearLine(std::vector<GpsPoint_ptr> & l){
    auto l2 = l;
    l.clear();
    GpsPoint_ptr old_point = nullptr;
    for(auto p : l2){
        if(old_point){
            double dx = p->m_x - old_point->m_x;
            double dy = p->m_y - old_point->m_y;
            
            double res = dx*dx+dy*dy;
            if(res > longeur*longeur){
                l.push_back(p);
                old_point = p;
            }
        } else {
            l.push_back(p);
            old_point = p;
        }
    }
}

void addPointsExtremite(std::vector<GpsPoint_ptr> & l){
    auto l2 = l;
    l.clear();
    if(l2.size()>0){
        {
            GpsPoint_ptr p0 = l2[0];
            GpsPoint_ptr p1 = l2[1];

            double d_x = p1->m_x - p0->m_x;
            double d_y = p1->m_y - p0->m_y;
            double d = sqrt(d_x*d_x+d_y*d_y);
            
            d_x = d_x/d;
            d_y = d_y/d;
            
            for(int i = -10; i < 0; ++i){
                GpsPoint_ptr p(new GpsPoint());
                p->m_x = p0->m_x+d_x*10*i;
                p->m_y = p0->m_y+d_y*10*i;
                
                l.push_back(p);
            }
        }
        
        for(auto p : l2){
            l.push_back(p);
        }
        
        {
            size_t s = l2.size()-2;
            GpsPoint_ptr p0 = l2[s];
            GpsPoint_ptr p1 = l2[s+1];
        
            double d_x = p1->m_x - p0->m_x;
            double d_y = p1->m_y - p0->m_y;
            double d = sqrt(d_x*d_x+d_y*d_y);
            
            d_x = d_x/d;
            d_y = d_y/d;
            
            for(int i = 0; i < 10; ++i){
                GpsPoint_ptr p(new GpsPoint());
                p->m_x = p0->m_x+d_x*10*i;
                p->m_y = p0->m_y+d_y*10*i;
                
                l.push_back(p);
            }
        }
    }
    
}

void addPoints(std::vector<GpsPoint_ptr> & l){
    auto l2 = l;
    l.clear();
    GpsPoint_ptr old_point = nullptr;
    for(auto p : l2){
        if(old_point){
            double dx = p->m_x - old_point->m_x;
            double dy = p->m_y - old_point->m_y;
            
            double res = dx*dx+dy*dy;
            if(res > longeur*longeur){
                GpsPoint_ptr p3(new GpsPoint());
                p3->m_x = (p->m_x + old_point->m_x)/2;
                p3->m_y = (p->m_y + old_point->m_y)/2;
                
                l.push_back(p3);
                l.push_back(p);
                old_point = p;
            }
        } else {
            l.push_back(p);
            old_point = p;
        }
    }
    
    if(l.size() < 4 && l.size() > 2){
        auto p1 = l[l.size()-2];
        auto p2 = l[l.size()-1];
        int x = p1->m_x-p2->m_x;
        int y = p1->m_y-p2->m_y;
        GpsPoint_ptr p3(new GpsPoint());
        p3->m_x = p2->m_x + y;
        p3->m_y = p2->m_y + x;
        
        l.push_back(p3);
    }
    
}

void CurveAB::addLine(int i){
    int j=i-1;
    int j2=i-2;
    if(i <0){
        j=i+1;
        j2=i+2;
    }
    
    
    m_curves[i] = Curve_ptr(new Curve(i));
    if(i<m_i_min){
        m_i_min = i;
    }
    if(i>m_i_max){
        m_i_max = i;
    }
    
    GpsPoint_ptr old_point = nullptr;
    for(auto p : m_curves[j]->m_points){
        if(old_point){
            
            double dx = p->m_x - old_point->m_x;
            double dy = p->m_y - old_point->m_y;
            
            double temp = sqrt(dx*dx + dy*dy);
            
            GpsPoint_ptr p2(new GpsPoint());
            p2->m_x = (old_point->m_x + p->m_x)/2 + dy/temp*m_largeur;
            p2->m_y = (old_point->m_y + p->m_y)/2 - dx/temp*m_largeur;
            
            GpsPoint_ptr p3(new GpsPoint());
            p3->m_x = (old_point->m_x + p->m_x)/2 - dy/temp*m_largeur;
            p3->m_y = (old_point->m_y + p->m_y)/2 + dx/temp*m_largeur;
            
            if(m_curves[i]->m_points.size() > 0){
                double d2 = m_curves[i]->m_points.back()->distanceCarre(*p2);
                double d3 = m_curves[i]->m_points.back()->distanceCarre(*p3);
                
                if(d2 < d3){
                    auto r = p3;
                    p3 = p2;
                    p2 = r;
                }
                
            } else {
                double d2 = m_curves[j2]->m_points.front()->distanceCarre(*p2);
                double d3 = m_curves[j2]->m_points.front()->distanceCarre(*p3);
                //INFO(d2 << " " << d3);
                if(d2 > d3){
                    p3 = p2;
                }
            }
            m_curves[i]->m_points.push_back(p3);
            
            
        }
        old_point = p;
    }
    computeCurve(m_curves[i]);
}

void CurveAB::computeCurve(Curve_ptr curve){
    clearLine(curve->m_points);
    addPoints(curve->m_points);
    addPoints(curve->m_points);
    
    for(size_t i = 0; i< curve->m_points.size(); ++i){
        curve->m_points_simpl.push_back(curve->m_points[i]);
        curve->m_points_to_draw.push_back(curve->m_points[i]);
    }
    
}

void CurveAB::setCurrent(int i){
    verify(i);
    m_i_current = i;
}

void CurveAB::savePointB(){
    clearLine(m_listAB);
    //addPointsExtremite(m_listAB);
    m_curves.clear();
    GpsPoint_ptr old_point = nullptr;
    
    m_curves[0] = Curve_ptr(new Curve(0));
    m_curves[1] = Curve_ptr(new Curve(1));
    m_curves[-1] = Curve_ptr(new Curve(-1));
    m_i_max = -1;
    m_i_max = 1;
    for(auto p : m_listAB){
        if(old_point){
            
            double dx = p->m_x - old_point->m_x;
            double dy = p->m_y - old_point->m_y;
            
            double temp = sqrt(dx*dx + dy*dy);
            
            GpsPoint_ptr p0(new GpsPoint());
            p0->m_x = (old_point->m_x + p->m_x)/2 + dy/temp*(m_offset);
            p0->m_y = (old_point->m_y + p->m_y)/2 - dx/temp*(m_offset);
            
            GpsPoint_ptr p2(new GpsPoint());
            p2->m_x = (old_point->m_x + p->m_x)/2 + dy/temp*(m_offset+m_largeur);
            p2->m_y = (old_point->m_y + p->m_y)/2 - dx/temp*(m_offset+m_largeur);
            
            GpsPoint_ptr p3(new GpsPoint());
            p3->m_x = (old_point->m_x + p->m_x)/2 + dy/temp*(m_offset-m_largeur);
            p3->m_y = (old_point->m_y + p->m_y)/2 - dx/temp*(m_offset-m_largeur);
            
            
            m_curves[-1]->m_points.push_back(p2);
            m_curves[1]->m_points.push_back(p3);
            m_curves[0]->m_points.push_back(p0);
        }
        old_point = p;
    }
    computeCurve(m_curves[0]);
    computeCurve(m_curves[1]);
    computeCurve(m_curves[-1]);
    
    for(int i = 2; i < 11; ++i){
        addLine(i);
        addLine(-i);
    }
    
    //saveABCurve();;
    
}



double CurveAB::getOffsetAB(GpsPoint_ptr p){
    double x = p->m_x;
    double y = p->m_y;
    
    Curve_ptr list = getCurrentLine();
    if(list == NULL || list->m_points.size() < 5){
        return 0;
    }
    list->m_curve_i_min = 0;
    list->m_curve_i_min2 = 0;
    double dist_min = 10000;
    
    
    
    for(int i = 0; i < (int)list->m_points.size(); ++i){
        double d = list->m_points[i]->distanceCarre(x, y);
        if(d < dist_min){
            dist_min = d;
            list->m_curve_i_min = i;
        }
    }
    if(list->m_curve_i_min == 0){
        list->m_curve_i_min2 = 1;
    } else if(list->m_curve_i_min == ((int)list->m_points.size())-1){
        list->m_curve_i_min2 = list->m_points.size()-2;
    } else {
        
        double d1 = list->m_points[list->m_curve_i_min-1]->distanceCarre(x, y);
        double d2 = list->m_points[list->m_curve_i_min+1]->distanceCarre(x, y);
        
        if(d1 < d2){
            list->m_curve_i_min = list->m_curve_i_min-1;
        }
        list->m_curve_i_min2 = list->m_curve_i_min+1;
    }
    
    double x_a = x;
    double y_a = y;
    
    double x_b = list->m_points[list->m_curve_i_min]->m_x;
    double y_b = list->m_points[list->m_curve_i_min]->m_y;
    double x_m = list->m_points[list->m_curve_i_min2]->m_x;
    double y_m = list->m_points[list->m_curve_i_min2]->m_y;

    //https://fr.wikipedia.org/wiki/Projection_orthogonale
    double x_v = x_m-x_b;
    double y_v = y_m-y_b;
    list->m_proj_x_segment = x_v;
    list->m_proj_y_segment = y_v;
    double d_v = sqrt(x_v*x_v + y_v*y_v);
    x_v = x_v/d_v;
    y_v = y_v/d_v;
    
    
    
    double bh = (x_a-x_b)*x_v+(y_a-y_b)*y_v;
    list->m_proj_x = x_b + bh*x_v;
    list->m_proj_y = y_b + bh*y_v;
    
    
    double ah = sqrt((list->m_proj_x-x_a)*(list->m_proj_x-x_a) + (list->m_proj_y-y_a)*(list->m_proj_y-y_a));
    list->m_proj_distance = ah;
    
    double deplacement_x = m_pointB.m_x - m_pointA.m_x;
    double deplacement_y = m_pointB.m_y - m_pointA.m_y;
    
    list->m_proj_prod_vect = deplacement_x*x_v+deplacement_y*y_v;
    
    double det = (list->m_proj_x-x_a)*deplacement_y-(list->m_proj_y-y_a)*deplacement_x;
    
    if(det < 0){
        list->m_proj_distance = -list->m_proj_distance;
    }
    
    return list->m_proj_distance;
}

void CurveAB::calculProjete(GpsPoint_ptr p, double deplacement_x, double deplacement_y, bool change_line){
    auto list = getCurrentLine();
    list->calculProjete2P(p, deplacement_x, deplacement_y);
    double dist = abs(list->m_proj_distance);
    
    if(change_line){
        if(dist > m_largeur/2){
            for(int i = 0; i < 100; ++i){
                int i_before = m_i_current;
                setCurrent(i_before + 1);
                list = getCurrentLine();
                list->calculProjete2P(p, deplacement_x, deplacement_y);
                double dist2 = abs(list->m_proj_distance);
                setCurrent(i_before - 1);
                list = getCurrentLine();
                list->calculProjete2P(p, deplacement_x, deplacement_y);
                double dist3 = abs(list->m_proj_distance);
                
                if(dist2 < dist){
                    setCurrent(i_before + 1);
                    Curve_ptr list = getCurrentLine();
                    list->calculProjete2P(p, deplacement_x, deplacement_y);
                    dist = abs(list->m_proj_distance);;
                }
                
                if(dist3 < dist){
                    setCurrent(i_before - 1);
                    Curve_ptr list = getCurrentLine();
                    list->calculProjete2P(p, deplacement_x, deplacement_y);
                    dist = abs(list->m_proj_distance);;
                }
                
                if(dist < m_largeur/2){
                    return;
                }
            }
        }
    }
}





double CurveAB::calculRearWheelPosition(double x_pont, double y_pont, double x, double y, double deplacement_x, double deplacement_y, double vitesse, double L, double KTH, double KE){
    double v = vitesse*10000.0/3600.0;
    
    Curve_ptr list = getCurrentLine();
    if(list == NULL || list->m_points.size() < 5){
        return 0;
    }
    list->calculProjete2(x_pont, y_pont, deplacement_x, deplacement_y);
    double e = -list->m_proj_distance;
    
    list->calculProjete2(x, y, deplacement_x, deplacement_y);
    
    double k = list->calculCurbature(list->m_curve_i_min);
    double d = list->m_proj_prod_vect;
    if(d < 0){
        k = -k;
    }
    
    double angle = list->getAngle(deplacement_x, deplacement_y);
    double th_e = angle;//todo;

    double xk = v * k * cos(th_e) / (1.0 - k * e) ;
    double xthe = KTH * abs(v) * th_e ;
    double xe = KE * v * sin(th_e) * e / th_e;

    double omega = xk - xe - xthe;


    m_d_e = e;
    m_d_angle = th_e;
    m_d_k = k;

    m_d_xk = xk;
    m_d_xthe = -xthe;
    m_d_xe = -xe;
    m_d_res = omega;

    double delta = atan2(L * omega / v, 1.0);

    m_d_delta = delta;

    return delta;
}
