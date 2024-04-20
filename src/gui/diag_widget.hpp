#pragma once

#include "base/base_widget.hpp"

class Config;


class DiagnosticWidget : public BaseWidget {
     ButtonGui m_button_ok;
    
public:
    int m_x2, m_y2;
    int m_width2, m_height2;
    DiagnosticWidget();
    
    void drawDiag2(const std::string & s, double x, double y, bool filter);
    void drawDiag(const std::string & s, double x, double y, double value, double min, double max);
    void drawDiagGray(const std::string & s, double x, double y, bool enable, bool filter);
    
    void setSize(int width, int height);
    void draw();
    int onMouse(int x, int y);
};

