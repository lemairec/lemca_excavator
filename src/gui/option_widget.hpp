#ifndef OPTION_WIDGET_H
#define OPTION_WIDGET_H

#include "base/base_widget.hpp"
#include "base/select_widget.hpp"
#include "base/keyboard_widget.hpp"

class OptionWidget : public BaseWidget {
    QPixmap * m_img_off;
    QPixmap * m_img_plus;
    QPixmap * m_img_moins;
    
    QPixmap * m_imgSatBlanc;
    QPixmap * m_imgVolantBlanc;
    QPixmap * m_imgOutilBlanc;
    QPixmap * m_imgOptionBlanc;
    QPixmap * m_imgParcelle;
    QPixmap * m_img_tracteur;
    QPixmap * m_img_outil;
    QPixmap * m_img_hydraulic;
    QPixmap * m_img_cadena;
    QPixmap * m_img_settings1;
    QPixmap * m_img_settings_key;
    
    QPixmap * m_imgLeft;
    QPixmap * m_imgRight;
    QPixmap * m_imgMiddle;
    
    QPixmap * m_imgLeft2;
    QPixmap * m_imgRight2;
    QPixmap * m_img_return;
    
    
    ButtonGui m_button_return;
    
    ButtonGui m_button_close;
    ButtonGui m_button_p1;
    ButtonGui m_button_p2;
    ButtonGui m_button_p3;
    ButtonGui m_button_p4;
    ButtonGui m_button_p5;
    ButtonGui m_button_p6;
    ButtonGui m_button_p7;
    ButtonGui m_button_p8;
    ButtonGui m_button_p9;
    
    int m_page = 1;
    
    void addSerials();
    
     
    void isActiveButtonSelect(SelectButtonGui * select, int x, int y);
    void drawValueGui3(ValueGui &valueGui, double value, const std::string & label, int x);

public:
    KeyPadWidget m_keypad_widget;
    KeyBoardWidget m_keyboard_widget;
    SelectWidget m_select_widget;
    
    OptionWidget();
    
    void setSize(int width, int height);
    void loadImages();
    void drawButtons();
    virtual void draw();
    virtual int onMouse(int x, int y);
    
    //page1
    ButtonGui m_lum0;
    ButtonGui m_lum1;
    ButtonGui m_lum2;
    ButtonGui m_lum3;
    
    ButtonGui m_son0;
    ButtonGui m_son1;
    ButtonGui m_son2;
    ButtonGui m_son3;
    
    ValueGuiKeyBoard m_company;
    void setSizePage1();
    void drawPage1();
    void onMousePage1(int x, int y);
    
    //page2
    ValueGui m_outil_l;
    ValueGui m_outil_lg;
    
    ValueGui m_outil_record_h;
    ValueGui m_outil_replay_h;
    
    void setSizePage2();
    void drawPage2();
    void onMousePage2(int x, int y);
    
    //page3
    void setSizePage3();
    void drawPage3();
    void onMousePage3(int x, int y);
    
    void setSizePage4();
    void drawPage4();
    void onMousePage4(int x, int y);
    
    ButtonGui m_button_encoder_inverse;
    ButtonGui m_button_motor_inverse;
     
    ValueGui m_motor_vitesse_agressivite;
    ValueGui m_motor_vitesse_max;
    ValueGui m_motor_vitesse_min;
    
    ValueGui m_pas_by_angle;
    ValueGui m_angle_p;
    ValueGui m_angle_i;
    ValueGui m_angle_d;
    
    ValueGui m_volant_frequence;
    
    //page5
    ButtonGui m_button0;
    ButtonGui m_button1;
    ButtonGui m_button2;
    ButtonGui m_button3;
    ButtonGui m_button4;
    ButtonGui m_button5;
    ButtonGui m_button6;
    ButtonGui m_button7;
    ButtonGui m_button8;
    ButtonGui m_button9;
    void myDrawButton(ButtonGui * b, QString s);
    
    ValueGuiKeyBoard m_panel;
    ValueGuiKeyBoard m_mcu;
    ValueGuiKeyBoard m_licence;
    
    void setSizePage5();
    void drawPage5();
    void onMousePage5(int x, int y);
    
    /*ButtonGui m_button_p6connect;
    ButtonGui m_button_p6disable;
    ButtonGui m_button_p6clearError;
    
    ButtonGui m_button_droite;
    ButtonGui m_button_gauche;
    
    ButtonGui m_button_p6testGoToVLeft;
    ButtonGui m_button_p6testGoToV0;
    ButtonGui m_button_p6testGoToVRight;
    
    
    ButtonGui m_button_param;*/
    ButtonGui m_button_p6ClearData;
    
    //page 6
    ValueGui m_command_capteur_vitesse_max_mm_s;
    ValueGui m_command_capteur_p;
    ValueGui m_command_capteur_i;
    ValueGui m_command_capteur_d;
    
    ButtonGui m_btn_amplitude;
    ButtonGui m_button_command_auto_0;
    ValueGui m_point_3_delai;
    ButtonGui m_balise_enable;
    ButtonGui m_offset_cam1_2_calculate;
    
    ButtonGui m_btnTempRight;
    ButtonGui m_btnTempLeft;
    
    ButtonGui m_btnCapteurRight;
    ButtonGui m_btnCapteurLeft;
    ButtonGui m_btnCapteurZero;
    
    ButtonGui m_parametrage;
    ButtonGui m_parametrage2;
    void setSizePage6();
    void drawPage6();
    void onMousePage6(int x, int y);
    
    //page7
    SelectButtonGui m_select_gps_serial;
    SelectButtonGui m_select_gps_baudrates;
    
    ButtonGui m_button_get_antenna;
    ButtonGui m_button_set_antenna;
    void setSizePage7();
    void drawPage7();
    void onMousePage7(int x, int y);
    
    SelectButtonGui m_select_port1_gps_serial;
    SelectButtonGui m_select_port2_mcu_serial;
    SelectButtonGui m_select_port3_soil_serial;
    
    void open();
    
    void loadConfig();
    
    /*
     BASE
     */
    
    int m_width2, m_height2, m_x2, m_y2;
    int m_width3, m_y_title, m_y_begin, m_y_inter;
    int m_part_1_x, m_part_1_w, m_part_1_m, m_part_1_x2, m_part_1_x3, m_part_2_m, m_part_2_x, m_part_2_w, m_part_2_x2, m_part_2_x3;
    
    void drawPart1Title(double y, double h, const std::string & title, bool separateur = false);
    void drawPart2Title(double y, double h, const std::string & title, bool separateur = false);
    void setPainter(QPainter * p);
};

#endif //OPTION_WIDGET_H
