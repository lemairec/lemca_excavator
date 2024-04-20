#include "lineAB.hpp"
#include <math.h>
#include "../util/log.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "environnement.hpp"
#include "../framework.hpp"
#include "../util/util.hpp"
#include "../util/directory_manager.hpp"




void LineAB::clearAll(){
    m_curves.clear();
}

void LineAB::clearWithoutAB(){
    m_curves.clear();
    m_i_min = 0;
    m_i_max = 0;
    m_i_current = 0;
    
    savePointB();
}

GpsLine_ptr LineAB::getCurrentLine(){
    return m_curves[m_i_current];
}

GpsLine_ptr LineAB::getCurrentLineRel(int i){
    int res = m_i_current+i;
    verify(res);
    return m_curves[res];
}

GpsLine_ptr LineAB::getCurrentLineI(int i){
    int res = i;
    verify(res);
    return m_curves[res];
}

void LineAB::verify(int i){
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

void LineAB::addLine(int i){
    m_curves[i] = GpsLine_ptr(new GpsLine(i));
    if(i<m_i_min){
        m_i_min = i;
    }
    if(i>m_i_max){
        m_i_max = i;
    }
    
    GpsPoint_ptr old_point = nullptr;
    
    m_x_ab = m_pointB.m_x - m_pointA.m_x;
    m_y_ab = m_pointB.m_y - m_pointA.m_y;
    double d  = std::sqrt(m_x_ab*m_x_ab + m_y_ab*m_y_ab);
    m_x_ab = m_x_ab/d;
    m_y_ab = m_y_ab/d;
    
    m_curves[i]->m_pointA.m_x = m_pointA.m_x + m_y_ab*(i*m_largeur+m_offset);
    m_curves[i]->m_pointA.m_y = m_pointA.m_y - m_x_ab*(i*m_largeur+m_offset);
    m_curves[i]->m_pointB.m_x = m_pointB.m_x + m_y_ab*(i*m_largeur+m_offset);
    m_curves[i]->m_pointB.m_y = m_pointB.m_y - m_x_ab*(i*m_largeur+m_offset);
    computeCurve(m_curves[i]);
}

void LineAB::computeCurve(GpsLine_ptr curve){
}

void LineAB::setCurrent(int i){
    verify(i);
    m_i_current = i;
}

void LineAB::savePointB(){
    m_curves.clear();
    GpsPoint_ptr old_point = nullptr;
    
    addLine(0);
    
    for(int i = 1; i < 11; ++i){
        addLine(i);
        addLine(-i);
    }
}


double LineAB::calculCurbature(GpsLine_ptr line, size_t i){
    return 1.0;
}




double LineAB::getOffsetAB(GpsPoint_ptr p){
    GpsLine_ptr list = getCurrentLine();
    
    double x_a = p->m_x;
    double y_a = p->m_y;
    
    double x_b = list->m_pointA.m_x;
    double y_b = list->m_pointA.m_y;
    double x_m = list->m_pointB.m_x;
    double y_m = list->m_pointB.m_y;

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

void LineAB::calculProjete(GpsPoint_ptr p, double deplacement_x, double deplacement_y, bool change_line){
    GpsLine_ptr list = getCurrentLine();
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
                    GpsLine_ptr list = getCurrentLine();
                    list->calculProjete2P(p, deplacement_x, deplacement_y);
                    dist = abs(list->m_proj_distance);;
                }
                
                if(dist3 < dist){
                    setCurrent(i_before - 1);
                    GpsLine_ptr list = getCurrentLine();
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


double LineAB::calculRearWheelPosition(double x_pont, double y_pont, double x, double y, double deplacement_x, double deplacement_y, double vitesse, double L, double KTH, double KE){
    double v = vitesse*10000.0/3600.0;
    
    GpsLine_ptr list = getCurrentLine();
    if(list == NULL){
        return 0;
    }
    list->calculProjete2(x_pont, y_pont, deplacement_x, deplacement_y);
    double e = -list->m_proj_distance;
    
    list->calculProjete2(x, y, deplacement_x, deplacement_y);
    
    double k = 0;
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





void LineCurves::load(){
    std::string file_parcelles = DirectoryManager::instance().getDataDirectory() + "/line/lines.txt";
    std::ifstream file(file_parcelles);
    std::string line;
    while (std::getline(file, line))
    {
        bool find = false;
        for(auto l : m_lines_curves){
            if(line == l){
                find = true;
            }
        }
        if(!find){
            m_lines_curves.push_back(line);
        }
    }
    INFO("lines_curves "  << m_lines_curves.size());
}

void LineCurves::save(){
    std::ofstream file;
    std::string file_parcelles = DirectoryManager::instance().getDataDirectory() + "/line/lines.txt";
    file.open(file_parcelles, std::ios::out);
    for(auto p : m_lines_curves){
        file << p << std::endl;
    }
    INFO("lines_curves "  << m_lines_curves.size());
}

void LineCurves::remove(std::string name){
    std::string path = DirectoryManager::instance().getDataDirectory() + "/line/" + name + ".txt";
    std::ofstream file;
    file.open(path, std::ios::out);
    file << "DELETE" << std::endl;
    
    std::vector<std::string> temp;
    for(auto s : m_lines_curves){
        temp.push_back(s);
    }
    m_lines_curves.clear();
    for(auto s : temp){
        if(s != name){
            m_lines_curves.push_back(s);
        }
    }
    save();
}

void LineCurves::loadCurveOrLine(std::string name){
    Framework & f = Framework::instance();
    
    std::string file_job = DirectoryManager::instance().getDataDirectory() + "/line/" + name + ".txt";
    std::ifstream file(file_job);
    std::string line;
    
    if(file.is_open()){
        
        std::getline(file, line);
        if(line == "LINE"){
            
        }
        
        std::vector<GpsPoint_ptr> l;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            double a, b;
            if (!(iss >> a >> b)) {
                INFO("error");
                break;
                
            }
            GpsPoint_ptr p = GpsPoint_ptr(new GpsPoint());
            p->m_latitude = a;
            p->m_longitude = b;
            l.push_back(p);
            // process pair (a,b)
        }
        
        if(l.size() == 2){
            INFO("line");
            f.m_etat_ab = EtatAB_Line;
            f.m_lineAB.m_pointA = *(l[0]);
            f.m_lineAB.m_pointB = *(l[1]);
            f.setAB();
        } else {
            f.m_etat_ab = EtatAB_Curve;
            f.m_curveAB.clearAll();
            for(auto p : l){
                f.m_curveAB.m_listAB.push_back(p);
            }
            f.setAB();
        }
        m_line_selected = name;
        /*m_name = name;
        compute();*/
    } else {
        INFO(file_job << " is close");
    }
}



void LineCurves::add(LineAB & p){
    Framework & f = Framework::instance();
    if(p.m_name.size() > 2){
        std::string name = "line_" + p.m_name;
        m_lines_curves.push_back(name);
        
        std::string path = DirectoryManager::instance().getDataDirectory() + "/line/" + name + ".txt";
        std::ofstream file;
        file.open(path, std::ios::out);
        file << "LINE" << std::endl;
        file << std::setprecision(14) << p.m_pointA.m_latitude << " " << p.m_pointA.m_longitude << std::endl;
        file << std::setprecision(14) << p.m_pointB.m_latitude << " " << p.m_pointB.m_longitude << std::endl;
        save();
    } else {
        f.addError("line nom trop petit");
    }
}

void LineCurves::add(CurveAB & p){
    Framework & f = Framework::instance();
    if(p.m_name.size() > 2){
        std::string name = "curve_" + p.m_name;
        m_lines_curves.push_back(name);
        
        std::string path = DirectoryManager::instance().getDataDirectory() + "/line/" + name + ".txt";
        std::ofstream file;
        file.open(path, std::ios::out);
        file << "CURVE" << std::endl;
        for(auto p : f.m_curveAB.m_listAB){
            file << std::setprecision(14) << p->m_latitude << " " << p->m_longitude << std::endl;
        }
        save();
    } else {
        f.addError("curve nom trop petit");
    }
}

