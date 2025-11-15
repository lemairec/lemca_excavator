#include "base_widget.hpp"
#include "../../util/directory_manager.hpp"
#include "../../util/log.hpp"
//#include "../../framework_vision.hpp"
#include <iostream>
#include <sstream>

//#define DEBUG_GUI

BaseWidget::BaseWidget(){
    
}


void BaseWidget::loadImages(){
    m_img_plus = loadImage("/gui/plus.png");
    m_img_moins = loadImage("/gui/moins.png");
}

void BaseWidget::setColors(){
    QColor red = QColor(0xFF374B);
    QColor orange = QColor(0xFF7818);
    QColor green = QColor(0x015A06);
    QColor blue = QColor(0, 0, 250);
    
    if(m_black_mode){
        green = QColor(0x35B856);
    }
    
    m_pen_no.setColor(QColor(0, 250, 0, 0));
    m_pen_black = QPen(Qt::black);
    m_pen_white = QPen(Qt::white);
    m_pen_red = QPen(red);
    m_pen_blue = QPen(blue);
    m_pen_green = QPen(Qt::green);
    m_pen_gray = QPen (QColor(120,120,120));
    
    m_brush_black = QBrush(Qt::black);
    m_brush_red = QBrush(red);
    m_brush_blue = QBrush(blue);
    m_brush_orange = QBrush(orange);
    m_brush_gray = QBrush (QColor(200,200,200));
    m_brush_white = QBrush(QColor(255,255,255));
    m_brush_green = QBrush (green);
    
    m_alert_warning = QBrush (orange);
    m_alert_error = m_brush_red;
    
    m_brush_button_close = QBrush (orange);
    m_brush_button_normal = QBrush (QColor(80,80,80));
    m_brush_button_validate = QBrush (green);
    if(m_black_mode){
        m_brush_background_1 = QBrush (QColor(0x141520));
        m_brush_background_2 = QBrush (QColor(0x1D1E2D));
        m_brush_background_3 = QBrush (QColor(0x2D2F42));

        m_brush_button_fail = QBrush (QColor(80,80,80));
        m_pen_black_inv = QPen(Qt::white);
    } else {
        m_brush_background_1 = QBrush (QColor(0xF3F3F3));
        m_brush_background_2 = QBrush (QColor(0xFFFFFF));
        m_brush_background_3 = QBrush (QColor(0xF3F3F3));
        
        m_brush_button_fail = QBrush (QColor(180,180,180));
        m_pen_black_inv = QPen(Qt::black);
    }

    QColor white = QColor(255,255,255);
    white.setAlphaF(0.9);
    m_brushWhiteAlpha =QBrush(white);
    
    m_brush_tracteur = QBrush(QColor(0,0,200));
    m_brush_outil = QBrush(QColor(60,60,200));
    m_brush_parcelle_1 = QBrush(QColor(183,166,138));
    m_brush_parcelle_2 = QBrush(QColor(183*1.2,166*1.2,138*1.2));
    m_brush_parcelle_green_alpha = QBrush(QColor(0, 150, 0, 100));
}

void BaseWidget::setPainter(QPainter * p){
    m_painter = p;
}

void BaseWidget::setSize(int width, int height){
    m_black_mode = false;//FrameworkVision::instance().m_config.m_black_mode;
    m_gros_button = 0.05*height;
    m_petit_button = 0.035*height;
    m_gros_gros_button = 0.12*height;
    m_width = width;
    m_height = height;
    
    setColors();
    loadImages();
}


void BaseWidget::open(){
    m_close = false;
}

bool BaseWidget::isOpen(){
    return !m_close;
}

void BaseWidget::drawButtonImage(ButtonGui & button, QPixmap * pixmap, double scale){
    double scale2 = 0.4*scale;
    int w = pixmap->size().width()*scale2;
    int h = pixmap->size().height()*scale2;

    m_painter->drawPixmap(button.m_x-w/2, button.m_y-h/2, w, h, *pixmap);

#ifdef DEBUG_GUI
    int x = button.m_x-button.m_rayon;
    int y = button.m_y-button.m_rayon;
    int d = button.m_rayon*2;
    m_painter->setPen(m_pen_black);
    m_painter->setBrush(m_brushWhiteAlpha);
    m_painter->drawEllipse(x, y, d, d);
#endif
}

void BaseWidget::drawButtonImageCarre(ButtonGui & button, QPixmap * pixmap, double scale, bool open, const std::string & s){
    int w = pixmap->size().width()*scale;
    int h = pixmap->size().height()*scale;

    int x = button.m_x-button.m_width/2;
    int y = button.m_y- button.m_height/2;
    
    int y_image = button.m_y-h/2-5;
    m_painter->setPen(m_pen_no);
    if(open){
        m_painter->setBrush(m_brush_button_validate);
        
        m_painter->drawRoundedRect(x, y, button.m_width, button.m_height, 5, 5);
        if(!m_black_mode){
            QImage im = pixmap->toImage();
            im.invertPixels();
            QPixmap p = QPixmap::fromImage(im);
            m_painter->drawPixmap(button.m_x-w/2, y_image, w, h, p);
        } else {
            m_painter->drawPixmap(button.m_x-w/2, y_image, w, h, *pixmap);
        }
    } else {
        m_painter->setBrush(m_brush_background_3);
        m_painter->drawRoundedRect(x, y, button.m_width, button.m_height, 5, 5);
        
        m_painter->drawPixmap(button.m_x-w/2, y_image, w, h, *pixmap);
    }
    if(!s.empty()){
        if(m_black_mode || (open && !m_black_mode)){
            m_painter->setPen(m_pen_white);
        } else {
            m_painter->setPen(m_pen_black);
        }
        drawQText(QString::fromStdString(s), button.m_x, button.m_y + button.m_height/4, sizeText_logo, true);
    }
}

void BaseWidget::drawMyImage(QPixmap & pixmap, int x, int y, double scale, bool center){
    int w = pixmap.size().width()*scale;
    int h = pixmap.size().height()*scale;

    m_painter->drawPixmap(x-w/2, y-h/2, w, h, pixmap);
}

void BaseWidget::drawButtonCheck(ButtonGui & button, bool check, const std::string & s){
    int x = button.m_x+2*button.m_rayon;
    int y = button.m_y;
    
    if(check){
        drawButtonImage(button, m_img_check_on, 0.6);
    } else {
        drawButtonImage(button, m_img_check_off, 0.6);
    }
    
    m_painter->setPen(m_pen_black_inv);
    drawText(s, x, y, sizeText_medium);
}

void BaseWidget::drawButtonCheck(ButtonGui & button, bool check){
    int x = button.m_x+2*button.m_rayon;
    int y = button.m_y;
    
    if(check){
        drawButtonImage(button, m_img_check_on, 0.6);
    } else {
        drawButtonImage(button, m_img_check_off, 0.6);
    }
    
    drawQText(button.m_label, x, y, sizeText_medium);
}

void BaseWidget::drawButtonLabel2(ButtonGui & button, int color){
    m_painter->setPen(m_pen_no);
    if(color == COLOR_CANCEL){
        m_painter->setBrush(m_brush_button_close);
    } else if(color == COLOR_VALIDATE){
        m_painter->setBrush(m_brush_button_validate);
    } else if(color == COLOR_FAIL){
        m_painter->setBrush(m_brush_button_fail);
    } else if(color == COLOR_CHECK){
        m_painter->setBrush(m_brush_button_fail);
    } else if(color == COLOR_WHITE){
        m_painter->setBrush(m_brush_white);
    } else {
        m_painter->setBrush(m_brush_button_normal);
    }
    m_painter->drawRoundedRect(button.m_x-button.m_width/2 , button.m_y-button.m_height/2, button.m_width, button.m_height, 5, 5);
    m_painter->setPen(m_pen_no);
    if(color == COLOR_WHITE){
        m_painter->setPen(m_pen_black);
    } else {
        m_painter->setPen(m_pen_white);
    }
    if(button.m_label.size()){
        drawQText(button.m_label, button.m_x, button.m_y, sizeText_medium, true, false);
    }
#ifdef DEBUG_GUI
    drawButton(button);
#endif
}


void BaseWidget::drawText(const std::string & text, int x, int y, SizeText size, bool center, bool white){
    QString s = QString::fromStdString(text);
    
    drawQText(s, x, y, size, center, white);
}

void BaseWidget::drawQText(const QString & s, int x, int y, SizeText size, bool center, bool white){
    int s2 = 10;
    switch (size) {
        case sizeText_bigbig:
            s2 = 22;
            break;
        case sizeText_big:
            s2 = 20;
            break;
        case sizeText_medium:
            s2 = 18;
            break;
        case sizeText_little:
            s2 = 14;
            break;
        case sizeText_logo:
            s2 = 12;
            break;
    }
    QFont font = QFont("Latin", s2, 1, false);
    font.setBold(true);
    //textItem->setFont(font);
    if(white){
        m_painter->setPen(m_pen_black_inv);
    }
    
    font.setPixelSize(s2);
    m_painter->setFont(font);
    
    if(center){
        //auto mBounds = textItem->boundingRect();
        auto fm = m_painter->fontMetrics();
        auto rect = fm.boundingRect(s);
        m_painter->drawText(x-rect.width()/2, y+5, s);
    } else {
        m_painter->drawText(x, y+2, s);
    }
   
}

void BaseWidget::drawQTexts(const QString & s, int x, int y, SizeText size, bool center, bool white){
    int s2 = 10;
    switch (size) {
        case sizeText_bigbig:
            s2 = 24;
            break;
        case sizeText_big:
            s2 = 22;
            break;
        case sizeText_medium:
            s2 = 18;
            break;
        case sizeText_little:
            s2 = 14;
            break;
        case sizeText_logo:
            s2 = 12;
            break;
    }
    QFont font = QFont("Latin", s2, 1, false);
    font.setBold(true);
    //textItem->setFont(font);
    if(white){
        m_painter->setPen(Qt::white);
    }
    
    QStringList list1 = s.split(QLatin1Char('\n'));
    
    font.setPixelSize(s2);
    m_painter->setFont(font);
    
    int y2 = y+5;
    for(auto s3 : list1){
        if(center){
            //auto mBounds = textItem->boundingRect();
            auto fm = m_painter->fontMetrics();
            auto rect = fm.boundingRect(s3);
            m_painter->drawText(x-rect.width()/2, y2, s3);
        } else {
            m_painter->drawText(x, y2, s3);
        }
        y2 += s2*1.2;
    }
   
}

QPixmap * BaseWidget::loadImage(const std::string & s, bool inv){
    std::string s2 = DirectoryManager::instance().getSourceDirectory()+s;
    QImage image2(QString::fromStdString(s2));
    INFO(s2);
    if(inv){
        image2.invertPixels();
    }
    QPixmap * res  = new QPixmap(QPixmap::fromImage(image2));
    return res;
}

QPixmap * BaseWidget::loadImageInv(const std::string & s, bool inv){
    std::string s2 = DirectoryManager::instance().getSourceDirectory()+s;
    QImage image2(QString::fromStdString(s2));
    if(image2.size().width() < 20){
        INFO("fail open " << s);
    }
    if(inv){
        if(!m_black_mode){
            image2.invertPixels();
        }
    } else {
        if(m_black_mode){
            image2.invertPixels();
        }
    }
    QPixmap * res  = new QPixmap(QPixmap::fromImage(image2));
    return res;
}

void BaseWidget::drawValueGuiKeyPadFalse(ValueGui & value){
    QString s = QString::number(value.m_value);
    m_painter->setPen(m_pen_gray);
    m_painter->setBrush(m_brushWhiteAlpha);
    m_painter->drawRoundedRect(value.m_x-40, value.m_y-15, 80, 30, 5, 5);
    drawQText(s, value.m_x, value.m_y, sizeText_medium, true);
}


void BaseWidget::drawValueGuiKeyPad2(ValueGui & keypad){
    drawButtonImage(keypad.m_button_plus, m_img_plus, 0.7);
    drawButtonImage(keypad.m_button_moins, m_img_moins, 0.7);
    
    QString s = QString::number(keypad.m_value);
    m_painter->setPen(m_pen_no);
    m_painter->setBrush(m_brush_background_3);
    m_painter->drawRoundedRect(keypad.m_x-40, keypad.m_y-15, 80, 30, 5, 5);
    m_painter->setPen(m_pen_black_inv);
    drawQText(s, keypad.m_x, keypad.m_y, sizeText_medium, true);
}

bool BaseWidget::isActiveValueGuiKeyPad(ValueGui & value, int x, int y){
    if(value.m_x - 40 < x && x < value.m_x + 40 && value.m_y-15 < y && y < value.m_y+15){
        return true;
    }
    return false;
    
}

double getNextValue(double value, bool positif){
    std::vector<double> t = {0.9, 1 , 1.1, 1.2, 1.3, 1.4, 1.5, 1.7, 2, 2.2, 2.4, 2.6, 3 , 3.3, 3.6, 4, 4.5,  5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5 ,9, 9.5 ,10,11};
    double res = value;
    int i = 0;
    for(int j = 0; j < 4; ++j){
        if(res > 10){
            res = res/10;
            ++i;
        }
        if(res < 1){
            res = res*10;
            --i;
        }
    }

    size_t j = 0;
    double min = 10;
    for(size_t k = 0; k < t.size(); ++k){
        double v = res/t[k];
        if(std::abs(v-1) < min){
            j = k;
            min = std::abs(v-1);
        }
        
    }
    double next_value = res;
    if(positif){
        if(j+1 < t.size()){
            next_value = t[j+1];
        }
    } else {
        if(j-1 >= 0){
            next_value = t[j-1];
        }
    }
    if(i>0){
        for(int j = 0; j < i; ++j){
            next_value = next_value*10.0;
        }
    }
    if(i<0){
        for(int j = 0; j < -i; ++j){
            next_value = next_value/10.0;
        }
    }
    return next_value;
}

bool BaseWidget::onMouseKeyPad2(ValueGui & keypad, double x, double y, double inter){
    if(keypad.m_button_plus.isActive(x, y)){
        if(inter == 0){
            keypad.m_value = getNextValue(keypad.m_value, true);
        } else {
            keypad.m_value += inter;
        }
        return true;
    }
    if(keypad.m_button_moins.isActive(x, y)){
        if(inter == 0){
            keypad.m_value = getNextValue(keypad.m_value, false);
        } else {
            keypad.m_value -= inter;
        }
        return true;
    }
       
    if(isActiveValueGuiKeyPad(keypad,x, y)){
        //INFO("ici2");
        //m_keypad_widget.m_close = false;
        //INFO("ici22");
        //m_keypad_widget.setValueGuiKeyPad(&keypad);
    }
    return false;
}

void BaseWidget::drawValueGuiKeyBoard(ValueGuiKeyBoard & value){
    
    m_painter->setPen(m_pen_no);
    m_painter->setBrush(m_brush_background_3);
    m_painter->drawRoundedRect(value.m_x-value.m_width/2, value.m_y-15, value.m_width, 30, 5, 5);
    m_painter->setPen(m_pen_black_inv);
    drawText(value.m_text, value.m_x, value.m_y, sizeText_medium, true);
}

void BaseWidget::drawValueGuiKeyBoardDisable(ValueGuiKeyBoard & value){
    drawText(value.m_text, value.m_x, value.m_y, sizeText_medium, true);
}

bool BaseWidget::isActiveValueGuiKeyBoard(ValueGuiKeyBoard & value, int x, int y){
    if(value.m_x-value.m_width/2 < x && x < value.m_x + value.m_width/2 && value.m_y-15 < y && y < value.m_y+15){
        return true;
    }
    return false;
}

/* Select Gui example
 m_langage.setResize(m_part_1_x+m_part_1_w/2, y, "", true, m_part_1_w/2);
 m_langage.clear();
 m_langage.addValue(Langage::getKey("FR"));
 m_langage.addValue(Langage::getKey("EN"));
 m_langage.addValue(Langage::getKey("DE"));
 drawPart1Title(m_langage.m_y-2*m_y_inter, m_y_inter*3, "langue");
 drawButtonLabel2(m_langage.m_buttonOpen);
 
 if(m_select_widget.m_close){
     m_langage.setValueString(Langage::getKey(config.m_langage.getStdValue()));
 }
 
 if(!m_select_widget.m_close){
     if(m_select_widget.onMouseSelect(x, y)){
         std::string s = m_select_widget.m_selectButton->getValueString();
         if(s == Langage::getKey("FR")){
             config.m_langage.setValue("FR");
         } else if(s == Langage::getKey("EN")){
             config.m_langage.setValue("EN");
         } else if(s == Langage::getKey("DE")){
             config.m_langage.setValue("DE");
         }
         loadConfig();
     }
    
     return;
 }
 
 if(m_langage.m_buttonOpen.isActive(x, y)){
     m_select_widget.open();
     m_select_widget.setValueGuiKeyPad(&m_langage);
 }
 */

/* ValueGui Exemple
 m_command_capteur_vitesse_cm_s.setResize(m_part_1_x3, y, m_petit_button);
{
     m_command_capteur_vitesse_cm_s.m_value = config.m_command_capteur_vitesse_mm_s.getValue()/10.0;
     drawText(Langage::getKey("HYDR_V_MAX"), m_part_1_x2,m_command_capteur_vitesse_cm_s.m_y, sizeText_medium);
     drawValueGuiKeyPad2(m_command_capteur_vitesse_cm_s);
 }
 if(onMouseKeyPad2(m_command_capteur_vitesse_cm_s, x, y, 0.5)){
     config.m_command_capteur_vitesse_mm_s.setValue(m_command_capteur_vitesse_cm_s.m_value*10);
 };
 */
