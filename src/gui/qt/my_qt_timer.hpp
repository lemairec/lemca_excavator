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
    Q_OBJECT
public :
    QTimer m_timer_100_ms;
    QTimer m_timer_25_ms;
    
    MyQTTimer();
    ~MyQTTimer();
    
    void init();

public slots:
    void handle100ms();
    void handle25ms();
};


#endif
