#ifndef DEBUG_WIDGET_H
#define DEBUG_WIDGET_H

#include "base/base_widget.hpp"
#include "../framework.hpp"


class DebugWidget : public BaseWidget {
    bool m_fullscreen = false;

    double m_y = 0;
    double m_height2 = 0;
    
    double m_x = 0;
    double m_width2 = 0;
    
    int m_curbe_i = 0;
    
    int m_select = 0;
    double m_zoom_y = 8;
public:
    DebugWidget();
    
    ButtonGui m_button_close;
    ButtonGui m_button_full;
    
    ButtonGui m_button_select;

    void setSize(int width, int height);
    
    virtual void draw();
    bool m_init_calcul;
    void drawStat(ValueStat & stat, const std::string & s);
    
    
    double m_max, m_min;
    void calculStat(ValueStat & stat);
    void prepareDrawAuto();
    void drawStatAuto(ValueStat & stat, const std::string & s);
    
    virtual int onMouse(int x, int y);
};

#endif
