#include "my_qt_serial_port.hpp"
#include "include_qt.hpp"
#include <QMetaEnum>
#include <QSerialPortInfo>


#include "environnement.hpp"
#include "../../util/directory_manager.hpp"


#include "../../framework.hpp"

MyQTSerialPorts::MyQTSerialPorts(){
    connect(&m_port1_gps, SIGNAL(readyRead()), this, SLOT(handlePort1GpsReadyRead()));
    connect(&m_port1_gps, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MyQTSerialPorts::handlePort1GpsError);
    
    connect(&m_port2_mcu, SIGNAL(readyRead()), this, SLOT(handlePort2McuReadyRead()));
    connect(&m_port2_mcu, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MyQTSerialPorts::handlePort2McuError);
    
    connect(&m_port3_soil, SIGNAL(readyRead()), this, SLOT(handlePort3SoilReadyRead()));
    connect(&m_port3_soil, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MyQTSerialPorts::handlePort3SoilError);
    
}

MyQTSerialPorts::~MyQTSerialPorts(){
}

void MyQTSerialPorts::addMessage(const std::string & s){
    INFO(s);
    //Framework & f = Framework::Instance();
    //f.addSerialString(s);
}

void MyQTSerialPorts::startConnect(int i, QSerialPort & port, std::string & old_serial, const std::string & serial, int baudrate){
    INFO("start " << serial << " " << baudrate);
    
    if(serial != "none" && serial != "file"){
        if(old_serial == serial && port.isOpen()){
            INFO("- port already open");
            return;
        }
        if(port.isOpen()){
            port.close();
        }
        old_serial = serial;
        port.setPortName(QString::fromStdString(serial));
        port.setBaudRate(baudrate);
        std::ostringstream oss;
        oss << "- " << i << " open  : "  << serial << " " << baudrate;
        addMessage(oss.str());
        
        if (!port.open(QIODevice::ReadWrite)) {
            std::ostringstream oss;
            oss << "- " << i << " failed error  : " << port.errorString().toUtf8().constData();
            addMessage(oss.str());
        } else {
            std::ostringstream oss;
            oss << "- " << i << " opened";
            addMessage(oss.str());
        }
    }
}


void MyQTSerialPorts::initOrLoad(const Config & config){
    startConnect(1, m_port1_gps, m_port1_gps_serial, config.m_port1_gps_serial, config.m_port1_gps_baudrate);
    startConnect(2, m_port2_mcu, m_port2_mcu_serial, config.m_port2_mcu_serial, config.m_port2_mcu_baudrate);
    startConnect(3, m_port3_soil, m_port3_soil_serial, config.m_port3_soil_serial, config.m_port3_soil_baudrate);
    //startConnect(2, m_port2_mcu, m_port1_gps_serial, config.m_gps_serial, config.m_gps_baudrate);
    //startConnect(3, m_port1_gps, m_port1_gps_serial, config.m_gps_serial, config.m_gps_baudrate);
};

void MyQTSerialPorts::closeAll(){
    INFO("###close all");
    if(m_port1_gps.isOpen()){
        INFO("m_port1_gps close");
        m_port1_gps.close();
    }
    if(m_port2_mcu.isOpen()){
        INFO("m_port2_mcu close");
        m_port2_mcu.close();
    }
    if(m_port3_soil.isOpen()){
        INFO("m_port3_soil close");
        m_port3_soil.close();
    }
}


//port1 Gps


void MyQTSerialPorts::handlePort1GpsReadyRead(){
    DEBUG("begin");
    QByteArray a = m_port1_gps.readAll();
    /*Framework & f = Framework::Instance();
    for(int i = 0; i < (int)a.size(); ++i){
        f.addSerialChar((char)(a.data()[i]));
    }*/
    
    DEBUG("end");
}
void MyQTSerialPorts::handlePort1GpsError(QSerialPort::SerialPortError error){
    /*DEBUG("begin");
    if(error != 0){
        std::ostringstream oss;
        oss << "handleErrorGps " << error << ", error:" << m_serialPortGps.errorString().toUtf8().constData();
        //TODOGpsFramework::Instance().m_pilot_last_error = oss.str();
        //GpsFramework::Instance().addError(oss.str());
        WARN(error);
    }
    DEBUG("end");*/
}

void MyQTSerialPorts::writePort1GpsStr(const std::string & l){
    if(m_port2_mcu.isOpen()){
        QByteArray b;
        for(long unsigned int i = 0; i < l.size(); ++i){
            b.append(l[i]);
        }
        m_port1_gps.write(b);
        //QString hex(b.toHex());
    }
}

//port2 Mcu


void MyQTSerialPorts::handlePort2McuReadyRead(){
    DEBUG("begin");
    QByteArray a = m_port2_mcu.readAll();
    /*Framework & f = Framework::Instance();
    for(int i = 0; i < (int)a.size(); ++i){
        f.addSerialChar((char)(a.data()[i]));
    }*/
    
    DEBUG("end");
}
void MyQTSerialPorts::handlePort2McuError(QSerialPort::SerialPortError error){
    /*DEBUG("begin");
    if(error != 0){
        std::ostringstream oss;
        oss << "handleErrorGps " << error << ", error:" << m_serialPortGps.errorString().toUtf8().constData();
        //TODOGpsFramework::Instance().m_pilot_last_error = oss.str();
        //GpsFramework::Instance().addError(oss.str());
        WARN(error);
    }
    DEBUG("end");*/
}

void MyQTSerialPorts::writePort2McuStr(const std::string & l){
    if(m_port2_mcu.isOpen()){
        QByteArray b;
        for(long unsigned int i = 0; i < l.size(); ++i){
            b.append(l[i]);
        }
        m_port2_mcu.write(b);
        //QString hex(b.toHex());
    }
}

//port3 Mcu


void MyQTSerialPorts::handlePort3SoilReadyRead(){
    DEBUG("begin");
    QByteArray a = m_port3_soil.readAll();
    /*Framework & f = Framework::Instance();
    for(int i = 0; i < (int)a.size(); ++i){
        f.addSerialChar((char)(a.data()[i]));
    }*/
    
    DEBUG("end");
}
void MyQTSerialPorts::handlePort3SoilError(QSerialPort::SerialPortError error){
    /*DEBUG("begin");
    if(error != 0){
        std::ostringstream oss;
        oss << "handleErrorGps " << error << ", error:" << m_serialPortGps.errorString().toUtf8().constData();
        //TODOGpsFramework::Instance().m_pilot_last_error = oss.str();
        //GpsFramework::Instance().addError(oss.str());
        WARN(error);
    }
    DEBUG("end");*/
}

/**
 * LIST PORT
 */


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

std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}


void MyQTSerialPorts::addSerialPorts(std::string s2){
    std::string res = execute3(s2);
    std::vector<std::string> res2 = split(res, "\n");
    for(auto s : res2){
        if(!s.empty()){
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
