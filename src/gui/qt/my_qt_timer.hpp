#ifndef SERIAL_QT_H
#define SERIAL_QT_H

#include <QtSerialPort/QSerialPort>

#include "../../config/config.hpp"
#include <QTextStream>
#include <QByteArray>
#include <QObject>
#include <QTimer>

QT_USE_NAMESPACE

class MyQTSerialPorts : public QObject{
    Q_OBJECT
    
    QSerialPort m_serialPort;
    
    std::string m_gps_serial;
    std::string m_new_gps_serial;
    QSerialPort m_gps_port;
    
    std::vector<std::string> m_serials;
public:
    QTimer m_timerPilot;
    
    MyQTSerialPorts();
    ~MyQTSerialPorts();
    
    void initOrLoad(const Config & config);
    void closeAll();
    
    bool pilotIsOpen(){return m_serialPort.isOpen();};
    void writePilotSerialS(const std::string & l);
    
    void addSerialPorts(std::string s);
    std::vector<std::string> & getAvailablePorts();
signals:
    void initSignal();
    void signalWritePilotSerialS(QByteArray b);
    
public slots:
    void initSlot();

    void handleGpsReadyRead();
    void handleGpsError(QSerialPort::SerialPortError error);

    void handlePilotReadyRead();
    void handlePilotError(QSerialPort::SerialPortError error);
    
    void handleTimer();
};


#endif
