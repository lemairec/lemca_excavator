#ifndef SELECT_WIDGET_H
#define SELECT_WIDGET_H

#include "base_widget.hpp"

class SelectWidget : public BaseWidget {
    int m_x = 0;
    int m_y = 0;
    int m_width2 = 0;
    int m_height2 = 0;
    
    QString m_res;
    //double m_lg;
    size_t m_pages_total = 0;
    size_t m_page = 0;
    
    ButtonGui m_page_up;
    ButtonGui m_page_down;
    
    ButtonGui m_ok;
    ButtonGui m_cancel;
    std::vector<ButtonGui *> m_buttons;
public:
    bool m_draw_rect = true;
    bool m_draw_selected = true;
    bool m_draw_button = true;
    
    SelectButtonGui * m_selectButton = NULL;
    
    void setSize(int width, int height);
    void setSizePart(int x, int y, int width, int height, int width_tot, int height_tot);
    
    void draw();
    bool onMouseSelect(int x, int y);
    
    void setValueGuiKeyPad(SelectButtonGui * value);
    
    void open(){
        m_close = false;
    }
};

#endif
