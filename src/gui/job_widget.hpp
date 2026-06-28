#pragma once

#include "base/base_widget.hpp"
#include "base/keyboard_widget.hpp"

class Config;

// Vue "Job" : saisie du client et du champ pour Land Manager.
class JobWidget : public BaseWidget {
    int m_x2, m_y2;
    int m_width2, m_height2;

    ValueGuiKeyBoard m_client;
    ValueGuiKeyBoard m_field;

    ButtonGui m_button_send;
    ButtonGui m_button_close;

public:
    KeyBoardWidget m_keyboard_widget;

    JobWidget();

    void setPainter(QPainter * p);
    void setSize(int width, int height);
    void open();
    void draw();
    int onMouse(int x, int y);
};
