#include "translator_widget.hpp"
#include <sstream>
#include <fstream>
#include <string>

#include "../../framework.hpp"
#include "../qt/main_window.hpp"

#include "../../config/langage.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>

TranslatorWidget::TranslatorWidget(){
    m_img_load = loadImage("/gui/load.png");
    m_img_save = loadImage("/gui/save.png");
    m_img_delete = loadImage("/gui/delete.png");
    //m_img_factory = loadImage("/gui/factory.png");
    
    m_close = false;
    m_keyboard_widget.m_just_maj = true;
}

void TranslatorWidget::loadImages(){
    m_img_option = loadImageInv("/gui/menu.png");
    
}

void TranslatorWidget::open(){
    m_page = 1;
    m_is_delete = false;
    m_name.m_text = "";
}

void TranslatorWidget::drawButton(ButtonGui& button, QPixmap * qpixmap){
    m_painter->setPen(m_pen_no);
    m_painter->setBrush(m_brush_gray);
    m_painter->drawRoundedRect(button.m_x-button.m_width/2 , button.m_y-button.m_height/2, button.m_width, button.m_height, 5, 5);
    
    m_painter->setPen(m_pen_black);
    drawMyImage(*qpixmap, button.m_x, button.m_y-button.m_height*0.1, 1.0, true);
    drawQTexts(button.m_label, button.m_x, button.m_y+button.m_height*0.25, sizeText_medium, true, false);
    drawText(Langage::getKey("TRANSLATOR_TRAJET"), button.m_x, button.m_y+button.m_height*0.35, sizeText_medium, true, false);
    //drawButtonValidate(button);
}


void TranslatorWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    m_y2 = m_height*0.04;
    m_x2 = m_y2;
    m_width2 = m_width-2*m_y2-m_gros_button*2.4;
    m_height2 = m_height-2*m_y2;
    
    m_x_middle = m_x2+m_width2/2;
    
    m_y_title = m_y2+m_height*0.08;
    
    int x_right = width-m_gros_button-20;
    int y = m_gros_button*1.2+10;
    m_button_option.setResize(x_right, y, m_gros_button);

    setSize1(width, height);
    setSize2(width, height);
    setSize3(width, height);
    setSize4(width, height);
    setSize5(width, height);
    setSize6(width, height);
    setSize7(width, height);
    setSize8(width, height);
    setSize9(width, height);
    
    m_btn_setting_yes.setResizeStd(m_x2+0.3*m_width2, 0.9*m_height, Langage::getKey("YES"), true);
    m_btn_setting_no.setResizeStd(m_x2+0.7*m_width2, 0.9*m_height, Langage::getKey("NO"), true);
    m_btn_setting_ok.setResizeStd(m_x2+0.3*m_width2, 0.9*m_height, Langage::getKey("OK"), true);
    m_btn_setting_cancel.setResizeStd(m_x2+0.7*m_width2, 0.9*m_height, Langage::getKey("CANCEL"), true);
    m_btn_setting_quit.setResizeStd(m_x2+0.9*m_width2, 0.9*m_height, Langage::getKey("QUIT"), true);
    
    m_keyboard_widget.setSize(width, height);
}

void TranslatorWidget::drawButtons(){
    {
        int h = m_height-20;
        int w = m_gros_button*2+20;
        
        m_painter->setBrush(m_brush_background_2);
        m_painter->setPen(m_pen_no);
        
        m_painter->drawRoundedRect(m_width-w-10, 10, w, h, 10, 10);
        
    }
    
    drawButtonImageCarre(m_button_option, m_img_option, 0.3);
}

void TranslatorWidget::draw(){
    m_painter->setPen(m_pen_no);
    m_painter->setBrush(m_brush_background_1);
    m_painter->drawRect(0 , 0, m_width, m_height);
    
    drawButtons();
    
    m_painter->setPen(m_pen_no);
    m_painter->setBrush(m_brush_white);
    m_painter->drawRoundedRect(m_x2, m_y2, m_width2, m_height2, RAYON_ROUNDED, RAYON_ROUNDED);
    
    m_painter->setPen(m_pen_black);
    m_painter->setBrush(m_brush_white);drawText(Langage::getKey("TRANSLATOR_TITLE"), m_x_middle, m_y_title, sizeText_bigbig, true);
    
    
    if(m_page == 1){
        draw1();
    } else if(m_page == 2){
        draw2();
    } else if(m_page == 3){
        draw3();
    } else if(m_page == 4){
        draw4();
    } else if(m_page == 5){
        draw5();
    } else if(m_page == 6){
        draw6();
    } else if(m_page == 7){
        draw7();
    } else if(m_page == 8){
        draw8();
    } else if(m_page == 9){
        draw9();
    }
    drawButtonLabel2(m_btn_setting_quit, COLOR_CANCEL);
    
    
    if(!m_keyboard_widget.m_close){
        m_keyboard_widget.draw();
    }
}

int TranslatorWidget::onMouse(int x, int y){
    if(m_page == 1){
        onMouse1(x, y);
    } else if(m_page == 2){
        onMouse2(x, y);
    } else if(m_page == 3){
        onMouse3(x, y);
    } else if(m_page == 4){
        onMouse4(x, y);
    } else if(m_page == 5){
        onMouse5(x, y);
    } else if(m_page == 6){
        onMouse6(x, y);
    } else if(m_page == 7){
        onMouse7(x, y);
    } else if(m_page == 8){
        onMouse8(x, y);
    } else if(m_page == 9){
        onMouse9(x, y);
    }
    if(m_btn_setting_quit.isActive(x, y)){
        Framework::instance().getAntennaH();
        m_close = true;
    }
    return 0;
}


void TranslatorWidget::setSize1(int width, int height){
    int inter = m_height*0.4;
    int y = m_height*0.5;
    int x = m_x2+m_width2/2;
    int w = m_height*0.3;
    x = m_x2+m_width2/2-inter*1.0;
    m_btn_setting_save.setResizeStd(x, y, Langage::getKey("TRANSLATOR_SAVE"), true);
    m_btn_setting_save.m_height = w;
    m_btn_setting_save.m_width = w;
    x+= inter;
    m_btn_setting_load.setResizeStd(x, y, Langage::getKey("TRANSLATOR_LOAD"), true);
    m_btn_setting_load.m_height = w;
    m_btn_setting_load.m_width = w;
    x+= inter;
    m_btn_setting_delete.setResizeStd(x, y, Langage::getKey("TRANSLATOR_DELETE"), true);
    m_btn_setting_delete.m_height = w;
    m_btn_setting_delete.m_width = w;
    /*x+= inter;
    m_btn_setting_reset.setResizeStd(x, y, Langage::getKey("REGLAGES_RESET"), true);
    m_btn_setting_reset.m_height = w;
    m_btn_setting_reset.m_width = w;*/
    
}

void TranslatorWidget::draw1(){
    drawButton(m_btn_setting_load, m_img_load);
    drawButton(m_btn_setting_save, m_img_save);
    drawButton(m_btn_setting_delete, m_img_delete);
    //drawButton(m_btn_setting_reset, m_img_factory);
}

int TranslatorWidget::onMouse1(int x, int y){
    if(m_btn_setting_save.isActive(x, y)){
        m_page = 4;
        //m_page = 8;
    } else if(m_btn_setting_delete.isActive(x, y)){
        //m_page = 8;
        m_is_delete = true;
        openLoad();
    } else if(m_btn_setting_load.isActive(x, y)){
        //m_page = 8;
        m_is_delete = false;
        openLoad();
    }
    
    
    
    return 0;
}

//reset
void TranslatorWidget::setSize2(int width, int height){
}

void TranslatorWidget::draw2(){
    int y = m_height*0.3;
    drawText(Langage::getKey("REGLAGES_WANT_RESET"), m_x_middle, y, sizeText_bigbig, true);
   
    drawButtonLabel2(m_btn_setting_yes, COLOR_VALIDATE);
    drawButtonLabel2(m_btn_setting_no);
}

int TranslatorWidget::onMouse2(int x, int y){
    if(m_btn_setting_yes.isActive(x, y)){
        //RobotFrameworkV2 & f = RobotFrameworkV2::instance();
        //f.m_reglages_module.reset();
        m_page = 3;
    } else if(m_btn_setting_no.isActive(x, y)){
        open();
    }
    return 0;
}

void TranslatorWidget::setSize3(int width, int height){
}

void TranslatorWidget::draw3(){
    int y = m_height*0.3;
    drawText(Langage::getKey("REGLAGES_IS_RESET"), m_x_middle, y, sizeText_bigbig, true);
   
    drawButtonLabel2(m_btn_setting_ok, COLOR_VALIDATE);
}

int TranslatorWidget::onMouse3(int x, int y){
    if(m_btn_setting_ok.isActive(x, y)){
        open();
    }
    return 0;
}


//save
void TranslatorWidget::setSize4(int width, int height){
    int y = m_height*0.4;
    
    m_name.setResize(m_x_middle, y, m_width2*0.4);
}

void TranslatorWidget::draw4(){
    if(!m_keyboard_widget.m_close){
        m_keyboard_widget.draw();
    }
    int y = m_height*0.3;
    drawText(Langage::getKey("TRANSLATOR_OPEN"), m_x_middle, y, sizeText_bigbig, true);
    
    drawValueGuiKeyBoard(m_name);
   
    drawButtonLabel2(m_btn_setting_ok, COLOR_VALIDATE);
    drawButtonLabel2(m_btn_setting_cancel);
}

int TranslatorWidget::onMouse4(int x, int y){
    if(!m_keyboard_widget.m_close){
        m_keyboard_widget.onMouse(x, y);
    } else {
        if(isActiveValueGuiKeyBoard(m_name,x,y)){
            m_keyboard_widget.m_close = false;
            m_keyboard_widget.setValueGuiKeyBoard(&m_name);
        }
        if(m_btn_setting_ok.isActive(x, y)){
            Framework & f = Framework::instance();
            if(m_name.m_text.size() > 0){
                f.m_auto_path_module.init_save(m_name.m_text);
                f.setAntennaH();
                m_close = true;
            }
            
        } else if(m_btn_setting_cancel.isActive(x, y)){
            open();
        }
    }
    return 0;
}

void TranslatorWidget::setSize5(int width, int height){
}

void TranslatorWidget::draw5(){
    int y = m_height*0.3;
    drawText(Langage::getKey("REGLAGE_IS_SAVED"), m_x_middle, y, sizeText_bigbig, true);
   
    drawButtonLabel2(m_btn_setting_ok, COLOR_VALIDATE);
}

int TranslatorWidget::onMouse5(int x, int y){
    if(m_btn_setting_ok.isActive(x, y)){
        open();
    }
    return 0;
}


//load

void TranslatorWidget::openLoad(){
    m_page = 6;
    Framework & f = Framework::instance();
    
    m_reglages.clear();
    for(auto p : f.m_auto_path_module.m_auto_paths){
        m_reglages.addValue(p->getLabel());
    }
    m_select_widget.m_close = false;
    m_select_widget.setValueGuiKeyPad(&m_reglages);
}


void TranslatorWidget::setSize6(int width, int height){
    m_select_widget.setSizePart(m_x2, m_height*0.25, m_width2,m_height*0.7, width, height);
    m_select_widget.m_draw_rect = false;
    m_select_widget.m_draw_selected = false;
    m_select_widget.m_draw_button = false;
}

void TranslatorWidget::draw6(){
    int y = m_height*0.20;
    if(m_is_delete){
        drawText(Langage::getKey("TRANSLATOR_DELETE"), m_x_middle, y, sizeText_bigbig, true);
    } else {
        drawText(Langage::getKey("TRANSLATOR_OPEN"), m_x_middle, y, sizeText_bigbig, true);
    }
    
    m_select_widget.draw();
    
    drawButtonLabel2(m_btn_setting_cancel);
}

int TranslatorWidget::onMouse6(int x, int y){
    if(!m_select_widget.m_close){
        if(m_is_delete){
            if(m_select_widget.onMouseSelect(x, y)){
                m_last_reglage = m_reglages.getValueString();
                m_page = 7;
            };
        } else {
            if(m_select_widget.onMouseSelect(x, y)){
                m_last_reglage = m_reglages.getValueString();
                Framework & f = Framework::instance();
                m_page = 9;
                INFO("beginAutoPath " << m_last_reglage)
                f.m_auto_path_module.beginAutoPath(m_last_reglage);
            };
        }
        
    }
    if(m_btn_setting_cancel.isActive(x, y)){
        open();
    }
    return 0;
}

void TranslatorWidget::setSize7(int width, int height){
}

void TranslatorWidget::draw7(){
    int y = m_height*0.3;
    if(m_is_delete){
        drawText(Langage::getKey("TRANSLATOR_WANT_DELETE"), m_x_middle, y, sizeText_bigbig, true);
    } else {
        drawText(Langage::getKey("TRANSLATOR_WANT_LOAD"), m_x_middle, y, sizeText_bigbig, true);
    }
   
    drawButtonLabel2(m_btn_setting_yes, COLOR_VALIDATE);
    drawButtonLabel2(m_btn_setting_no);
}

int TranslatorWidget::onMouse7(int x, int y){
    if(m_btn_setting_yes.isActive(x, y)){
        Framework & f = Framework::instance();
        if(m_is_delete){
            f.m_auto_path_module.deletePath(m_last_reglage);
        } else {
            
        }
        openLoad();
    } else if(m_btn_setting_no.isActive(x, y)){
        open();
    }
    return 0;
}

void TranslatorWidget::setSize8(int width, int height){
}

void TranslatorWidget::draw8(){
    int y = m_height*0.3;
    if(m_is_delete){
        drawText(Langage::getKey("TRANSLATOR_IS_DELETE"), m_x_middle, y, sizeText_bigbig, true);
    } else {
        drawText(Langage::getKey("TRANSLATOR_IS_LOAD"), m_x_middle, y, sizeText_bigbig, true);
    }
    //drawText(Langage::getKey("REGLAGE_NOT_AVAILABLE"), m_x_middle, y, sizeText_bigbig, true);
   
    drawButtonLabel2(m_btn_setting_ok, COLOR_VALIDATE);
}

int TranslatorWidget::onMouse8(int x, int y){
    if(m_btn_setting_ok.isActive(x, y)){
        open();
    }
    return 0;
}

//load..
void TranslatorWidget::setSize9(int width, int height){
}

void TranslatorWidget::draw9(){
    Framework & f = Framework::instance();
    
    int y = m_height*0.3;
    if(m_is_delete){
        drawText(Langage::getKey("TRANSLATOR_IS_DELETE"), m_x_middle, y, sizeText_bigbig, true);
        drawButtonLabel2(m_btn_setting_ok, COLOR_VALIDATE);
    } else {
        double perc = f.m_auto_path_module.m_load_progress/100.0;
        if(perc >= 1.0){
            drawText(Langage::getKey("TRANSLATOR_IS_LOAD"), m_x_middle, y, sizeText_bigbig, true);
            drawButtonLabel2(m_btn_setting_ok, COLOR_VALIDATE);
        } else {
            drawText(Langage::getKey("TRANSLATOR_LOADING"), m_x_middle, y, sizeText_bigbig, true);
        }
    }
    //drawText(Langage::getKey("REGLAGE_NOT_AVAILABLE"), m_x_middle, y, sizeText_bigbig, true);
    
    y = m_height*0.5;
    int h =  m_height*0.1;
    double perc = f.m_auto_path_module.m_load_progress/100.0;
    double time_s = std::round(f.m_auto_path_module.m_time_ms/100)*0.1;
    if(perc > 1.0){
        perc = 1.0;
    }
    m_painter->setBrush(m_brush_white);
    m_painter->drawRoundedRect(m_width*0.05, y-h/2, m_width*0.8, h, 5, 5);
    m_painter->setBrush(m_brush_button_validate);
    m_painter->drawRoundedRect(m_width*0.05, y-h/2, perc*m_width*0.8, h, 5, 5);
    QString s = QString::number(std::round(perc*100))+ " % - " + QString::number(time_s) + " s ";
    drawQText(s, m_width*0.5, y, sizeText_medium, true);
    
    

}

int TranslatorWidget::onMouse9(int x, int y){
    if(m_btn_setting_ok.isActive(x, y)){
        Framework & f = Framework::instance();
        f.setAntennaH();
        m_close = true;
    }
    return 0;
}

void TranslatorWidget::setPainter(QPainter * s){
    BaseWidget::setPainter(s);
    m_select_widget.setPainter(s);
    m_keyboard_widget.setPainter(s);
}
