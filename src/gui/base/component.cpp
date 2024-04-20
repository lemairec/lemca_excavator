#include "base_widget.hpp"
#include "../../util/directory_manager.hpp"
#include <iostream>
#include <sstream>

void ButtonGui::setResize(int x, int y, int rayon){
    m_x = x;
    m_y = y;
    m_rayon = rayon;
    m_width = 2*rayon;
    m_height = 2*rayon;
}

void ButtonGui::setResize(int x, int y, QString label, bool center, int width, int heigth){
    m_x = x;
    if(!center){
        m_x = x+width/2;
    }
    m_y = y;
    m_width = width;
    m_height = heigth;
    m_label = label;
}

void ButtonGui::setResizeStd(int x, int y,  const std::string & label, bool center, int width, int heigth){
    QString s = QString::fromStdString(label);
    setResize(x, y, s, center, width, heigth);
}

bool ButtonGui::isActive(int x, int y){
    if(x > this->m_x-m_width/2  && x < this->m_x+m_width/2 && y > this->m_y-m_height/2  && y < this->m_y+m_height/2){
        return true;
    }
    return false;
}




void ValueGui::setResize(int x, int y, int rayon, std::string label){
    m_x = x;
    m_y = y;
    m_rayon = rayon;
    m_label = label;
    m_button_plus.setResize(x+65, y, rayon);
    m_button_moins.setResize(x-65, y, rayon);
    
}
void ValueGui::setResizeLabel(int x, int y, std::string label, bool center){
    m_label = label;
    m_label_x = x;
    m_label_y = y;
    m_label_center = center;
}





int ValueGui::isActive(double x, double y){
    if(m_button_plus.isActive(x, y)){
        return 1;
    }
    if(m_button_moins.isActive(x, y)){
        return -1;
    }
    return 0;
}

double ValueGui::getMultValue2(double x, double y, double value){
    if(m_button_plus.isActive(x, y)){
        return value;
    }
    if(m_button_moins.isActive(x, y)){
        return 1.0/value;
    }
    return 1.0;
}

double ValueGui::getMultValue(double x, double y){
    return getMultValue2(x, y, 1.2);
}

double ValueGui::getIntValue(double x, double y){
    if(m_button_plus.isActive(x, y)){
        return 1;
    }
    if(m_button_moins.isActive(x, y)){
        return -1;
    }
    return 0;
}


void SelectButtonGui::setResize(int x, int y, int rayon){
    m_x = x;
    m_y = y;
    m_rayon = rayon;
    m_buttonOpen.setResize(x, m_y, rayon);
    for(size_t i = 0; i < m_values.size(); ++i){
        m_buttons[i]->setResize(x, m_y+m_rayon*2*(i+1), rayon*0.8);
    }
    
}

void SelectButtonGui::setResize(int x, int y, const std::string & label, bool center, int width, int heigth){
    m_x = x;
    m_y = y;
    m_label = label;
    //m_rayon = rayon;
    m_buttonOpen.setResizeStd( x,  y, "",  center,  width ,  heigth);
    for(size_t i = 0; i < m_values.size(); ++i){
        //m_buttons[i]->setResize(x, m_y+m_rayon*2*(i+1), 0*0.8);
    }
}

void SelectButtonGui::addValue(std::string s){
    m_values.push_back(s);
    m_buttons.clear();
    for(size_t i = 0; i < m_values.size(); ++i){
        ButtonGui * b = new ButtonGui();
        b->setResize(m_x+m_rayon, m_y+m_rayon*2*(i+1), m_rayon*0.8);
        m_buttons.push_back(b);
    }
}

void SelectButtonGui::addValueInt(std::string s, int i){
    m_values.push_back(s);
    m_values_int.push_back(i);
    m_buttons.clear();
    for(size_t i = 0; i < m_values.size(); ++i){
        ButtonGui * b = new ButtonGui();
        b->setResize(m_x+m_rayon, m_y+m_rayon*2*(i+1), m_rayon*0.8);
        m_buttons.push_back(b);
    }
}

void SelectButtonGui::clear(){
    m_values.clear();
    m_buttons.clear();
    m_values_int.clear();
}

std::string SelectButtonGui::getValueString(){
    if(m_selectedValue >=0 && m_selectedValue < ((int)m_values.size())){
        return m_values[m_selectedValue];
    } else {
        return "ERROR";
    }
 }
 
 int SelectButtonGui::getValueInt(){
    if(m_selectedValue >=0 && m_selectedValue < ((int)m_values.size())){
        return m_values_int[m_selectedValue];
    } else {
        return 0;
    }
}
 
void SelectButtonGui::setValueInt(int i){
    m_selectedValue = -1;
    for(size_t j = 0; j < m_values_int.size(); ++j){
        if(m_values_int[j] == i){
            m_selectedValue = j;
            m_buttonOpen.m_label = QString::number(m_values_int[j]);
            return;
        }
    }
    m_buttonOpen.m_label = QString::fromStdString("ERROR");
}
void SelectButtonGui::setValueString(std::string s){
    m_selectedValue = -1;
    for(size_t j = 0; j < m_values.size(); ++j){
        if(m_values[j] == s){
            m_selectedValue = j;
            m_buttonOpen.m_label = QString::fromStdString(s);
            return;
        }
    }
    m_buttonOpen.m_label = QString::fromStdString("ERROR");
}

void SelectButtonGui::setSelectedValue(int i){
    m_selectedValue = i;
    m_buttonOpen.m_label = QString::fromStdString(getValueString());
}

void ValueGuiKeyBoard::setResize(int x, int y, int width){
    m_x = x;
    m_y = y;
    m_width = width;
}

int ValueGuiKeyBoard::isActive(double x, double y){
    return 0;
}
