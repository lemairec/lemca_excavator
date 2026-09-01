#include "calib_widget.hpp"

#include "../framework.hpp"
#include "../config/config.hpp"

#include <ctime>

// Bande d'acceptation du score, reprise de la procedure Veris : au dessus de
// 102 % la pente est physiquement impossible (cablage / bruit), en dessous de
// 75 % l'electrode antimoine ne repond plus assez pour etre fiable.
static const double SCORE_MIN = 75.0;
static const double SCORE_MAX = 102.0;

CalibWidget::CalibWidget(){
    m_close = true;
}

void CalibWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    m_width2 = m_width*0.52;
    m_height2 = m_height*0.72;
    m_x2 = (m_width - m_width2)/2;
    m_y2 = (m_height - m_height2)/2;

    m_temp.setResize(m_x2 + 0.62*m_width2, m_y2 + 0.42*m_height2, m_petit_button);

    m_button_next.setResizeStd  (m_x2 + 0.72*m_width2, m_y2 + 0.88*m_height2, "Continuer", true, 0.4*m_width2);
    m_button_back.setResizeStd  (m_x2 + 0.28*m_width2, m_y2 + 0.88*m_height2, "Retour",    true, 0.3*m_width2);
    m_button_cancel.setResizeStd(m_x2 + 0.28*m_width2, m_y2 + 0.88*m_height2, "Annuler",   true, 0.3*m_width2);
}

void CalibWidget::open(){
    BaseWidget::open();
    Framework & f = Framework::instance();
    m_step = 0;
    m_u_bas = 0;
    m_u_haut = 0;
    m_temp.m_value = f.m_config.m_soil_temp_cal;   // derniere T de calib comme point de depart
}

//pente signee mV/pH, comme celle utilisee par Framework::setPh
double CalibWidget::getPenteMesuree(){
    Config & c = Framework::instance().m_config;
    double dpH = c.m_soil_ph_bas - c.m_soil_ph_haut;
    if(fabs(dpH) < 1e-6){
        return 0;
    }
    return (m_u_bas - m_u_haut)/dpH;
}

//Nernst : 2.303*R*T/F = 0.19841 * T(K) mV/pH  (59.16 mV/pH a 25 degC)
double CalibWidget::getPenteTheorique(){
    Config & c = Framework::instance().m_config;
    if(c.m_soil_slope_ref_mv_ph > 0){
        return c.m_soil_slope_ref_mv_ph;   // reference mesuree sur sonde neuve
    }
    return 0.19841*(273.15 + m_temp.m_value);
}

double CalibWidget::getScore(){
    double theo = getPenteTheorique();
    if(fabs(theo) < 1e-9){
        return 0;
    }
    return 100.0*fabs(getPenteMesuree())/theo;
}

void CalibWidget::draw(){
    Framework & f = Framework::instance();
    Config & c = f.m_config;

    m_painter->setPen(m_pen_black);
    m_painter->setBrush(m_brush_white);
    m_painter->drawRoundedRect(m_x2, m_y2, m_width2, m_height2, RAYON_ROUNDED, RAYON_ROUNDED);

    drawText("Calibrage sonde pH", m_x2+0.5*m_width2, m_y2+0.10*m_height2, sizeText_big, true);

    if(m_step == 0){
        drawTemp();
    } else if(m_step == 1){
        drawBain(c.m_soil_ph_bas, "2/4");
    } else if(m_step == 2){
        drawBain(c.m_soil_ph_haut, "3/4");
    } else {
        drawResultat();
    }

    drawButtonLabel2(m_button_cancel, COLOR_CANCEL);
    if(m_step > 0){
        drawButtonLabel2(m_button_back);
    }
}

void CalibWidget::drawTemp(){
    Config & c = Framework::instance().m_config;

    drawText("1/4  Temperature des tampons", m_x2+0.5*m_width2, m_y2+0.22*m_height2, sizeText_medium, true);
    drawText("Mesure la temperature des solutions tampon.", m_x2+0.08*m_width2, m_y2+0.32*m_height2, sizeText_little);
    drawText("Elle sert de reference de pente et de compensation.", m_x2+0.08*m_width2, m_y2+0.37*m_height2, sizeText_little);

    drawText("Temperature (degC)", m_x2+0.08*m_width2, m_temp.m_y, sizeText_medium);
    drawValueGuiKeyPad2(m_temp);

    if(c.m_soil_check_score > 0){
        std::string s = strprintf("Dernier controle : %.0f %%  le %s",
                                  c.m_soil_check_score, c.m_soil_check_date.data());
        drawText(s, m_x2+0.08*m_width2, m_y2+0.60*m_height2, sizeText_little);
    } else {
        drawText("Aucun controle enregistre.", m_x2+0.08*m_width2, m_y2+0.60*m_height2, sizeText_little);
    }
    drawText("Les boutons pH bas / pH haut de la page Sol restent utilisables.",
             m_x2+0.08*m_width2, m_y2+0.67*m_height2, sizeText_little);

    drawButtonLabel2(m_button_next, COLOR_VALIDATE);
}

//Bain de tampon : on affiche la tension lissee en direct et l'etat de
//stabilisation. La lecture n'est acceptee que stabilisee (comme les boutons
//de calibrage existants) : une lecture qui derive fausse la pente.
void CalibWidget::drawBain(double ph_tampon, const std::string & etape){
    Framework & f = Framework::instance();

    std::string s = strprintf("%s  Plonge la sonde dans le tampon pH %.1f", etape.data(), ph_tampon);
    drawText(s, m_x2+0.5*m_width2, m_y2+0.22*m_height2, sizeText_medium, true);
    drawText("Rince et essuie la sonde avant de la plonger.", m_x2+0.08*m_width2, m_y2+0.32*m_height2, sizeText_little);

    s = strprintf("U : %.1f mV", f.m_last_soil_volt);
    drawText(s, m_x2+0.5*m_width2, m_y2+0.46*m_height2, sizeText_big, true);

    bool settled = f.isSoilSettled();
    QColor col = settled ? QColor(0x35,0xB8,0x56) : QColor(0xE8,0x8A,0x2E);
    m_painter->setPen(QPen(col));
    s = settled ? strprintf("STABLE  (%.1f mV/s)", f.m_last_soil_slope_mv_s)
                : strprintf("stabilisation...  (%.1f mV/s)", f.m_last_soil_slope_mv_s);
    drawText(s, m_x2+0.5*m_width2, m_y2+0.56*m_height2, sizeText_medium, true);
    m_painter->setPen(m_pen_black);

    if(!settled){
        drawText("Attends la stabilisation avant de lire.", m_x2+0.5*m_width2, m_y2+0.66*m_height2, sizeText_little, true);
    }

    m_button_next.m_label = "Lire";
    drawButtonLabel2(m_button_next, settled ? COLOR_VALIDATE : COLOR_OTHER);
}

void CalibWidget::drawResultat(){
    double pente = getPenteMesuree();
    double theo = getPenteTheorique();
    double score = getScore();
    bool ok = (score >= SCORE_MIN && score <= SCORE_MAX);

    drawText("4/4  Etat de la sonde", m_x2+0.5*m_width2, m_y2+0.22*m_height2, sizeText_medium, true);

    double y = m_y2+0.33*m_height2;
    double inter = 0.06*m_height2;
    double x1 = m_x2+0.08*m_width2;
    double x2 = m_x2+0.55*m_width2;

    drawText("Tampon bas", x1, y, sizeText_little);
    drawText(strprintf("%.1f mV", m_u_bas), x2, y, sizeText_little);  y += inter;
    drawText("Tampon haut (E0)", x1, y, sizeText_little);
    drawText(strprintf("%.1f mV", m_u_haut), x2, y, sizeText_little); y += inter;
    drawText("Pente mesuree", x1, y, sizeText_little);
    drawText(strprintf("%.1f mV/pH", pente), x2, y, sizeText_little); y += inter;
    drawText("Pente de reference", x1, y, sizeText_little);
    drawText(strprintf("%.1f mV/pH a %.1f degC", theo, m_temp.m_value), x2, y, sizeText_little); y += inter;

    QColor col = ok ? QColor(0x35,0xB8,0x56) : QColor(0xFF,0x37,0x4B);
    m_painter->setPen(QPen(col));
    std::string s = strprintf("Score : %.1f %%   (tolerance %.0f - %.0f)", score, SCORE_MIN, SCORE_MAX);
    drawText(s, m_x2+0.5*m_width2, y+inter*0.6, sizeText_medium, true);
    s = ok ? "SONDE OK" : (score > SCORE_MAX ? "SCORE IMPOSSIBLE : verifier cablage / bruit"
                                             : "SONDE HORS TOLERANCE : nettoyer ou remplacer");
    drawText(s, m_x2+0.5*m_width2, y+inter*1.5, sizeText_medium, true);
    m_painter->setPen(m_pen_black);

    m_button_next.m_label = "Valider";
    drawButtonLabel2(m_button_next, ok ? COLOR_VALIDATE : COLOR_FAIL);
    m_button_back.m_label = "Refaire";
}

int CalibWidget::onMouse(int x, int y){
    Framework & f = Framework::instance();
    Config & c = f.m_config;

    if(m_button_cancel.isActive(x, y)){   //rien n'est ecrit dans la config
        m_close = true;
        return 0;
    }
    if(m_step > 0 && m_button_back.isActive(x, y)){
        m_step = (m_step == 3) ? 1 : m_step-1;   //depuis le resultat : on refait les bains
        m_button_next.m_label = "Continuer";
        return 0;
    }

    if(m_step == 0){
        onMouseKeyPad2(m_temp, x, y, 0.5);
        if(m_button_next.isActive(x, y)){
            m_step = 1;
        }
        return 0;
    }

    if(m_step == 1 || m_step == 2){
        if(m_button_next.isActive(x, y) && f.isSoilSettled()){
            if(m_step == 1){
                m_u_bas = f.m_last_soil_volt;
                m_step = 2;
            } else {
                m_u_haut = f.m_last_soil_volt;
                m_step = 3;
            }
        }
        return 0;
    }

    if(m_step == 3 && m_button_next.isActive(x, y)){
        //Seul endroit ou la calibration est appliquee.
        c.m_soil_ph_bas_m = m_u_bas;
        c.m_soil_ph_haut_m = m_u_haut;
        c.m_soil_temp_cal = m_temp.m_value;
        c.m_soil_check_score = getScore();

        time_t now = time(0);
        struct tm tstruct = *localtime(&now);
        char buf[80];
        strftime(buf, sizeof(buf), "%Y_%m_%d %H:%M", &tstruct);
        c.m_soil_check_date = buf;

        f.initOrLoadConfig();
        m_close = true;
    }
    return 0;
}
