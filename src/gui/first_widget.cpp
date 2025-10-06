#include "first_widget.hpp"
#include <sstream>
#include <fstream>
#include <string>

#include "qt/main_window.hpp"

#include "environnement.hpp"
#include "../config/langage.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

FirstWidget::FirstWidget(){
    //m_close = true;
    m_close = false;
    m_img_logo = loadImage("/gui/logo.png");
    
    m_img_check_on = loadImage("/gui/check_on.png");
    m_img_check_off = loadImage("/gui/check_off.png");
}

void FirstWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    m_x = 80;
    m_y = 80;
    m_width2 = m_width-160;
    m_height2 = m_height-160;
    
    m_button_ok.setResizeStd(m_x+0.7*m_width2, m_y+0.9*m_height2, "Accepter", true);
}


void FirstWidget::draw(){
    int m_y2 = m_height*0.04;
    int m_x2 = m_y2;
    int m_width2 = m_width-2*m_y2;
    int m_height2 = m_height-2*m_y2;
    
    m_painter->setBrush(m_brush_background_1);
    m_painter->drawRect(0 , 0, m_width, m_height);
    
   
    m_painter->setPen(m_pen_no);
    m_painter->setBrush(m_brush_white);
    m_painter->drawRoundedRect(m_x2, m_y2, m_width2, m_height2, RAYON_ROUNDED, RAYON_ROUNDED);
    
    m_painter->setPen(m_pen_black);
    std::ostringstream strs;
    strs << Langage::getKey("HOME_VERSION") << ProjectVersion;
    drawText(strs.str(), 0.1*m_width, 0.8*m_height, sizeText_medium);
    
    double y = 0.5*m_height2;
    double inter = 0.05*m_height2;
    
    drawText(("Lemca GPS (beta)"), 0.1*m_width2, 0.2*m_height2, sizeText_bigbig);
    
    
    drawText(Langage::getKey("HOME_L1"), 0.1*m_width2, y, sizeText_medium);
    y+=inter;
    drawText(Langage::getKey("HOME_L2"), 0.1*m_width2, y, sizeText_medium);
    y+=inter;
    drawText(Langage::getKey("HOME_L3"), 0.1*m_width2, y, sizeText_medium);
    y+=inter;
    drawText(Langage::getKey("HOME_L4"), 0.1*m_width2, y, sizeText_medium);
    y+=inter;
    
    std::ostringstream strs2;
    strs2 << Langage::getKey("HOME_VERSION") << ProjectVersion;
    drawText(strs2.str(), 0.1*m_width, 0.8*m_height, sizeText_medium);
    
    drawButtonLabel2(m_button_ok, COLOR_VALIDATE);
    
}

int FirstWidget::onMouse(int x, int y){
    if(m_button_ok.isActive(x, y)){
        close();
    }
    return 0;
}

void FirstWidget::close(){
    m_close = true;
}
