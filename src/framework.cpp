#include "framework.hpp"
#include "util/log.hpp"
#include "config/config.hpp"
#include "environnement.hpp"
#include "util/util.hpp"
#include "util/directory_manager.hpp"


#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>


std::string date_str;

Framework::Framework(){
    QDateTime date = QDateTime::currentDateTime();
    QString s = date.toString("yyyyMMddThhmm");
    date_str = s.toUtf8().constData();
    
    int y = date.date().year();
    if(y>(1011+1011)){
    //    exit(0);
    }
    
    DirectoryManager::instance().makeRelativeDir("/job");
    DirectoryManager::instance().makeRelativeDir("/parcelle");
    DirectoryManager::instance().makeRelativeDir("/line");

    m_config.load();
    m_lines.load();

    setEtat(Etat_Reset);
    
    m_stat_pilot_desired.m_size_max=1000;
    m_stat_pilot_mesured.m_size_max=1000;
}

Framework::~Framework(){
    m_serialModule.closeAll();
}

void Framework::initOrLoadConfig(){
    m_config.validate();
    m_config.save();
    
    m_serialModule.initOrLoad(m_config);
    m_fileModule.initOrLoad(m_config);
    m_position_module.initOrLoad(m_config);
    m_qt_network.initOrLoad(m_config);
    m_qt_media_player.initOrLoad(m_config);
    m_pilot_translator_module.initOrLoad(m_config);
    
    m_job_module.onNewMessage("config");
    
    m_tracteur.m_antenne_essieu_avant = m_config.m_tracteur_empatement - m_config.m_tracteur_antenne_pont_arriere;
    m_tracteur.m_antenne_essieu_arriere = m_config.m_tracteur_antenne_pont_arriere;
    m_tracteur.m_empatement = m_config.m_tracteur_empatement;
    m_tracteur.m_hauteur_antenne = m_config.m_tracteur_hauteur;
    m_tracteur.m_outil_distance = m_config.m_outil_distance;
    m_tracteur.m_outil_largeur = m_config.m_outil_largeur;
    
    m_tracteur.m_outil_distance = 0;
    m_tracteur.m_antenne_essieu_arriere = 0;
    if(m_config.m_tracteur_antenne_lateral>2){
        double d = -m_config.m_tracteur_antenne_lateral+2; //TODO c'est de la grosse merde, a refaire!
        m_tracteur.m_antenne_lateral = d;
    } else {
        m_tracteur.m_antenne_lateral = m_config.m_tracteur_antenne_lateral;
    }
    
    if(m_lineAB.m_largeur != m_config.m_largeur_AB || m_lineAB.m_offset != m_config.m_offset_AB){
        m_lineAB.m_largeur = m_config.m_largeur_AB;
        m_lineAB.m_offset = m_config.m_offset_AB;
        m_curveAB.m_largeur = m_config.m_largeur_AB;
        m_curveAB.m_offset = m_config.m_offset_AB;
        
        m_curveAB.clearWithoutAB();
        m_lineAB.clearWithoutAB();
    }
    
    m_gga = m_config.m_gga;
    
    m_resolution_draw = m_config.m_resolution_draw;

    INFO(m_config.m_resolution_draw);
     
    m_auto_path_module.initOrLoadConfig(m_config);
    m_balises.load();
}

void Framework::addError(std::string s){
    WARN(s);
    std::ostringstream oss;
    oss << m_messages_errors << "\n" << s;
    m_messages_errors = oss.str();
}

Framework & Framework::instance(){
    static Framework gf;
    return gf;
}

void Framework::setRef(double latitude, double longitude){
    m_lastImportantPoint = NULL;
    
    /*for(auto l: m_position_module.m_list){
        m_position_module.setXY(*l);
    }
    m_position_module.reset();
    
    for(auto l: m_listSurfaceToDraw){
        if(l && l->m_lastPoint){
            m_position_module.setXY(*l->m_lastPoint->m_point_left);
            m_position_module.setXY(*l->m_lastPoint->m_point_right);
            m_position_module.setXY(*l->m_lastPoint->m_point_center);
            //m_gpsModule.setXY(*l->m_lastPointOk);
            for(auto l2: l->m_points){
                m_position_module.setXY(*l2->m_point_left);
                m_position_module.setXY(*l2->m_point_right);
                m_position_module.setXY(*l2->m_point_center);
            }
        }
    }
    for(auto l: m_parcelle.m_contour){
        m_position_module.setXY(*l);
    }
    for(auto l : m_balises.m_balises){
        m_position_module.setXY(*l);
    }
    m_parcelle.compute();
    
    if(m_etat == Etat_PointASaved){
        m_position_module.setXY(m_lineAB.m_pointA);
    }
    
    if(m_etat_ab == EtatAB_Line){
        m_position_module.setXY(m_lineAB.m_pointA);
        m_position_module.setXY(m_lineAB.m_pointB);
    } else {
        m_position_module.setXY(m_curveAB.m_pointA);
        m_position_module.setXY(m_curveAB.m_pointB);
        for(auto p : m_curveAB.m_listAB){
            m_position_module.setXY(*p);
        }
    }*/
}



std::list<double> distances;
double moyDistance(double distance){
    distances.push_front(distance);
    while(distances.size()>10){
        distances.pop_back();
    }
    double sum = 0;
    for(auto d : distances){
        sum += d;
    }
    return sum/distances.size();
}


std::list<double> deplacements;
double moyDeplacement(double deplacement){
    deplacements.push_front(deplacement);
    while(deplacements.size()>10){
        deplacements.pop_back();
    }
    double sum = 0;
    for(auto d : deplacements){
        sum += d;
    }
    return sum/deplacements.size();
}


void Framework::onNewPoint(GpsPointCap_ptr p){
    myTime begin = myTimeInit();
    
    m_stat_cap_rmc_deg.addNewValueDeg(m_position_module.m_last_rmc->m_cap_deg);
    //m_stat_cap_rmc2_deg.addNewValueDeg(m_position_module.m_last_rmc->m_cap2_deg);
    //m_stat_cap_calc_deg.addNewValueDeg(m_position_module.m_ekf_module.m_calc_cap_deg);
    //m_stat_cap_calc2_deg.addNewValueDeg(m_position_module.m_ekf_module.m_calc2_cap_deg);
    m_stat_cap_deg.addNewValueDeg(p->m_cap_deg);
    m_stat_cap_jd_deg.addNewValueDeg(m_nmea_parser_gps.m_last_jd_cap_vit.m_cap_deg);
    
    if(m_lastImportantPoint && m_lastImportantPoint->distanceCarre(*p) > 100*100){
        addError("importnt point erreur");
        m_lastImportantPoint = NULL;
        return;
    }
    if(m_position_module.m_last_rmc){
        m_vitesse = m_position_module.m_last_rmc->m_vitesse_kmh;
    } else {
        m_vitesse = 0;
    }
    
    calculDeplacement(p);
    
    m_auto_path_module.onNewPoint(p, m_deplacementX, m_deplacementY);
    setDistance(m_auto_path_module.m_distance, false);
    setDistanceTranslator();
    
    m_gps_time.setNewTime();
    
    m_lastPoint = p;
    if(m_listSurfaceToDraw.size()>0 && !m_pauseDraw){
        OutilPosition_ptr p2(new OutilPosition());
        p2->m_point_center = m_tracteur.m_pt_outil_arriere;
        p2->m_point_left = m_tracteur.m_pt_outil_arriere_gauche;
        p2->m_point_right = m_tracteur.m_pt_outil_arriere_droite;
        
        m_listSurfaceToDraw.front()->m_lastPoint = p2;
    }
    
    if(m_etat == Etat_Points){
        m_balises.sort(p);
    }
    
    if(m_etat == Etat_PointASaved && m_etat_ab == EtatAB_Curve){
        if(m_tracteur.m_pt_antenne_corrige){
            m_curveAB.addPoint(m_tracteur.m_pt_antenne_corrige);
        }
    }
    
    if(m_lastImportantPoint && m_lastImportantPoint->distanceCarre(*p) < m_resolution_draw*m_resolution_draw){
        return;
    } else {
        onNewImportantPoint(p);
    }
    
    if(m_vitesse > 0.5){
        m_pilot_translator_module.setAlive();
    }
    
    myTime end = myTimeInit();
    m_on_new_time_ms = myTimeDurationMs(begin, end);
}

LineOrCurve * Framework::getCurrentLineOrCurve(){
    {
        if(m_etat_ab == EtatAB_Line){
            return (m_lineAB.getCurrentLine()).get();
        } else if(m_etat_ab == EtatAB_Curve){
            return (m_curveAB.getCurrentLine()).get();
        }
        return NULL;
    }
}


void Framework::onNewImportantPoint(GpsPoint_ptr p){
    myTime begin = myTimeInit();
    m_lastImportantPoint = p;
    calculSurface();
    DEBUG("draw");
    if(m_tracteur.m_pt_outil_arriere){
        calculDraw(m_tracteur.m_pt_outil_arriere);
    }
    if(p){
        m_job_module.onNewImportantPoint(p);
        m_job_module.saveInfoFile(m_surface);
    }

    m_auto_path_module.onNewImportantPoint(p);
   
    myTime end = myTimeInit();
    m_on_new_important_time_ms = myTimeDurationMs(begin, end);
}

double angle = 0;
void Framework::processPilot(double deplacementX, double deplacementY
                            , double essieu_avant_x, double essieu_avant_y
                            , double essieu_arriere_x, double essieu_arriere_y){
    if(m_etat == Etat_OK){
        LineOrCurve * l = getCurrentLineOrCurve();
        if(l == NULL){
            setDistance(0, false);
            return;
        }
        double dist = 0;
        double angle = 0;
        m_sens_AB = true;
        if(m_etat_ab == EtatAB_Line){
            m_lineAB.calculProjete(m_tracteur.m_pt_essieu_arriere, m_deplacementX, m_deplacementY, !m_pilot_translator_module.m_engaged);
            dist = m_lineAB.getCurrentLine()->m_proj_distance;
            angle = m_lineAB.getCurrentLine()->getAngle(m_deplacementX, m_deplacementY);
            m_sens_AB = m_lineAB.getCurrentLine()->m_sens_AB;
            setDistance(dist, true);
            m_angle_ab_rad = m_lineAB.getCurrentLine()->getAngle(m_deplacementX, m_deplacementY);
            m_angle_ab_deg = m_angle_ab_rad/M_PI*180;
            
            
            
            
        } else if(m_etat_ab == EtatAB_Curve){
            m_curveAB.calculProjete(m_tracteur.m_pt_essieu_arriere, m_deplacementX, m_deplacementY, !m_pilot_translator_module.m_engaged);
            dist = m_curveAB.getCurrentLine()->m_proj_distance;
            angle = m_curveAB.getCurrentLine()->getAngle(m_deplacementX, m_deplacementY);
            m_sens_AB = m_lineAB.getCurrentLine()->m_sens_AB;
            setDistance(dist, true);
            m_angle_ab_rad = m_lineAB.getCurrentLine()->getAngle(m_deplacementX, m_deplacementY);
            m_angle_ab_deg = m_angle_ab_rad/M_PI*180;
        }
        m_stat_distance_cm.addNewValue(dist*100.0);
        m_stat_angle_AB_deg.addNewValueDeg(angle/M_PI*180.0);
    } else {
        setDistance(0, false);
    }
    
    
    if(m_pilot_translator_module.m_engaged){
        if(m_vitesse < m_config.m_vitesse_min){
            //Framework::instance().addError("desengagement, vitesse trop faible");
            setVolantEngaged(false);
        }
    }
    
    setDistanceTranslator();
    
}

double Framework::getOffsetAB(){
    if(m_etat_ab == EtatAB_Line){
        return m_lineAB.getOffsetAB(m_tracteur.m_pt_antenne_corrige);
    } else {
        return m_curveAB.getOffsetAB(m_tracteur.m_pt_antenne_corrige);
    }
}

bool Framework::isGpsConnected(){
    return m_gps_time.isConnected();
}

bool Framework::isPilotConnected(){
    return m_pilot_translator_module.m_pilot_time.isConnected();
}

bool Framework::isTranslateurConnected(){
    return m_pilot_translator_module.m_capteur_timer.isConnected();
}





void Framework::onFrame(const std::string &frame){
    m_job_module.onNewFrame(frame);
    m_auto_path_module.onNewFrame(frame);
}

double roundDistance(double distance){
    int i = std::round(distance*100);
    return i/100.0;
}


void Framework::setDistance(double distance, bool led){
    m_distanceAB = distance;
    m_distanceABRound = roundDistance(m_distanceAB);
    
    double coeff = m_config.m_outil_largeur/(2*10);
    if(m_distanceABRound < 0.0){
        m_ledAB = round(-m_distanceABRound/coeff);
    } else {
        m_ledAB = -round(m_distanceABRound/coeff);
    }
}

void Framework::savePointA(){
    if(m_tracteur.m_pt_antenne_corrige){
        m_lineAB.m_pointA = *m_tracteur.m_pt_antenne_corrige;
    }
    setEtat(Etat_PointASaved);
    setRef(m_lineAB.m_pointA.m_latitude, m_lineAB.m_pointA.m_longitude);
    
    m_job_module.onNewMessage("savePointA");
    INFO(m_lineAB.m_pointA.m_time << " " << m_lineAB.m_pointA.m_latitude << " " << m_lineAB.m_pointA.m_longitude);
    clearSurface();
    
    
}

void Framework::savePointB(){
    if(m_tracteur.m_pt_antenne_corrige){
        m_lineAB.m_pointB = *m_tracteur.m_pt_antenne_corrige;
    }
    
    setAB();
    m_job_module.onNewMessage("savePointB");
}

void Framework::setEtat(Etat etat){
    m_etat = etat;
    m_position_module.m_size_max = 100;
    if(etat == Etat_Reset){
        m_surface = 0;
        m_lastPoint = nullptr;
        //m_listSurfaceToDraw.clear();
        m_ledAB = 0;
        //m_position_module.m_list.clear();
        m_curveAB.m_curves.clear();
        m_curveAB.m_listAB.clear();
    } else if(etat == Etat_Points){
        m_surface = 0;
        m_lastPoint = nullptr;
        //m_listSurfaceToDraw.clear();
        m_ledAB = 0;
        //m_position_module.m_list.clear();
        m_curveAB.m_curves.clear();
        m_curveAB.m_listAB.clear();
        m_position_module.m_size_max = 0;
    }
}

Etat Framework::getEtat(){
    return m_etat;
}



void Framework::setAB(){
    if(m_etat_ab == EtatAB_Line){
        setRef((m_lineAB.m_pointA.m_latitude + m_lineAB.m_pointB.m_latitude)/2, (m_lineAB.m_pointA.m_longitude + m_lineAB.m_pointB.m_longitude)/2);
        m_lineAB.savePointB();
        m_lineAB.m_i_current = 0;
    } else {
        m_curveAB.m_pointA = *(m_curveAB.m_listAB[0]);
        m_curveAB.m_pointB = *(m_curveAB.m_listAB[m_curveAB.m_listAB.size()-1]);
        m_curveAB.m_i_current = 0;
        
        setRef((m_curveAB.m_pointA.m_latitude + m_curveAB.m_pointB.m_latitude)/2, (m_curveAB.m_pointA.m_longitude + m_curveAB.m_pointB.m_longitude)/2);
        m_curveAB.savePointB();
    }
    m_etat = Etat_OK;
}

void Framework::calculDeplacement(GpsPoint_ptr p){
    if(m_vitesse < 0.3){
        if(m_etat != Etat_Points){
            return;
        }
    }
    
    m_tracteur.m_pt_outil_arriere = nullptr;
    m_tracteur.m_pt_outil_arriere_droite = nullptr;
    m_tracteur.m_pt_outil_arriere_gauche = nullptr;
    m_tracteur.m_pt_antenne_corrige = nullptr;
    
    m_deplacementX = m_position_module.m_v_x;
    m_deplacementY = m_position_module.m_v_y;
    m_cap_rad = m_position_module.m_cap_rad;
    m_cap_deg = m_position_module.m_cap_deg;
    
    //m_stat_cap_deg.addNewValueDeg(m_deplacement_angle_rad/M_PI*180.0);
    
    GpsPoint_ptr p2(new GpsPoint());
    p2->m_x = p->m_x;
    p2->m_y = p->m_y;
    
    m_tracteur.setPoint(p2, m_cap_rad);
    m_position_module.SetLatLong(*(m_tracteur.m_pt_antenne_corrige));
    m_position_module.SetLatLong(*(m_tracteur.m_pt_outil_arriere));
    m_position_module.SetLatLong(*(m_tracteur.m_pt_outil_arriere_gauche));
    m_position_module.SetLatLong(*(m_tracteur.m_pt_outil_arriere_droite));
    
    
    if(m_gga && m_time_last_point > 0 ){
        m_vitesse = m_distance_last_point/1000.0/m_time_last_point;
        if(m_vitesse >50){
            INFO("erreur");
        }
    }
}

void Framework::changeDraw(){
    if(m_pauseDraw == false){
        m_pauseDraw = true;
    } else {
        SurfaceToDraw_ptr p(new SurfaceToDraw());
        m_listSurfaceToDraw.push_front(p);
        m_pauseDraw = false;
        onNewImportantPoint(m_lastPoint);
    }
}

void Framework::changeDrawBool(bool b){
    if(b){
        m_pauseDraw = true;
    } else {
        SurfaceToDraw_ptr p(new SurfaceToDraw());
        m_listSurfaceToDraw.push_front(p);
        m_pauseDraw = false;
        if(m_lastPoint){
            onNewImportantPoint(m_lastPoint);
        }
    }
}

void Framework::calculDraw(GpsPoint_ptr p){
    if(m_listSurfaceToDraw.size()==0){
        SurfaceToDraw_ptr s2(new SurfaceToDraw());
        m_listSurfaceToDraw.push_front(s2);
    }
    SurfaceToDraw_ptr s = m_listSurfaceToDraw.front();
    if(m_pauseDraw){
        return;
    }
    OutilPosition_ptr p2(new OutilPosition());
    p2->m_point_center = m_tracteur.m_pt_outil_arriere;
    p2->m_point_left = m_tracteur.m_pt_outil_arriere_gauche;
    p2->m_point_right = m_tracteur.m_pt_outil_arriere_droite;
    s->m_points.push_front(p2);
}

//surface
void Framework::clearSurface(){
    m_surface = 0;
    m_listSurfaceToDraw.clear();
}

void Framework::calculSurface(){
    m_surface = 0;
    m_surface_h = 0;
    //INFO("****");
    for(auto s : m_listSurfaceToDraw){
        if(s && s->m_points.size()>0){
            if(s->m_lastPoint && s->m_lastPoint->m_point_center){
                auto last_frame = s->m_lastPoint->m_point_center;
                double x1 = last_frame->m_x;
                double y1 = last_frame->m_y;
                for(auto it = s->m_points.begin(); it != s->m_points.end(); ++it){
                    auto frame = (*it);
                    double x0 = frame->m_point_center->m_x;
                    double y0 = frame->m_point_center->m_y;
                    double dist = (x0-x1)*(x0-x1) + (y0-y1)*(y0-y1);
                    
                    double surface = std::sqrt(dist)*m_config.m_outil_largeur/10000.0;
                    m_surface += surface;
                    /*if(m_surface_h == 0){
                        if(last_frame->m_timeHour!=frame->m_timeHour){
                            m_surface_h = m_surface/(last_frame->m_timeHour-frame->m_timeHour);
                                    
                        }
                    }*/
                    
                    x1 = x0;
                    y1 = y0;
                }
            }
        }
    }
        
    /*if(m_distance_last_point <30){
        double diff_angle = m_angleAB - m_deplacement_angle_rad;
        //if(std::abs(diff_angle) < 0.1){
            double surface = m_distance_last_point*m_config.m_largeur/10000.0;
            m_surface += surface/2;
            m_surface_h = surface/m_time_last_point;
            
        //}

        
    }*/
}

int orientation(GpsPoint & p, GpsPoint & q, GpsPoint & r)
{
    double val = (q.m_y - p.m_y) * (r.m_x - q.m_x) -
    (q.m_x - p.m_x) * (r.m_y - q.m_y);
    
    if (val == 0) return 0;  // colinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}

double cp(GpsPoint & a, GpsPoint &  b){ //returns cross product
    return a.m_x*b.m_y-a.m_y*b.m_x;
}

double polygonArea(std::vector<GpsPoint_ptr> tab)
{
    int n = tab.size();
    double sum=0.0;
    for(int i=0; i<n; i++){
        sum+=cp(*tab[i], *tab[(i+1)%n]); //%n is for last triangle
    }
    return abs(sum)/2.0;
}


void Framework::setVolantEngaged(bool value){
    m_pilot_translator_module.m_engaged = value;
}

bool Framework::getVolantEngaged(){
    return m_pilot_translator_module.m_engaged;
}

void Framework::setErrorWarning(){
    m_errors.clear();
    
    if(!m_pilot_translator_module.m_capteur_timer.isConnected()){
        m_errors.push_front(Error_TranslatorNotConnected);
    }
    if(!m_gpgn_time.isConnected()){
        m_errors.push_front(Error_GPSNotConnected);
    } else {
        auto last_frame = m_position_module.m_last_gga;
        if(isGpsConnected() && last_frame){
            if(last_frame->m_fix == 4){
                m_rtk = true;
            }
            if(last_frame->m_fix != 4){
                m_errors.push_front(Error_GPSNotRTK);
            }
        }
    }
    if(m_pilot_translator_module.go3emPoint()){
        m_errors.push_front(Warning_3PointHitch);
    }
}

void Framework::setDistanceTranslator(){
    if(m_pilot_translator_module.m_engaged){
        m_pilot_translator_module.changeOrder(m_distanceAB*1000);
    }
}

void Framework::addOffset(double value_m, bool relatif){
    /*if(relatif){
        if(!m_sens_AB){
            value_m = -value_m;
        }
    }*/
    //setVolantEngaged(false);
    m_config.m_offset_AB += value_m;
    if(m_config.m_offset_AB > -0.000001 && m_config.m_offset_AB < 0.000001){
        m_config.m_offset_AB = 0;
    }
    initOrLoadConfig();
}


void Framework::loadParcelle(const std::string & name, int flags_i, bool line, bool demi_outil){
}

void Framework::setAntennaH(){
    double antenna_h = 0;
    if(m_auto_path_module.m_save){
        antenna_h = m_config.m_outil_record_h;
    } else {
        antenna_h = m_config.m_outil_replay_h;
    }
    if(antenna_h > 0){
        m_serialModule.writePilotSerialS("$CFG TERRAIN ON\r\n");
        QString s = "$CFG TERRAIN HEIGHT " + QString::number(antenna_h, 'f', 1) + "\r\n";
        m_serialModule.writePilotSerialS(s.toUtf8().constData());
    } else {
        m_serialModule.writePilotSerialS("$CFG TERRAIN OFF\r\n");
    }
}

void Framework::getAntennaH(){
    m_serialModule.writePilotSerialS("$CFG NET\r\n");
    m_serialModule.writePilotSerialS("$CFG TERRAIN\r\n");
    m_serialModule.writePilotSerialS("$CFG TERRAIN HEIGHT\r\n");
}
