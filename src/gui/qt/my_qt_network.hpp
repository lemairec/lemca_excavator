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

    // Envoi du fichier soil.txt du job courant a Land Manager
    bool uploadSoil(Config & config, const std::string & file_path);
    bool m_soil_uploading = false;
    std::string m_soil_status;   // "" / "envoi..." / "carte envoyee" / "echec ..."

    // Verification du login Land Manager (sans envoi)
    void checkLogin(Config & config);
    bool m_lm_connected = false;
    bool m_lm_checking = false;
    std::string m_lm_status;   // texte affiche (connecte / erreur)
private slots:
    void handleNetwork(QNetworkReply *reply);
    void handleSoilUpload(QNetworkReply *reply);
    void handleLoginCheck(QNetworkReply *reply);
    void handleErrorGps(QSerialPort::SerialPortError error);
};


#endif
