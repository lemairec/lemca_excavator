#include "select_widget.hpp"
#include "../../config/langage.hpp"

void SelectWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    m_width = width;
    m_height = height;
    //m_close = false;
    setSizePart(m_width*0.25, m_height*0.1, m_width*0.5, m_height*0.8, m_width, m_height);
}

void SelectWidget::setSizePart(int x, int y, int width, int height, int width_tot, int height_tot){
    BaseWidget::setSize(width_tot, height_tot);
    m_width = width_tot;
    m_height = height_tot;
    m_width2 = width;
    m_height2 = height;
    m_x = x;
    m_y = y;
    //m_close = false;
    m_ok.setResizeStd(m_x+m_width2*0.25, m_y+m_height2*0.9, Langage::getKey("OK"), true);
    m_cancel.setResizeStd(m_x+m_width2*0.75, m_y+m_height2*0.9, Langage::getKey("CANCEL"), true);
    m_page_down.setResize(m_x+m_width2*0.35, m_y+m_height2*0.77, m_petit_button);
    m_page_down.m_label = "<";
    m_page_up.setResize(m_x+m_width2*0.65, m_y+m_height2*0.77, m_petit_button);
    m_page_up.m_label = ">";
   
}

void SelectWidget::draw(){
    std::string m_title = m_selectButton->m_label;
    int m_open = true;
    
    if(m_open){
        m_painter->setPen(m_pen_black);
        //m_painter->drawRect(0, 0, m_width, m_height);
        m_painter->setBrush(m_brush_white);
        if(m_draw_rect){
            m_painter->drawRoundedRect(m_x, m_y, m_width2, m_height2, RAYON_ROUNDED, RAYON_ROUNDED);
        }
        
        drawText(m_title, m_x+m_width2/2, m_height*0.15, sizeText_big, true);
        
        for(size_t i = 0; i < m_buttons.size(); ++i){
            if(i/6 == m_page){
                auto b = m_buttons[i];
                if(((int)i) == m_selectButton->m_selectedValue && m_draw_selected){
                    drawButtonLabel2(*b, COLOR_CHECK);
                } else {
                    drawButtonLabel2(*b);
                }
            }
        }
        
        if(m_draw_button){
            drawButtonLabel2(m_ok, COLOR_VALIDATE);
            drawButtonLabel2(m_cancel, COLOR_CANCEL);
        }
        
        if(m_pages_total>1){
            if(m_page+1 != m_pages_total){
                drawButtonLabel2(m_page_up);
            }
            if(m_page != 0){
                drawButtonLabel2(m_page_down);
            }
            QString s = QString::number(m_page+1)+"/"+QString::number(m_pages_total);
            drawQText(s, (m_page_down.m_x + m_page_up.m_x)/2, m_page_down.m_y, sizeText_medium, true);
        }
    }
}

bool SelectWidget::onMouseSelect(int x, int y){
    if(m_draw_button){
        if(m_ok.isActive(x,y)){
            m_close = true;
            return true;
        }
        if(m_cancel.isActive(x,y)){
            m_close = true;
        }
    }
    if(m_pages_total>1){
        if(m_page_up.isActive(x,y)){
            if(m_page+1 < m_pages_total){
                m_page++;
            }
        }
        if(m_page_down.isActive(x,y)){
            if(m_page > 0){
                m_page--;
            }
        }
        
    }
    for(size_t i = 0; i < m_buttons.size(); ++i){
        if(i/6 == m_page){
            auto b = m_buttons[i];
            if(b->isActive(x, y)){
                m_selectButton->m_selectedValue = i;
                m_close = true;
                return true;
            }
        }
    }
    return false;
}

void SelectWidget::setValueGuiKeyPad(SelectButtonGui * value){
    m_selectButton = value;
    m_res = QString();
    m_buttons.clear();
    m_pages_total = 0;
    m_page = 0;
    
    int inter = m_height2/10;
    int y_begin = m_y+inter;
    int y = y_begin;
    for(size_t i = 0; i < m_selectButton->m_values.size(); ++i){
        if(i%6 == 0){
            y = y_begin;
            m_pages_total++;
        }
        std::string l = m_selectButton->m_values[i];
        ButtonGui * b = new ButtonGui;
        b->setResizeStd(m_x+m_width2*0.5, y, l, true, m_width2*0.5);
        b->m_labelInt = i;
        m_buttons.push_back(b);
        y+= inter;
        
    }
};
