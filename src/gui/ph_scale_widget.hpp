#pragma once

#include "base/base_widget.hpp"

// Echelle de couleur du pH : PH_BANDE_N bornes reglables -> PH_BANDE_N+1 bandes
// contigues, avec la palette fixe de phBandeColor(). Ouverte depuis le menu Infos,
// pilote la couleur des pastilles de mesure sur la carte.
class PhScaleWidget : public BaseWidget {
    int m_x2, m_y2;
    int m_width2, m_height2;

    ValueGui m_borne[PH_BANDE_N];
    ButtonGui m_button_defaut;
    ButtonGui m_button_close;

    void saveBornes();

public:
    PhScaleWidget();

    void setSize(int width, int height);
    void open();
    void draw();
    int onMouse(int x, int y);
};
