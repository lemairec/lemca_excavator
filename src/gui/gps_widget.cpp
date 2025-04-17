#include "gps_widget.hpp"

#include <sstream>
#include <fstream>
#include "../util/log.hpp"
#include "../framework.hpp"
#include <string>
#include "environnement.hpp"

#include "../config/langage.hpp"


GpsWidget::GpsWidget(){
    //m_firspage = false;
    m_zoom = 50;
    
    //m_widgets.push_back(&m_satWidget);
     m_widgets.push_back(&m_option_widget);
    m_widgets.push_back(&m_param_translateur_widget);
    m_widgets.push_back(&m_key_pad_widget);
    
    m_option_widget.m_key_board_widget = &m_key_board_widget;
    
    m_vue_3D = false;
}

GpsWidget * GpsWidget::instance(){
    static GpsWidget obj;
    return &obj;
}

void GpsWidget::loadImages(){
    m_imgSatBlanc = loadImage("/gui/sat.png");
    m_imgSatVert = loadImage("/gui/sat_vert.png");
    m_imgSatOrange = loadImage("/gui/sat_orange.png");
    m_imgSatRouge = loadImage("/gui/sat_rouge.png");
    
    m_imgOk = loadImage("/gui/ok.png");
    m_img_plus = loadImage("/gui/plus.png");
    m_img_moins = loadImage("/gui/moins.png");
    m_imgGuidage = loadImage("/gui/guidage.png");
    m_imgParcelle = loadImage("/gui/parcelle.png");
    BaseWidget::loadImages();
    
    m_img_option = loadImageInv("/gui/menu.png");
    
    m_img_volant = loadImageInv("/gui/volant.png");
    m_img_volant_green = loadImage("/gui/volant_green.png");
    m_img_volant_red = loadImage("/gui/volant_red.png");
#ifdef MANUAL_ENABLE
    m_img_volant_red = loadImageInv("/gui/volant.png");
#endif
    
    m_imgFleche = loadImage("/gui/fleche.png");
    m_excavator_2d = loadImage("/gui/excavator.png");
    
    m_img_right = loadImageInv("/gui/right.png");
    m_img_middle = loadImageInv("/gui/middle.png");
    m_img_left = loadImageInv("/gui/left.png");
    
    m_img_interrogation = loadImage("/gui/interrogation.png");
    m_img_balise = loadImage("/gui/balise.png", true);
    m_img_infos = loadImageInv("/gui/infos.png");
    
    m_img_offset = loadImageInv("/gui/offset.png");
    m_img_3_point_up = loadImageInv("/gui/3_point_up.png");
    m_img_3_point_down = loadImageInv("/gui/3_point_down.png");
    m_img_metre = loadImageInv("/gui/metre.png");
    m_img_compteur = loadImageInv("/gui/compteur.png");
}
    
void GpsWidget::setPainter(QPainter * p){
    BaseWidget::setPainter(p);
    for(auto p2 : m_widgets){
        p2->setPainter(p);
    }
    m_rapide_option_widget.setPainter(p);
    m_debug_widget.setPainter(p);
    m_key_board_widget.setPainter(p);
    
    m_first_widget.setPainter(p);
    m_diagnostic_widget.setPainter(p);
}


void GpsWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    
    m_widthMax = m_width/2-50;
    m_heightMax = m_height/2-50;
    
    for(auto p : m_widgets){
        p->setSize(m_width, m_height);
    }
    m_first_widget.setSize(m_width, m_height);
    m_key_board_widget.setSize(m_width, m_height);
    m_rapide_option_widget.setSize(m_width, m_height);
    m_diagnostic_widget.setSize(m_width, m_height);
    m_debug_widget.setSize(m_width, m_height);
   
    m_button_debug.setResize(40, 20, m_gros_button);
    int x_right = width-m_gros_button-20;
    int inter = m_gros_button*2.1;
    int y = m_gros_button*1.2+10;
    m_button_option.setResize(x_right, y, m_gros_button);
    y += inter*1.5;
    m_button_plus.setResize(x_right, y, m_gros_button);
    y += inter;
    m_button_moins.setResize(x_right, y, m_gros_button);
    //y += inter;
    
    y += inter*1.5;
    m_button_offset.setResize(x_right, y, m_gros_button);
    y += inter;
    m_button_balise.setResize(x_right, y, m_gros_button);
    y += inter;
    m_button_diag.setResize(x_right, y, m_gros_button);
    
    y += inter;
    //m_button_volant.setResize(x_right, height-m_gros_button*1.2-10, m_gros_button);
    
    int x = x_right;
    x -= inter;
    m_button_vitesse_plus.setResize(x-30, m_height-30, m_gros_button);
    x -= inter;
    m_button_play.setResize(x, m_height-30, m_gros_button);
    x -= inter;
    m_button_vitesse_moins.setResize(x, m_height-30, m_gros_button);
    m_button3d.setResize(m_width/2, m_height/2, m_gros_button);
    
    m_key_board_widget.setSize(width, height);
    m_first_widget.setSize(width, height);
    m_diagnostic_widget.setSize(width, height);
    m_option_widget.setSize(width, height);

    //m_infos_widget.setSize(width, height);
    m_param_translateur_widget.setSize(width, height);
}

void GpsWidget::drawButtons(){
    Framework & f = Framework::instance();
    
    if(m_is_develop){
        {
            int h = m_height-20;
            int w = m_gros_button*2+20;
            
            m_painter->setBrush(m_brush_background_2);
            m_painter->setPen(m_pen_no);
            
            m_painter->drawRoundedRect(m_width-w-10, 10, w, h, 10, 10);
            
        }
        
        drawButtonImageCarre(m_button_option, m_img_option, 0.4);
        drawButtonImageCarre(m_button_offset, m_img_offset, 0.3, (!m_rapide_option_widget.m_close && m_rapide_option_widget.m_page == 4), Langage::getKey("LOGO_OFFSET"));
        if(f.m_config.m_balise_enable){
            drawButtonImageCarre(m_button_balise, m_img_balise, 0.3,  !m_rapide_option_widget.m_close && m_rapide_option_widget.m_page == 3, Langage::getKey("LOGO_BALISE"));
        }
        drawButtonImageCarre(m_button_diag, m_img_infos, 0.3,  !m_rapide_option_widget.m_close && m_rapide_option_widget.m_page == 6, Langage::getKey("LOGO_INFOS"));
        
    }
    
    drawButtonImageCarre(m_button_plus, m_img_plus, 0.4);
    drawButtonImageCarre(m_button_moins, m_img_moins, 0.4);
    
    //TODO m_painter->setBrush(m_brushDarkGray);
    m_painter->setPen(m_pen_no);
    
    if(f.m_config.m_gps_serial == "file"){
        drawButtonImageCarre(m_button_vitesse_plus, m_img_plus, 0.4);
        drawButtonImageCarre(m_button_vitesse_moins, m_img_moins, 0.4);
        QString s = "v "+QString::number(f.m_fileModule.m_vitesse);
        drawQText(s, m_button_play.m_x, m_button_play.m_y, sizeText_medium, true);
    }
}

void GpsWidget::drawInfos(){
    Framework & f = Framework::instance();
    
    int h = 0.15*m_height;
    int y = m_height - h - 10;

    if(!m_is_develop){
        y = 0;
    }
    int y_top = y+h/2+10;
    
    double inter = 0.075*m_width;
    int x = 0.5*m_width+10;
    int x_q = -inter*1+x;
    int x_v =  inter*1+x;
    
    m_painter->setBrush(m_brush_background_2);
    m_painter->setPen(m_pen_no);
    
    m_painter->drawRoundedRect(0.35*m_width, y, 0.3*m_width+10, h, 10, 10);
    
    auto last_frame = f.m_position_module.m_last_gga;
    QPixmap * img = NULL;
    std::string s = "";
    if(f.isGpsConnected() && last_frame){
        if(last_frame->m_fix == 4){
            f.m_rtk = true;
        }
        
        if(last_frame->m_fix == 1 || last_frame->m_fix == 2 || last_frame->m_fix == 5 || last_frame->m_fix == 9){
            img = m_imgSatOrange;
        } else if(last_frame->m_fix == 4){
            img = m_imgSatVert;
        } else {
            img = m_imgSatBlanc;
        }
        
        
        if(last_frame->m_fix == 1){
            s = "GPS";
        } else if(last_frame->m_fix == 2){
            s = "DGPS";
        } else if(last_frame->m_fix == 3){
            s = "PPSFIX";
        } else if(last_frame->m_fix == 4){
            s = "RTK";
        } else if(last_frame->m_fix == 5){
            s = "FRTK";
        } else if(last_frame->m_fix == 9){
            s = "DGPS compas";
        } else {
            s = "invalid";
        }
    } else {
        if(f.m_gpgn_time.isConnected()){
            img = m_imgSatBlanc;
        } else {
            img = m_imgSatRouge;
        }
    }
    
    if(img){
        //int y = m_height*0.95;
        drawMyImage(*img, x_q, y_top-25, 2*0.3, true);
        drawText(s, x_q, y_top, sizeText_little, true, true);
    }
    
    
    {
        QString s = QString::number(f.m_vitesse, 'f', 1);
        drawQText(s, x_v, y_top-20, sizeText_little, true, true);
        drawQText("km/h", x_v, y_top, sizeText_little, true, true);
        
        drawMyImage(*m_img_compteur,x_v, y_top-21, 1.5*0.3, true);
    }
    
}

void GpsWidget::drawInfosBasLeft(){
    Framework & f = Framework::instance();
    
    m_painter->setBrush(m_brush_background_2);
    m_painter->setPen(m_pen_no);
    
    int y = m_height*0.66+15;
    int h = 0.34*m_height-25;
    int inter = 0.03*m_height;

    m_painter->drawRoundedRect(10, y, 0.3*m_width+10, h, 10, 10);
    
    int x1 = 50;
    y+=inter;
    y+=inter;
    std::string s = strprintf("altitude : %+.1f m", f.m_hauteur_current);
    drawText(s, x1, y, sizeText_little, false, true);
    y+=inter;
    s = strprintf("h : %+.0f cm", f.m_hauteur_diff*100);
    drawText(s, x1, y, sizeText_little, false, true);
    y+=inter;
    y+=inter;
    s = strprintf("dist : %+.1f m", f.m_distance_last_point);
    drawText(s, x1, y, sizeText_little, false, true);
    y+=inter;
    if(f.m_point_current){
        y+=inter;
        s = strprintf("lat : %+.8f", f.m_point_current->m_latitude);
        drawText(s, x1, y, sizeText_little, false, true);
        y+=inter;
        s = strprintf("long : %+.8f", f.m_point_current->m_longitude);
        drawText(s, x1, y, sizeText_little, false, true);
    }
    
}

void GpsWidget::drawInfosExcavator(){
    Framework & f = Framework::instance();
    
    m_painter->setBrush(m_brush_background_2);
    m_painter->setPen(m_pen_no);
    
    int x = m_width*0.66+15;
    int y = 10;
    int h = 0.66*m_height-15;
    int w = m_width*0.1;
    
    
    
    m_painter->drawRoundedRect(x, y, w, h, 10, 10);
    
    int y2 = y+h/2;
    int x2 = x+10;
    int w2 = w-20;
    int h2 = 40;
    int h3 = 20;
    int inter = 30;
    
    
    
    
    
    m_painter->setPen(m_pen_black);
    if(f.m_hauteur_save){
        double diff = f.m_hauteur_diff*100;
        if(diff > 10){
            m_painter->setBrush(m_brush_green);
        }
        if(diff < -10){
            m_painter->setBrush(m_brush_red);
        }
        m_painter->drawRoundedRect(x2, y2, w2, h2, 10, 10);
        
        std::string s = strprintf("%+.0f cm", f.m_hauteur_diff*100);
        drawText(s, x2+w2/2, y2+25, sizeText_little, true, true);
        
        y2 -=inter;
        if(diff > 10) m_painter->drawRoundedRect(x2, y2, w2, h3, 10, 10);
        y2 -=inter;
        if(diff > 20) m_painter->drawRoundedRect(x2, y2, w2, h3, 10, 10);
        y2 -=inter;
        if(diff > 30) m_painter->drawRoundedRect(x2, y2, w2, h3, 10, 10);
        y2 -=inter;
        if(diff > 40) m_painter->drawRoundedRect(x2, y2, w2, h3, 10, 10);
        
        y2 = y+h/2+20;
        
        y2 +=inter;
        if(diff < -10) m_painter->drawRoundedRect(x2, y2, w2, h3, 10, 10);
        y2 +=inter;
        if(diff < -20) m_painter->drawRoundedRect(x2, y2, w2, h3, 10, 10);
        y2 +=inter;
        if(diff < -30) m_painter->drawRoundedRect(x2, y2, w2, h3, 10, 10);
        y2 +=inter;
        if(diff < -40) m_painter->drawRoundedRect(x2, y2, w2, h3, 10, 10);
    } else {
        std::string s = strprintf("---", f.m_hauteur_diff*100);
        drawText(s, x2+w2/2, y2+25, sizeText_little, true, true);
        
        m_painter->drawRoundedRect(x2, y2, w2, h2, 10, 10);
    }
}

void GpsWidget::drawRightLeft(){
}

void GpsWidget::drawAlertes(){
    Framework & f = Framework::instance();
    
    f.setErrorWarning();
    
    double y = 0.4*m_height;
    int x = -RAYON_ROUNDED/2;
    if(m_is_develop){
        y = 0.68*m_height;
        x = 0.35*m_width;
    }
    int inter_x = m_gros_button*2.1;
    
    int i = 0;
    for(auto e : f.m_errors){
        std::string s = "";
        QString s2 = "";
        QPixmap * p = NULL;
        m_painter->setBrush(m_alert_warning);
        switch(e){
            case Error_TranslatorNotConnected:
            {
                m_painter->setBrush(m_alert_error);
                s = "Translator not connected";
                break;
            }
            case Error_GPSNotConnected:
            {
                m_painter->setBrush(m_alert_error);
                s = "GPS not connected";
                break;
            }
            case Error_GPSNotRTK:
            {
                m_painter->setBrush(m_alert_warning);
                 s = "RTK perdu";
                break;
            }
            case Warning_VitesseLow:
            {
                m_painter->setBrush(m_alert_warning);
                s = "vitesse faible";
                break;
            }
            case Warning_NotEngaged:
            {
                m_painter->setBrush(m_alert_warning);
                s = "non engagé";
                break;
            }
            case Warning_3PointHitch:
            {
                m_painter->setBrush(m_alert_warning);
                s = "3 point hitch";
                break;
            }
                
        }
        
        m_painter->setPen(m_pen_no);
        m_painter->drawRoundedRect(x, y, m_gros_button*4, m_gros_button*1.5, 10, 10);
        m_painter->setPen(m_pen_white);
        int y2 = y + m_gros_button*0.75;
        if(p){
            drawMyImage(*p, x+m_gros_button*0.8, y2, 0.2, true);
            drawText(s, x+m_gros_button*2.5,  y2, SizeText::sizeText_little, true);
        } else if(!s2.isEmpty()){
            drawText(s, x+m_gros_button*2,  y2-12, SizeText::sizeText_little, true);
            drawQText(s2, x+m_gros_button*2,  y2+12, SizeText::sizeText_little, true);
        } else {
            drawText(s, x+m_gros_button*2,  y2, SizeText::sizeText_little, true);
        }
        if(!m_is_develop){
            y+=inter_x;
        } else {
            x += m_gros_button*4.1;
            if(i>2){
                break;
            }
        }
        ++i;
    }
}

void GpsWidget::drawLicence(){
    int y2 = m_height*0.2;
    int x2 = m_width*0.2;
    int width2 = m_width*0.6;
    int height2 = m_height*0.6;
    
    m_painter->setPen(m_pen_black);
    m_painter->setBrush(m_brush_white);
    m_painter->drawRoundedRect(x2, y2, width2, height2, RAYON_ROUNDED, RAYON_ROUNDED);
    
    drawText(Langage::getKey("LICENCE_NOT_VALID"), 0.5*m_width, m_height*0.5, sizeText_medium, true);
}

void GpsWidget::draw(){
    Framework & f = Framework::instance();
    //myTime begin = myTimeInit();
    
    m_painter->setPen(m_pen_no);
    m_painter->setBrush(m_brush_background_1);
    m_painter->drawRect(0, 0, m_width, m_height);
    
    m_debug = f.m_config.isTechnicien();
    m_vue_3D = f.m_config.m_3d;
    
    m_widthMax = m_width/2+f.m_config.m_outil_largeur*m_zoom/2;
    m_heightMax = m_height/2+f.m_config.m_outil_largeur*m_zoom/2;
    
    if(f.m_tracteur.m_pt_antenne_corrige){
        m_xref = f.m_tracteur.m_pt_antenne_corrige->m_x;
        m_yref = f.m_tracteur.m_pt_antenne_corrige->m_y;
    }
    
    drawGpsWidget();
    
    drawButtons();
    drawInfos();
    drawInfosBasLeft();
    drawInfosExcavator();
    
    drawRightLeft();
    drawAlertes();
    
    
    for(auto p : m_widgets){
        if(!p->m_close){
            p->draw();
        }
    }
    if(!m_key_board_widget.m_close){
        m_key_board_widget.draw();
    }
    if(!m_rapide_option_widget.m_close){
        m_rapide_option_widget.draw();
    }
    if(!m_diagnostic_widget.m_close){
        m_diagnostic_widget.draw();
    }
    if(!m_debug_widget.m_close){
        m_debug_widget.draw();
    }
    if(false){
        drawLicence();
    }

    /*auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff2 = end - begin;
    f.m_draw_time.addNewValue(diff2.count()*1000);
    
#ifdef DEBUG_GUI
    m_painter->setBrush(m_brushGreen);
    m_painter->drawEllipse(m_last_x-2, m_last_y-2, 4, 4);
#endif*/
    
    if(!m_first_widget.m_close){
        m_first_widget.draw();
    }
    
    /*myTime end = myTimeInit();
    int millis = myTimeDurationMs(begin, end);
    INFO(millis);*/
    
}


void GpsWidget::openRapideWidget(int page){
   // Framework & f = Framework::instance();
    
    //m_infos_widget.m_close = true;
    if(m_rapide_option_widget.m_close || m_rapide_option_widget.m_page != page){
        m_rapide_option_widget.m_page = page;
        m_rapide_option_widget.open();
    } else {
        //m_option_widget.loadConfig();
        m_rapide_option_widget.m_close = true;
    }
}


int GpsWidget::onMouse(int x, int y){
    LOG_FUNCTION();
    
    if(!m_first_widget.m_close){
        m_first_widget.onMouse(x, y);
    }
    
    
    size_t n = m_widgets.size();
    for(size_t i = 0; i < n; ++i){
        auto p = m_widgets[n-i-1];
        if(!p->m_close){
            p->onMouse(x, y);
            return 0;
        }
    }
    if(!m_rapide_option_widget.m_close){
        m_rapide_option_widget.onMouse(x, y);
    }
    if(!m_diagnostic_widget.m_close){
        m_diagnostic_widget.onMouse(x, y);
    }
    if(!m_debug_widget.m_close){
        m_debug_widget.onMouse(x, y);
    }
    
    double x2 = x;
    double y2 = y;
    
    Framework & f = Framework::instance();
    if(m_button_plus.isActive(x2, y2)){
        m_zoom *= 1.2;
        m_zoom = std::round(m_zoom*10.0)/10.0;
    } else if(m_button_moins.isActive(x2, y2)){
        m_zoom /= 1.2;
        m_zoom = std::round(m_zoom*10.0)/10.0;
    }else if(m_button_option.isActive(x2, y2)){
        m_option_widget.open();
    } else if(m_button_diag.isActive(x2, y2)){
        openRapideWidget(6);
    } else if(m_button_debug.isActive(x2, y2)){
        m_debug_widget.m_close = false;
    } else if(m_button_play.isActive(x2, y2)){
        if(f.m_config.m_gps_serial == "file"){
            Framework::instance().m_fileModule.m_pause = !Framework::instance().m_fileModule.m_pause;
        }
    } else if(m_button_vitesse_plus.isActive(x2, y2)){
        if(f.m_config.m_gps_serial == "file"){
            Framework::instance().m_fileModule.vitessePlus();
        }
    } else if(m_button_vitesse_moins.isActive(x2, y2)){
        if(f.m_config.m_gps_serial == "file"){
            Framework::instance().m_fileModule.vitesseMoins();
        }
    } else if(m_button3d.isActive(x2, y2)){
        Framework::instance().m_config.m_3d = !Framework::instance().m_config.m_3d;
    } else if(m_button_offset.isActive(x, y)){
        openRapideWidget(4);
    } else if(m_button_balise.isActive(x, y)){
        if(f.m_config.m_balise_enable){
            openRapideWidget(3);
        }
    }
    
    return 0;
}


/**
 GPS
 */
double distanceAb  =0;
int max = 10000;

void GpsWidget::myProjete(double x, double y, double & x_res, double & y_res){
    double x1_temp = (x - m_xref)*m_zoom;
    double y1_temp = (y - m_yref)*m_zoom;
    
    double h00 = -m_sinA, h01 = -m_cosA;//, h02 = 1;
    double h10 = m_cosA, h11 = -m_sinA;// h12 = 1;
    //double h20 = -m_sinA, h21 = m_cosA, h22 = 1;
    
    double x_temp = (x1_temp*h00 + y1_temp*h01);
    double y_temp  = (x1_temp*h10  + y1_temp*h11);
    
    if(m_vue_3D){
        double a = 1;
        double b = 1.005;
        double res = ((a-1)*x_temp+(b-1)*y_temp+1);
        if(res<0){
            res = 0.0001;
        }
        x_res = a*x_temp/res;
        y_res  = b*y_temp/res;
    } else {
        x_res = x_temp;
        y_res = y_temp;
    }
}

void GpsWidget::myProjete2(double x, double y, double & x_res, double & y_res){
    double x_temp, y_temp;
    myProjete(x,y,x_temp, y_temp);

    if(m_is_develop){
        x_res = m_width*0.66/2 + x_temp;
        y_res = m_height*0.66*9/16 - y_temp;
    } else {
        x_res = m_width/2 + x_temp;
        y_res = m_height*9/16 - y_temp;
    }
}

void GpsWidget::myProjete2Pt(GpsPoint_ptr pt, double & x_res, double & y_res){
    myProjete2(pt->m_x, pt->m_y, x_res, y_res);
}

bool GpsWidget::mustBeDraw(double x, double y){
    return true;
    double x_temp = (x - m_xref);
    double y_temp = (y - m_yref);
    double res = x_temp*x_temp+y_temp*y_temp;
    
    double l = m_width/m_zoom*0.9;
    return res<l*l;
}


void GpsWidget::drawCurve(Curve_ptr l, QPen & pen){
    if(!l){
        return;
    }
    m_painter->setPen(pen);
    m_painter->setBrush(m_brush_no);
    
    bool first = true;
    GpsPoint_ptr old_point = nullptr;
    QPainterPath polygonPath;
    for(auto p :l->m_points_to_draw){
        double xB, yB;
        myProjete2(p->m_x, p->m_y, xB, yB);
        if(m_debug){
            m_painter->drawRect(xB-1, yB-1, 2, 2);
        }
        if(!first){
            polygonPath.lineTo(xB, yB);
        } else {
            polygonPath.moveTo(xB, yB);
        }
        first = false;
    }
    m_painter->drawPath(polygonPath);
}

void GpsWidget::drawLine2(GpsLine_ptr l, QPen & pen){
    if(!l){
        return;
    }
    m_painter->setPen(pen);
    m_painter->setBrush(m_brush_no);
    
    GpsPoint_ptr old_point = nullptr;
    
    Framework & f = Framework::instance();
    
    l->calculProjete2(m_xref, m_yref);
    double xH, yH;
    myProjete2(l->m_proj_x-f.m_lineAB.m_x_ab*0, l->m_proj_y-f.m_lineAB.m_y_ab*0, xH, yH);
    
    
    QPainterPath polygonPath;
    polygonPath.moveTo(xH, yH);
    
    for(int i = 1; i < 10; ++i){
        double xB, yB;
        myProjete2(l->m_proj_x+i*f.m_lineAB.m_x_ab*1, l->m_proj_y+i*f.m_lineAB.m_y_ab*1, xB, yB);
        polygonPath.lineTo(xB, yB);
    }
    for(int i = 1; i < 10; ++i){
        double xB, yB;
        myProjete2(l->m_proj_x+i*f.m_lineAB.m_x_ab*11, l->m_proj_y+i*f.m_lineAB.m_y_ab*11, xB, yB);
        polygonPath.lineTo(xB, yB);
    }
    m_painter->drawPath(polygonPath);
   
    QPainterPath polygonPath2;
    polygonPath2.moveTo(xH, yH);
    for(int i = 1; i < 10; ++i){
        double xB, yB;
        myProjete2(l->m_proj_x-i*f.m_lineAB.m_x_ab*1, l->m_proj_y-i*f.m_lineAB.m_y_ab*1, xB, yB);
        polygonPath2.lineTo(xB, yB);
    }
    for(int i = 1; i < 10; ++i){
        double xB, yB;
        myProjete2(l->m_proj_x-i*f.m_lineAB.m_x_ab*11, l->m_proj_y-i*f.m_lineAB.m_y_ab*11, xB, yB);
        polygonPath2.lineTo(xB, yB);
    }
    m_painter->drawPath(polygonPath2);
}

void GpsWidget::drawPoint(GpsPoint & pt, std::string s){
    double x, y;
    myProjete2(pt.m_x, pt.m_y, x, y);
    m_painter->setBrush(m_brush_red);
    m_painter->setPen(m_pen_red);
    m_painter->drawEllipse(x-3, y-3, 6, 6);
    drawText(s, x, y-15, sizeText_medium, true);
}

void GpsWidget::drawPoint_l(GpsPoint & pt, std::string s){
    double x, y;
    myProjete2(pt.m_x, pt.m_y, x, y);
    m_painter->setBrush(m_brush_blue);
    m_painter->setPen(m_pen_blue);
    m_painter->drawEllipse(x-1, y-1, 2, 2);
    drawText(s, x, y-12, sizeText_little, true);
}

void GpsWidget::drawLineCurve(){
    Framework & f = Framework::instance();
    
    if(f.getEtat() == Etat_OK){
        if(f.m_etat_ab == EtatAB_Line){
            auto list = f.m_lineAB.getCurrentLine();
            drawLine2(list, m_pen_black);
            for(int i = 1; i<2; ++i){
                auto list2 = f.m_lineAB.getCurrentLineRel(i);
                drawLine2(list2, m_pen_gray);
                auto list3 = f.m_lineAB.getCurrentLineRel(-i);
                drawLine2(list3, m_pen_gray);
            }
            if(f.m_demitour_next_i != 8888){
                auto list4 = f.m_lineAB.getCurrentLineI(f.m_demitour_next_i);
                drawLine2(list4, m_pen_blue);
            }
            drawPoint(f.m_lineAB.m_pointA, "A");
            drawPoint(f.m_lineAB.m_pointB, "B");
        } else {
            auto list = f.m_curveAB.getCurrentLine();
            drawCurve(list, m_pen_black);
            for(int i = 1; i<2; ++i){
                auto list2 = f.m_curveAB.getCurrentLineRel(i);
                drawCurve(list2, m_pen_gray);
                auto list3 = f.m_curveAB.getCurrentLineRel(-i);
                drawCurve(list3, m_pen_gray);
            }
            drawPoint(f.m_curveAB.m_pointA, "A");
            drawPoint(f.m_curveAB.m_pointB, "B");
        }
    }
}

void GpsWidget::drawParcelle(bool force){
}

void GpsWidget::drawSurfaceToDraw(){
    Framework & f = Framework::instance();
    
    m_painter->setBrush(m_brush_parcelle_green_alpha);
    m_painter->setPen(m_pen_no);
    for(auto s: f.m_listSurfaceToDraw){
        if(s->m_points.size() > 0 && s->m_lastPoint && s->m_lastPoint->m_point_left && s->m_lastPoint->m_point_right){
            double xA1 = 0, yA1 = 0, xB1 = 0, yB1 = 0;
            myProjete2(s->m_lastPoint->m_point_left->m_x, s->m_lastPoint->m_point_left->m_y, xA1, yA1);
            myProjete2(s->m_lastPoint->m_point_right->m_x, s->m_lastPoint->m_point_right->m_y, xB1, yB1);
            
            
            int j = 0;
            int init = 1;
            
            for(auto it = s->m_points.begin(); it != s->m_points.end(); ++it){
                auto frame = (*it);
                
                
                if(!mustBeDraw(frame->m_point_center->m_x, frame->m_point_center->m_y)){
                    init = 0;
                    //x1 = x0;
                    //y1 = y0;
                    continue;
                } else {
                    double xA = 0, yA = 0, xB = 0, yB = 0;
                    myProjete2(frame->m_point_left->m_x, frame->m_point_left->m_y, xA, yA);
                    myProjete2(frame->m_point_right->m_x, frame->m_point_right->m_y, xB, yB);
                    if(init != 0){
                        QPointF points[4] = {
                            QPointF(xA, yA),
                            QPointF(xB, yB),
                            QPointF(xB1, yB1),
                            QPointF(xA1, yA1)};
                        m_painter->drawPolygon(points, 4);
                    }
                    init = 1;
                    xA1 = xA, yA1 = yA, xB1 = xB, yB1 = yB;
                    
                }
                
                j ++;
            }
        }
    }
}

void GpsWidget::drawBackgroundGps(){
    double begin_ref_x = round(m_xref/20.0)*20;
    double begin_ref_y = round(m_yref/20.0)*20;
    
    m_painter->setPen(m_pen_no);
    
    int nb = -1;
    if(m_zoom > 4){
        nb = 10;
        if(m_zoom > 10){
            nb = 5;
            if(m_zoom > 20){
                nb = 4;
            }
        }
    }
    nb=10;
    for(int j = -nb; j < nb+1; ++j){
        for(int i = -nb; i < nb+1; i+=1){
            if((i+j)%2==0){
                m_painter->setBrush(m_brush_parcelle_1);
                
            } else {
                m_painter->setBrush(m_brush_parcelle_2);
                
            }
            double x0 = begin_ref_x+i*10;
            double y0 = begin_ref_y+j*10;
            double x02, y02;
            myProjete2(x0, y0, x02, y02);
            double x1 = begin_ref_x+(i+1)*(10);
            double y1 = begin_ref_y+j*10;
            double x12, y12;
            myProjete2(x1, y1, x12, y12);
            double x3 = begin_ref_x+(i+1)*(10);
            double y3 = begin_ref_y+(j+1)*(10);
            double x32, y32;
            myProjete2(x3, y3, x32, y32);
            double x4 = begin_ref_x+i*(10);
            double y4 = begin_ref_y+(j+1)*(10);
            double x42, y42;
            myProjete2(x4, y4, x42, y42);
            
            QPointF points[4] = {
                QPointF(x02, y02),
                QPointF(x12, y12),
                QPointF(x32, y32),
                QPointF(x42, y42)};
            m_painter->drawPolygon(points, 4);
        }
    }
}

void GpsWidget::drawGpsWidget(){
    Framework & f = Framework::instance();
    
    m_a = -3.14/2+f.m_cap_rad;
    
    m_cosA = cos(m_a);
    m_sinA = sin(m_a);
    
    QPainterPath clipPath;
    clipPath.addRoundedRect(10, 10, m_width*0.66-15, m_height*0.66-15, 10, 10);
    m_painter->setClipPath(clipPath);
    
    drawBackgroundGps();
    
    //scene->clear();
    
    if(f.getEtat() == Etat_Points){
        m_painter->setPen(m_pen_red); // personnaliser
        //double x_last = 0, y_last = 0;
        
        m_painter->setPen(m_pen_black); // personnaliser
        for(auto p: f.m_position_module.m_list_after_ekf){
            double x1, y1;
            myProjete2(p->m_x, p->m_y, x1, y1);
            m_painter->drawPoint(x1, y1);
        }
        
        drawBalises();
        drawCercles();
    } else {
        drawParcelle();
        if(f.getEtat() == Etat_PointASaved){
            drawPoint(f.m_lineAB.m_pointA, "A");
        }
        drawLineCurve();
        
        //drawSurfaceToDraw();
        
        drawExcavator();
        
        drawDebugEkf();
        
        if(m_vue_3D){
            m_painter->setBrush(QColor(135,206,235));
            m_painter->setPen(m_pen_no);
            QPolygonF points;
            points.append(QPointF(0, 0));
            points.append(QPointF(m_width, 0));
            points.append(QPointF(m_width, m_height*0.25));
            points.append(QPointF(m_width/2, m_height*0.20));
            points.append(QPointF(0, m_height*0.25));
            
            m_painter->drawPolygon(points);
            m_painter->drawRect(0,0, m_width, m_height/6);
        }
        
        
    }
    
    m_painter->setClipRect(0, 0, m_width, m_height);
}

void GpsWidget::drawDebugEkf(){
    Framework & f = Framework::instance();
    if(m_debug){
        
        
        m_painter->setPen(m_pen_red); // personnaliser
        double x_last = 0, y_last = 0;
        for(auto p: f.m_position_module.m_list_before_ekf){
            double x1, y1;
            myProjete2(p->m_x, p->m_y, x1, y1);
            if(x_last != 0){
                m_painter->drawLine(x1, y1, x_last, y_last);
            }
            x_last = x1;
            y_last = y1;
            
        }
        x_last = 0;
        m_painter->setPen(m_pen_green); // personnaliser
        for(auto p: f.m_position_module.m_list_after_ekf){
            double x1, y1;
            myProjete2(p->m_x, p->m_y, x1, y1);
            if(x_last != 0){
                m_painter->drawLine(x1, y1, x_last, y_last);
            }
            x_last = x1;
            y_last = y1;
            
        }
        
        /*m_painter->setPen( QPen(Qt::black, 2) ); // personnaliser
        
        for(auto p: f.m_position_module.m_list){
            double x1, y1;
            
            myProjete2(p->m_x, p->m_y, x1, y1);
            m_painter->drawPoint(x1, y1);
        }*/
    }
    
    m_painter->setPen(m_pen_red); // personnaliser
    double x_last = 0, y_last = 0;
    int i = 0;
    for(auto p: f.m_auto_path_module.m_paths){
        double x1, y1;
        myProjete2(p->m_x, p->m_y, x1, y1);
        if(x_last != 0){
            m_painter->drawLine(x1, y1, x_last, y_last);
        }
        if(i>10){
            double dx = x1-x_last;
            double dy = y1-y_last;
            double d = std::sqrt(dx*dx+dy*dy);
            
            dy = dy/d*10;
            dx = dx/d*10;
            
            int x2 = x1-dx-dy;
            int y2 = y1-dy-dx;
            m_painter->drawLine(x1, y1, x2, y2);
            
            x2 = x1-dx+dy;
            y2 = y1-dy+dx;
            m_painter->drawLine(x1, y1, x2, y2);
            
            i = 0;
        }
        ++i;
        x_last = x1;
        y_last = y1;
        
    }
    
    auto p1 = f.m_auto_path_module.getPointBefore();
    if(p1){
        double x1, y1;
        myProjete2(p1->m_x, p1->m_y, x1, y1);
        m_painter->drawEllipse(x1-2, y1-2, 4, 4);
        
    }
    
    auto p2 = f.m_auto_path_module.getPointAfter();
    if(p2){
        double x1, y1;
        myProjete2(p2->m_x, p2->m_y, x1, y1);
        m_painter->drawEllipse(x1-4, y1-4, 8, 8);
        
    }
}


double cosa = 0;
double sina = 0;
void GpsWidget::drawRect3D(double x, double y, double l, double lg){
    
    double x1 = x - sina*lg/2;
    double y1 = y + cosa*lg/2;
    myProjete2(x1, y1, x1, y1);
    double x2 = x + sina*lg/2;;
    double y2 = y - cosa*lg/2;;
    myProjete2(x2, y2, x2, y2);
    double x_outil2 = x + cosa*l;
    double y_outil2 = y + sina*l;
    double x3 = x_outil2 - sina*lg/2;
    double y3 = y_outil2 + cosa*lg/2;
    myProjete2(x3, y3, x3, y3);
    double x4 = x_outil2 + sina*lg/2;;
    double y4 = y_outil2 - cosa*lg/2;;
    myProjete2(x4, y4, x4, y4);
    QPolygonF points;
    points.append(QPointF(x3, y3));
    points.append(QPointF(x4, y4));
    points.append(QPointF(x2, y2));
    points.append(QPointF(x1, y1));
    
    m_painter->drawPolygon(points);
}



void GpsWidget::drawCercles(){
    m_painter->setBrush(m_brush_no);
    m_painter->setPen(m_pen_black);
   
    Framework & f = Framework::instance();
    double x_cercle = m_width/2, y_cercle = m_height/2;
    if(f.m_tracteur.m_pt_antenne_corrige){
        myProjete2(f.m_tracteur.m_pt_antenne_corrige->m_x, f.m_tracteur.m_pt_antenne_corrige->m_y, x_cercle, y_cercle);
    }
    
    double l = 1.0*m_zoom;
    m_painter->drawEllipse(x_cercle-l, y_cercle-l, 2*l, 2*l);
    if(m_zoom > 20){
        m_painter->drawText(x_cercle, y_cercle-l, "1 m");
    }
    l = 10.0*m_zoom;
    m_painter->drawEllipse(x_cercle-l, y_cercle-l, 2*l, 2*l);
    m_painter->drawText(x_cercle, y_cercle-l, "10 m");
    
    if(m_zoom > 50){
        l = 0.1*m_zoom;
        m_painter->drawEllipse(x_cercle-l, y_cercle-l, 2*l, 2*l);
        m_painter->drawText(x_cercle, y_cercle-l, "10 cm");
    }
    if(m_zoom > 100){
        l = 0.05*m_zoom;
        m_painter->drawEllipse(x_cercle-l, y_cercle-l, 2*l, 2*l);
        m_painter->drawText(x_cercle, y_cercle-l, "5 cm");
    }
}

void GpsWidget::drawExcavator(){
    Framework & f = Framework::instance();
    
    double x_tracteur = m_width/2, y_tracteur = m_height/2;
    if(f.m_tracteur.m_pt_antenne_corrige){
        myProjete2(f.m_tracteur.m_pt_antenne_corrige->m_x, f.m_tracteur.m_pt_antenne_corrige->m_y, x_tracteur, y_tracteur);
    }
    
    int h = 4*m_zoom;
    int w = 2*m_zoom;
    
    m_painter->drawPixmap(x_tracteur-w/2, y_tracteur-h*0.55, w, h, *m_excavator_2d);
}

void GpsWidget::drawBalises(){
    Framework & f = Framework::instance();
    
    m_painter->setPen(m_pen_black);
    m_painter->setBrush(m_brush_red);
    for(auto p: f.m_balises.m_balises){
        double x1, y1;
        myProjete2(p->m_x, p->m_y, x1, y1);
        double l = 0.3*m_zoom/2;
        m_painter->drawEllipse(x1-l, y1-l, 2*l, 2*l);
        if(!p->m_name.empty()){
            drawText(p->m_name, x1, y1-l*1.5, sizeText_little, true);
        }
    }
}

