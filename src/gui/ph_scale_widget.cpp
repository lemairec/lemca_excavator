#include "ph_scale_widget.hpp"

#include "../framework.hpp"
#include "../config/config.hpp"

const double PH_MIN = 0.0;
const double PH_MAX = 14.0;
const double PH_PAS = 0.05;      //pas du +/- : une grille de chaulage se regle au 1/20 de pH

PhScaleWidget::PhScaleWidget(){
    m_close = true;
}

void PhScaleWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    // carte centree
    m_width2 = m_width*0.46;
    m_height2 = m_height*0.72;
    m_x2 = (m_width - m_width2)/2;
    m_y2 = (m_height - m_height2)/2;

    //une ligne par bande ; la derniere n'a pas de keypad (sa borne haute est PH_MAX)
    for(int i = 0; i < PH_BANDE_N; ++i){
        m_borne[i].setResize(m_x2 + 0.55*m_width2, m_y2 + (0.26+0.11*i)*m_height2, m_petit_button);
    }
    m_button_defaut.setResizeStd(m_x2 + 0.30*m_width2, m_y2 + 0.88*m_height2, "Defaut", true, 0.32*m_width2);
    m_button_close.setResizeStd (m_x2 + 0.70*m_width2, m_y2 + 0.88*m_height2, "Fermer", true, 0.32*m_width2);
}

void PhScaleWidget::open(){
    BaseWidget::open();
    Config & config = Framework::instance().m_config;
    for(int i = 0; i < PH_BANDE_N; ++i){
        m_borne[i].m_value = config.m_soil_ph_bande[i];
    }
}

//borne i = max de la bande i = min de la bande i+1 : on la garde entre ses voisines,
//l'egalite est permise (bande vide) mais jamais l'inversion.
void PhScaleWidget::saveBornes(){
    Config & config = Framework::instance().m_config;
    for(int i = 0; i < PH_BANDE_N; ++i){
        double bas  = (i == 0) ? PH_MIN : m_borne[i-1].m_value;
        double haut = (i == PH_BANDE_N-1) ? PH_MAX : m_borne[i+1].m_value;
        if(m_borne[i].m_value < bas ){ m_borne[i].m_value = bas;  }
        if(m_borne[i].m_value > haut){ m_borne[i].m_value = haut; }
        config.m_soil_ph_bande[i] = m_borne[i].m_value;
    }
    config.save();
}

void PhScaleWidget::draw(){
    m_painter->setPen(m_pen_black);
    m_painter->setBrush(m_brush_white);
    m_painter->drawRoundedRect(m_x2, m_y2, m_width2, m_height2, RADIUS_CARD, RADIUS_CARD);

    drawText("Echelle pH", m_x2+0.5*m_width2, m_y2+0.10*m_height2, sizeText_big, true);
    drawText("Couleur des pastilles de la carte", m_x2+0.5*m_width2, m_y2+0.16*m_height2, sizeText_little, true);

    int x_min = m_x2 + 0.14*m_width2;
    int x_max = m_x2 + 0.55*m_width2;
    int y_head = m_y2 + 0.21*m_height2;
    drawText("min", x_min, y_head, sizeText_little, true);
    drawText("max", x_max, y_head, sizeText_little, true);

    for(int i = 0; i < PH_BANDE_N+1; ++i){
        int y = m_y2 + (0.26+0.11*i)*m_height2;

        double bas = (i == 0) ? PH_MIN : m_borne[i-1].m_value;
        m_painter->setPen(m_pen_black);
        drawText(strprintf("%.2f", bas), x_min, y, sizeText_medium, true);

        if(i < PH_BANDE_N){
            drawValueGuiKeyPad2(m_borne[i]);
        } else {
            drawText(strprintf("%.2f", PH_MAX), x_max, y, sizeText_medium, true);
        }

        //pastille : exactement la couleur que prendront les points de cette bande
        m_painter->setPen(m_pen_black);
        m_painter->setBrush(QBrush(phBandeColor(i)));
        m_painter->drawRoundedRect(m_x2+0.80*m_width2, y-0.03*m_height2, 0.12*m_width2, 0.06*m_height2,
                                   RADIUS_CONTROL, RADIUS_CONTROL);
    }

    m_painter->setPen(m_pen_black);
    drawButtonLabel2(m_button_defaut);
    drawButtonLabel2(m_button_close);
}

int PhScaleWidget::onMouse(int x, int y){
    for(int i = 0; i < PH_BANDE_N; ++i){
        if(onMouseKeyPad2(m_borne[i], x, y, PH_PAS)){
            saveBornes();
            return 0;
        }
    }
    if(m_button_defaut.isActive(x, y)){
        double defaut[PH_BANDE_N] = {5.5, 6.0, 6.5, 7.0};
        for(int i = 0; i < PH_BANDE_N; ++i){
            m_borne[i].m_value = defaut[i];
        }
        saveBornes();
        return 0;
    }
    if(m_button_close.isActive(x, y)){
        m_close = true;
    }
    return 0;
}
