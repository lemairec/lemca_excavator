#pragma once

#include "base/base_widget.hpp"

class Config;

class FirstWidget : public BaseWidget {
    QPixmap * m_img_logo;
    
    ButtonGui m_button_ok;
    
    //int m_page = 0;
public:
    int m_x, m_y;
    int m_width2, m_height2;
    FirstWidget();
    
    void setSize(int width, int height);
    void draw();
    int onMouse(int x, int y);
    
    void close();
};
