#include "option_widget.hpp"
#include "../util/directory_manager.hpp"
#include <sstream>
#include <fstream>
#include <string>

#include "../framework.hpp"
#include "qt/main_window.hpp"
#include "gps_widget.hpp"
#include "../config/langage.hpp"
//#include <QMediaPlayer>


#include <iostream>
#include <fstream>
#include <sstream>

OptionWidget::OptionWidget(){
    
    m_close=true;
    //m_page =5;
    //addSerials();
}


void OptionWidget::loadImages(){
    BaseWidget::loadImages();
    
    m_img_outil = loadImageInv("/gui/outil_blanc.png", true);
    m_img_hydraulic = loadImageInv("/gui/hydraulic.png");
    m_img_cadena = loadImageInv("/gui/cadena.png", true);
    m_img_settings1 = loadImageInv("/gui/settings1.png", true);
    m_img_settings_key = loadImageInv("/gui/settings_key.png", true);
    m_imgSatBlanc = loadImageInv("/gui/sat_blanc.png", true);
    
    m_img_off = loadImageInv("/gui/off.png", true);
    m_img_plus = loadImage("/gui/plus.png");
    m_img_moins = loadImage("/gui/moins.png");
    m_imgVolantBlanc = loadImage("/gui/volant_blanc2.png");
    m_imgOutilBlanc = loadImage("/gui/outil_blanc.png");
    m_imgOptionBlanc = loadImage("/gui/option_blanc.png");
    m_imgParcelle = loadImage("/gui/parcelle.png");
    m_img_tracteur = loadImage("/gui/tracteur.png");
     
    m_imgLeft = loadImage("/gui/left.png");
    m_imgRight = loadImage("/gui/right.png");
    m_imgMiddle = loadImage("/gui/middle.png");
    
    m_imgLeft2 = loadImage("/gui/left2.png");
    m_imgRight2 = loadImage("/gui/right2.png");
    
    m_img_check_on = loadImage("/gui/check_on.png");
    m_img_check_off = loadImage("/gui/check_off.png");
    
    m_img_return = loadImageInv("/gui/return.png");
}

void OptionWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    m_y2 = m_height*0.04;
    m_x2 = m_y2;
    m_width2 = m_width-2*m_y2-m_gros_button*2.4;
    m_height2 = m_height-2*m_y2;

    m_y_title = m_y2+m_height*0.08;
    m_y_inter = 0.08*m_height2;
    m_y_begin = m_y_title+4*m_y_inter;
    
    m_width3 = m_width2+m_y2+m_y2;
    m_part_1_x = m_width3*0.04;
    m_part_1_w = m_width3*0.44;
    m_part_1_m = m_part_1_x+0.5*m_width3*0.44;
    m_part_1_x2 = m_part_1_x+0.1*m_width3*0.44;
    m_part_1_x3 = m_part_1_x+0.8*m_width3*0.44;
    m_part_2_x = m_width3*0.52;
    m_part_2_w = m_width3*0.44;
    m_part_2_m = m_part_2_x+0.5*m_width3*0.44;
    m_part_2_x2 = m_part_2_x+0.1*m_width3*0.44;
    m_part_2_x3 = m_part_2_x+0.8*m_width3*0.44;
    
    
    int x_right = width-m_gros_button-20;
    int inter = m_gros_button*2.1;
    int y = m_gros_button*1.2+10;
    m_button_close.setResize(x_right, y, m_gros_button);
    y += 1.5*inter;
    m_button_p1.setResize(x_right, y, m_gros_button);
    y += inter;
    m_button_p2.setResize(x_right, y, m_gros_button);
    y += inter;
    m_button_p3.setResize(x_right, y, m_gros_button);
    y += inter;
    m_button_p4.setResize(x_right, y, m_gros_button);
    y += inter;
    m_button_p5.setResize(x_right, y, m_gros_button);
    y += inter;
    m_button_p6.setResize(x_right, y, m_gros_button);
    y += inter;
    m_button_p7.setResize(x_right, y, m_gros_button);
    y += inter;
    m_button_p8.setResize(x_right, y, m_gros_button);
    y += inter;
    m_button_p9.setResize(x_right, y, m_gros_button);
    
    m_button_return.setResize(x_right, m_height-m_gros_button*1.2-10, m_gros_button);
    
    setSizePage1();
    setSizePage2();
    setSizePage3();
    setSizePage4();
    setSizePage5();
    setSizePage6();
    setSizePage7();
    
    m_select_widget.setSize(width, height);
}


void OptionWidget::drawButtons(){
    Config & config = Framework::instance().m_config;

    {
        int h = m_height-20;
        int w = m_gros_button*2+20;
        
        m_painter->setBrush(m_brush_background_2);
        m_painter->setPen(m_pen_no);
        
        m_painter->drawRoundedRect(m_width-w-10, 10, w, h, 10, 10);
        
    }
    
    drawButtonImageCarre(m_button_return, m_img_return, 0.3, false, Langage::getKey("LOGO_RETURN"));
    
    drawButtonImageCarre(m_button_p1, m_img_settings1, 0.3, (m_page == 1));
    drawButtonImageCarre(m_button_p2, m_img_outil, 0.5, (m_page == 2));
    drawButtonImageCarre(m_button_p3, m_img_hydraulic, 0.35, (m_page == 3));
    drawButtonImageCarre(m_button_p4, m_img_settings_key, 0.3, (m_page == 4));
    drawButtonImageCarre(m_button_p5, m_img_cadena, 0.5, (m_page == 5));
    drawButtonImageCarre(m_button_close, m_img_off, 0.3, false, Langage::getKey("LOGO_CLOSE"));
    
    if(config.isTechnicien()){
        drawButtonImageCarre(m_button_p6, m_img_settings_key, 0.3, (m_page == 6));
        drawButtonImageCarre(m_button_p7, m_imgSatBlanc, 0.6, (m_page == 7));
        
    }
}
void OptionWidget::draw(){
    Config & config = Framework::instance().m_config;

    m_painter->setPen(m_pen_no);
    m_painter->setBrush(m_brush_background_1);
    m_painter->drawRect(0 , 0, m_width, m_height);
    
    drawButtons();
    
    if(m_page == 1){
        drawPage1();
    } else if(m_page == 2){
        drawPage2();
    } else if(m_page == 3){
        drawPage3();
    } else if(m_page == 4){
        drawPage4();
    } else if(m_page == 5){
        drawPage5();
    } else if(m_page == 6){
        if(config.isTechnicien()){
            drawPage6();
        }
    } else if(m_page == 7){
        if(config.isDeveloppeur()){
            drawPage7();
        }
    }
    if(!m_select_widget.m_close){
        m_select_widget.draw();
    }
}

int OptionWidget::onMouse(int x, int y){
    Framework & f = Framework::instance();
    Config & config = f.m_config;
    
    if(m_button_return.isActive(x,y)){
        m_close = true;
    }
    if(m_button_close.isActive(x, y)){
        exit(0);
    }
    
    if(m_button_p1.isActive(x,y)){
        m_page = 1;
    } else if(m_button_p2.isActive(x,y)){
        m_page = 2;
    } else if(m_button_p3.isActive(x,y)){
        m_page = 3;
    } else if(m_button_p4.isActive(x,y)){
        m_page = 4;
    } else if(m_button_p5.isActive(x,y)){
        m_page = 5;
    } else if(m_button_p6.isActive(x,y)){
        if(config.isTechnicien()){
            m_page = 6;
        }
    } else if(m_button_p7.isActive(x,y)){
        if(config.isTechnicien()){m_page = 7;};
    } else if(m_button_p8.isActive(x,y)){
        if(config.isTechnicien()){m_page = 8;};
    } else if(m_button_p9.isActive(x,y)){
        if(config.isTechnicien()){m_page = 9;};
    } else {
        if(m_page == 1){
            onMousePage1(x, y);
        } else if(m_page == 2){
            onMousePage2(x, y);
        } else if(m_page == 3){
            onMousePage3(x, y);
        } else if(m_page == 4){
            onMousePage4(x, y);
        } else if(m_page == 5){
            onMousePage5(x, y);
        } else if(m_page == 6){
            onMousePage6(x, y);
        } else if(m_page == 7){
            onMousePage7(x, y);
        }
    }
    return 0;
}


/**
 PAGE 1
 */

void OptionWidget::setSizePage1(){
    int y = m_y_begin;
    m_lum0.setResize(m_width3*(0.04+0.2*0.44), y, m_petit_button);
    m_lum1.setResize(m_width3*(0.04+0.4*0.44), y, m_petit_button);
    m_lum2.setResize(m_width3*(0.04+0.6*0.44), y, m_petit_button);
    m_lum3.setResize(m_width3*(0.04+0.8*0.44), y, m_petit_button);
    y+= m_y_inter;
    y+= m_y_inter;
    
    y += m_y_inter;
   
    
    
    y = m_y_begin;
    m_son0.setResize(m_width3*(0.52+0.44*0.2), y, m_petit_button);
    m_son1.setResize(m_width3*(0.52+0.44*0.4), y, m_petit_button);
    m_son2.setResize(m_width3*(0.52+0.44*0.6), y, m_petit_button);
    m_son3.setResize(m_width3*(0.52+0.44*0.8), y, m_petit_button);
    
    
    y += m_y_inter;
    y += m_y_inter;
    y += m_y_inter;
    m_company.setResize(m_part_2_x+m_part_2_w/2, y);
    
};

void OptionWidget::drawPage1(){
    Framework & f = Framework::instance();
    Config & config = f.m_config;
    
    drawText(Langage::getKey("OPT_GEN_TITLE"), 0.45*m_width, m_y_title, sizeText_bigbig, true);
    
    drawPart1Title(m_lum1.m_y-2*m_y_inter, m_y_inter*4, Langage::getKey("OPT_LUM"));
    m_painter->setPen(m_pen_no);
    m_painter->setBrush(QBrush(QColor(115, 115, 115)));
    double r = m_lum1.m_rayon*0.8;
    m_painter->drawRect(m_lum0.m_x, m_lum0.m_y-5, m_lum3.m_x-m_lum0.m_x, 10);
    m_painter->drawEllipse(m_lum0.m_x-r, m_lum0.m_y-r, 2*r, 2*r);
    m_painter->drawEllipse(m_lum1.m_x-r, m_lum1.m_y-r, 2*r, 2*r);
    m_painter->drawEllipse(m_lum2.m_x-r, m_lum2.m_y-r, 2*r, 2*r);
    m_painter->drawEllipse(m_lum3.m_x-r, m_lum3.m_y-r, 2*r, 2*r);
    m_painter->setPen(m_pen_black_inv);
    
    r = m_lum1.m_rayon*0.5;
    m_painter->setBrush(m_brush_white);
    if(config.m_luminosite == 0){
        m_painter->drawEllipse(m_lum0.m_x-r-1, m_lum0.m_y-r-1, 2*r, 2*r);
    } else if(config.m_luminosite == 1){
        m_painter->drawEllipse(m_lum1.m_x-r-1, m_lum1.m_y-r-1, 2*r, 2*r);
    } else if(config.m_luminosite == 2){
        m_painter->drawEllipse(m_lum2.m_x-r-1, m_lum2.m_y-r-1, 2*r, 2*r);
    } else if(config.m_luminosite == 3){
        m_painter->drawEllipse(m_lum3.m_x-r-1, m_lum3.m_y-r-1, 2*r, 2*r);
    }
    
    m_painter->setPen(m_pen_black_inv);
    
    
    
    drawPart2Title(m_son1.m_y-2*m_y_inter, m_y_inter*6, Langage::getKey("OPT_SON"));
    m_painter->setBrush(QBrush(QColor(115, 115, 115)));
    m_painter->setPen(m_pen_no);
    r = m_son1.m_rayon*0.8;
    m_painter->drawRect(m_son0.m_x, m_son3.m_y-5, m_son3.m_x-m_son0.m_x, 10);
    m_painter->drawEllipse(m_son0.m_x-r, m_son0.m_y-r, 2*r, 2*r);
    m_painter->drawEllipse(m_son1.m_x-r, m_son1.m_y-r, 2*r, 2*r);
    m_painter->drawEllipse(m_son2.m_x-r, m_son2.m_y-r, 2*r, 2*r);
    m_painter->drawEllipse(m_son3.m_x-r, m_son3.m_y-r, 2*r, 2*r);
    
    r = m_lum1.m_rayon*0.5;
    m_painter->setBrush(m_brush_white);
    if(config.m_son == 0){
        m_painter->drawEllipse(m_son0.m_x-r-1, m_son0.m_y-r-1, 2*r, 2*r);
    } else if(config.m_son == 1){
        m_painter->drawEllipse(m_son1.m_x-r-1, m_son1.m_y-r-1, 2*r, 2*r);
    } else if(config.m_son == 2){
        m_painter->drawEllipse(m_son2.m_x-r-1, m_son2.m_y-r-1, 2*r, 2*r);
    } else if(config.m_son == 3){
        m_painter->drawEllipse(m_son3.m_x-r-1, m_son3.m_y-r-1, 2*r, 2*r);
    }
    m_painter->setPen(m_pen_black);
    
    
    drawPart1Title(m_company.m_y-2*m_y_inter, m_y_inter*4, "");
    drawPart2Title(m_company.m_y-2*m_y_inter, m_y_inter*3, "companie");
    m_company.m_text = f.m_config.m_company;
    
    drawValueGuiKeyBoard(m_company);
    
    
}

void OptionWidget::onMousePage1(int x, int y){
    Framework & f = Framework::instance();
    Config & config = f.m_config;
    
    
    if(!m_key_board_widget->m_close){
        if(m_key_board_widget->onMouse(x, y)){
            f.m_config.m_company = m_company.m_text;
            f.initOrLoadConfig();
        };
    }
    
    if(m_lum0.isActive(x, y)){
        config.m_luminosite = 0;
        execute2("xrandr --output eDP-1 --brightness 0.25");
    }
    if(m_lum1.isActive(x, y)){
        config.m_luminosite = 1;
        execute2("xrandr --output eDP-1 --brightness 0.5");
    }
    if(m_lum2.isActive(x, y)){
        config.m_luminosite = 2;
        execute2("xrandr --output eDP-1 --brightness 0.75");
    }
    if(m_lum3.isActive(x, y)){
        config.m_luminosite = 3;
        execute2("xrandr --output eDP-1 --brightness 1.0");
    }
    
    if(m_son0.isActive(x, y)){
        config.m_son = 0;
        f.initOrLoadConfig();
    }
    if(m_son1.isActive(x, y)){
        config.m_son = 1;
        f.initOrLoadConfig();
    }
    if(m_son2.isActive(x, y)){
        config.m_son = 2;
        f.initOrLoadConfig();
    }
    if(m_son3.isActive(x, y)){
        config.m_son = 3;
        f.initOrLoadConfig();
    }
    
    if(isActiveValueGuiKeyBoard(m_company,x,y)){
        m_key_board_widget->m_close = false;
        m_key_board_widget->setValueGuiKeyBoard(&m_company);
    }
    
}

/**
 PAGE 2
 */
void OptionWidget::setSizePage2(){
    int y = m_y_begin;
    m_outil_l.setResize(m_part_1_x3, y, m_petit_button);
    y+= m_y_inter;
    //y+= m_y_inter;
    //m_outil_lg.setResize(m_part_1_x3, y, m_petit_button);
    y+= m_y_inter;
    m_outil_record_h.setResize(m_part_1_x3, y, m_petit_button);
    y+= m_y_inter;
    m_outil_replay_h.setResize(m_part_1_x3, y, m_petit_button);
}
void OptionWidget::drawPage2(){
    Framework & f = Framework::instance();
    Config & config = f.m_config;
    
    drawText(Langage::getKey("OPT_OUTIL"), 0.45*m_width, m_y_title, sizeText_bigbig, true);
    
    drawPart1Title(m_outil_l.m_y-2*m_y_inter, m_y_inter*4, Langage::getKey("OPT_OUTIL"));
    {
        m_outil_l.m_value = config.m_outil_largeur;
        drawText(Langage::getKey("OPT_OUTIL_L"), m_part_1_x2,m_outil_l.m_y, sizeText_medium);
        drawValueGuiKeyPad2(m_outil_l);
    }
    /*{
        m_outil_lg.m_value = config.m_outil_distance;
        drawText(Langage::getKey("OPT_OUTIL_LG"), m_part_1_x2,m_outil_lg.m_y, sizeText_medium);
        drawValueGuiKeyPad2(m_outil_lg);
    }*/
    {
        m_outil_record_h.m_value = config.m_outil_record_h;
        drawText(Langage::getKey("OPT_OUTIL_RECORD_H"), m_part_1_x2,m_outil_record_h.m_y, sizeText_medium);
        drawValueGuiKeyPad2(m_outil_record_h);
    }
    {
        m_outil_replay_h.m_value = config.m_outil_replay_h;
        drawText(Langage::getKey("OPT_OUTIL_REPLAY_H"), m_part_1_x2,m_outil_replay_h.m_y, sizeText_medium);
        drawValueGuiKeyPad2(m_outil_replay_h);
    }
}
void OptionWidget::onMousePage2(int x, int y){
    Config & config = Framework::instance().m_config;
    if(onMouseKeyPad2(m_outil_l, x, y, 0.05)){
        config.m_outil_largeur = m_outil_l.m_value;
        loadConfig();
    };
    if(onMouseKeyPad2(m_outil_record_h, x, y, 0.05)){
        config.m_outil_record_h = m_outil_record_h.m_value;
        loadConfig();
    };
    if(onMouseKeyPad2(m_outil_replay_h, x, y, 0.05)){
        config.m_outil_replay_h = m_outil_replay_h.m_value;
        loadConfig();
    };
}



/**
 PAGE 3
 */

void OptionWidget::setSizePage3(){
    int y = m_y_begin;
    m_command_capteur_vitesse_max_mm_s.setResize(m_part_1_x3, y, m_petit_button);
    y+= m_y_inter;
    m_command_capteur_p.setResize(m_part_1_x3, y, m_petit_button);
    
    
    y = m_y_begin;
    m_btnTempLeft.setResize(m_part_2_x+0.3*m_part_2_w, y, m_petit_button);
    m_btnTempRight.setResize(m_part_2_x+0.7*m_part_2_w, y, m_petit_button);
    y+= m_y_inter;
    y+= m_y_inter;
    y+= m_y_inter;
    m_btnCapteurLeft.setResize(m_part_2_x+0.3*m_part_2_w, y, m_petit_button);
    m_btnCapteurZero.setResize(m_part_2_x+0.5*m_part_2_w, y, m_petit_button);
    m_btnCapteurRight.setResize(m_part_2_x+0.7*m_part_2_w, y, m_petit_button);
    y+= m_y_inter;
    y+= m_y_inter;
    y+= m_y_inter;
    m_parametrage.setResize(m_part_2_x+0.25*m_part_2_w, y,  QString::fromStdString(Langage::getKey("OPT_TRANSLATEUR_PARAM")), true, 0.45*m_part_2_w);
    m_parametrage2.setResize(m_part_2_x+0.75*m_part_2_w, y,  QString::fromStdString(Langage::getKey("OPT_TRANSLATEUR_CARD")), true, 0.45*m_part_2_w);
    y += m_y_inter;
};

void OptionWidget::drawPage3(){
    Framework & f = Framework::instance();
    Config & config = Framework::instance().m_config;
    
    m_painter->setPen(m_pen_black_inv);
    drawText(Langage::getKey("OPT_PARAM_TRANSLATEUR"), 0.45*m_width, m_y_title, sizeText_bigbig, true);
    
    drawPart1Title(m_command_capteur_vitesse_max_mm_s.m_y-2*m_y_inter, m_y_inter*4, Langage::getKey("OPT_HYDRAULIQUE"));
    {
        m_command_capteur_vitesse_max_mm_s.m_value = config.m_command_capteur_vitesse_max_mm_s/10.0;
        drawText(Langage::getKey("HYDR_V_MAX"), m_part_1_x2,m_command_capteur_vitesse_max_mm_s.m_y, sizeText_medium);
        drawValueGuiKeyPad2(m_command_capteur_vitesse_max_mm_s);
    }
    {
        
        m_command_capteur_p.m_value = config.m_command_capteur_p;
        drawText(Langage::getKey("HYDR_AGRESS"), m_part_1_x2,m_command_capteur_p.m_y, sizeText_medium);
        drawValueGuiKeyPad2(m_command_capteur_p);
    }
    
    drawPart1Title(m_command_capteur_vitesse_max_mm_s.m_y+2*m_y_inter, m_y_inter*2, Langage::getKey("OPT_DEPLACEMENT"));
    QString s = QString::number(std::round(f.m_pilot_translator_module.m_pas_mesured_mm_liss)/10.0, 'f', 1) + " cm";
    drawQText(s, m_part_1_m, m_btnCapteurZero.m_y+m_btnCapteurZero.m_rayon*1, sizeText_little, true);
    
    
    drawPart2Title(m_btnTempLeft.m_y-2*m_y_inter, m_y_inter*3, Langage::getKey("OPT_TEST_TRANSLATEUR"));
    drawButtonImage(m_btnTempLeft, m_imgLeft2, 0.6);
    drawText(Langage::getKey("LEFT"), m_btnTempLeft.m_x, m_btnTempLeft.m_y+m_btnTempLeft.m_rayon*1, sizeText_little, true);
    drawButtonImage(m_btnTempRight, m_imgRight2, 0.6);
    drawText(Langage::getKey("RIGHT"), m_btnTempRight.m_x, m_btnTempRight.m_y+m_btnTempRight.m_rayon*1, sizeText_little, true);

    drawPart2Title(m_btnCapteurLeft.m_y-2*m_y_inter, m_y_inter*3, Langage::getKey("OPT_TEST_TRANSLATEUR2"), true);
    drawButtonImage(m_btnCapteurLeft, m_imgLeft);
    drawText("-10 cm", m_btnCapteurLeft.m_x, m_btnCapteurLeft.m_y+m_btnCapteurLeft.m_rayon*1, sizeText_little, true);
    drawButtonImage(m_btnCapteurZero, m_imgMiddle);
    drawText("0", m_btnCapteurZero.m_x, m_btnCapteurZero.m_y+m_btnCapteurZero.m_rayon*1, sizeText_little, true);
    drawButtonImage(m_btnCapteurRight, m_imgRight);
    drawText("+10 cm", m_btnCapteurRight.m_x, m_btnCapteurRight.m_y+m_btnCapteurLeft.m_rayon*1, sizeText_little, true);
    drawPart2Title(m_parametrage.m_y-2*m_y_inter, m_y_inter*3, Langage::getKey("OPT_TRANSLATEUR"));
    drawButtonLabel2(m_parametrage);
    drawButtonLabel2(m_parametrage2);
    
};

void OptionWidget::onMousePage3(int x, int y){
    Config & config = Framework::instance().m_config;
    if(onMouseKeyPad2(m_command_capteur_vitesse_max_mm_s, x, y, 0.5)){
        config.m_command_capteur_vitesse_max_mm_s = m_command_capteur_vitesse_max_mm_s.m_value*10;
        loadConfig();
    };
    if(onMouseKeyPad2(m_command_capteur_p, x, y, 5)){
        config.m_command_capteur_p = m_command_capteur_p.m_value;
        loadConfig();
    };
    
    if(m_btnTempRight.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.openRelayRight(config.m_temp_deplacement_ms);
    }
    if(m_btnTempLeft.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.openRelayLeft(config.m_temp_deplacement_ms);
    }
    if(m_btnCapteurLeft.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.test_mm(-100);
    }
    if(m_btnCapteurZero.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.test_mm(0);
    }
    if(m_btnCapteurRight.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.test_mm(100);
    }
    
    if(m_parametrage.isActive(x, y) != 0){
        GpsWidget::instance()->m_param_translateur_widget.open();
        loadConfig();
    }
    
    if(m_parametrage2.isActive(x, y) != 0){
        GpsWidget::instance()->m_param_translateur_widget.open();
        GpsWidget::instance()->m_param_translateur_widget.m_page = 9;
        loadConfig();
    }
};

/**
 PAGE 4
 */

void OptionWidget::setSizePage4(){
};

void OptionWidget::drawPage4(){
}

void OptionWidget::onMousePage4(int x, int y){
}



/**
 PAGE 5
 */

void OptionWidget::myDrawButton(ButtonGui * b, QString s){
    drawButtonLabel2(*b, COLOR_CHECK);
}

int y_licence = 0;

void OptionWidget::setSizePage5(){
    int inter = m_width*0.08;
    int x = m_part_1_x2+m_part_1_w/2-60;
    int rayon = m_gros_button;
    int y_begin = m_height*0.5;
    
    int y = y_begin - inter;
    m_button7.setResizeStd(x-inter, y, "7", true, 2*rayon, 2*rayon);
    m_button8.setResizeStd(x, y, "8", true, 2*rayon, 2*rayon);
    m_button9.setResizeStd(x+inter, y, "9", true, 2*rayon, 2*rayon);
    
    y = y_begin;
    m_button4.setResizeStd(x-inter, y, "4", true, 2*rayon, 2*rayon);
    m_button5.setResizeStd(x, y, "5", true, 2*rayon, 2*rayon);
    m_button6.setResizeStd(x+inter, y, "6", true, 2*rayon, 2*rayon);
    
    y = y_begin + inter;
    m_button1.setResizeStd(x-inter, y, "1", true, 2*rayon, 2*rayon);
    m_button2.setResizeStd(x, y, "2", true, 2*rayon, 2*rayon);
    m_button3.setResizeStd(x+inter, y, "3", true, 2*rayon, 2*rayon);
    
    y = y_begin + 2*inter;
    m_button0.setResizeStd(x, y, "0", true, 2*rayon, 2*rayon);
    
    y = m_height*0.3;
    
    y+= m_y_inter;
    m_panel.setResize(m_part_2_x+m_part_2_w*0.6, y, m_height2*0.35);
    y+= m_y_inter;
    m_mcu.setResize(m_part_2_x+m_part_2_w*0.6, y, m_height2*0.35);
    y+= m_y_inter;
    m_licence.setResize(m_part_2_x+m_part_2_w*0.6, y, m_height2*0.35);
    y+= m_y_inter;
    y_licence = y;
    y+= m_y_inter;
};

void OptionWidget::drawPage5(){
    m_painter->setPen(m_pen_black_inv);
    drawText(Langage::getKey("OPT_AV_TITLE"), 0.45*m_width, m_y_title, sizeText_bigbig, true);
    
    int y = m_y_title+1*m_y_inter;
    drawPart1Title(y, m_y_inter*10, Langage::getKey("OPT_AV_TECHNICIEN"), false);
    myDrawButton(&m_button0, "0");
    myDrawButton(&m_button1, "1");
    myDrawButton(&m_button2, "2");
    myDrawButton(&m_button3, "3");
    myDrawButton(&m_button4, "4");
    myDrawButton(&m_button5, "5");
    myDrawButton(&m_button6, "6");
    myDrawButton(&m_button7, "7");
    myDrawButton(&m_button8, "8");
    myDrawButton(&m_button9, "9");
    
    Config & config = Framework::instance().m_config;
    
    y = m_y_title+1*m_y_inter;
    drawPart2Title(y, m_y_inter*10, Langage::getKey("OPT_LICENCE_TITLE"), false);
    
    //Framework & f = Framework::instance();
    drawText(Langage::getKey("OPT_LIC_PANEL") , m_part_2_x2, m_panel.m_y, sizeText_medium, false);
    m_panel.m_text = "";//f.m_config.m_licence_panel;
    
    drawText(Langage::getKey("OPT_LIC_MCU") , m_part_2_x2, m_mcu.m_y, sizeText_medium, false);
    m_mcu.m_text = "";//f.m_config.m_licence_boitier;
    
    drawText(Langage::getKey("OPT_LIC_LICENCE") , m_part_2_x2, m_licence.m_y, sizeText_medium, false);
    m_licence.m_text = "";//f.m_config.m_licence;
    
    /*LicenceResult l = f.m_licenseModule.m_licence_result;
    if(l == LicenceOK){
        drawText(Langage::getKey("OPT_LIC_OK") , m_part_2_x2, y_licence, sizeText_medium, false);
    } else if(l == LicenceFailLen){
        drawText(Langage::getKey("OPT_LIC_ERR_L1") , m_part_2_x2, y_licence, sizeText_medium, false);
    } else if(l == LicenceFailBegin){
        drawText(Langage::getKey("OPT_LIC_ERR_B1") , m_part_2_x2, y_licence, sizeText_medium, false);
    } else if(l == LicenceFail){
        drawText(Langage::getKey("OPT_LIC_ERR_F") , m_part_2_x2, y_licence, sizeText_medium, false);
    }*/
    
    if(config.isTechnicien()){
        
        drawValueGuiKeyBoardDisable(m_panel);
        drawValueGuiKeyBoard(m_mcu);
        drawValueGuiKeyBoard(m_licence);
    } else {
        drawValueGuiKeyBoardDisable(m_panel);
        drawValueGuiKeyBoardDisable(m_mcu);
        drawValueGuiKeyBoardDisable(m_licence);
    }
    
    if(!m_keyboard_widget.m_close){
        m_keyboard_widget.draw();
    }
}

int i_technicien = 0;
int i_avance = 0;
void OptionWidget::onMousePage5(int x, int y){
    if(!m_keyboard_widget.m_close){
        m_keyboard_widget.onMouse(x, y);
        //Framework & f = Framework::instance();
        /*f.m_config.m_licence = m_licence.m_text;
        f.m_config.m_licence_panel = m_panel.m_text;
        f.m_config.m_licence_boitier = m_mcu.m_text;*/
        loadConfig();
    } else {
        Config & config = Framework::instance().m_config;
        if(i_avance == 0 && m_button6.isActive(x, y)){
            i_avance = 1;
        } else if(i_avance == 1 && m_button1.isActive(x, y)){
            i_avance = 2;
        } else if(i_avance == 2 && m_button5.isActive(x, y)){
            i_avance = 3;
        } else if(i_avance == 3 && m_button5.isActive(x, y)){
            config.m_user_mode = 2;
        } else if(i_technicien == 0 && m_button0.isActive(x, y)){
            i_technicien = 1;
        } else if(i_technicien == 1 && m_button0.isActive(x, y)){
            i_technicien = 2;
        } else if(i_technicien == 2 && m_button0.isActive(x, y)){
            i_technicien = 3;
        } else if(i_technicien == 3 && m_button0.isActive(x, y)){
            config.m_user_mode = 1;
        } else {
            i_technicien = 0;
            i_avance = 0;
        }
        //INFO(i_technicien << " " << i_avance << " " << f.m_config.m_user_mode);
        if(config.isTechnicien()){
            if(isActiveValueGuiKeyBoard(m_licence,x,y)){
                m_keyboard_widget.m_close = false;
                m_keyboard_widget.setValueGuiKeyBoard(&m_licence);
            }
            if(isActiveValueGuiKeyBoard(m_mcu,x,y)){
                m_keyboard_widget.m_close = false;
                m_keyboard_widget.setValueGuiKeyBoard(&m_mcu);
            }
        }
    }
}



/**
 PAGE 6
 */

void OptionWidget::setSizePage6(){
    int y = m_y_begin;
    m_balise_enable.setResize(m_part_1_x2, y, m_petit_button);
    m_balise_enable.m_label = QString::fromStdString(Langage::getKey("OPT_BALISE"));
    
    m_select_port1_gps_serial.setResize(m_part_2_x+m_part_2_w/2, y, "", true, m_part_2_w/2);
    y += m_y_inter;
    y += m_y_inter;
    y += m_y_inter;
    m_select_port2_mcu_serial.setResize(m_part_2_x+m_part_2_w/2, y, "", true, m_part_2_w/2);
    y += m_y_inter;
    y += m_y_inter;
    y += m_y_inter;
    m_select_port3_soil_serial.setResize(m_part_2_x+m_part_2_w/2, y, "", true, m_part_2_w/2);
    
};

void OptionWidget::drawPage6(){
    Framework & f = Framework::instance();
    drawText("Pilotage", 0.45*m_width, m_y_title, sizeText_big, true);
    
    drawPart1Title(m_select_port1_gps_serial.m_y-2*m_y_inter, 5*m_y_inter, "options");
    drawButtonCheck(m_balise_enable, f.m_config.m_balise_enable);
   
    drawPart2Title(m_select_port1_gps_serial.m_y-2*m_y_inter, 3*m_y_inter, "port1 GPS");
    drawButtonLabel2(m_select_port1_gps_serial.m_buttonOpen);
    
    drawPart2Title(m_select_port2_mcu_serial.m_y-2*m_y_inter, 3*m_y_inter, "port2 MCU");
    drawButtonLabel2(m_select_port2_mcu_serial.m_buttonOpen);
    
    drawPart2Title(m_select_port3_soil_serial.m_y-2*m_y_inter, 3*m_y_inter, "port3 Soil");
    drawButtonLabel2(m_select_port3_soil_serial.m_buttonOpen);
    
    if(m_select_widget.m_close){
        m_select_port1_gps_serial.setValueString(f.m_config.m_port1_gps_serial);
        m_select_port2_mcu_serial.setValueString(f.m_config.m_port2_mcu_serial);
        m_select_port3_soil_serial.setValueString(f.m_config.m_port3_soil_serial);
    }
    
    
    
}

void OptionWidget::onMousePage6(int x, int y){
    Framework & f = Framework::instance();
    
    if(!m_select_widget.m_close){
        if(m_select_widget.onMouseSelect(x, y)){
            f.m_config.m_port1_gps_serial = m_select_port1_gps_serial.getValueString();
            f.m_config.m_port2_mcu_serial = m_select_port2_mcu_serial.getValueString();
            f.m_config.m_port3_soil_serial = m_select_port3_soil_serial.getValueString();
        }
        f.initOrLoadConfig();
        return;
    }
    
    if(m_balise_enable.isActive(x, y)){
        f.m_config.m_balise_enable = !f.m_config.m_balise_enable;
    }
    
    isActiveButtonSelect(&m_select_port1_gps_serial, x, y);
    isActiveButtonSelect(&m_select_port2_mcu_serial, x, y);
    isActiveButtonSelect(&m_select_port3_soil_serial, x, y);
    
    f.initOrLoadConfig();
    
}

/**
 PAGE 7
 */

void OptionWidget::setSizePage7(){
    int y = m_y_begin;
    m_select_gps_serial.setResize(m_part_1_x+m_part_1_w/2, y, "", true, m_part_1_w/2);
    
    y += m_y_inter;
    m_select_gps_baudrates.setResize(m_part_1_x+m_part_1_w/2, y, "", true, m_part_1_w/2);
    m_select_gps_baudrates.clear();
    m_select_gps_baudrates.addValueInt("9600", 9600);
    m_select_gps_baudrates.addValueInt("115200", 115200);
    
    
    y = m_y_begin;
    
    m_button_get_antenna.setResizeStd(m_part_2_x+m_part_2_w/2, y, "ANTENNA GET CONFIG", true, m_part_2_w/2);
    
    y += 4*m_y_inter;
    m_button_set_antenna.setResizeStd(m_part_2_x+m_part_2_w/2, y, "ANTENNA SET CONFIG", true, m_part_2_w/2);
}



void OptionWidget::drawPage7(){
    Framework & f = Framework::instance();
    //Config & config = Framework::instance().m_config;
    
    drawText("GPS", 0.45*m_width, m_y_title, sizeText_big, true);
    
    drawPart1Title(m_select_gps_serial.m_y - 2*m_y_inter, 4*m_y_inter, "connection");
    drawButtonLabel2(m_select_gps_serial.m_buttonOpen);
    drawButtonLabel2(m_select_gps_baudrates.m_buttonOpen);
    
    if(m_select_widget.m_close){
        //m_select_gps_baudrates.setValueInt(f.m_config.m_gps_baudrate);
        //m_select_gps_serial.setValueString(f.m_config.m_gps_serial);
    }
    
    drawButtonLabel2(m_button_get_antenna);
    int y = m_button_get_antenna.m_y + m_y_inter;
    int inter = m_y_inter/2;
    drawText(f.m_nmea_parser_pilot.m_cfg_net_str, m_part_2_x, y);
    y += inter;
    drawText(f.m_nmea_parser_pilot.m_cfg_terrain_str, m_part_2_x, y);
    y += inter;
    drawText(f.m_nmea_parser_pilot.m_cfg_height_str, m_part_2_x, y);
    
    drawButtonLabel2(m_button_set_antenna);
}



void OptionWidget::onMousePage7(int x, int y){
    Framework & f = Framework::instance();
    //Config & config = Framework::instance().m_config;
    
    if(!m_select_widget.m_close){
        if(m_select_widget.onMouseSelect(x, y)){
            //f.m_config.m_gps_baudrate = m_select_gps_baudrates.getValueInt();;
            //f.m_config.m_gps_serial = m_select_gps_serial.getValueString();
        }
        f.initOrLoadConfig();
        return;
    }
    
    isActiveButtonSelect(&m_select_gps_serial, x, y);
    isActiveButtonSelect(&m_select_gps_baudrates, x, y);
    
    if(m_button_get_antenna.isActive(x, y)){
        f.getAntennaH();
    }
    
    if(m_button_set_antenna.isActive(x, y)){
        f.setAntennaH();
    }
    
    f.initOrLoadConfig();
}


void OptionWidget::open(){
    m_close = false;
    addSerials();
}

void OptionWidget::loadConfig(){
    Framework & f = Framework::instance();
    f.initOrLoadConfig();
}

void OptionWidget::addSerials(){
    Framework & f = Framework::instance();
    
    std::vector<std::string> serials;
    serials.push_back("none");
    serials.push_back("file");
    std::vector<std::string> & s2 = f.m_serialModule.getAvailablePorts();
    for(auto s : s2){
        serials.push_back(s);
    }
    
    
    
    
    m_select_gps_serial.clear();
    m_select_port1_gps_serial.clear();
    m_select_port2_mcu_serial.clear();
    m_select_port3_soil_serial.clear();
    for(auto serial: serials){
        m_select_gps_serial.addValue(serial);
        
        m_select_port1_gps_serial.addValue(serial);
        if(serial != "file"){
            m_select_port2_mcu_serial.addValue(serial);
            m_select_port3_soil_serial.addValue(serial);
            
        }
    }
}


/**
 BASE
 */



void OptionWidget::isActiveButtonSelect(SelectButtonGui * select, int x, int y){
    if(select->m_buttonOpen.isActive(x, y)){
        m_select_widget.open();
        m_select_widget.setValueGuiKeyPad(select);
    }
}
void OptionWidget::setPainter(QPainter *p){
    m_painter = p;
    m_keypad_widget.setPainter(p);
    m_select_widget.setPainter(p);
    m_keyboard_widget.setPainter(p);
}

void OptionWidget::drawPart1Title(double y, double h, const std::string & title, bool separateur){
    {
        m_painter->setBrush(m_brush_background_2);
        m_painter->setPen(m_pen_no);
        
        m_painter->drawRoundedRect(m_part_1_x, y+5, m_width3*0.44, h-15, 10, 10);
        
    }
    
    m_painter->setPen(m_pen_black_inv);
    drawText(title, m_part_1_x+m_part_1_w/2, y+m_y_inter*0.8, sizeText_big, true);
    
}

void OptionWidget::drawPart2Title(double y, double h, const std::string & title, bool separateur){
    {
        m_painter->setBrush(m_brush_background_2);
        m_painter->setPen(m_pen_no);
        
        m_painter->drawRoundedRect(m_part_2_x, y+5, m_width3*0.44, h-15, 10, 10);
        
    }
    
    m_painter->setPen(m_pen_black_inv);
    drawText(title, m_part_2_x+m_part_2_w/2, y+m_y_inter*0.8, sizeText_big, true);
}
