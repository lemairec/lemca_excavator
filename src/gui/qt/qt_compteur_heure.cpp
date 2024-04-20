#include "qt_compteur_heure.hpp"
#include "include_qt.hpp"
#include "../../util/directory_manager.hpp"
#include "../../util/log.hpp"
#include "../../framework.hpp"
#include <sstream>
#include <QMetaEnum>
#include <QSerialPortInfo>

#include "environnement.hpp"

QtCompteurHeure::QtCompteurHeure(){
    connect(&m_timerMinutes, SIGNAL(timeout()), this, SLOT(handleSeconde()));
    
    m_timerMinutes.start(1000);
    
    std::string s2 = DirectoryManager::instance().getDataDirectory() + "/compteur_h.txt";
    INFO(s2);
    QString s3 = QString::fromStdString(s2);
    QFile f(s3);
    f.open(QIODevice::ReadOnly);
    QTextStream s(&f);

    s >> m_heure_total ;
    s >> m_ha_total;
    s >> m_heure_partiel;
    s >> m_ha_partiel;
    
    INFO(m_heure_total);
    INFO(m_ha_total);
    INFO(m_heure_partiel);
    INFO(m_ha_partiel);
}

QtCompteurHeure::~QtCompteurHeure(){
    INFO("destructor");
}

void QtCompteurHeure::handleSeconde(){
    LOG_FUNCTION();
    double time = 1.0/3600.0;
    
    m_heure_session = m_heure_session+time;
    m_heure_total = m_heure_total+time;
    m_heure_partiel = m_heure_partiel+time;
    
    /*double dist_m = f.m_current_vitesse*time*1000;
    
    
    
    double ha = dist_m*f.m_config.m_largeur_m/10000.0;
    
    
    if(!f.m_pilot_module.m_point_3){
        INFO("**** session " << m_heure_session << " h - " << m_ha_session << " ha ");
        m_ha_total = m_ha_total+ha;
        m_ha_partiel = m_ha_partiel+ha;
        m_ha_session = m_ha_session+ha;
    }*/
        
    save();
    
}

void QtCompteurHeure::resetPartiel(){
    m_heure_partiel = 0;
    m_ha_partiel = 0;
    save();
}
void QtCompteurHeure::save(){
    std::string s2 = DirectoryManager::instance().getDataDirectory() + "/compteur_h.txt";
    QString s3 = QString::fromStdString(s2);
    QFile file(s3);
    file.open(QIODevice::WriteOnly);
    QTextStream s(&file);

    s << QString::number(m_heure_total) << "\n";
    s << QString::number(m_ha_total) << "\n";
    s << QString::number(m_heure_partiel) << "\n";
    s << QString::number(m_ha_partiel) << "\n";
    //INFO("************ total " << m_heure_total);
}
