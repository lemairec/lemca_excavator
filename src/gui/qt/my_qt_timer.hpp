#ifndef TIMER_QT_H
#define TIMER_QT_H

#include <QtSerialPort/QSerialPort>

#include "../../config/config.hpp"
#include <QTextStream>
#include <QByteArray>
#include <QObject>
#include <QTimer>

QT_USE_NAMESPACE

class MyQTTimer : public QObject{
    QTimer m_timer_100_ms;
    
    MyQTTimer();
    ~MyQTTimer();
    
    void handle100ms();
};


#endif
