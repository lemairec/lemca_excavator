#include "rapide_option_widget.hpp"

#include "../framework.hpp"
#include "qt/main_window.hpp"

#include "../config/langage.hpp"

RapideOptionWidget::RapideOptionWidget(){
}

void RapideOptionWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    m_x = m_width*0.65+20;
    m_width2 = m_width*0.24;
    
    m_y = 10;
    m_height2 = m_height*0.81;
    
    m_x_middle = m_x+m_width2/2;
    m_x_left = m_x+m_width2*1/3;
    m_x1 = m_x+m_width2*1/4;
    m_x2 = m_x+m_width2*2/3;
    
    m_y_title = m_height*0.1;
    
    m_keypad_widget.setSize(width, height);
    m_button_close.setResize(m_x_middle, m_y+0.90*m_height2, QString::fromStdString(Langage::getKey("CLOSE")), true);
    setSizePage1(width, height);
    setSizePage2(width, height);
    setSizePage3(width, height);
    setSizePage4(width, height);
    setSizePage5(width, height);
    setSizePage6(width, height);
    
    m_button_next.setResizeStd(m_x+0.85*m_width2, m_y+0.95*m_height2, "", true, 0.1*m_height);
}

void RapideOptionWidget::loadImages(){
    BaseWidget::loadImages();
    m_img_check_on = loadImageInv("/gui/check_on.png");
    m_img_check_off = loadImageInv("/gui/check_off.png");

}


void RapideOptionWidget::loadConfig(){
    Framework::instance().m_config.save();
}

void RapideOptionWidget::draw(){
    m_painter->setPen(m_pen_no);
    m_painter->setBrush(m_brush_background_2);
    m_painter->drawRoundedRect(m_x, m_y, m_width2, m_height2, 10, 10);
    m_painter->setPen(m_pen_black_inv);
    
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
        drawPage6();
    } else if(m_page == 7){
        drawPage7();
    } else if(m_page == 8){
        drawPage8();
    }
    
    Config & config = Framework::instance().m_config;
    if(config.isTechnicien()){
        QString s = QString::number(m_page)+"/"+QString::number(m_nbr_page);
        m_button_next.m_label = s.toUtf8().constData();
        drawButtonLabel2(m_button_next);
    }
    
    drawButtonLabel2(m_button_close, COLOR_CANCEL);
}

int RapideOptionWidget::onMouse(int x, int y){
    if(m_button_close.isActive(x,y)){
        loadConfig();
        m_close = true;
        return 0;
    }
    
    
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
    }
    if(m_button_next.isActive(x, y)){
        Config & config = Framework::instance().m_config;
        
        if(config.isTechnicien()){
            m_page++;
            if(m_page > m_nbr_page){
                m_page = 1;
            }
        }
    }
    return 0;
}

void RapideOptionWidget::open(){
    m_close = false;
    //addSerials();
}


int m_y_begin = 0;
int m_y_inter = 0;


/**
    PAGE 1
 */

void RapideOptionWidget::setSizePage1(int width, int height){
    m_y_begin = 0.25*m_height2;
    m_y_inter = 0.1*m_height2;;
    int y = m_y_begin;
    m_new_point.setResizeStd(m_x+m_width2*0.5, y, "New Point", true);
};

void RapideOptionWidget::drawPage1(){
    drawQText("Arpentage", m_x+m_width2/2, m_y_title, sizeText_big, true);
    
    
    drawButtonLabel2(m_new_point);
    
    Framework & f = Framework::instance();
    
    m_painter->setPen(m_pen_black_inv);
    int y = m_y_begin;
    y += m_y_inter;
    y += m_y_inter;
    QString s = "nbr new balises : "+QString::number(f.m_balises.m_balises_new.size());
    drawQTexts(s, m_x+m_width2*0.2, y);
    
    y += m_y_inter;
    size_t len = f.m_balises.m_balises_new.size();
    if(len > 10){
        len = 10;
    }
    for(size_t i = 0; i < len; ++i){
        Balise * b = f.m_balises.m_balises_new[i];
        std::string s1 = strprintf("- %.7f,%.7f,%.1f m", b->m_latitude, b->m_longitude, b->m_altitude);
        drawText(s1, m_x+m_width2*0.2, y);
        y += m_y_inter/4;
        
    }
}

void RapideOptionWidget::onMousePage1(int x, int y){
    Framework & f = Framework::instance();
    
    if(m_new_point.isActive(x, y)){
        f.m_balises.newBalise();
    }
}

/**
    PAGE 2
 */

void RapideOptionWidget::setSizePage2(int width, int height){
    m_y_begin = 0.25*m_height2;
    m_y_inter = 0.1*m_height2;;
    int y = m_y_begin;
    y += m_y_inter;
    m_synchro.setResizeStd(m_x+m_width2*0.5, y, "synchro", true);
    y += m_y_inter;
    y += m_y_inter;
    y += m_y_inter;
}

void RapideOptionWidget::drawPage2(){
    drawQText("Menu Synchro", m_x+m_width2/2, m_y_title, sizeText_big, true);
    
    
    drawButtonLabel2(m_synchro);
    
    Framework & f = Framework::instance();
    m_painter->setPen(m_pen_black_inv);
    int y = m_y_begin;
    y += 3*m_y_inter;
    QString s = QString::fromStdString(f.m_synchro.m_logs);
    drawQTexts(s, m_x+m_width2*0.2, y);
}

void RapideOptionWidget::onMousePage2(int x, int y){
    Framework & f = Framework::instance();
    
    if(m_synchro.isActive(x, y)){
        f.m_synchro.beginSynchro();
    }
}


void RapideOptionWidget::setSizePage3(int width, int height){
    int y = m_y_begin;
    m_mode_balise.setResizeStd(m_x+m_width2*0.5, y, "mode balises", true);
}

void RapideOptionWidget::drawPage3(){
    drawQText("Balises", m_x+m_width2/2, m_y_title, sizeText_big, true);
    
    Framework & f = Framework::instance();
    int y = m_y_begin;
    drawButtonLabel2(m_mode_balise);
    m_painter->setPen(m_pen_black_inv);
    y += m_y_inter;
    QString s = "nbr balises : "+QString::number(f.m_balises.m_balises.size());
    drawQTexts(s, m_x+m_width2*0.2, y);
    
    y += m_y_inter;
    size_t len = f.m_balises.m_balises_sort.size();
    if(len > 10){
        len = 10;
    }
    for(size_t i = 0; i < len; ++i){
        Balise * b = f.m_balises.m_balises_sort[i];
        std::string s1 = strprintf("%s : %.2f m", b->m_name.data(), b->m_distance);
        drawText(s1, m_x+m_width2*0.2, y);
        y += m_y_inter/4;
        
    }
}
void RapideOptionWidget::onMousePage3(int x, int y){
    if(m_mode_balise.isActive(x, y)){
        Framework & f = Framework::instance();
        f.setEtat(Etat_Points);
    }
}


/*
page 4

*/

void RapideOptionWidget::setSizePage4(int width, int height){
    int inter = 0.1*m_height2;
    int y = 0.2*m_height2;
    y = y+inter;
    m_save_point.setResizeStd(m_x_middle, y, "Point Reference", true);
    y = y+inter;
    y = y+inter;
    y = y+inter;
    y = y+inter;
    m_valuegui_profondeur.setResize(m_x_middle, y, m_petit_button);
    y = y+inter;
    
};



void RapideOptionWidget::drawPage4(){
    Framework & f = Framework::instance();
    Config & config = f.m_config;
    
    m_painter->setPen(m_pen_black_inv);
    
    drawButtonLabel2(m_save_point);
    
    m_painter->setPen(m_pen_black_inv);
    m_valuegui_profondeur.m_value = config.m_profondeur_mm/10.0;
    draw(m_valuegui_profondeur, "profondeur cm");
    
    
    
    if(!m_keypad_widget.m_close){
        m_keypad_widget.draw();
    }
}

void RapideOptionWidget::onMousePage4(int x, int y){
    Framework & f = Framework::instance();
    Config & config = f.m_config;
    
    onMouse(m_valuegui_profondeur, x, y, 5);
    
    config.m_profondeur_mm = m_valuegui_profondeur.m_value*10;
    
    if(m_save_point.isActive(x, y)){
        f.m_hauteur_save =  f.m_hauteur_current;
        f.m_point_save =  f.m_point_current;
    }
    
    loadConfig();
}


// Page 5
void RapideOptionWidget::setSizePage5(int width, int height){
   
}

void RapideOptionWidget::drawPage5(){
    
    //y+=inter;
}

void RapideOptionWidget::onMousePage5(int x, int y){
   
}


// Page 6
void RapideOptionWidget::setSizePage6(int width, int height){
    m_button_reset.setResizeStd(m_x_middle, 0.6*m_height2, Langage::getKey("RAPIDE_RESET"), true);
    m_button_diag.setResizeStd(m_x_middle, 0.7*m_height2, Langage::getKey("DIAGNOSTIC"), true);
}

void RapideOptionWidget::drawPage6(){
    drawText(Langage::getKey("RAPIDE_COUNTER"), m_x+m_width2/2, m_y_title, sizeText_big, true);
    
    /*int y = 0.25*m_height2;
    int inter = 0.04*m_height2;;
    
    {
        drawText(Langage::getKey("RAPIDE_TOTAL"), m_x1, y, sizeText_medium);
        QString qs = QString::number(f.m_qt_compteur_heure.m_heure_total, 'f', 1) + " h";
        drawQText(qs, m_x2, y, sizeText_medium);
    }
    y+=inter;
    
    {
        QString qs = QString::number(f.m_qt_compteur_heure.m_ha_total, 'f', 1) + " ha";
        drawQText(qs, m_x2, y, sizeText_medium);
    }
    y+=2*inter;
    
    {
        drawText(Langage::getKey("RAPIDE_PARTIAL"), m_x1, y, sizeText_medium);
        QString qs = QString::number(f.m_qt_compteur_heure.m_heure_partiel, 'f', 1) + " h";
        drawQText(qs, m_x2, y, sizeText_medium);
    }
    y+=inter;
    {
        QString qs = QString::number(f.m_qt_compteur_heure.m_ha_partiel, 'f', 1) + " ha";
        drawQText(qs, m_x2, y, sizeText_medium);
    }
    y+=2*inter;
    
    {
        drawText(Langage::getKey("RAPIDE_SESSION"), m_x1, y, sizeText_medium);
        QString qs = QString::number(f.m_qt_compteur_heure.m_heure_session, 'f', 1) + " h";
        drawQText(qs, m_x2, y, sizeText_medium);
    }
    y+=inter;
    {
        QString qs = QString::number(f.m_qt_compteur_heure.m_ha_session, 'f', 1) + " ha";
        drawQText(qs, m_x2, y, sizeText_medium);
    }
    
    if(f.m_config.m_vitesse_type == VitesseType_GPS){
        y = 0.8*m_height2;
        drawText(Langage::getKey("RAPIDE_GPS"), m_x+m_width2/2, y, sizeText_big, true);
        y+=inter;
        QString qs = QString::number(f.m_gps_vitesse, 'i', 0) + " " + QString::fromStdString(Langage::getKey("RAPIDE_SATELLITES"));
        drawQText(qs, m_x+m_width2/2, y, sizeText_medium, true);
    }*/
    
    drawButtonLabel2(m_button_reset);
    drawButtonLabel2(m_button_diag);
}

void RapideOptionWidget::onMousePage6(int x, int y){
    /*FrameworkVision & f = FrameworkVision::instance();
    if(m_button_reset.isActive(x, y) != 0){
        f.m_qt_compteur_heure.resetPartiel();
    }*/
    if(m_button_diag.isActive(x, y) != 0){
        GpsWidget::instance()->m_diagnostic_widget.open();
    }
}


//Page7
void RapideOptionWidget::drawPage7(){
   
}

void RapideOptionWidget::drawPage8(){
    
}


/**
  COMMON
 */

void RapideOptionWidget::draw(ValueGui & keypad, std::string s){
    int dy = 30;
    drawText(s, keypad.m_x, keypad.m_y-dy, sizeText_medium, true);

    drawValueGuiKeyPad2(keypad);
}
void RapideOptionWidget::drawFalse(ValueGui & keypad, std::string s){
    int dy = 30;
    drawText(s, keypad.m_x, keypad.m_y-dy, sizeText_medium, true);

    drawValueGuiKeyPadFalse(keypad);
}

void RapideOptionWidget::onMouse(ValueGui & keypad, double x, double y, double pas){
    if(keypad.m_button_plus.isActive(x, y)){
        keypad.m_value += pas;
    }
    if(keypad.m_button_moins.isActive(x, y)){
        keypad.m_value -= pas;
    }
       
    if(isActiveValueGuiKeyPad(keypad,x, y)){
        m_keypad_widget.m_close = false;
        m_keypad_widget.setValueGuiKeyPad(&keypad);
    }
}
