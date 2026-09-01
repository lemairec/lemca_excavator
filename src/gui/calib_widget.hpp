#pragma once

#include "base/base_widget.hpp"

class Config;

// Procedure guidee de calibrage pH : temperature des tampons -> tampon bas ->
// tampon haut -> etat de sante de la sonde (score de pente, facon Veris).
// Rien n'est ecrit dans la config avant la validation finale : les boutons
// "pH bas" / "pH haut" de la page Sol restent le fallback en cas de souci.
class CalibWidget : public BaseWidget {
    int m_x2, m_y2;
    int m_width2, m_height2;

    // 0 = temperature, 1 = tampon bas, 2 = tampon haut, 3 = resultat
    int m_step = 0;

    ValueGui m_temp;
    double m_u_bas = 0;      // mV lus dans le tampon bas
    double m_u_haut = 0;     // mV lus dans le tampon haut

    ButtonGui m_button_next;
    ButtonGui m_button_back;
    ButtonGui m_button_cancel;

    void drawTemp();
    void drawBain(double ph_tampon, const std::string & etape);
    void drawResultat();

    double getPenteMesuree();     // mV/pH signee, corrigee en temperature
    double getPenteTheorique();   // mV/pH a la temperature des tampons
    double getScore();            // % de la pente de reference

public:
    CalibWidget();

    void setSize(int width, int height);
    void open();
    void draw();
    int onMouse(int x, int y);
};
