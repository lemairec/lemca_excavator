#include "debug_widget.hpp"

#include "../framework.hpp"
#include "gps_widget.hpp"

DebugWidget::DebugWidget(){
}

void DebugWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    m_zoom_y = 0.8;
    
    m_y = m_height*0.1;
    m_height2 = m_height*0.4;
    
    m_x = m_width*0;
    m_width2 = m_width*0.4;
    
    if(m_fullscreen){
        m_height2 = m_height*0.8;
        m_width2 = m_width*0.8;
    }
    
    m_button_full.setResizeStd(m_x+120, m_y+m_height2-20, "/", true, 20,20);
    
    m_button_select.setResizeStd(m_x+m_width2/2, m_y+m_height2-20, "-", true, 100,20);
    m_button_close.setResizeStd(m_x+m_width2-120, m_y+m_height2-20, "x", true, 100,20);
}

bool m_auto = true;
void DebugWidget::draw(){
    
    
    m_painter->setPen(m_pen_black);
    m_painter->setBrush(m_brush_white);
    m_painter->drawRoundedRect(m_x, m_y, m_width2, m_height2, RAYON_ROUNDED, RAYON_ROUNDED);
    
    m_painter->drawLine(m_x+15, m_y+15, m_x+15, m_y+m_height2-30);
    m_painter->drawLine(m_x+15, m_y+m_height2/2, m_x+m_width2-30, m_y+m_height2/2);
    
    Framework & f = Framework::instance();
    
    
    m_curbe_i = 0;
        if(!m_auto){
        int y01_ = round(0.1*m_height2/4*m_zoom_y);
        int y1_ = round(1*m_height2/4*m_zoom_y);
        int y10_ = round(10*m_height2/4*m_zoom_y);
   
        if(y10_ <m_height2/12) {
            int y_ = m_y+m_height2/2-y10_;
            drawText("10", m_x+15,y_);
            m_painter->drawLine(m_x+15,y_, m_x+m_width2-30, y_);
            y_ = m_y+m_height2/2+y10_;
            m_painter->drawLine(m_x+15,y_, m_x+m_width2-30, y_);
        } else if(y01_ <m_height2/12) {
            int y_ = m_y+m_height2/2-y1_;
            drawText("1.0", m_x+15,y_);
            m_painter->drawLine(m_x+15,y_, m_x+m_width2-30, y_);
            y_ = m_y+m_height2/2+y1_;
            m_painter->drawLine(m_x+15,y_, m_x+m_width2-30, y_);
        } else {
            int y_ = m_y+m_height2/2-y01_;
            drawText("0.1", m_x+15,y_);
            m_painter->drawLine(m_x+15,y_, m_x+m_width2-30, y_);
            y_ = m_y+m_height2/2+y01_;
            m_painter->drawLine(m_x+15,y_, m_x+m_width2-30, y_);
        }
    }

    QString s = "?";
    m_init_calcul = true;
    if(m_select == 0){
        s = "aAB";
        calculStat(f.m_stat_distance_cm);
        calculStat(f.m_stat_angle_AB_deg);
        calculStat(f.m_stat_angle_correction_roue_deg);
        
        
        prepareDrawAuto();
        drawStatAuto(f.m_stat_angle_AB_deg, "aAB");
        m_painter->setPen(m_pen_red);
        drawStatAuto(f.m_stat_distance_cm, "dAB");
        m_painter->setPen(m_pen_blue);
        drawStatAuto(f.m_stat_angle_correction_roue_deg, "roue");
        
    } else if(m_select == 1){
        calculStat(f.m_stat_cap_deg);
        calculStat(f.m_stat_cap_vtg_mag_deg);
        calculStat(f.m_stat_cap_vtg_real_deg);
        calculStat(f.m_stat_cap_rmc_deg);
        calculStat(f.m_stat_cap_rmc2_deg);
        calculStat(f.m_stat_cap_calc_deg);
        calculStat(f.m_stat_cap_calc2_deg);
        calculStat(f.m_stat_cap_att_deg);
        calculStat(f.m_stat_cap_custom_d_deg);
        calculStat(f.m_stat_cap_custom_p_deg);
        
        prepareDrawAuto();
        s = "caps";
        m_painter->setPen(m_pen_red);
        drawStatAuto(f.m_stat_cap_vtg_mag_deg, "vtg mag");
        m_painter->setPen(m_pen_blue);
        drawStatAuto(f.m_stat_cap_vtg_real_deg, "vtg real");
        m_painter->setPen(m_pen_gray);
        drawStatAuto(f.m_stat_cap_rmc_deg, "rmc");
        m_painter->setPen(m_pen_green);
        drawStatAuto(f.m_stat_cap_rmc2_deg, "rmc2");
        m_painter->setPen(Qt::magenta);
        drawStatAuto(f.m_stat_cap_calc_deg, "calc");
        m_painter->setPen(Qt::red);
        drawStatAuto(f.m_stat_cap_calc2_deg, "calc2");
        m_painter->setPen(Qt::yellow);
        drawStatAuto(f.m_stat_cap_jd_deg, "jd");
        
        m_painter->setPen(Qt::black);
        drawStatAuto(f.m_stat_cap_deg, "cap");
        
    } else if(m_select == 2){
        s = "volznt";
        calculStat(f.m_stat_pilot_desired);
        calculStat(f.m_stat_pilot_mesured);
        prepareDrawAuto();
        m_painter->setPen(m_pen_blue);
        drawStatAuto(f.m_stat_pilot_desired, "desired");
        m_painter->setPen(m_pen_red);
        drawStatAuto(f.m_stat_pilot_mesured, "mesured");
        
    } else if(m_select == 3){
    }
    
    
    drawButtonLabel2(m_button_close);
    drawButtonLabel2(m_button_full);
    
    m_button_select.m_label = s;
    drawButtonLabel2(m_button_select);
}
int DebugWidget::onMouse(int x, int y){
    if(m_button_full.isActive(x, y)){
        m_fullscreen = !m_fullscreen;
        setSize(m_width, m_height);
    }
    if(m_button_select.isActive(x, y)){
        m_select = (m_select+1)%4;
    }
    
    if(m_button_close.isActive(x, y)){
        m_close = true;
    }
    return 0;
}

void DebugWidget::calculStat(ValueStat & stat){
    for(auto value : stat.m_values){
        if(m_init_calcul){
            m_min = value;
            m_max = value;
            m_init_calcul = false;
        }
        if(value < m_min){
            m_min = value;
        }
        if(value > m_max){
            m_max = value;
        }
    }
}

void DebugWidget::drawStat(ValueStat & stat, const std::string & s){
    int x_prev = 0;
    int y_prev = 0;
    int i = 0;
    double size = stat.m_size_max;
    //INFO(stat.m_values.size());
    
    for(auto value : stat.m_values){
        int x = round(i*(m_width2-30)/size);
        int y = round((double)value*m_height2/4*m_zoom_y);
        x = m_x+15+x;
        y = m_y+m_height2/2-y;
        if(y < m_y+15){
            y = m_y+15;
        }
        if(y > m_y+m_height2-30){
            y = m_y+m_height2-30;
        }
        //INFO(i << " " << value << " "  << x << " "<< y);
        if(x_prev != 0 && y_prev != 0){
            m_painter->drawLine(x_prev, y_prev, x, y);
        }
        ++i;
        x_prev = x;
        y_prev = y;
    }
    
    drawText(s, m_x+m_width2-30, m_y+15+m_curbe_i*30);
    m_curbe_i++;
}

double my_round(double value, double prc){
    return round(value/prc)*prc;
}

double m_min_value_auto;
double m_max_value_auto;
double m_y_min_stat;
double m_y_max_stat;

void DebugWidget::prepareDrawAuto(){
    double diff = m_max-m_min;
    double middle = (m_max+m_min)/2;
    
    if(diff < 0.1){
        diff = 0.1;
    } else if(diff < 1){
        diff = 1;
    } else if(diff < 10){
        diff = 10;
    } else if(diff < 50){
        diff = 50;
    } else if(diff < 100){
        diff = 100;
    }
    middle = my_round(middle,diff/10);
    m_min_value_auto = m_min-diff/4;
    m_max_value_auto = m_max+diff/4;
    m_y_min_stat = m_y+m_height2-30;
    m_y_max_stat = m_y+15;
    
    int y_ = m_y+m_height2/2;
    drawText(std::to_string(middle), m_x+15,y_);
    m_painter->drawLine(m_x+15,y_, m_x+m_width2-30, y_);
    
    y_ = my_map(m_min, m_min_value_auto, m_max_value_auto, m_y_min_stat, m_y_max_stat);
    drawText(std::to_string(m_min), m_x+15,y_);
    m_painter->drawLine(m_x+15,y_, m_x+m_width2-30, y_);
    
    y_ = my_map(m_max, m_min_value_auto, m_max_value_auto, m_y_min_stat, m_y_max_stat);
    drawText(std::to_string(m_max), m_x+15,y_);
    m_painter->drawLine(m_x+15,y_, m_x+m_width2-30, y_);
}

int m_i_legend;
void DebugWidget::drawStatAuto(ValueStat & stat, const std::string & s){
    int x_prev = 0;
    int y_prev = 0;
    int i = 0;
    double size = stat.m_size_max;
   
    //INFO(stat.m_values.size());
    
    for(auto value : stat.m_values){
        int x = round(i*(m_width2-30)/size);
        int y = my_map(value, m_min_value_auto, m_max_value_auto, m_y_min_stat, m_y_max_stat);
        /*if(y<y_min){
            INFO(m_min << " " << m_max);
            INFO(value << " " << min << " " << max << " - " << y << " " << y_min << " " << y_max);
        }*/
        x = m_x+15+x;
        
        //INFO(i << " " << value << " "  << x << " "<< y);
        if(x_prev != 0 && y_prev != 0){
            m_painter->drawLine(x_prev, y_prev, x, y);
        }
        ++i;
        x_prev = x;
        y_prev = y;
    }
    
    drawText(s, m_x+m_width2-30, m_y+15+m_curbe_i*30);
    m_curbe_i++;
    

}
