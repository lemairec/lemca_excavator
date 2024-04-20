#ifndef REGLAGES_WIDGET_H
#define REGLAGES_WIDGET_H

#include "../base/base_widget.hpp"
#include "../base/keyboard_widget.hpp"
#include "../base/select_widget.hpp"


class TranslatorWidget : public BaseWidget {
    
public:
    TranslatorWidget();
    
    KeyBoardWidget m_keyboard_widget;
    SelectWidget m_select_widget;
    
    int m_y_title;
    int m_x2, m_y2;
    int m_width2, m_height2;
    
    int m_x_middle;
    
    int m_page = 1;
    
    QPixmap * m_img_save;
    QPixmap * m_img_load;
    QPixmap * m_img_delete;
    //QPixmap * m_img_factory;
    
    QPixmap * m_img_option;
    ButtonGui m_button_option;
    
    
    void drawButton(ButtonGui& button, QPixmap * qpixmap);
  
    
    ButtonGui m_btn_setting_yes;
    ButtonGui m_btn_setting_no;
    ButtonGui m_btn_setting_ok;
    ButtonGui m_btn_setting_cancel;
    ButtonGui m_btn_setting_quit;
    
    void loadImages();
    void setSize(int width, int height);
    void drawButtons();
    void draw();
    int onMouse(int x, int y);
    void open();
    
    //page1
    ButtonGui m_btn_setting_load;
    ButtonGui m_btn_setting_save;
    ButtonGui m_btn_setting_delete;
    //ButtonGui m_btn_setting_reset;
    
    void setSize1(int width, int height);
    void draw1();
    int onMouse1(int x, int y);
    
    //page2 reset_valid
    void setSize2(int width, int height);
    void draw2();
    int onMouse2(int x, int y);
    
    //page3 reset_ok
    void setSize3(int width, int height);
    void draw3();
    int onMouse3(int x, int y);
    
    //page4 save
    ValueGuiKeyBoard m_name;
    void setSize4(int width, int height);
    void draw4();
    int onMouse4(int x, int y);
    
    //page5 save_ok
    void setSize5(int width, int height);
    void draw5();
    int onMouse5(int x, int y);
    
    //page6 load
    std::string m_last_reglage;
    bool m_is_delete = false;
    SelectButtonGui m_reglages;

    void openLoad();
    void setSize6(int width, int height);
    void draw6();
    int onMouse6(int x, int y);
    
    //page7 delete_valid
    void setSize7(int width, int height);
    void draw7();
    int onMouse7(int x, int y);
    
    //page8 delete_ok
    void setSize8(int width, int height);
    void draw8();
    int onMouse8(int x, int y);
    
    //page8 load_ok
    void setSize9(int width, int height);
    void draw9();
    int onMouse9(int x, int y);
    
    void setPainter(QPainter * s);
};

#endif
