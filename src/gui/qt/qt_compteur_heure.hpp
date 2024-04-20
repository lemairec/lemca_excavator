#ifndef QT_COMPTEUR_HEURE_H
#define QT_COMPTEUR_HEURE_H

#include <QtSerialPort/QSerialPort>

#include "../../config/config.hpp"
#include <QTextStream>
#include <QByteArray>
#include <QObject>
#include <QTimer>

QT_USE_NAMESPACE

class QtCompteurHeure : public QObject{
    Q_OBJECT
    
public:
    QtCompteurHeure();
    ~QtCompteurHeure();
    
    void initOrLoad(const Config & config);
    
    void open();
    void save();

    void resetPartiel();
public slots:
    void handleSeconde();
private:
    QTimer m_timerMinutes;

public:
    double m_heure_session = 0;
    double m_ha_session = 0;
    
    double m_heure_total = 0;
    double m_ha_total = 0;
    
    double m_ha_partiel = 0;
    double m_heure_partiel = 0;
};



#endif
