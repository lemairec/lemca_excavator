#include "my_qt_file.hpp"
#include "include_qt.hpp"
#include "../../framework.hpp"
#include <sstream>
#include <QMetaEnum>
#include <QSerialPortInfo>

#include "environnement.hpp"

#define FILE_TIME 100

MyQTFile::MyQTFile(){
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(handleLineFile()));
}

void MyQTFile::setVitesse(){
    m_timer.setInterval(100/2/m_vitesse);
}

void MyQTFile::vitessePlus(){
    if(m_vitesse == 0.4){
        m_vitesse = 1.0;
    } else if(m_vitesse == 1.0){
        m_vitesse = 4.0;
    } else if(m_vitesse == 4.0){
        m_vitesse = 16.0;
    } else if(m_vitesse == 16.0){
        m_vitesse = 32.0;
    }
    
    setVitesse();
}

void MyQTFile::vitesseMoins(){
    if(m_vitesse == 1.0){
        m_vitesse = 0.4;
    } else if(m_vitesse == 4.0){
        m_vitesse = 1.0;
    } else if(m_vitesse == 16.0){
        m_vitesse = 4.0;
    } else if(m_vitesse == 32.0){
        m_vitesse = 16.0;
    }
    
    setVitesse();
}


void MyQTFile::initOrLoad(Config & config){
    DEBUG("begin");
    if(config.m_port1_gps_serial == "file"){
        INFO("file " << config.m_gps_file);
        openFile(config);
    }
    DEBUG("end");
};

void MyQTFile::handleLineFile(){
    DEBUG("begin");
    if(m_text_stream == NULL){
        WARN("null");
        return;
    }
    if(m_pause){
        return;
    }
    QString line = m_text_stream->readLine();
    if(!line.isNull()){
       std::string line_s = line.toUtf8().constData();
        if(line_s == "[savePointA]"){
            //Framework::instance().savePointA();
        } else if(line_s == "[savePointB]"){
            //Framework::instance().savePointB();
        }
       for(size_t i = 0; i < line_s.size(); ++i){
           
           Framework::instance().m_nmea_parser_gps.readChar(line_s[i]);
       }
       Framework::instance().m_nmea_parser_gps.readChar('\n');
       //line = m_text_stream->readLine();
    }
    DEBUG("end");
}


void MyQTFile::openFile(Config & config){
    DEBUG("begin");
    if(m_text_stream == NULL){
        QFile * file = new QFile(QString::fromStdString(config.m_gps_file));
        if (!file->open(QIODevice::ReadOnly | QIODevice::Text)){
            WARN("can not open file");
            return;
        }

        m_text_stream = new QTextStream(file);
        m_timer.start(FILE_TIME);
        setVitesse();
    }
    DEBUG("end");
}
