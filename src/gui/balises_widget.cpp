#include "balises_widget.hpp"

#include "../framework.hpp"
#include "qt/main_window.hpp"

#include "../config/langage.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

const size_t LEN = 15;

BalisesWidget::BalisesWidget(){
}

void BalisesWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    m_y2 = m_height*0.04;
    m_x2 = m_y2;
    m_width2 = m_width*2/3-2*m_y2;
    m_height2 = m_height-2*m_y2;
    
    m_button_close.setResizeStd(m_x2 + 0.8*m_width2, 0.9*m_height2, "close", true);
    m_button_page_up.setResizeStd(m_x2 + 0.4*m_width2, 0.9*m_height2, ">", true);
    m_button_page_down.setResizeStd(m_x2 + 0.2*m_width2, 0.9*m_height2, "<", true);
    m_button_add.setResizeStd(m_x2 + 0.6*m_width2, 0.9*m_height2, "add", true);
    m_button_import.setResizeStd(m_x2 + 0.6*m_width2, 0.95*m_height2, "import", true);
    
    m_latitude.setResize(m_x2 + 0.5*m_width2, 0.3*m_height2, m_petit_button);
    m_longitude.setResize(m_x2 + 0.5*m_width2, 0.4*m_height2, m_petit_button);
    m_name.setResize(m_x2 + 0.5*m_width2, 0.5*m_height2, m_height2*0.35);
    
    m_button_save.setResizeStd(m_x2 + 0.6*m_width2, 0.7*m_height2, "save", true);
    
    m_keypad_widget.setSize(width, height);
    m_keyboard_widget.setSize(width, height);
}

void BalisesWidget::open(){
    BaseWidget::open();
    m_page = 0;
    m_mode = 0;
}

void BalisesWidget::setPainter(QPainter *p){
    m_painter = p;
    m_keypad_widget.setPainter(p);
    m_keyboard_widget.setPainter(p);
}

void BalisesWidget::draw(){
    
    m_painter->setPen(m_pen_black);
    m_painter->setBrush(m_brush_white);
    m_painter->drawRoundedRect(m_x2, m_y2, m_width2, m_height2, RAYON_ROUNDED, RAYON_ROUNDED);
    
    if(m_mode == 0){
        drawBalises();
    } else if(m_mode == 1){
        drawAdd();
    } else if(m_mode == 2){
        drawImport();
    }
    
    if(!m_keypad_widget.m_close){
        m_keypad_widget.draw();
    }
};

int BalisesWidget::onMouse(int x, int y){
    if(m_mode == 0){
        onMouseBalises(x, y);
    } else if(m_mode == 1){
        onMouseAdd(x, y);
    } else if(m_mode == 2){
        onMouseImport(x, y);
    }
    
    return 0;
}

void BalisesWidget::drawBalises(){
    Framework & f = Framework::instance();
    double y = 0.1*m_height2;
    double m_y_inter = 0.04*m_height2;
    double x1 = m_x2+0.05*m_width2;
    double x2 = m_x2+0.15*m_width2;
    double x3 = m_x2+0.65*m_width2;
    drawText("Balises", m_x2+0.5*m_width2, y, sizeText_medium, true);
    
    QString s = "nbr balises : "+QString::number(f.m_balises.m_balises.size());
    drawQTexts(s, x1, y);
    
    y += m_y_inter;
    y += m_y_inter;

    for(size_t i = m_page*LEN; i < m_page*LEN+LEN; ++i){
        if(i >=0 && i < f.m_balises.m_balises.size() ){
            size_t j = f.m_balises.m_balises.size()-i-1;
            Balise * b = f.m_balises.m_balises[j];
            std::string s1 = strprintf("%i - %s", j, b->m_name.data());
            drawText(s1, x1, y);
            
            std::string s2 = strprintf("%.7f, %.7f, %.2f m", b->m_latitude, b->m_longitude, b->m_altitude);
            drawText(s2, x2, y);
            
            std::string s3 = strprintf("%.2f m", b->m_distance);
            drawText(s3, x3, y);
            
            y += m_y_inter;
        }
        
    }
    
    y += m_y_inter;
    s = "page : "+QString::number(m_page+1);
    drawQTexts(s, x1, y);
    drawButtonLabel2(m_button_page_up);
    drawButtonLabel2(m_button_page_down);
    
    drawButtonLabel2(m_button_close);
    drawButtonLabel2(m_button_import, COLOR_VALIDATE);
    drawButtonLabel2(m_button_add, COLOR_VALIDATE);
}

int BalisesWidget::onMouseBalises(int x, int y){
    if(m_button_close.isActive(x, y)){
        m_close = true;
    }
    if(m_button_page_up.isActive(x, y)){
        m_page++;
    }
    if(m_button_page_down.isActive(x, y)){
        m_page--;
    }
    if(m_button_add.isActive(x, y)){
        m_mode =1;
    }
    if(m_button_import.isActive(x, y)){
        m_mode =2;
    }
    return 0;
}



void BalisesWidget::drawAdd(){
    double y = 0.1*m_height2;
    drawText("Add", m_x2+0.5*m_width2, y, sizeText_medium, true);
    drawText("latitude (49.xx)", m_x2+0.1*m_width2, m_latitude.m_y, sizeText_medium);
    drawValueGuiKeyPad3(m_latitude);
    drawText("longitude (4.xx)", m_x2+0.1*m_width2, m_longitude.m_y, sizeText_medium);
     drawValueGuiKeyPad3(m_longitude);
    
    std::string s2 = strprintf("%s => %.7f, %.7f", m_name.m_text.c_str(), m_latitude.m_value, m_longitude.m_value);
    drawText(s2, m_x2+0.7*m_width2, 0.6*m_height2, sizeText_medium, true);
    
    
    drawText("name", m_x2+0.1*m_width2, m_name.m_y, sizeText_medium);
    drawValueGuiKeyBoard(m_name);
    
    drawButtonLabel2(m_button_save);
    drawButtonLabel2(m_button_close);
    
    if(!m_keyboard_widget.m_close){
        m_keyboard_widget.draw();
    }
    
    
    
}

int BalisesWidget::onMouseAdd(int x, int y){
    if(!m_keypad_widget.m_close){
        if(m_keypad_widget.onMouse(x, y)){
            std::string s2 = strprintf("%.7f, %.7f", m_latitude.m_value, m_longitude.m_value);
            INFO(s2);
        }
        return 0;
        
       
    }
    if(!m_keyboard_widget.m_close){
        m_keyboard_widget.onMouse(x, y);
        //loadConfig();
        return 0;
    }
    if(isActiveValueGuiKeyBoard(m_name,x,y)){
        m_keyboard_widget.m_close = false;
        m_keyboard_widget.setValueGuiKeyBoard(&m_name);
    }
    
    if(isActiveValueGuiKeyPad(m_latitude, x, y)){
        m_keypad_widget.open();
        m_keypad_widget.setValueGuiKeyPad(&m_latitude);
    }
    if(isActiveValueGuiKeyPad(m_longitude, x, y)){
        m_keypad_widget.open();
        m_keypad_widget.setValueGuiKeyPad(&m_longitude);
    }
    
    if(m_button_close.isActive(x, y)){
        m_close = true;
    }
    
    if(m_button_save.isActive(x, y)){
        Framework::instance().m_balises.addBalise(m_name.m_text.c_str(), m_latitude.m_value, m_longitude.m_value);
        m_page = 0;
        m_mode = 0;
    }
    
    return 0;
}

void BalisesWidget::drawImport(){
    double y = 0.1*m_height2;
    double inter = 0.05*m_height2;
    drawText("Import", m_x2+0.5*m_width2, y, sizeText_medium, true);
    y+=inter;
    y+=inter;
    drawText("Fichier import.csv dans la clef usb", m_x2+0.2*m_width2, y, sizeText_little);
    y+=inter;
    drawText("Format name,latitude,longitude (exemple test, 49.xx, 4.xx)", m_x2+0.2*m_width2, y, sizeText_little);
    
}

int BalisesWidget::onMouseImport(int x, int y){
    
    return 0;
}
