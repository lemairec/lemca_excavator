#include "keyboard_widget.hpp"

void KeyPadWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    m_x = m_width*0.35;
    m_lg = m_width*0.3;
    
    int inter = m_width*0.07;
    int x = m_x+m_lg/2;
    int rayon = m_gros_button;
    
    int y = m_height*0.5- inter;
    m_button7.setResizeStd(x-inter, y, "7", true, 2*rayon, 2*rayon);
    m_button8.setResizeStd(x, y, "8", true, 2*rayon, 2*rayon);
    m_button9.setResizeStd(x+inter, y, "9", true, 2*rayon, 2*rayon);
    
    y = m_height*0.5;
    m_button4.setResizeStd(x-inter, y, "4", true, 2*rayon, 2*rayon);
    m_button5.setResizeStd(x, y, "5", true, 2*rayon, 2*rayon);
    m_button6.setResizeStd(x+inter, y, "6", true, 2*rayon, 2*rayon);
    
    y = m_height*0.5 + inter;
    m_button1.setResizeStd(x-inter, y, "1", true, 2*rayon, 2*rayon);
    m_button2.setResizeStd(x, y, "2", true, 2*rayon, 2*rayon);
    m_button3.setResizeStd(x+inter, y, "3", true, 2*rayon, 2*rayon);
    
    y = m_height*0.5 + 2*inter;
    m_button0.setResizeStd(x-inter, y, "0", true, 2*rayon, 2*rayon);
    m_button_point.setResizeStd(x, y, ".", true, 2*rayon, 2*rayon);
    m_button_ok.setResizeStd(x+inter, y, "OK", true, 2*rayon, 2*rayon);
    
}


void KeyPadWidget::draw(){
    m_painter->setPen(m_pen_black);
    m_painter->setBrush(m_brush_background_1);
    m_painter->drawRoundedRect(m_x, m_height*0.1, m_width*0.3, m_height*0.8, RAYON_ROUNDED, RAYON_ROUNDED);
    
    drawQText(m_title, m_x+m_lg/2, 0.15*m_height, sizeText_big, true);
    
    
    m_painter->setPen(m_pen_no);
    if(m_black_mode){
        m_painter->setBrush(m_brush_background_3);
    } else {
        m_painter->setBrush(m_brush_background_2);
    }
    m_painter->drawRoundedRect(m_x+m_width*0.05, m_height*0.2, m_width*0.2, m_height*0.1, 10, 10);
    QString s = m_res;
    m_painter->setPen(m_pen_black_inv);
    drawQText(s, m_x+m_lg/2, 0.25*m_height, sizeText_big, true);
    
    
    drawButtonLabel2(m_button1);
    drawButtonLabel2(m_button2);
    drawButtonLabel2(m_button3);
    drawButtonLabel2(m_button4);
    drawButtonLabel2(m_button5);
    drawButtonLabel2(m_button6);
    drawButtonLabel2(m_button7);
    drawButtonLabel2(m_button8);
    drawButtonLabel2(m_button9);
    drawButtonLabel2(m_button0);
    drawButtonLabel2(m_button_point);
    drawButtonLabel2(m_button_ok, COLOR_VALIDATE);
}

int KeyPadWidget::onMouse(int x, int y){
    if(m_button0.isActive(x, y)){
        addChiffre("0");
    } else if(m_button1.isActive(x, y)){
        addChiffre("1");
    } else if(m_button2.isActive(x, y)){
        addChiffre("2");
    } else if(m_button3.isActive(x, y)){
        addChiffre("3");
    } else if(m_button4.isActive(x, y)){
        addChiffre("4");
    } else if(m_button5.isActive(x, y)){
        addChiffre("5");
    } else if(m_button6.isActive(x, y)){
        addChiffre("6");
    } else if(m_button7.isActive(x, y)){
        addChiffre("7");
    } else if(m_button8.isActive(x, y)){
        addChiffre("8");
    } else if(m_button9.isActive(x, y)){
        addChiffre("9");
    } else if(m_button_point.isActive(x, y)){
        addChiffre(".");
    } else if(m_button_ok.isActive(x, y)){
        double res = m_res.toFloat();
        if(m_value){
            m_value->m_value = res;
        }
        m_close = true;
        return 1;
    }
    return 0;
};

void KeyPadWidget::addChiffre(QString s){
    m_res = m_res + s;
}


/*if(m_buttonA.isActive(x, y)){
 addLetter("A");
 } else if(m_buttonB.isActive(x, y)){
 addLetter("B");
 }*/

void KeyBoardWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    m_x = 20;
    m_lg = m_width*0.88;
    
    int inter = m_width*0.07;
    int rayon = m_gros_button;
    
    int y1 = m_height*0.5 - 2*inter;
    int y = y1;
    int x = m_x+m_lg/2-4.5*inter;

    y = y1+inter;
    x = m_x+m_lg/2-4.5*inter;
    m_button1_et.setResize(x, y, rayon);
    m_button2_home.setResize(x+inter, y, rayon);
    m_button3_virgule.setResize(x+2*inter, y, rayon);
    m_button4_dollard.setResize(x+3*inter, y, rayon);
    m_button5_slash.setResize(x+4*inter, y, rayon);
    m_button6_tiret.setResize(x+5*inter, y, rayon);
    m_button7_point_virgule.setResize(x+6*inter, y, rayon);
    m_button8_tiret_bas.setResize(x+7*inter, y, rayon);
    m_button9_tiret_bas.setResize(x+8*inter, y, rayon);
    m_button0.setResize(x+9*inter, y, rayon);
    
    y = y1+2*inter;
    x = m_x+m_lg/2-4.5*inter;
    m_button_maj.setResize(x-inter, y, rayon);
    m_buttonA.setResize(x, y, rayon);
    m_buttonZ.setResize(x+inter, y, rayon);
    m_buttonE.setResize(x+2*inter, y, rayon);
    m_buttonR.setResize(x+3*inter, y, rayon);
    m_buttonT.setResize(x+4*inter, y, rayon);
    m_buttonY.setResize(x+5*inter, y, rayon);
    m_buttonU.setResize(x+6*inter, y, rayon);
    m_buttonI.setResize(x+7*inter, y, rayon);
    m_buttonO.setResize(x+8*inter, y, rayon);
    m_buttonP.setResize(x+9*inter, y, rayon);
    
    y = y1+3*inter;
    x = m_x+m_lg/2-4.5*inter;
    m_button_remove.setResize(x-inter, y, rayon);
    m_buttonQ.setResize(x, y, rayon);
    m_buttonS.setResize(x+inter, y, rayon);
    m_buttonD.setResize(x+2*inter, y, rayon);
    m_buttonF.setResize(x+3*inter, y, rayon);
    m_buttonG.setResize(x+4*inter, y, rayon);
    m_buttonH.setResize(x+5*inter, y, rayon);
    m_buttonJ.setResize(x+6*inter, y, rayon);
    m_buttonK.setResize(x+7*inter, y, rayon);
    m_buttonL.setResize(x+8*inter, y, rayon);
    m_buttonM.setResize(x+9*inter, y, rayon);
    
    y = y1+4*inter;
    x = m_x+m_lg/2-4.5*inter;
    m_buttonW.setResize(x, y, rayon);
    m_buttonX.setResize(x+inter, y, rayon);
    m_buttonC.setResize(x+2*inter, y, rayon);
    m_buttonV.setResize(x+3*inter, y, rayon);
    m_buttonB.setResize(x+4*inter, y, rayon);
    m_buttonN.setResize(x+5*inter, y, rayon);
    m_buttonSpace.setResize(x+6*inter, y, rayon);
    
    
    //m_button_point.setResize(x, y, rayon);
    m_button_ok.setResize(x+8*inter, y, rayon);
    m_button_cancel.setResize(x+9*inter, y, rayon);
    
}

KeyBoardWidget::KeyBoardWidget(){
    //m_close = false;
}


void KeyBoardWidget::myDrawButton(ButtonGui * b, QString s, int color){
    b->m_label = s;
    drawButtonLabel2(*b, color);
}


void KeyBoardWidget::draw(){
    m_painter->setPen(m_pen_black_inv);
    m_painter->setBrush(m_brush_background_1);
    m_painter->drawRoundedRect(m_x, m_height*0.1, m_lg, m_height*0.8, RAYON_ROUNDED, RAYON_ROUNDED);
    
    drawQText("KeyBoard", m_x+m_lg/2, 0.15*m_height, sizeText_big, true);
    
    
    m_painter->setPen(m_pen_no);
    if(m_black_mode){
        m_painter->setBrush(m_brush_background_3);
    } else {
        m_painter->setBrush(m_brush_background_2);
    }
    m_painter->drawRoundedRect(m_x+m_lg/2-m_width*0.1, m_height*0.2, m_width*0.2, m_height*0.1, 10, 10);
    
    m_painter->setPen(m_pen_black_inv);
    drawQText(m_res, m_x+m_lg/2, 0.25*m_height, sizeText_big, true);
    
    
    if(m_maj){
        myDrawButton(&m_button_remove, "<-");
        if(!m_just_maj){
            myDrawButton(&m_button_maj, "maj");
        }
        myDrawButton(&m_button1_et, "1");
        myDrawButton(&m_button2_home, "2");
        myDrawButton(&m_button3_virgule, "3");
        myDrawButton(&m_button4_dollard, "4");
        myDrawButton(&m_button5_slash, "5");
        myDrawButton(&m_button6_tiret, "6");
        myDrawButton(&m_button7_point_virgule, "7");
        myDrawButton(&m_button8_tiret_bas, "8");
        myDrawButton(&m_button9_tiret_bas, "9");
        myDrawButton(&m_button0, "0");
        myDrawButton(&m_buttonA, "A");
        myDrawButton(&m_buttonB, "B");
        myDrawButton(&m_buttonC, "C");
        myDrawButton(&m_buttonD, "D");
        myDrawButton(&m_buttonE, "E");
        myDrawButton(&m_buttonF, "F");
        myDrawButton(&m_buttonG, "G");
        myDrawButton(&m_buttonH, "H");
        myDrawButton(&m_buttonI, "I");
        myDrawButton(&m_buttonJ, "J");
        myDrawButton(&m_buttonK, "K");
        myDrawButton(&m_buttonL, "L");
        myDrawButton(&m_buttonM, "M");
        myDrawButton(&m_buttonN, "N");
        myDrawButton(&m_buttonO, "O");
        myDrawButton(&m_buttonP, "P");
        myDrawButton(&m_buttonQ, "Q");
        myDrawButton(&m_buttonR, "R");
        myDrawButton(&m_buttonS, "S");
        myDrawButton(&m_buttonT, "T");
        myDrawButton(&m_buttonU, "U");
        myDrawButton(&m_buttonV, "V");
        myDrawButton(&m_buttonW, "W");
        myDrawButton(&m_buttonX, "X");
        myDrawButton(&m_buttonY, "Y");
        myDrawButton(&m_buttonZ, "Z");
        myDrawButton(&m_buttonSpace, "_");
    } else {
        myDrawButton(&m_button_remove, "<-");
        myDrawButton(&m_button_maj, "MAJ");
        myDrawButton(&m_button1_et, "&");
        myDrawButton(&m_button2_home, "~");
        myDrawButton(&m_button3_virgule, ",");
        myDrawButton(&m_button4_dollard, "$");
        myDrawButton(&m_button5_slash, "/");
        myDrawButton(&m_button6_tiret, "-");
        myDrawButton(&m_button7_point_virgule, ";");
        myDrawButton(&m_button8_tiret_bas, "_");
        myDrawButton(&m_button9_tiret_bas, "_");
        myDrawButton(&m_button0, "0");
        myDrawButton(&m_buttonA, "a");
        myDrawButton(&m_buttonB, "b");
        myDrawButton(&m_buttonC, "c");
        myDrawButton(&m_buttonD, "d");
        myDrawButton(&m_buttonE, "e");
        myDrawButton(&m_buttonF, "f");
        myDrawButton(&m_buttonG, "g");
        myDrawButton(&m_buttonH, "h");
        myDrawButton(&m_buttonI, "i");
        myDrawButton(&m_buttonJ, "j");
        myDrawButton(&m_buttonK, "k");
        myDrawButton(&m_buttonL, "l");
        myDrawButton(&m_buttonM, "m");
        myDrawButton(&m_buttonN, "n");
        myDrawButton(&m_buttonO, "o");
        myDrawButton(&m_buttonP, "p");
        myDrawButton(&m_buttonQ, "q");
        myDrawButton(&m_buttonR, "r");
        myDrawButton(&m_buttonS, "s");
        myDrawButton(&m_buttonT, "t");
        myDrawButton(&m_buttonU, "u");
        myDrawButton(&m_buttonV, "v");
        myDrawButton(&m_buttonW, "w");
        myDrawButton(&m_buttonX, "x");
        myDrawButton(&m_buttonY, "y");
        myDrawButton(&m_buttonZ, "z");
        myDrawButton(&m_buttonSpace, " ");
    }

    
    myDrawButton(&m_button_ok, "ok", COLOR_VALIDATE);
    myDrawButton(&m_button_cancel, "cancel", COLOR_CANCEL);
}

void KeyBoardWidget::addLetter(QString se){
    m_res = m_res + se;
}



void KeyBoardWidget::addLetter2(QString se1, QString se2){
    if(m_maj){
        m_res = m_res + se1;
    } else {
        m_res = m_res + se2;
    }
}

void KeyBoardWidget::removeLetter(){
    m_res = m_res.left(m_res.size()-1);
}


int KeyBoardWidget::onMouse(int x, int y){
    if(m_button_maj.isActive(x, y)){
        if(!m_just_maj){
            m_maj = !m_maj;
        }
    } else if(m_button_remove.isActive(x, y)){
        removeLetter();
    } else if(m_button0.isActive(x, y)){
        addLetter("0");
    } else if(m_button1_et.isActive(x, y)){
        addLetter2("1", "&");
    } else if(m_button2_home.isActive(x, y)){
        addLetter2("2", "~");
    } else if(m_button3_virgule.isActive(x, y)){
        addLetter2("3", ",");
    } else if(m_button4_dollard.isActive(x, y)){
        addLetter2("4", "$");
    } else if(m_button5_slash.isActive(x, y)){
        addLetter2("5", "/");
    } else if(m_button6_tiret.isActive(x, y)){
        addLetter2("6", "-");
    } else if(m_button7_point_virgule.isActive(x, y)){
        addLetter2("7", ";");
    } else if(m_button8_tiret_bas.isActive(x, y)){
        addLetter2("8", "_");
    } else if(m_button9_tiret_bas.isActive(x, y)){
        addLetter2("9", "_");
    } else if(m_buttonA.isActive(x, y)){
        addLetter2("A", "a");
    } else if(m_buttonB.isActive(x, y)){
        addLetter2("B", "b");
    } else if(m_buttonC.isActive(x, y)){
        addLetter2("C", "c");
    } else if(m_buttonD.isActive(x, y)){
        addLetter2("D", "d");
    } else if(m_buttonE.isActive(x, y)){
        addLetter2("E", "e");
    } else if(m_buttonF.isActive(x, y)){
        addLetter2("F", "f");
    } else if(m_buttonG.isActive(x, y)){
        addLetter2("G", "g");
    } else if(m_buttonH.isActive(x, y)){
        addLetter2("H", "h");
    } else if(m_buttonI.isActive(x, y)){
        addLetter2("I", "i");
    } else if(m_buttonJ.isActive(x, y)){
        addLetter2("J", "j");
    } else if(m_buttonK.isActive(x, y)){
        addLetter2("K", "k");
    } else if(m_buttonL.isActive(x, y)){
        addLetter2("L", "l");
    } else if(m_buttonM.isActive(x, y)){
        addLetter2("M", "m");
    } else if(m_buttonN.isActive(x, y)){
        addLetter2("N", "n");
    } else if(m_buttonO.isActive(x, y)){
        addLetter2("O", "o");
    } else if(m_buttonP.isActive(x, y)){
        addLetter2("P", "p");
    } else if(m_buttonQ.isActive(x, y)){
        addLetter2("Q", "q");
    } else if(m_buttonR.isActive(x, y)){
        addLetter2("R", "r");
    } else if(m_buttonS.isActive(x, y)){
        addLetter2("S", "s");
    } else if(m_buttonT.isActive(x, y)){
        addLetter2("T", "t");
    } else if(m_buttonU.isActive(x, y)){
        addLetter2("U", "u");
     } else if(m_buttonV.isActive(x, y)){
        addLetter2("V", "v");
    } else if(m_buttonW.isActive(x, y)){
        addLetter2("W", "w");
    } else if(m_buttonX.isActive(x, y)){
        addLetter2("X", "x");
    } else if(m_buttonY.isActive(x, y)){
        addLetter2("Y", "y");
    } else if(m_buttonZ.isActive(x, y)){
        addLetter2("Z", "z");
    } else if(m_buttonSpace.isActive(x, y)){
       addLetter2("_", " ");
    } else if(m_button_ok.isActive(x, y)){
        if(m_value){
            m_value->m_text = m_res.toUtf8().constData();
        }
        //m_res.clear();
        m_close = true;
        return 1;
    } else if(m_button_cancel.isActive(x, y)){
        m_res.clear();
        m_close = true;
    }
    return 0;
}
