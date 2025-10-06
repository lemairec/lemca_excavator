#include "my_qt_serial_port.hpp"
#include "include_qt.hpp"
#include "../../framework.hpp"
#include "../../util/directory_manager.hpp"
#include <sstream>
#include <QMetaEnum>
#include <QSerialPortInfo>

#include "environnement.hpp"

MyQTSerialPorts::MyQTSerialPorts(){
    QObject::connect(this, SIGNAL(initSignal()), this, SLOT(initSlot()));
    
    connect(&m_gps_port, SIGNAL(readyRead()), this, SLOT(handleGpsReadyRead()));
    connect(&m_gps_port, SIGNAL(error()), this, SLOT(handleGpsError()));
    connect(&m_pilot_port, SIGNAL(readyRead()), this, SLOT(handlePilotReadyRead()));
    connect(&m_pilot_port, SIGNAL(error()), this, SLOT(handlePilotError()));
    connect(&m_timerPilot, SIGNAL(timeout()), this, SLOT(handleTimer()));
}

MyQTSerialPorts::~MyQTSerialPorts(){
    INFO("destructor");
}

void MyQTSerialPorts::initOrLoad(const Config & config){
    INFO("initOrLoad");
    m_new_gps_serial = config.m_gps_serial;
    m_new_pilot_serial = config.m_pilot_serial;
    
    m_pilot_frequence = config.m_pilot_frequence;
    
    INFO("ici");
    emit initSignal();
    INFO("open!!!!!!");
};

void MyQTSerialPorts::initSlot(){
    if(m_new_gps_serial != "none" && m_new_gps_serial != "file"){
        if(m_gps_serial == m_new_gps_serial && m_gps_port.isOpen()){
            INFO("gps port already open");
        } else {
            if(m_gps_port.isOpen()){
                m_gps_port.close();
            }
            m_gps_serial = m_new_gps_serial;
            m_gps_port.setPortName(QString::fromStdString(m_gps_serial));
            m_gps_port.setBaudRate(115200);
            if (!m_gps_port.open(QIODevice::ReadWrite)) {
                std::ostringstream oss;
                oss << "Failed to open gps port " << m_gps_serial << ", error:" << m_gps_port.errorString().toUtf8().constData();
                //Framework::instance().addError(oss.str());
            }
        }
    }
    
    if(m_new_pilot_serial != "none"){
        if(m_pilot_serial == m_new_pilot_serial && m_pilot_port.isOpen()){
            INFO("pilot port already open");
        } else {
            if(m_pilot_port.isOpen()){
                m_pilot_port.close();
            }
            m_pilot_port.close();
            m_pilot_serial = m_new_pilot_serial;
            m_pilot_port.setPortName(QString::fromStdString(m_pilot_serial));
            m_pilot_port.setBaudRate(115200);
            if (!m_pilot_port.open(QIODevice::ReadWrite)) {
                std::ostringstream oss;
                oss << "Failed to open pilot \nport " << m_pilot_serial << "\nerror:\n" << m_pilot_port.errorString().toUtf8().constData();
                //Framework::instance().m_pilot_last_error = oss.str();
                //Framework::instance().addError(oss.str());
            }
        }
    }
    
    m_timerPilot.stop();
    m_timerPilot.start(1000/m_pilot_frequence);
    DEBUG("end");
};

void MyQTSerialPorts::closeAll(){
    INFO("###close all");
    if(m_gps_port.isOpen()){
        INFO("close gps");
        m_gps_port.close();
    }
    if(m_pilot_port.isOpen()){
        INFO("close pilot");
        m_pilot_port.close();
    }
}

/**
 * PILOT
 */

void MyQTSerialPorts::handlePilotReadyRead(){
    LOG_FUNCTION();
    QByteArray b = m_pilot_port.readAll();
    QString hex(b);
    std::string s = (hex.toUtf8().constData());
    for(auto c : s){
        Framework::instance().m_nmea_parser_pilot.readChar(c);
    }
}


void MyQTSerialPorts::handlePilotError(QSerialPort::SerialPortError error){
    if(error != 0){
        std::ostringstream oss;
        //auto error_s = std::string(QMetaEnum::fromType<QSerialPort::SerialPortError>().valueToKey(error));
        auto error_s = "fix";
        oss << "handleErrorPilot " << error << " " << error_s << ", error:" << m_pilot_port.errorString().toUtf8().constData();
        //GpsFramework::instance().addError(oss.str());
        ERROR("handleErrorPilot " << error << " " << error_s << ", error:" << m_pilot_port.errorString().toUtf8().constData());
    }
    
}

void MyQTSerialPorts::writePilotSerialS(const std::string & l){
    if(m_pilot_port.isOpen()){
        QByteArray b;
        b.append(l.c_str());
        //INFO(l.c_str());
        m_pilot_port.write(b);
    }
}


/**
 * GPS
 */
void MyQTSerialPorts::handleGpsReadyRead(){
    DEBUG("begin");
    /*QByteArray b = m_gps_port.readAll();
    QString hex(b);
    std::string s = (hex.toUtf8().constData());
    for(auto c : s){
        Framework::instance().m_pilot_translator_module.m_arduino_parser.readChar(c);
    }*/
    DEBUG("end");
}
void MyQTSerialPorts::handleGpsError(QSerialPort::SerialPortError error){
    /*DEBUG("begin");
    if(error != 0){
        std::ostringstream oss;
        oss << "handleErrorGps " << error << ", error:" << m_gps_port.errorString().toUtf8().constData();
        Framework::instance().m_pilot_last_error = oss.str();
        //Framework::instance().addError(oss.str());
        WARN(error);
    }
    DEBUG("end");haha*/
}

/**
 * TIMER
 */
void MyQTSerialPorts::handleTimer(){
    //FrameworkVision::instance().m_pilot_module.setLemcaTrame(1024, 0, 100);
    Framework::instance().m_pilot_translator_module.handleArduino();
}

/**
 * LIST PORT
 */

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

std::string execute3(std::string cmd){
    std::string file = DirectoryManager::instance().getDataDirectory() + "/tmp_cmd";
    std::string cmd2 = cmd + " > " + file;
    system(cmd2.c_str());
    std::ifstream infile(file);
    std::stringstream strStream;
    strStream << infile.rdbuf();//read the file
    std::string res = strStream.str();
    return res;
}

void MyQTSerialPorts::addSerialPorts(std::string s){
    std::string res = execute3(s);
    std::vector<std::string> strs;
    boost::split(strs, res, boost::is_any_of("\n"));
    for(auto s : strs){
        if(!s.empty()){
            INFO(s);
            m_serials.push_back(s);
        }
    }
}

std::vector<std::string> & MyQTSerialPorts::getAvailablePorts(){
    m_serials.clear();
    addSerialPorts("ls /dev/cu.*");
    addSerialPorts("ls /dev/ttyACM*");
    addSerialPorts("ls /dev/ttymxc*");
    addSerialPorts("ls /dev/ttyUSB*");
    addSerialPorts("ls /dev/ttyS*");

    return m_serials;
}

