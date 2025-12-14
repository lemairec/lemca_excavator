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
    
    double y = 0.98*m_height2;
    m_button_add.setResizeStd(m_x2 + 0.4*m_width2, y, "add", true);
    m_button_import.setResizeStd(m_x2 + 0.6*m_width2, y, "import", true);
    m_button_clear.setResizeStd(m_x2 + 0.2*m_width2, y, "clear", true);
    
    
    m_button_close.setResizeStd(m_x2 + 0.8*m_width2, 0.9*m_height2, "close", true);
    m_button_page_up.setResizeStd(m_x2 + 0.4*m_width2, 0.9*m_height2, ">", true);
    m_button_page_down.setResizeStd(m_x2 + 0.2*m_width2, 0.9*m_height2, "<", true);
    
    m_latitude.setResize(m_x2 + 0.5*m_width2, 0.3*m_height2, m_petit_button);
    m_longitude.setResize(m_x2 + 0.5*m_width2, 0.4*m_height2, m_petit_button);
    m_name.setResize(m_x2 + 0.5*m_width2, 0.5*m_height2, m_height2*0.35);
    
    m_button_save.setResizeStd(m_x2 + 0.6*m_width2, 0.7*m_height2, "save", true);
    
    m_keypad_widget.setSize(width, height);
    m_keyboard_widget.setSize(width, height);
    m_select_widget.setSize(width, height);
    setSizeClear(width, height);
    setSizeImport(width, height);
}

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (std::getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}

void BalisesWidget::open(){
    BaseWidget::open();
    m_page = 0;
    m_mode = 0;
    
    std::string res = execute2("ls /media/lemca/");
    std::vector<std::string> strs;
    split(res, '\n');
    
    
    m_file_select.clear();
    for(auto s : strs){
        if(!s.empty()){
            INFO(s);
            m_file_select.addValue(s);
        }
    }
}

void BalisesWidget::setPainter(QPainter *p){
    m_painter = p;
    m_keypad_widget.setPainter(p);
    m_keyboard_widget.setPainter(p);
    m_select_widget.setPainter(p);
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
    } else if(m_mode == 3){
        drawClear();
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
    } else if(m_mode == 3   ){
        onMouseClear(x, y);
    }
    
    return 0;
}

void BalisesWidget::drawBalises(){
    Framework & f = Framework::instance();
    double y = 0.1*m_height2;
    double m_y_inter = 0.04*m_height2;
    double x1 = m_x2+0.05*m_width2;
    double x2 = m_x2+0.2*m_width2;
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
    drawButtonLabel2(m_button_clear, COLOR_CANCEL);
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
    if(m_button_clear.isActive(x, y)){
        m_mode =3;
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




/*if(m_select_widget.m_close){
    //m_select_gps_baudrates.setValueInt(f.m_config.m_gps_baudrate);
    //m_select_gps_serial.setValueString(f.m_config.m_gps_serial);
}
isActiveButtonSelect(&m_select_gps_serial, x, y);
isActiveButtonSelect(&m_select_gps_baudrates, x, y);*/
    
void BalisesWidget::setSizeImport(int width, int height){
    m_file_select.setResize(m_x2+0.5*m_width2, 0.5*m_height2,"", true, 0.4*m_width2);

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
    
    drawButtonLabel2(m_file_select.m_buttonOpen);
    if(!m_select_widget.m_close){
        m_select_widget.draw();
    }
}

/*m_close = false;

m_select_widget.setValueGuiKeyPad(&m_select_files);
*/

    /*if(m_file_widget.onMouse(x, y)){
        std::string s = "/media/lemca/"+m_file_widget.m_select_files.getValueString()+"/bineuse.tar.gz";
        call("sh " + DirectoryManager::instance().getSourceDirectory() + "/src/sh/bineuse_update_usb.sh " + s);
    } else {
        call("echo fail; exit 0;");
    }*/

int BalisesWidget::onMouseImport(int x, int y){
    if(!m_select_widget.m_close){
        if(m_select_widget.onMouseSelect(x, y)){
            //std::string s = "/media/lemca/"+m_file_widget.m_select_files.getValueString()+"/bineuse.tar.gz";
            //call("sh " + DirectoryManager::instance().getSourceDirectory() + "/src/sh/bineuse_update_usb.sh " + s);
        }
    }
    if(m_file_select.m_buttonOpen.isActive(x, y)){
        m_select_widget.open();
        m_select_widget.setValueGuiKeyPad(&m_file_select);
    }
    return 0;
}



void BalisesWidget::setSizeClear(int width, int height){
    double y = 0.6*m_height2;
    m_button_yes.setResizeStd(m_x2 + 0.6*m_width2, y, "Oui", true);
    m_button_no.setResizeStd(m_x2 + 0.2*m_width2, y, "Non", true);
    
}

void BalisesWidget::drawClear(){
    double y = 0.1*m_height2;
    double inter = 0.05*m_height2;
    drawText("Clear", m_x2+0.5*m_width2, y, sizeText_medium, true);
    y+=inter;
    y+=inter;
    drawText("Voulez vous tout supprimer?", m_x2+0.2*m_width2, y, sizeText_little);
    
    drawButtonLabel2(m_button_yes);
    drawButtonLabel2(m_button_no);
    
    
}

int BalisesWidget::onMouseClear(int x, int y){
    if(m_button_yes.isActive(x, y)){
        Framework::instance().m_balises.clear();
        open();
    }
    if(m_button_no.isActive(x, y)){
        open();
    }
    return 0;
}
