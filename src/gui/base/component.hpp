#ifndef BASE_COMPONENT_WIDGET_H
#define BASE_COMPONENT_WIDGET_H

#include "../qt/include_qt.hpp"

const int COLOR_RED = 1;
const int COLOR_GREEN = 2;
const int COLOR_CHECK = 3;

const int COLOR_OTHER = 4;

const int COLOR_CANCEL = 5;
const int COLOR_VALIDATE = 6;
const int COLOR_FAIL = 7;
const int COLOR_WHITE = 8;

class KeyPadWidget;
class KeyBoardWidget;

enum SizeText{
    sizeText_logo,
    sizeText_little,
    sizeText_medium,
    sizeText_big,
    sizeText_bigbig
};

struct ButtonGui{
    ButtonGui(){};
    
    void setResize(int x, int y, int rayon);
    void setResize(int x, int y, QString label, bool center = false, int width = 120, int heigth = 50);
    void setResizeStd(int x, int y, const std::string & label, bool center = false, int width = 120, int heigth = 50);
    
    bool isActive(int x, int y);
    int m_x = 0;
    int m_y = 0;
    int m_rayon = 0;
    
    int m_width = 0;
    int m_height = 0;
    
    int m_type = 0;
    
    QString m_label = "";
    int m_labelInt = 0;
};

struct ValueGui{
    ValueGui(){};
    
    void setResize(int x, int y, int rayon, std::string label = "");
    void setResizeLabel(int x, int y, std::string label, bool center = false);
    
    int isActive(double x, double y);
    
    double getMultValue(double x, double y);
    double getMultValue2(double x, double y, double value);
    double getIntValue(double x, double y);
    
    double m_x = 0;
    double m_y = 0;
    double m_rayon = 0;
    double m_label_x = 0;
    double m_label_y = 0;
    bool m_label_center = true;
    std::string m_label;
    ButtonGui m_button_plus;
    ButtonGui m_button_moins;
    
    int m_type = 0;
    double m_value = 0;
};

struct ValueGuiKeyBoard{
    ValueGuiKeyBoard(){};
    
    void setResize(int x, int y, int width = 160);
    
    int isActive(double x, double y);
    
    int m_x = 0;
    int m_y = 0;
    int m_width = 160;
    std::string m_text;
};


struct SelectButtonGui{
    SelectButtonGui(){};

    void setResize(int x, int y, int rayon);
    void setResize(int x, int y, const std::string & label, bool center = false, int width = 120, int heigth = 50);
   
    double m_x = 0;
    double m_y = 0;
    double m_rayon = 0;
    
    std::string m_label;
    std::vector<std::string> m_values;
    std::vector<int> m_values_int;
    int m_selectedValue = 0;
    ButtonGui m_buttonOpen;
    bool m_open = false;
    
    std::vector<ButtonGui *> m_buttons;
    
    void addValue(std::string s);
    void addValueInt(std::string s, int i);
    void clear();
    
    std::string getValueString();
    int getValueInt();
    
    void setValueInt(int i);
    void setSelectedValue(int i);
    void setValueString(std::string s);
    
    int m_type = 0;
};

#endif
