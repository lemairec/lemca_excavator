#include "diag_widget.hpp"

#include "../framework.hpp"
#include "qt/main_window.hpp"

#include "../config/langage.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

DiagnosticWidget::DiagnosticWidget(){
}

void DiagnosticWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    m_y2 = m_height*0.04;
    m_x2 = m_y2;
    m_width2 = m_width*2/3-2*m_y2;
    m_height2 = m_height-2*m_y2;
    
    m_button_ok.setResizeStd(m_x2 + 0.5*m_width2, 0.9*m_height2, "", true);
}

void DiagnosticWidget::drawDiag(const std::string & s, double x, double y, double value, double min, double max){
    drawText(s, x+20, y, sizeText_little);
    if(min <= value && value <= max){
        m_painter->setBrush(m_brush_green);
    } else {
        m_painter->setBrush(m_brush_orange);
    }
    m_painter->drawEllipse(x, y-7, 10, 10);
}

void DiagnosticWidget::drawDiag2(const std::string & s, double x, double y, bool filter){
    drawText(s, x+20, y, sizeText_little);
    if(filter){
        m_painter->setBrush(m_brush_green);
    } else {
        m_painter->setBrush(m_brush_orange);
    }
    m_painter->drawEllipse(x, y-7, 10, 10);
}


void DiagnosticWidget::drawDiagGray(const std::string & s, double x, double y, bool enable, bool filter){
    drawText(s, x+20, y, sizeText_little);
    if(enable){
        if(filter){
            m_painter->setBrush(m_brush_green);
        } else {
            m_painter->setBrush(m_brush_orange);
        }
    } else {
        m_painter->setBrush(m_brush_gray);
    }
    m_painter->drawEllipse(x, y-7, 10, 10);
}



void DiagnosticWidget::draw(){
    
    m_painter->setPen(m_pen_black);
    m_painter->setBrush(m_brush_white);
    m_painter->drawRoundedRect(m_x2, m_y2, m_width2, m_height2, RAYON_ROUNDED, RAYON_ROUNDED);
    
    
    Framework & f = Framework::instance();
    double y = 0.1*m_height2;
    double inter = 0.03*m_height2;
    double x1 = m_x2+0.05*m_width2;
    double x2 = m_x2+0.35*m_width2;
    double x3 = m_x2+0.65*m_width2;
    drawText("DIAGNOSTIC", m_x2+0.5*m_width2, y, sizeText_medium, true);
    
    //colonne 1
    y+=3*inter;
    drawText("B&W", x1, y, sizeText_medium);
    y+=1.2*inter;
    {
        QString s2 = "error : " + QString::fromStdString(f.m_messages_errors);
        drawQText(s2, x1, y, sizeText_little);
    }
    /*{
        QString s = "coeff : " + QString::number(f.m_config.m_coeff_color);
        drawQText(s, x1, y, sizeText_little);
        
    }
    y+=inter;
    {
        QString s = "contrast : " + QString::number(f.m_config.m_coeff_res);
        drawQText(s, x1, y, sizeText_little);
        
    }
    y+=2*inter;
    drawText("Pattern", x1, y, sizeText_medium);
    y+=1.2*inter;
    {
        QString s = "height : " + QString::number(f.m_config.m_calibration_ha_hauteur/10) + " cm";
        drawQText(s, x1, y, sizeText_little);
        
    }
    y+=inter;
    {
        QString s = "angle : " + QString::number(f.m_config.m_calibration_ha_angle) + " °";
        drawQText(s, x1, y, sizeText_little);
        
    }
    y+=inter;
    {
        QString s = "inter row : " + QString::number(f.m_config.m_pattern_inter_rang_mm/10) + " cm";
        drawQText(s, x1, y, sizeText_little);
        
    }
    y+=inter;
    {
        QString s = "nb row : " + QString::number(f.m_config.m_nbr_rangs);
        drawQText(s, x1, y, sizeText_little);
        
    }
    y+=inter;
    {
        QString s = "thickness : " + QString::number(f.m_config.m_pattern_epaisseur_mm/10) + " cm";
        drawQText(s, x1, y, sizeText_little);
        
    }
    y+=inter;
    {
        QString s = "score min : " + QString::number(f.m_config.m_score_min) + " %";
        drawQText(s, x1, y, sizeText_little);
        
    }
    y+=2*inter;
    drawText("Offset", x1, y, sizeText_medium);
    y+=1.2*inter;
    {
        QString s = "offset : " + QString::number(f.m_config.m_offset_mm/10) + " cm";
        drawQText(s, x1, y, sizeText_little);
        
    }
    y+=inter;
    {
        QString s = "tilt : " + QString::number(f.m_config.m_tilt) + " - " + QString::number(f.m_config.m_tilt_l_mm/10) + " cm";
        drawQText(s, x1, y, sizeText_little);
        
    }y+=inter;
    {
        QString s = "   agress : " + QString::number(f.m_config.m_tilt_agress*100) + " %";
        drawQText(s, x1, y, sizeText_little);
        
    }
    y+= 2*inter;
    {
        QString s = "version : " + QString::fromStdString(ProjectVersion);
        drawQText(s, x1, y, sizeText_little);
        
    }
    y+=inter;
    {
        QString s = "licence : " + QString::fromStdString(f.m_config.m_licence);
        drawQText(s, x1, y, sizeText_little);
        
    }
    y+=inter;
    {
        QString s = "panel : " + QString::fromStdString(f.m_config.m_licence_panel);
        drawQText(s, x1, y, sizeText_little);
        
    }
    y+=inter;
    {
        QString s = "mcu : " + QString::fromStdString(f.m_config.m_licence_boitier);
        drawQText(s, x1, y, sizeText_little);
        
    }
    y+=inter;*/
    
    //colonne 2
    y = 0.1*m_height2;
    y+=3*inter;
    drawText("Port1 GPS", x2, y, sizeText_medium);
    y+=1.2*inter;
    {
        QString s = "conn : " + QString::fromStdString(f.m_config.m_port1_gps_serial);
        drawQText(s, x2, y, sizeText_little);
    }
    y+=1*inter;
    {
        QString s = "count : " + QString::number(f.m_serialModule.m_port1_gps_count);
        drawQText(s, x2, y, sizeText_little);
    }
    y+=1*inter;
    {
        std::string s = strprintf("conn : %.1f ms (%.1f)" ,f.m_gps_time.m_moy, f.m_gps_time.m_et);
        drawDiag(s, x2, y, f.m_gps_time.m_moy, 80, 120);
    }
    y+=1*inter;
    {
        std::string s = strprintf("conn nb error : %d %d" ,f.m_nmea_parser_gps.m_nb_error_check, f.m_nmea_parser_gps.m_nbr_error);
        drawText(s, x2, y, sizeText_little);
    }
    y+=1*inter;
    {
        GGAFrame_ptr gga = f.m_position_module.m_last_gga;
        if(gga){
            std::string s1 = "";
            if(gga->m_fix == 1){
                s1 = "GPS";
            } else if(gga->m_fix == 2){
                s1 = "DGPS";
            } else if(gga->m_fix == 3){
                s1 = "PPSFIX";
            } else if(gga->m_fix == 4){
                s1 = "RTK";
            } else if(gga->m_fix == 5){
                s1 = "FRTK";
            } else if(gga->m_fix == 9){
                s1 = "DGPS compas";
            } else {
                s1 = "invalid";
            }
            std::string s = strprintf("hdop %.1f, nb stat %d, fix %d, %s" ,gga->m_hdop, gga->m_nbrSat, gga->m_fix, s1.data());
            drawText(s, x2, y, sizeText_little);
        }
    }
    y+=1*inter;
    {
        std::string s = f.m_nmea_parser_gps.m_last_gga_str;
        drawText(s, x2, y, sizeText_little);
    }
    
    
    y+=1.4*inter;
    
    drawText("Port2 MCU", x2, y, sizeText_medium);
    y+=1.2*inter;
    {
        QString s = "conn : " + QString::fromStdString(f.m_config.m_port2_mcu_serial);
        drawQText(s, x2, y, sizeText_little);
    }
    y+=1.2*inter;
    {
        QString s = "count : " + QString::number(f.m_serialModule.m_port2_mcu_count);
        drawQText(s, x2, y, sizeText_little);
    }
    y+=1*inter;
    {
        std::string s = strprintf("conn : %.1f ms (%.1f)" ,f.m_pilot_translator_module.m_pilot_time.m_moy, f.m_pilot_translator_module.m_pilot_time.m_et);
        drawText(s, x2, y, sizeText_little);
    }
    y+=1*inter;
    {
        std::string s = strprintf("conn error nb : %d %d" ,f.m_nmea_parser_mcu.m_nb_error_check, f.m_nmea_parser_mcu.m_nbr_error);
        drawText(s, x2, y, sizeText_little);
    }
    y+=1*inter;
    {
        drawText(f.m_nmea_parser_mcu.m_last_error, x3, y, sizeText_little);
    }
    y+=1*inter;
    {
        drawText(f.m_nmea_parser_mcu.m_other_str, x3, y, sizeText_little);
    }
    y+=1*inter;
    drawText(f.m_pilot_translator_module.m_last_order_send, x1, y, sizeText_little, false);
    
    y+=1.4*inter;
   drawText("Port3 Soil", x2, y, sizeText_medium);
    y+=1.2*inter;
    {
        QString s = "conn : " + QString::fromStdString(f.m_config.m_port3_soil_serial);
        drawQText(s, x2, y, sizeText_little);
    }
    y+=1*inter;
    {
        QString s = "count : " + QString::number(f.m_serialModule.m_port3_soil_count);
        drawQText(s, x2, y, sizeText_little);
    }
    y+=1.4*inter;
    for(auto s : f.m_serialModule.messages){
        QString s2 = "-- " + QString::fromStdString(s);
        drawQText(s2, x2, y, sizeText_little);
        y+=1*inter;
    }
    /*{
        std::string s = strprintf("conn : %.1f ms (%.1f)" ,f.m_pilot_translator_module.m_pilot_time.m_moy, f.m_pilot_translator_module.m_pilot_time.m_et);
        drawText(s, x2, y, sizeText_little);
    }
    y+=1*inter;
    {
        std::string s = strprintf("conn error nb : %d %d" ,f.m_nmea_parser_mcu.m_nb_error_check, f.m_nmea_parser_mcu.m_nbr_error);
        drawText(s, x2, y, sizeText_little);
    }
    y+=1*inter;
    {
        drawText(f.m_nmea_parser_mcu.m_last_error, x3, y, sizeText_little);
    }
    y+=1*inter;
    {
        drawText(f.m_nmea_parser_mcu.m_other_str, x3, y, sizeText_little);
    }*/
    
    
   
    //colonne 3
    y = 0.1*m_height2;
    y+=3*inter;
    
    
    y+=2*inter;
    drawText("times", x3, y, sizeText_medium);
    y+=1.2*inter;
    {
        std::string s = strprintf("on_new_time %i ms" ,f.m_on_new_time_ms);
        drawText(s, x3, y, sizeText_little);
    }
    y+=1*inter;
    {
        std::string s = strprintf("on_new_important_time %i ms" ,f.m_on_new_time_ms);
        drawText(s, x3, y, sizeText_little);
    }
    y+=2*inter;
    drawText("Debug", x3, y, sizeText_medium);
    y+=1.2*inter;
    {
        drawText(f.m_debug_str_1, x3, y, sizeText_little);
    }
    y+=1*inter;
    {
        drawText(f.m_debug_str_2, x3, y, sizeText_little);
    }
    y+=1*inter;
    {
        drawText(f.m_debug_str_3, x3, y, sizeText_little);
    }
    y+=1*inter;
    {
        drawText(f.m_debug_str_4, x3, y, sizeText_little);
    }
    /*if(f.m_cameras_nbr > 1){
        drawQText("cam 2 : " + QString::number(f.m_images[1]->m_deplacement_mm/10, 'f', 1) + " cm , (" + QString::number(f.m_images[1]->m_deplacement_ang_deg, 'f', 1) + " °)", x3, y, sizeText_little, false);
        y+=inter;
        drawQText("  " + QString::number(f.m_images[1]->m_score*100, 'f', 0) + " %", x3, y, sizeText_little, false);
    } else {
        drawQText("cam 2 : -- ", x3, y, sizeText_little, false);
        y+=inter;
        drawQText("  --  %", x3, y, sizeText_little, false);
    }
    y+=2*inter;
    drawText("times", x3, y, sizeText_medium);
    y+=1.2*inter;
    drawQText("cam 1 : " + QString::number(f.m_camera1_timer.m_moy, 'f', 1) + " ms", x3, y, sizeText_little, false);
    y+=inter;
    drawQText("cam 2 : " + QString::number(f.m_camera2_timer.m_moy, 'f', 1) + " ms", x3, y, sizeText_little, false);
    y+=inter;
    if(f.m_lastImage){
        drawQText("calcul : " + QString::number(f.m_lastImage->m_timePrint, 'f', 1) + " ms", x3, y, sizeText_little, false);
    }
    y+=inter;
    {
        QString qs = "charge proc : " + QString::number(f.getChargeThread(), 'f', 1) + " %";
        drawQText(qs, x3, y, sizeText_little, false);
    }
    y+=2*inter;
    drawText("Errors", x3, y, sizeText_medium);
    y+=1.2*inter;
    
    auto etats = f.getEtats();
    for(auto e : etats){
        std::string s = "";
        QString s2 = "";
        switch(e){
            case Etat_3eme_point:
            {
                s = Langage::getKey("MAIN_3POINT");
                break;
            }
            case Etat_not_engaged:
            {
                s = Langage::getKey("MAIN_NOT_ENGAGED");
                break;
            }
            case Etat_score_low:
            {
                s = Langage::getKey("MAIN_SCORE_LOW");
                break;
            }
            case Etat_serial_not_connected:
            {
                s = Langage::getKey("MAIN_NOT_CONNECTED");
                break;
            }
            case Etat_pas_mesured_0:
            {
                s = Langage::getKey("MAIN_PAS_MESURED_0");
                break;
            }
            case Etat_translateur_contraint:
            {
                s = Langage::getKey("MAIN_CONTRAINT");
                s2 = QString::number(f.m_config.m_pattern_amplitude_mm/10) + " cm";
            }
            case Etat_translateur_temp:
            {
                s = Langage::getKey("MAIN_TEMP");
                break;
            }
            case Etat_vitesse_low:
            {
                s = Langage::getKey("MAIN_VITESSE_LOW");
                break;
            }
            case Etat_vitesse_sat:
            {
                s = Langage::getKey("MAIN_VITESSE_SAT");
                break;
            }
            case Etat_vitesse_joystick_auto:
            {
                s = Langage::getKey("MAIN_JOYSTICK_AUTO");
                break;
            }
            case Etat_vitesse_joystick_left:
            {
                s = Langage::getKey("MAIN_JOYSTICK_LEFT");
                break;
            }
            case Etat_vitesse_joystick_right:
            {
                s = Langage::getKey("MAIN_JOYSTICK_RIGHT");
                break;
            }
            case Etat_Butee_Gauche:
            {
                s = Langage::getKey("MAIN_MAX_LEFT");
                break;
            }
            case Etat_Butee_Droite:
            {
                s = Langage::getKey("MAIN_MAX_RIGHT");
                break;
            }
            case Etat_KO:
            {
                s = Langage::getKey("MAIN_UNKNOWN_ERROR");
                break;
            }
        }
        s2 = QString::fromStdString(s) + s2;
        drawQText(s2, x3, y, sizeText_little);
        y+= inter;
    }
    y+=2*inter;
    drawText("Debug", x3, y, sizeText_medium);
    y+=1.2*inter;
    drawText(f.m_debug_mess, x3, y, sizeText_medium);*/
    
    
    m_button_ok.m_label = QString::fromStdString(Langage::getKey("CLOSE"));
    drawButtonLabel2(m_button_ok, COLOR_VALIDATE);
}

int DiagnosticWidget::onMouse(int x, int y){
    if(m_button_ok.isActive(x, y)){
        m_close = true;
    }
    return 0;
}
