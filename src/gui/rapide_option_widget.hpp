#pragma once

#include "base/base_widget.hpp"
#include "base/keyboard_widget.hpp"

class Config;

class RapideOptionWidget : public BaseWidget {
    KeyPadWidget m_keypad_widget;
public:
    int m_x = 0;
    int m_width2 = 0;
    int m_x_middle = 0;
    int m_x_left = 0;
    int m_x1 = 0;
    int m_x2 = 0;
    int m_y = 0;
    int m_height2 = 0;
    int m_y_title = 0;
    
    int m_page = 1;
    int m_nbr_page = 8;
    
    ButtonGui m_button_close;
    ButtonGui m_button_next;
    
    
    RapideOptionWidget();

    void loadConfig();
    void resetConfig();
    
    void open();
    
    void setPainter(QPainter * p){
        m_painter = p;
        m_keypad_widget.setPainter(p);
    }
    void setSize(int width, int height);
    void loadImages();
    void draw();
    int onMouse(int x, int y);
    
    QString m_avance_label_av = "";
    QString m_avance_label_re = "";
    ButtonGui m_avanced_bw;
    bool m_avance_black_white = false;
    
    //page1
    ButtonGui m_new_point;
    
    
    
    void setSizePage1(int width, int height);
    void drawPage1();
    void onMousePage1(int x, int y);
    
    //page2
    ButtonGui m_synchro;
    void setSizePage2(int width, int height);
    void drawPage2();
    void onMousePage2(int x, int y);
    
    //page3
    ButtonGui m_mode_balise;
    void setSizePage3(int width, int height);
    void drawPage3();
    void onMousePage3(int x, int y);
    
    //page4
    ButtonGui m_save_point;
    ValueGui m_valuegui_profondeur;
    void setSizePage4(int width, int height);
    void drawPage4();
    void onMousePage4(int x, int y);
    
    void draw(ValueGui & keypad, std::string);
    void drawFalse(ValueGui & keypad, std::string);
    void onMouse(ValueGui & keypad, double x, double y, double pas = 1.0);
    
    //page5
    ButtonGui m_button_right;
    ButtonGui m_button_left;
    ButtonGui m_button_lamp;
    void setSizePage5(int width, int height);
    void drawPage5();
    void onMousePage5(int x, int y);
    
    //page5
    ButtonGui m_button_reset;
    ButtonGui m_button_diag;
    ValueGui m_largeur;
    void setSizePage6(int width, int height);
    void drawPage6();
    void onMousePage6(int x, int y);
    
    
    void drawPage7();
    void drawPage8();
};
