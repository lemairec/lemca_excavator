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

    //m_mode 0 = job courant, 1 = liste des jobs enregistres
    int m_mode = 0;
    size_t m_page = 0;
    std::vector<std::string> m_jobs;
    std::vector<int> m_jobs_points;   //nbr de points, lu une fois a l'ouverture de la liste
    std::vector<ButtonGui> m_job_rows;
    ButtonGui m_button_open_list;
    ButtonGui m_button_back;
    ButtonGui m_button_page_up;
    ButtonGui m_button_page_down;

    void drawJob();
    void drawList();
    int onMouseJob(int x, int y);
    int onMouseList(int x, int y);

public:
    KeyBoardWidget m_keyboard_widget;

    JobWidget();

    void setPainter(QPainter * p);
    void setSize(int width, int height);
    void open();
    void draw();
    int onMouse(int x, int y);
};
