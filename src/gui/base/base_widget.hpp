#ifndef BASE_WIDGET_H
#define BASE_WIDGET_H

#include "component.hpp"

#define RAYON_ROUNDED 10

class BaseWidget{
public:
    QPainter * m_painter;
    
    QPen m_pen_black_inv;
    QPen m_pen_black;
    QPen m_pen_white;
    QPen m_pen_red;
    QPen m_pen_blue;
    QPen m_pen_green;
    QPen m_pen_no;
    QPen m_pen_gray;

    QBrush m_brush_no;
    QBrush m_brush_white;
    QBrush m_brushWhiteAlpha;
    
    
    QBrush m_brush_black;
    QBrush m_brush_red;
    QBrush m_brush_blue;
    QBrush m_brush_orange;
    QBrush m_brush_gray;
    QBrush m_brush_green;
    
    
    QBrush m_brush_background_1;
    QBrush m_brush_background_2;
    QBrush m_brush_background_3;
    
    QBrush m_brush_button_validate;
    QBrush m_brush_button_close;
    QBrush m_brush_button_normal;
    QBrush m_brush_button_fail;
    
    QBrush m_alert_warning;
    QBrush m_alert_error;
    
    QBrush m_brush_tracteur;
    QBrush m_brush_outil;
    QBrush m_brush_parcelle_1;
    QBrush m_brush_parcelle_2;
    QBrush m_brush_parcelle_green_alpha;
    
    QPixmap * m_img_check_on;
    QPixmap * m_img_check_off;
    
    QPixmap * m_img_plus;
    QPixmap * m_img_moins;

public:
    bool m_black_mode = true;
    
    int m_gros_button = 10;
    int m_gros_gros_button = 10;
    int m_petit_button = 10;
    
    double m_width;
    double m_height;
    bool m_close = true;
    
    KeyPadWidget * m_key_pad_widget;
    KeyBoardWidget * m_key_board_widget;
    BaseWidget();
    
    virtual void loadImages();
    virtual void setColors();
    virtual void setPainter(QPainter * p);
    virtual void setSize(int width, int height);
    
    virtual void draw() = 0;
        
    virtual int onMouse(int x, int y){return 0;};
    
    virtual void open();
    virtual bool isOpen();
    
    void drawButtonCheck(ButtonGui & button, bool check, const std::string & s);
    void drawButtonCheck(ButtonGui & button, bool check);
    void drawButtonLabel2(ButtonGui & button, int color = 0);
    void drawButtonImage(ButtonGui & button, QPixmap * pixmap, double scale=1.0);
    void drawButtonImageCarre(ButtonGui & button, QPixmap * pixmap, double scale=1.0, bool open = false, const std::string & s = "");
   
    void drawMyImage(QPixmap & pixmap, int x, int y, double scale, bool center);
    
    void drawText(const std::string & text, int x, int y, SizeText = sizeText_little, bool center = false, bool white = false);
    void drawQText(const QString & text, int x, int y, SizeText = sizeText_little, bool center = false, bool white = false);
    void drawQTexts(const QString & text, int x, int y, SizeText = sizeText_little, bool center = false, bool white = false);
    
    void drawValueGuiKeyPadFalse(ValueGui & value);
    void drawValueGuiKeyPad2(ValueGui & keypad);
    bool isActiveValueGuiKeyPad(ValueGui & value, int x, int y);
    bool onMouseKeyPad2(ValueGui & keypad, double x, double y, double inter = 1.0);

    void drawValueGuiKeyBoard(ValueGuiKeyBoard & value);
    void drawValueGuiKeyBoardDisable(ValueGuiKeyBoard & value);
    bool isActiveValueGuiKeyBoard(ValueGuiKeyBoard & value, int x, int y);

    QPixmap * loadImage(const std::string & s, bool inv = false);
    QPixmap * loadImageInv(const std::string & s, bool inv = false);
};

#endif
