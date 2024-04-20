#include "param_translateur_widget.hpp"

#include "../framework.hpp"
#include "qt/main_window.hpp"
#include "../config/langage.hpp"

ParamTranslateurWidget::ParamTranslateurWidget(){
    
    m_img_plus = loadImage("/gui/plus.png");
    m_img_moins = loadImage("/gui/moins.png");
    
    m_imgLeft = loadImage("/gui/left_n.png");
    m_imgRight = loadImage("/gui/right_n.png");
    m_imgMiddle = loadImage("/gui/middle_n.png");
    
    m_imgLeft2 = loadImage("/gui/left2.png");
    m_imgRight2 = loadImage("/gui/right2.png");
    
    m_img_check_on = loadImage("/gui/check_on.png");
    m_img_check_off = loadImage("/gui/check_off.png");
    
    m_close = true;
}

void ParamTranslateurWidget::open(){
    BaseWidget::open();
    m_page = 1;
}


void ParamTranslateurWidget::loadConfig(){
    Framework::instance().m_config.save();
    Framework::instance().initOrLoadConfig();
}


void ParamTranslateurWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    m_width2 = m_width;
    m_height2 = m_height;
    
    m_button_next.setResize(0.6*m_width, 0.8*m_height, QString::fromStdString(Langage::getKey("NEXT")), true);
    m_button_cancel.setResize(0.4*m_width, 0.8*m_height, QString::fromStdString(Langage::getKey("CANCEL")), true);
    m_button_end.setResize(0.7*m_width, 0.8*m_height, QString::fromStdString(Langage::getKey("END")), true);
    setSizePage1(width, height);
    setSizePage2(width, height);
    setSizePage3(width, height);
    setSizePage4(width, height);
    setSizePage5(width, height);
    setSizePage6(width, height);
    setSizePage7(width, height);
    setSizePage8(width, height);
    setSizePage9(width, height);
}


void ParamTranslateurWidget::draw(){
    Framework & f = Framework::instance();
    int m_y2 = m_height*0.04;
    int m_x2 = m_y2;
    int m_width2 = m_width-2*m_y2;
    int m_height2 = m_height-2*m_y2;
    
    m_painter->setBrush(m_brush_background_1);
    m_painter->drawRect(0 , 0, m_width, m_height);
    
    m_painter->setPen(m_pen_black);
    m_painter->setBrush(m_brush_white);
    m_painter->drawRoundedRect(m_x2, m_y2, m_width2, m_height2, RAYON_ROUNDED, RAYON_ROUNDED);
    
    drawText(Langage::getKey("PARAMS_TITLE"), m_width2*0.5, m_height2*0.1+40, sizeText_big, true);
    //m_painter->setBrush(m_brushDarkGray);
    //m_painter->drawRect(m_width*0.943+5, m_height*0.1, m_width*0.07-5, m_height*0.8);
    
    double value = std::round(f.m_pilot_translator_module.m_pas_mesured_2*10*5.0/1024)/10.0;
    QString s = QString::fromStdString(Langage::getKey("PARAMS_PAS")) + QString::number(value, 'f', 1) + " V (0-5 V)";
    drawQText(s, m_width2*0.1, m_height2*0.95, sizeText_medium, false);
    
    if(m_page == 7 || m_page == 8 || m_page == 9){
        QString s = "mesured : " + QString::number(std::round(f.m_pilot_translator_module.m_pas_mesured_mm_liss)/10.0, 'f', 1) + " cm";
        drawQText(s, m_width2*0.4, m_height2*0.92, sizeText_medium, false);
        QString s2 = "desired : " + QString::number(std::round(f.m_pilot_translator_module.m_pas_desired_mm)/10.0, 'f', 1) + " cm ( " + QString::number(std::round(f.m_pilot_translator_module.m_pas_desired2_mm)/10.0, 'f', 1) + " )";
        drawQText(s2, m_width2*0.4, m_height2*0.95, sizeText_medium, false);
        
        QString s3 = "[ " + QString::number(f.m_config.m_command_capteur_left) + " - " + QString::number(f.m_config.m_command_capteur_middle) + " - " + QString::number(f.m_config.m_command_capteur_right) + " ]";
        drawQText(s3, m_width2*0.4, m_height2*0.98, sizeText_medium, false);
    
    }
    
    m_painter->setPen(m_pen_black);
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
    } else if(m_page == 9){
        drawPage9();
    }
    
}

int ParamTranslateurWidget::onMouse(int x, int y){
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
    } else if(m_page == 7){
        onMousePage7(x, y);
    } else if(m_page == 8){
        onMousePage8(x, y);
    } else if(m_page == 9){
        onMousePage9(x, y);
    }
    return 0;
}

/*void ParamTranslateurWidget::open(){
 m_close = false;
 //addSerials();
 }*/


/**
 PAGE 1
 */

void ParamTranslateurWidget::setSizePage1(int width, int height){
};

void ParamTranslateurWidget::drawPage1(){
    int x = m_width*0.2;
    
    drawText(Langage::getKey("PARAMS_P1_BEGIN"), x, m_height*0.4, sizeText_medium);
    drawText(Langage::getKey("PARAMS_P1_SECURITY"), x, m_height*0.5, sizeText_medium);
    
    drawButtonLabel2(m_button_next, COLOR_VALIDATE);
    drawButtonLabel2(m_button_cancel, COLOR_CANCEL);
}

void ParamTranslateurWidget::onMousePage1(int x, int y){
    if(m_button_next.isActive(x, y)){
        m_page++;
    }
    if(m_button_cancel.isActive(x, y)){
        m_close = true;
    }
}


/**
 PAGE 2
 */

void ParamTranslateurWidget::setSizePage2(int width, int height){
    m_button_temp_droite.setResize(0.6*m_width, 0.6*m_height, m_gros_button);
    m_button_temp_gauche.setResize(0.4*m_width, 0.6*m_height, m_gros_button);
    m_button_temp_droite2.setResize(0.6*m_width, 0.7*m_height, m_petit_button);
    m_button_temp_gauche2.setResize(0.4*m_width, 0.7*m_height, m_petit_button);
    m_inverse.setResize(0.25*m_width, 0.45*m_height, m_petit_button);
};



void ParamTranslateurWidget::drawPage2(){
    Config & config = Framework::instance().m_config;
    int x = m_width*0.2;
    
    drawText(Langage::getKey("PARAMS_P2_TEST"), x, m_height*0.3, sizeText_medium);
    drawText(Langage::getKey("PARAMS_P2_INVERSE_QUESTION"), x, m_height*0.35, sizeText_medium);
    
    {
        drawButtonImage(m_button_temp_gauche, m_imgLeft2);
        drawButtonImage(m_button_temp_gauche2, m_imgLeft2, 0.5);
        drawText(Langage::getKey("LEFT"), m_button_temp_gauche.m_x, m_button_temp_gauche.m_y-40, sizeText_medium, true);
    }
    {
        drawButtonImage(m_button_temp_droite, m_imgRight2);
        drawButtonImage(m_button_temp_droite2, m_imgRight2, 0.5);
        drawText(Langage::getKey("RIGHT"), m_button_temp_droite.m_x, m_button_temp_droite.m_y-40, sizeText_medium, true);
    }
    
    drawButtonCheck(m_inverse, config.m_inverse_relai, Langage::getKey("INVERSE"));
    
    drawButtonLabel2(m_button_next, COLOR_VALIDATE);
}

void ParamTranslateurWidget::onMousePage2(int x, int y){
    Config & config = Framework::instance().m_config;
    
    if(m_inverse.isActive(x, y) != 0){
        config.m_inverse_relai = !config.m_inverse_relai;
        loadConfig();
    }
    
    if(m_button_temp_gauche.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.openRelayLeft(config.m_temp_deplacement_ms*4);
    }
    if(m_button_temp_droite.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.openRelayRight(config.m_temp_deplacement_ms*4);
    }
    if(m_button_temp_gauche2.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.openRelayLeft(config.m_temp_deplacement_ms);
    }
    if(m_button_temp_droite2.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.openRelayRight(config.m_temp_deplacement_ms);
    }
    
    if(m_button_next.isActive(x, y)){
        m_page++;
    }
}

/**
 PAGE3
 */
double pas_left = 0;
double pas_right = 0;


void ParamTranslateurWidget::setSizePage3(int width, int height){
    /*m_button_temp_droite.setResize(0.4*m_width, 0.6*m_height, m_petit_button);
     m_button_temp_gauche.setResize(0.6*m_width, 0.6*m_height, m_petit_button);
     m_inverse.setResize(0.25*m_width, 0.5*m_height, m_petit_button);*/
};



void ParamTranslateurWidget::drawPage3(){
    int x = m_width*0.2;
    
    drawText(Langage::getKey("PARAMS_P3_MAX_LEFT"), x, m_height*0.4, sizeText_medium);
    
    {
        drawButtonImage(m_button_temp_gauche, m_imgLeft2);
        drawButtonImage(m_button_temp_gauche2, m_imgLeft2, 0.5);
        drawText(Langage::getKey("LEFT"), m_button_temp_gauche.m_x, m_button_temp_gauche.m_y-40, sizeText_medium, true);
    }
    
    drawButtonLabel2(m_button_next, COLOR_VALIDATE);
}

void ParamTranslateurWidget::onMousePage3(int x, int y){
    Framework & f = Framework::instance();
    Config & config = f.m_config;
    
    if(m_button_temp_gauche.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.openRelayLeft(config.m_temp_deplacement_ms*4);
    }
    if(m_button_temp_gauche2.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.openRelayLeft(config.m_temp_deplacement_ms);
    }
    
    if(m_button_next.isActive(x, y)){
        pas_left = f.m_pilot_translator_module.m_pas_mesured_2;
        m_page++;
    }
}

/**
 PAGE4
 */

void ParamTranslateurWidget::setSizePage4(int width, int height){
    /*m_button_temp_droite.setResize(0.4*m_width, 0.6*m_height, m_petit_button);
     m_button_temp_gauche.setResize(0.6*m_width, 0.6*m_height, m_petit_button);
     m_inverse.setResize(0.25*m_width, 0.5*m_height, m_petit_button);*/
};



void ParamTranslateurWidget::drawPage4(){
    int x = m_width*0.2;
    
    drawText(Langage::getKey("PARAMS_P4_MAX_RIGHT"), x, m_height*0.4, sizeText_medium);
    
    {
        drawButtonImage(m_button_temp_droite, m_imgRight2);
        drawButtonImage(m_button_temp_droite2, m_imgRight2, 0.5);
        drawText(Langage::getKey("RIGHT"), m_button_temp_droite.m_x, m_button_temp_droite.m_y-40, sizeText_medium, true);
    }
    
    drawButtonLabel2(m_button_next, COLOR_VALIDATE);
}

void ParamTranslateurWidget::onMousePage4(int x, int y){
    Framework & f = Framework::instance();
    Config & config = Framework::instance().m_config;
    
    if(m_inverse.isActive(x, y) != 0){
        config.m_inverse_relai = !config.m_inverse_relai;
        loadConfig();
    }
    
    if(m_button_temp_gauche.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.openRelayLeft(config.m_temp_deplacement_ms*4);
    }
    if(m_button_temp_droite.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.openRelayRight(config.m_temp_deplacement_ms*4);
    }
    if(m_button_temp_gauche2.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.openRelayLeft(config.m_temp_deplacement_ms);
    }
    if(m_button_temp_droite2.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.openRelayRight(config.m_temp_deplacement_ms);
    }
    
    if(m_button_next.isActive(x, y)){
        pas_right = f.m_pilot_translator_module.m_pas_mesured_2;
        m_page++;
    }
}


/**
 PAGE5
 */

void ParamTranslateurWidget::setSizePage5(int width, int height){
    m_command_bineuse_debatement_mm.setResize(0.25*m_width, 0.5*m_height, m_petit_button, "");
};



void ParamTranslateurWidget::drawPage5(){
    int x = m_width*0.2;
    
    drawText(Langage::getKey("PARAMS_P5_CENTER"), x, m_height*0.4, sizeText_medium);
    
    {
        drawButtonImage(m_button_temp_gauche, m_imgLeft2);
        drawButtonImage(m_button_temp_gauche2, m_imgLeft2, 0.5);
        drawText(Langage::getKey("LEFT"), m_button_temp_gauche.m_x, m_button_temp_gauche.m_y-40, sizeText_medium, true);
    }
    {
        drawButtonImage(m_button_temp_droite, m_imgRight2);
        drawButtonImage(m_button_temp_droite2, m_imgRight2, 0.5);
        drawText(Langage::getKey("RIGHT"), m_button_temp_droite.m_x, m_button_temp_droite.m_y-40, sizeText_medium, true);
    }
    
    Framework & f = Framework::instance();
    Config & config = f.m_config;
    if(config.isTechnicien()){
        drawQText(QString::number(pas_left/1024.0*5.0, 'f', 1)+" V", 0.3*m_width, 0.5*m_height, sizeText_medium, true);
        drawQText(QString::number(f.m_pilot_translator_module.m_pas_mesured_2/1024.0*5.0, 'f', 1)+" V", 0.5*m_width, 0.5*m_height, sizeText_medium, true);
        drawQText(QString::number(pas_right/1024.0*5.0, 'f', 1)+" V", 0.7*m_width, 0.5*m_height, sizeText_medium, true);
    }
    
    drawButtonLabel2(m_button_next, COLOR_VALIDATE);
}

void ParamTranslateurWidget::onMousePage5(int x, int y){
    Framework & f = Framework::instance();
    Config & config = f.m_config;
    
    if(m_button_temp_gauche.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.openRelayLeft(config.m_temp_deplacement_ms*4);
    }
    if(m_button_temp_droite.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.openRelayRight(config.m_temp_deplacement_ms*4);
    }
    if(m_button_temp_gauche2.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.openRelayLeft(config.m_temp_deplacement_ms);
    }
    if(m_button_temp_droite2.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.openRelayRight(config.m_temp_deplacement_ms);
    }
    
    if(m_button_next.isActive(x, y)){
        double l = pas_right - pas_left;
        if(l == 0){
            l=2;
        }
        double middle = f.m_pilot_translator_module.m_pas_mesured_2;
        
        config.m_command_capteur_middle = middle;
        config.m_command_capteur_left = pas_left;
        config.m_command_capteur_right = pas_right;
        
        
        loadConfig();
        m_page++;
    }
}

/**
 PAGE6
 */

void ParamTranslateurWidget::setSizePage6(int width, int height){
    m_command_bineuse_debatement_mm.setResize(0.6*m_width, 0.5*m_height, m_petit_button, "");
};



void ParamTranslateurWidget::drawPage6(){
    Config & config = Framework::instance().m_config;
    int x = m_width*0.2;
    
    drawText(Langage::getKey("PARAMS_P6_MESURE"), x, m_height*0.4, sizeText_medium);
    
    {
        drawText(Langage::getKey("PARAMS_P6_DEBATTEMENT_CM"), m_width*0.3, m_command_bineuse_debatement_mm.m_y, sizeText_medium);
        m_command_bineuse_debatement_mm.m_value = config.m_command_bineuse_debatement_mm/10.0;
        drawValueGuiKeyPad2(m_command_bineuse_debatement_mm);
    }
    
    drawButtonLabel2(m_button_next, COLOR_VALIDATE);
}

void ParamTranslateurWidget::onMousePage6(int x, int y){
    Config & config = Framework::instance().m_config;
    
    if(m_command_bineuse_debatement_mm.isActive(x, y) != 0){
        config.m_command_bineuse_debatement_mm += 10*m_command_bineuse_debatement_mm.isActive(x, y);
        loadConfig();
    }
    
    if(m_button_next.isActive(x, y)){
        //config.m_algo_command = TypeAlgoCommand_Capteur;
        loadConfig();
        m_page++;
    }
}


/**
 PAGE7
 */

void ParamTranslateurWidget::setSizePage7(int width, int height){
    m_proportionnal.setResize(0.3*m_width, 0.5*m_height, m_petit_button);
    

};



void ParamTranslateurWidget::drawPage7(){
    Config & config = Framework::instance().m_config;
    int x = m_width*0.2;
    
    drawText(Langage::getKey("PARAMS_P7_PROPORTIONNAL"), x, m_height*0.3, sizeText_medium);
    
    {
        drawButtonCheck(m_proportionnal, config.m_proportionnal, Langage::getKey("PARAMS_P7_PROPORTIONNAL2"));
    }
    
    
    drawButtonLabel2(m_button_next, COLOR_VALIDATE);
}

void ParamTranslateurWidget::onMousePage7(int x, int y){
    Config & config = Framework::instance().m_config;
    
    if(m_proportionnal.isActive(x, y) != 0){
        config.m_proportionnal = !config.m_proportionnal;
        loadConfig();
    }
    if(m_button_next.isActive(x, y)){
        m_page++;
    }
}


/**
 PAGE8
 */

void ParamTranslateurWidget::setSizePage8(int width, int height){
    m_command_capteur_p.setResize(0.6*m_width, 0.5*m_height, m_petit_button, "");
    //m_command_capteur_i.setResize(0.75*m_width, 0.45*m_height, m_petit_button, "");
    m_command_gauche.setResize(0.4*m_width, 0.65*m_height, m_petit_button);
    m_command_middle.setResize(0.5*m_width, 0.65*m_height, m_petit_button);
    m_command_droite.setResize(0.6*m_width, 0.65*m_height, m_petit_button);

};



void ParamTranslateurWidget::drawPage8(){
    Config & config = Framework::instance().m_config;
    int x = m_width*0.2;
    
    drawText(Langage::getKey("PARAMS_P7_AGRESSIVITY"), x, m_height*0.3, sizeText_medium);
    drawText(Langage::getKey("PARAMS_P7_PUT_MAX"), x, m_height*0.35, sizeText_medium);
    
    {
        drawText(Langage::getKey("PARAMS_P7_AGRESS"), m_width*0.3, m_command_capteur_p.m_y, sizeText_medium);
        m_command_capteur_p.m_value = config.m_command_capteur_p;
        drawValueGuiKeyPad2(m_command_capteur_p);
    }
    
    {
        drawButtonImage(m_command_gauche, m_imgLeft);
        QString s = QString::fromStdString(Langage::getKey("LEFT")) + " " + QString::number(-100/10.0) + " cm";
        drawQText(s, m_command_gauche.m_x, m_command_gauche.m_y-30, sizeText_medium, true);
    }
    {
        drawButtonImage(m_command_middle, m_imgMiddle);
        drawQText(QString::fromStdString(Langage::getKey("MIDDLE")), m_command_middle.m_x, m_command_middle.m_y-30, sizeText_medium, true);
    }
    
    {
        drawButtonImage(m_command_droite, m_imgRight);
        QString s = QString::fromStdString(Langage::getKey("RIGHT")) + " " + QString::number(100/10.0) + " cm";
        drawQText(s, m_command_droite.m_x, m_command_droite.m_y-30, sizeText_medium, true);
    }
    
    drawButtonLabel2(m_button_end, COLOR_VALIDATE);
}

void ParamTranslateurWidget::onMousePage8(int x, int y){
    Config & config = Framework::instance().m_config;
    
    if(m_command_capteur_p.isActive(x, y) != 0){
        config.m_command_capteur_p += 5*m_command_capteur_p.isActive(x, y);
        loadConfig();
    }
    
    /*if(m_command_capteur_i.isActive(x, y) != 0){
        config.m_command_capteur_i.multBy(m_command_capteur_i.getMultValue(x, y));
        loadConfig();
    }*/
    
    if(m_command_gauche.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.test_mm(-100);
    
    }
    if(m_command_middle.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.test_mm(0);
    
    }
    if(m_command_droite.isActive(x, y) != 0){
        Framework::instance().m_pilot_translator_module.test_mm(100);
    
    }
    if(m_button_end.isActive(x, y)){
        m_close = true;
    }
}


/**
 PAGE 9
 */

int y_prop;
int y_onoff;
void ParamTranslateurWidget::setSizePage9(int width, int height){
    int y = m_height*0.2;
    int inter = m_height*0.06;
    m_button_next_time.setResizeStd(0.85*m_width, y, "next", true);
    y+=2*inter;
    m_time.setResize(0.8*m_width, y, m_petit_button);
    y+=inter*1.2;
    y_prop = y;
    y+=inter;
    m_propo_value.setResize(0.8*m_width, y, m_petit_button);
    y+=inter;
    m_button_temp_gauche_prop.setResizeStd(0.7*m_width, y, "left prop", true);
    m_button_temp_droite_prop.setResizeStd(0.8*m_width, y, "right prop", true);
    y+=inter*1.2;
    y_onoff = y;
    y+=inter;
    m_button_temp_gauche_onoff.setResizeStd(0.7*m_width, y, "left", true);
    m_button_temp_droite_onoff.setResizeStd(0.8*m_width, y, "right", true);
    y+=inter*1.5;
    
    
    
    y = m_height*0.2;
    y+=2*inter;
    m_translateur_p.setResize(0.8*m_width, y, m_petit_button);
    y+=inter;
    m_translateur_cm_s.setResize(0.8*m_width, y, m_petit_button);
    y+=inter*1.5;
    m_translateur_d_mm.setResize(0.8*m_width, y, m_petit_button);
    y+=inter*1.5;
    m_button_dist_gauche.setResizeStd(0.65*m_width, y, "left dist", true);
    m_button_dist_middle.setResizeStd(0.75*m_width, y, "middle", true);
    m_button_dist_droite.setResizeStd(0.85*m_width, y, "right dist", true);
    
};



void ParamTranslateurWidget::drawPage9(){
    Framework & f = Framework::instance();
    int x = m_width*0.05;
    int y = m_height*0.2;
    int width = m_width*0.5;
    int height = m_height*0.45;
    //drawText("Courbe", x, m_height*0.2, sizeText_medium);
    m_painter->setPen(m_pen_black);
    m_painter->setBrush(m_brush_white);
    m_painter->drawRect(x, y, width, height);
    m_painter->setBrush(m_brush_button_fail);
    
    {
        double y2 = my_map(f.m_config.m_command_capteur_left, 0, 1025, 0, height);
        y2 = y+height-y2;
        m_painter->drawLine(x, y2, x+width, y2);
    }
    {
        double y2 = my_map(f.m_config.m_command_capteur_right, 0, 1025, 0, height);
        y2 = y+height-y2;
        m_painter->drawLine(x, y2, x+width, y2);
    }
    {
        double y2 = my_map(0, 0, 1025, 0, height);
        y2 = y+height-y2;
        m_painter->drawLine(x, y2, x+width, y2);
    }
                           
    m_painter->setBrush(m_brush_button_validate);
    /*for(size_t i = 0; i < f.m_pilot_module.m_last_values_pas_left.size(); ++i){
        int res = f.m_pilot_module.m_last_values_pas_left[i];
        double x2 = my_map(i, 0, frequence*time_record, x, x+width);
        double y2 = my_map(res, 0, 1025, 0, height);
        y2 = y+height-y2;
        
        INFO(i << " " << x2 << " " << y2 << "-" << x << " " << x+width << " - " << y << " " << y+height)
        
        m_painter->drawEllipse(x2,y2,3,3);
        
    }*/
    
    m_painter->setBrush(m_brush_green);
    /*for(size_t i = 0; i < f.m_pilot_module.m_last_values_pas_right.size(); ++i){
        int res = f.m_pilot_module.m_last_values_pas_right[i];
        double x2 = my_map(i, 0, frequence*time_record, x, x+width);
        double y2 = my_map(res, 0, 1025, 0, height);
        y2 = y+height-y2;
        
        INFO(i << " " << x2 << " " << y2 << "-" << x << " " << x+width << " - " << y << " " << y+height)
        
        m_painter->drawEllipse(x2,y2,3,3);
        
    }*/
    
    //int x1 = 0.1*m_width;
    y +=height;
    int inter = 0.03*m_height;
    y+=inter;
    /*if(f.getLastSerial().size() > 0){
        drawQText("version : ", x1, y, sizeText_little, false);
    } else {
        drawQText("no_serial", x1, y, sizeText_little, false);
    }*/
    /*y+=inter;
    drawText(f.m_pilot_module.m_pilot_version, x1, y, sizeText_little, false);
    y+=inter;
    drawQText("---", x1, y, sizeText_little, false);
    y+=inter;
    drawText(f.getLastSerial(), x1, y, sizeText_little, false);
    y+=inter;
    drawText(f.m_pilot_module.m_last_order_send, x1, y, sizeText_little, false);*/
    
    
    if(m_time_page){
        drawText("TIME MOVING", 0.65*m_width, m_button_next_time.m_y, sizeText_medium, true);
        drawButtonLabel2(m_button_temp_droite_prop, COLOR_VALIDATE);
        drawButtonLabel2(m_button_temp_gauche_prop, COLOR_VALIDATE);
        
        {
            drawText("time (s)", 0.6*m_width, m_time.m_y, sizeText_little);
            
            m_time.m_value = m_time2;
            drawValueGuiKeyPad2(m_time);
        }
        if(true){
            drawText("proportionnal enable", 0.75*m_width, y_prop, sizeText_little, true);
        } else {
            drawText("proportionnal disable", 0.75*m_width, y_prop, sizeText_little, true);
        }
        {
            drawText("pwm %", 0.6*m_width, m_propo_value.m_y, sizeText_little);
            
            m_propo_value.m_value = m_propo_value2;
            drawValueGuiKeyPad2(m_propo_value);
        }
        {
            drawText("on / off", 0.75*m_width, y_onoff, sizeText_little, true);
        }
        drawButtonLabel2(m_button_temp_droite_onoff, COLOR_VALIDATE);
        drawButtonLabel2(m_button_temp_gauche_onoff, COLOR_VALIDATE);
    } else {
        drawText("POSITION MOVING", 0.65*m_width, m_button_next_time.m_y, sizeText_medium, true);
        Config & config = Framework::instance().m_config;
        
        {
            drawText("agressivité %", 0.6*m_width, m_translateur_p.m_y, sizeText_little);
            m_translateur_p.m_value = config.m_command_capteur_p;
            drawValueGuiKeyPad2(m_translateur_p);
        }
        {
            drawText("vitesse cm/s", 0.6*m_width, m_translateur_cm_s.m_y, sizeText_little);
            
            m_translateur_cm_s.m_value = 0;
            drawValueGuiKeyPad2(m_translateur_cm_s);
        }
        {
            drawText("dist cm", 0.6*m_width, m_translateur_d_mm.m_y, sizeText_little);
            
            m_translateur_d_mm.m_value = m_d_mm/10;
            drawValueGuiKeyPad2(m_translateur_d_mm);
        }
        drawButtonLabel2(m_button_dist_droite, COLOR_VALIDATE);
        drawButtonLabel2(m_button_dist_gauche, COLOR_VALIDATE);
        drawButtonLabel2(m_button_dist_middle, COLOR_VALIDATE);
    }
    drawButtonLabel2(m_button_end, COLOR_CANCEL);
    drawButtonLabel2(m_button_next_time, COLOR_CANCEL);
}

void ParamTranslateurWidget::onMousePage9(int x, int y){
    //Framework & f = Framework::instance();
    if(m_time_page){
        if(m_button_temp_droite_prop.isActive(x, y) != 0){
           
        }
        
        if(m_button_temp_droite_prop.isActive(x, y) != 0){
            
        }
        if(m_button_temp_gauche_prop.isActive(x, y) != 0){
         
        }
        if(m_button_temp_droite_onoff.isActive(x, y) != 0){
         
        }
        if(m_button_temp_gauche_onoff.isActive(x, y) != 0){
   
        }
        if(onMouseKeyPad2(m_propo_value, x, y, 5)){
            m_propo_value2 = m_propo_value.m_value;
            if(m_propo_value2 < 0){
                m_propo_value2 = 0;
            }
            if(m_propo_value2 > 100){
                m_propo_value2 = 100;
            }
        };
        if(onMouseKeyPad2(m_time, x, y, 0.5)){
            m_time2 = m_time.m_value;
            if(m_time2 < 0.5){
                m_time2 = 0.5;
            }
        };
    } else {
        Config & config = Framework::instance().m_config;
        
        if(m_translateur_p.isActive(x, y) != 0){
            config.m_command_capteur_p += 5*m_translateur_p.isActive(x, y);
            loadConfig();
        }
        if(onMouseKeyPad2(m_translateur_cm_s, x, y, 0.5)){
            config.m_command_capteur_vitesse_max_mm_s = m_translateur_cm_s.m_value*10;
            loadConfig();
        };
        if(onMouseKeyPad2(m_translateur_d_mm, x, y, 0.5)){
            m_d_mm = m_translateur_d_mm.m_value*10;
            if(m_d_mm < 0.5){
                m_d_mm = 0.5;
            }
        };
        if(m_button_dist_gauche.isActive(x, y) != 0){
        }
        if(m_button_dist_middle.isActive(x, y) != 0){
            
        }
        if(m_button_dist_droite.isActive(x, y) != 0){
        }
    }
    
    if(m_button_next_time.isActive(x, y)){
        m_time_page = !m_time_page;
    }
    
    if(m_button_end.isActive(x, y)){
        m_close = true;
    }
}
