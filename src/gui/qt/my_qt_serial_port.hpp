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
    
    
public:
    QSerialPort m_port1_gps;
    std::string m_port1_gps_serial;
    int m_port1_gps_count = 0;
    std::string m_port1_gps_etat;
    
    QSerialPort m_port2_mcu;
    std::string m_port2_mcu_serial;
    int m_port2_mcu_count = 0;
    std::string m_port2_mcu_etat;
    
    QSerialPort m_port3_soil;
    std::string m_port3_soil_serial;
    int m_port3_soil_count = 0;
    std::string m_port3_soil_etat;
    
    std::list<std::string> messages;
    
    MyQTSerialPorts();
    ~MyQTSerialPorts();
    
    void initOrLoad(const Config & config);
    
    void addMessage(const std::string & s);
    void startConnect(int i, QSerialPort & port, std::string & old_serial, const std::string & serial, int baudrate);
    
    void closeAll();
    
    bool isPort1GpsOpen(){return m_port1_gps.isOpen();};
    bool isPort2SerialOpen(){return m_port2_mcu.isOpen();};
    bool isPort3SoilOpen(){return m_port3_soil.isOpen();};
    
    void writePort1GpsStr(const std::string & l);
    void writePort2McuStr(const std::string & l);
    void writePort3SoilHexa(std::vector<unsigned char> & l);
    
    std::vector<std::string> m_serials;
    void addSerialPorts(std::string s);
    std::vector<std::string> & getAvailablePorts();
    
public slots:
    void handlePort1GpsReadyRead();
    void handlePort1GpsError(QSerialPort::SerialPortError error);

    void handlePort2McuReadyRead();
    void handlePort2McuError(QSerialPort::SerialPortError error);

    void handlePort3SoilReadyRead();
    void handlePort3SoilError(QSerialPort::SerialPortError error);

};


#endif
