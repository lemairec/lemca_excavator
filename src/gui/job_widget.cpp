#include "job_widget.hpp"

#include "../framework.hpp"
#include "../config/config.hpp"

JobWidget::JobWidget(){
    m_close = true;
}

void JobWidget::setPainter(QPainter *p){
    m_painter = p;
    m_keyboard_widget.setPainter(p);
}

void JobWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    // carte centree
    m_width2 = m_width*0.5;
    m_height2 = m_height*0.8;
    m_x2 = (m_width - m_width2)/2;
    m_y2 = (m_height - m_height2)/2;

    int cx = m_x2 + 0.62*m_width2;
    int cw = 0.42*m_width2;
    m_client.setResize(cx, m_y2 + 0.32*m_height2, cw);
    m_field.setResize (cx, m_y2 + 0.47*m_height2, cw);

    m_button_send.setResizeStd(m_x2 + 0.5*m_width2, m_y2 + 0.74*m_height2, "Envoyer sol", true, 0.4*m_width2);
    m_button_close.setResizeStd(m_x2 + 0.5*m_width2, m_y2 + 0.90*m_height2, "Fermer", true, 0.3*m_width2);

    m_keyboard_widget.setSize(width, height);
}

void JobWidget::open(){
    BaseWidget::open();
    Framework & f = Framework::instance();
    Config & config = f.m_config;
    m_client.m_text = config.m_landmanager_client;
    m_field.m_text  = config.m_landmanager_field;
    if(!f.m_qt_network.m_soil_uploading){
        f.m_qt_network.m_soil_status.clear(); // pas de statut perime a la reouverture
    }
}

void JobWidget::draw(){
    Framework & f = Framework::instance();
    Config & config = f.m_config;

    m_painter->setPen(m_pen_black);
    m_painter->setBrush(m_brush_white);
    m_painter->drawRoundedRect(m_x2, m_y2, m_width2, m_height2, RAYON_ROUNDED, RAYON_ROUNDED);

    drawText("Job - Land Manager", m_x2+0.5*m_width2, m_y2+0.12*m_height2, sizeText_big, true);

    m_painter->setPen(m_pen_black);
    drawText("Client", m_x2+0.12*m_width2, m_client.m_y, sizeText_medium);
    drawValueGuiKeyBoard(m_client);
    drawText("Champ",  m_x2+0.12*m_width2, m_field.m_y, sizeText_medium);
    drawValueGuiKeyBoard(m_field);

    // statut connexion Land Manager
    MyQTNetwork & net = f.m_qt_network;
    QColor c = net.m_lm_connected ? QColor(0x35,0xB8,0x56) : QColor(0xFF,0x37,0x4B);
    if(net.m_lm_checking){ c = QColor(0x88,0x88,0x88); }
    m_painter->setPen(QPen(c));
    std::string st = "Land Manager : " + (net.m_lm_status.empty() ? std::string("non connecte") : net.m_lm_status);
    drawText(st, m_x2+0.5*m_width2, m_y2+0.62*m_height2, sizeText_medium, true);
    m_painter->setPen(m_pen_black);

    drawButtonLabel2(m_button_send, net.m_soil_uploading ? COLOR_OTHER
                                  : (net.m_lm_connected ? COLOR_VALIDATE : COLOR_FAIL));
    drawButtonLabel2(m_button_close);

    // statut de l'envoi de la carte
    if(!net.m_soil_status.empty()){
        QColor cs = QColor(0x88,0x88,0x88); // en cours
        if(!net.m_soil_uploading){
            cs = net.m_soil_status == "carte envoyee" ? QColor(0x35,0xB8,0x56)
                                                      : QColor(0xFF,0x37,0x4B);
        }
        m_painter->setPen(QPen(cs));
        drawText(net.m_soil_status, m_x2+0.5*m_width2, m_y2+0.82*m_height2, sizeText_medium, true);
        m_painter->setPen(m_pen_black);
    }

    if(!m_keyboard_widget.m_close){
        m_keyboard_widget.draw();
    }
}

int JobWidget::onMouse(int x, int y){
    Framework & f = Framework::instance();
    Config & config = f.m_config;

    if(!m_keyboard_widget.m_close){
        if(m_keyboard_widget.onMouse(x, y)){
            config.m_landmanager_client = m_client.m_text;
            config.m_landmanager_field  = m_field.m_text;
            config.save();
        }
        return 0;
    }

    if(isActiveValueGuiKeyBoard(m_client, x, y)){
        m_keyboard_widget.m_close = false;
        m_keyboard_widget.setValueGuiKeyBoard(&m_client);
    }
    if(isActiveValueGuiKeyBoard(m_field, x, y)){
        m_keyboard_widget.m_close = false;
        m_keyboard_widget.setValueGuiKeyBoard(&m_field);
    }

    if(m_button_send.isActive(x, y) && !f.m_qt_network.m_soil_uploading){
        config.m_landmanager_client = m_client.m_text;
        config.m_landmanager_field  = m_field.m_text;
        config.save();
        const std::string & path = f.m_job_manager.m_data_path;
        if(path.empty()){
            f.m_qt_network.m_soil_status = "aucun job actif";
            WARN("envoi sol: aucun job actif");
        } else {
            f.m_qt_network.uploadSoil(config, path);
        }
    }
    if(m_button_close.isActive(x, y)){
        m_close = true;
    }
    return 0;
}
