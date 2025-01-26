#ifndef GPS_WIDGET_H
#define GPS_WIDGET_H

#include "../framework.hpp"
#include "base/base_widget.hpp"
#include "option_widget.hpp"
#include "diag_widget.hpp"
#include "debug_widget.hpp"
#include "rapide_option_widget.hpp"
#include "param_translateur_widget.hpp"
#include "first_widget.hpp"
#include "base/keyboard_widget.hpp"

enum EtatDraw {
    GuidageAutosteer,
    RecordTranslator,
    ReplayTranslator,
};

class GpsWidget : public BaseWidget {
    GpsWidget();
public:
    static GpsWidget * instance();
    
    FirstWidget m_first_widget;
    RapideOptionWidget m_rapide_option_widget;
    DiagnosticWidget m_diagnostic_widget;
    DebugWidget m_debug_widget;
    
    OptionWidget m_option_widget;
    ParamTranslateurWidget m_param_translateur_widget;
    std::vector<BaseWidget *> m_widgets;
    
    KeyPadWidget m_key_pad_widget;
    KeyBoardWidget m_key_board_widget;
    
    
    int m_widthMax;
    int m_heightMax;

    QPixmap * m_img_plus;
    QPixmap * m_img_moins;
    QPixmap * m_imgGuidage;
    QPixmap * m_imgParcelle;
    QPixmap * m_img_option;
    QPixmap * m_imgSatBlanc;
    QPixmap * m_imgSatVert;
    QPixmap * m_imgSatOrange;
    QPixmap * m_imgSatRouge;
    QPixmap * m_imgFleche;
    QPixmap * m_img_volant;
    QPixmap * m_img_volant_red;
    QPixmap * m_img_volant_green;
    QPixmap * m_imgVolantAutoGris;
    QPixmap * m_imgVolantAutoVert;
    QPixmap * m_imgOk;
    QPixmap * m_img_interrogation;
    QPixmap * m_img_balise;
    QPixmap * m_img_metre;
    QPixmap * m_img_offset;
    QPixmap * m_img_infos;
    
    QPixmap * m_img_compteur;
    QPixmap * m_img_3_point_up;
    QPixmap * m_img_3_point_down;
    
    QPixmap * m_excavator_2d;
    
    QPixmap * m_img_right;
    QPixmap * m_img_left;
    QPixmap * m_img_middle;
    
    ButtonGui m_button_debug;
    ButtonGui m_button3d;
    ButtonGui m_button_plus;
    ButtonGui m_button_moins;
    ButtonGui m_button_option;
    ButtonGui m_button_offset;
    ButtonGui m_button_balise;
    ButtonGui m_button_diag;
    
    ButtonGui m_buttonErrorOk;
    
    ButtonGui m_button_play;
    ButtonGui m_button_vitesse_plus;
    ButtonGui m_button_vitesse_moins;
    
    //ButtonGui m_button_volant;
    
    bool m_debug = false;
    bool m_is_develop = true;
    
    void loadImages();
    void setSize(int width, int height);
    void setPainter(QPainter * s);
    
    void drawButtons();
    void drawExcavator();
    void drawInfos();
    void drawInfosBasLeft();
    void drawInfosExcavator();
    void drawRightLeft();
    void drawAlertes();
    void drawLicence();
    
    void draw();

    int onMouse(int x, int y);
    void openRapideWidget(int page);
    
    //GPS
    double m_zoom;
    bool m_vue_3D;
    int m_last_x = 0, m_last_y = 0;
    
    void drawBackgroundGps();
    double m_xref;
    double m_yref;
    double m_a;
    double m_cosA;
    double m_sinA;
    
    void myProjete(double x, double y, double & x_res, double & y_res);
    void myProjete2(double x, double y, double & x_res, double & y_res);
    void myProjete2Pt(GpsPoint_ptr pt, double & x_res, double & y_res);
    bool mustBeDraw(double x, double y);
    
    void drawLine2(GpsLine_ptr l, QPen & pen);
    void drawCurve(Curve_ptr l, QPen & pen);
    void drawPoint(GpsPoint & pt, std::string s);
    void drawPoint_l(GpsPoint & pt, std::string s);
    void drawLines();
    void drawLineCurve();
    
    void drawParcelle(bool force = false);
    void drawSurfaceToDraw();
    
    void drawRect3D(double x, double y, double l, double lg);
    void drawBalises();
    
    void drawDebugEkf();
    
    void drawCercles();
    
    void drawGpsWidget();
};

#endif // main_widget_H
