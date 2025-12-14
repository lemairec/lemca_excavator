#pragma once

#include "base/base_widget.hpp"
#include "base/keyboard_widget.hpp"

class Config;


class BalisesWidget : public BaseWidget {
    ButtonGui m_button_close;
    
    ButtonGui m_button_page_up;
    ButtonGui m_button_page_down;
    
    ButtonGui m_button_add;
    ButtonGui m_button_import;
    ButtonGui m_button_clear;
    
    
    int m_page = 0;
    
    int m_mode = 0;
public:
    int m_x2, m_y2;
    int m_width2, m_height2;
    BalisesWidget();
    
    KeyPadWidget m_keypad_widget;
    KeyBoardWidget m_keyboard_widget;
    
    void setPainter(QPainter *p);
    
    void setSize(int width, int height);
    void draw();
    
    void open();
    
    void setSizeBalises(int width, int height);
    void drawBalises();
    int onMouseBalises(int x, int y);
    
    ValueGui m_latitude;
    ValueGui m_longitude;
    ValueGuiKeyBoard m_name;
    
    ButtonGui m_button_save;
    void setSizeAdd(int width, int height);
    void drawAdd();
    int onMouseAdd(int x, int y);
    
    void setSizeImport(int width, int height);
    void drawImport();
    int onMouseImport(int x, int y);
    
    ButtonGui m_button_yes;
    ButtonGui m_button_no;
    
    void setSizeClear(int width, int height);
    void drawClear();
    int onMouseClear(int x, int y);
    
    void importFile(const std::string & path);
    
    
    int onMouse(int x, int y);
};

