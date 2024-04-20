#ifndef NETWORK_QT_H
#define NETWORK_QT_H

#include <QtSerialPort/QSerialPort>

#include "../../config/config.hpp"
#include <QTextStream>
#include <QByteArray>
#include <QObject>
#include <QTimer>
#include <QtNetwork>

QT_USE_NAMESPACE

class MyQTNetwork : public QObject{
    Q_OBJECT
    
    QString m_host_url;
    QString m_company;
public:
    MyQTNetwork();
    
    void initOrLoad(Config & config);
    void closeAll();
    
    void getParcelles();
    void getParcelle(const std::string & name);
    void saveParcelle(const std::string & name, const std::string & json);
    
    void postBalises(const std::string & json);
private slots:
    void handleNetwork(QNetworkReply *reply);
    void handleErrorGps(QSerialPort::SerialPortError error);
};


#endif
