#pragma once

#include "base/base_widget.hpp"

class Config;

class ParamTranslateurWidget : public BaseWidget {
public:
    int m_page = 8;
    
    QPixmap * m_img_plus;
    QPixmap * m_img_moins;
    QPixmap * m_imgLeft;
    QPixmap * m_imgRight;
    QPixmap * m_imgMiddle;
    
    QPixmap * m_imgLeft2;
    QPixmap * m_imgRight2;
    
 
    ButtonGui m_button_next;
    ButtonGui m_button_cancel;
    ButtonGui m_button_end;
    
    double m_width2, m_height2;
    
    ParamTranslateurWidget();
    void loadConfig();
    
    void setSize(int width, int height);
    void draw();
    int onMouse(int x, int y);
    
    void open();
    
    //page1
    void setSizePage1(int width, int height);
    void drawPage1();
    void onMousePage1(int x, int y);
    
    //page2
    ButtonGui m_button_temp_gauche;
    ButtonGui m_button_temp_droite;
    ButtonGui m_button_temp_gauche2;
    ButtonGui m_button_temp_droite2;
    ButtonGui m_inverse;
    void setSizePage2(int width, int height);
    void drawPage2();
    void onMousePage2(int x, int y);
    
    //page3
    void setSizePage3(int width, int height);
    void drawPage3();
    void onMousePage3(int x, int y);
    
    //page4
    void setSizePage4(int width, int height);
    void drawPage4();
    void onMousePage4(int x, int y);
    
    //page5
    void setSizePage5(int width, int height);
    void drawPage5();
    void onMousePage5(int x, int y);
    
    //page5
    ValueGui m_command_bineuse_debatement_mm;
    void setSizePage6(int width, int height);
    void drawPage6();
    void onMousePage6(int x, int y);
    
    //page6
    ValueGui m_command_capteur_p;
    ValueGui m_command_capteur_i;
    ButtonGui m_command_gauche;
    ButtonGui m_command_middle;
    ButtonGui m_command_droite;
    void setSizePage7(int width, int height);
    void drawPage7();
    void onMousePage7(int x, int y);
    
    ButtonGui m_proportionnal;
    void setSizePage8(int width, int height);
    void drawPage8();
    void onMousePage8(int x, int y);
    
    
    //page9
    bool m_time_page = true;
    double m_time2 = 10;
    int m_propo_value2 = 10;
    ValueGui m_propo_value;
    ValueGui m_time;
    ButtonGui m_button_temp_gauche_prop;
    ButtonGui m_button_temp_droite_prop;
    ButtonGui m_button_temp_gauche_onoff;
    ButtonGui m_button_temp_droite_onoff;
    
    
    double m_d_mm = 100;
    ValueGui m_translateur_p;
    ValueGui m_translateur_cm_s;
    ValueGui m_translateur_d_mm;
    ButtonGui m_button_dist_droite;
    ButtonGui m_button_dist_gauche;
    ButtonGui m_button_dist_middle;
    
    
    ButtonGui m_button_next_time;
    
    
    void setSizePage9(int width, int height);
    void drawPage9();
    void onMousePage9(int x, int y);
    
};
