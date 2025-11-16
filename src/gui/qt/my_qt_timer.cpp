#include "my_qt_serial_port.hpp"
#include "include_qt.hpp"
#include <QMetaEnum>
#include <QSerialPortInfo>


#include "environnement.hpp"
#include "../../util/directory_manager.hpp"


#include "../../framework.hpp"

MyQTSerialPorts::MyQTSerialPorts(){
    connect(&m_serialPort, SIGNAL(readyRead()), this, SLOT(handleReadyReadGps()));
    connect(&m_serialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MyQTSerialPorts::handleErrorGps);
    
    connect(&m_timerPilot, SIGNAL(timeout()), this, SLOT(handlePilot()));
    connect(&m_timer100ms, SIGNAL(timeout()), this, SLOT(handle100Ms()));
    
}

double convertToDeg(double d){
    int h = d/100.0;
    double minu = (d-(h*100.0));
    return h + minu/60.0;
}

double convertToDec(double d){
    int h = d;
    double minu = (d-(h));
    return h*100 + minu*60.0/100*100;
}


void MyQTSerialPorts::initOrLoad(Config & config){
    DEBUG("begin");

    INFO(m_serial << ", " << config.m_serial);
    //m_pilot_langage = config.m_pilot_langage;

    if(config.m_serial != "none" && config.m_serial != "file"){
        if(m_serial == config.m_serial && m_serialPort.isOpen()){
            INFO("gps port already open");
        } else {
            m_last_error = "";
            if(m_serialPort.isOpen()){
                m_serialPort.close();
            }
            m_serial = config.m_serial;
            m_serialPort.setPortName(QString::fromStdString(m_serial));
            m_serialPort.setBaudRate(config.m_baudrate);
            
            std::ostringstream oss;
            oss << "open : " << m_serial << " " << config.m_baudrate;
            std::string s = oss.str();
            INFO(s);
            Framework & f = Framework::Instance();
            f.addSerialString(s);
            
            if (!m_serialPort.open(QIODevice::ReadWrite)) {
                std::ostringstream oss;
                oss << "Failed error:" << m_serialPort.errorString().toUtf8().constData();
                std::string s = oss.str();
                m_last_error = s;
                f.addSerialString(s);
            } else {
                m_last_error = "opened";
                f.addSerialString("opened");
            }
        }
    }
    m_timerPilot.stop();
    m_timerPilot.start(1000/40);
    m_timer100ms.stop();
    m_timer100ms.start(1000/10);
    DEBUG("end");
    
    double lat = 4937.6041777;
    double lon = 401.3406431;
    double gga_latitude = convertToDeg(lat);
    double gga_longitude = convertToDeg(lon);
    double res_la = convertToDec(gga_latitude);
    double res_lo = convertToDec(gga_longitude);
    
    INFO((int64_t)(lat*10000000) << " " << (int64_t)(lon*10000000))
    INFO((int64_t)(res_la*10000000) << " " << (int64_t)(res_lo*10000000))
    
    uint32_t u0 = round((gga_latitude + 210.0) * 10000000);
    uint32_t u1 = round((gga_longitude + 210.0) * 10000000);
    double res_la1 = u0;
    double res_lo1 = u1;
    res_la1 = res_la1/10000000.0-210.0;
    res_lo1 = res_lo1/10000000.0-210.0;
    double res_la2 = convertToDec(res_la1);
    double res_lo2 = convertToDec(res_lo1);
    INFO((int64_t)(res_la2*10000000) << " " << (int64_t)(res_lo2*10000000))
    
    //49376041777 4013406431
    //49376041719 4013406400
    //49376041777 4013406431
    //49376041780 4013406459
};

void MyQTSerialPorts::closeAll(){
    INFO("###close all");
    if(m_serialPort.isOpen()){
        INFO("close gps");
        m_serialPort.close();
    }
}

void MyQTSerialPorts::handleReadyReadGps(){
    DEBUG("begin");
    QByteArray a = m_serialPort.readAll();
    Framework & f = Framework::Instance();
    for(int i = 0; i < (int)a.size(); ++i){
        f.addSerialChar((char)(a.data()[i]));
    }
    
    DEBUG("end");
}
void MyQTSerialPorts::handleErrorGps(QSerialPort::SerialPortError error){
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

void MyQTSerialPorts::handleReadyReadPilot(){
}
void MyQTSerialPorts::handleErrorPilot(QSerialPort::SerialPortError error){
}

/**
 IMU
 */

void MyQTSerialPorts::handleReadyReadImu(){
}
void MyQTSerialPorts::handleErrorImu(QSerialPort::SerialPortError error){
}



void MyQTSerialPorts::writeGpsSerialS(const std::string & l){
    if(m_serialPort.isOpen()){
        INFO("open");
        QByteArray b;
        b.append(l.c_str());
        m_serialPort.write(b);
    }
}

void MyQTSerialPorts::writeGpsSerialD(std::vector<unsigned char> & l){
    if(m_serialPort.isOpen()){
        QByteArray b;
        for(long unsigned int i = 0; i < l.size(); ++i){
            b.append(l[i]);
        }
        m_serialPort.write(b);
    }
}



void MyQTSerialPorts::writePilotSerialD(std::vector<unsigned char> & l){
    if(m_serialPort.isOpen()){
        QByteArray b;
        for(long unsigned int i = 0; i < l.size(); ++i){
            b.append(l[i]);
        }
        m_serialPort.write(b);
        //QString hex(b.toHex());
    }
}

void MyQTSerialPorts::writePilotSerialDAndWait(std::vector<unsigned char> & l){
    DEBUG("begin");
    writePilotSerialD(l);
    
    DEBUG("end");
}

#include <QThread>
int i = 0;
void MyQTSerialPorts::searchBegin(){
    INFO("##0 open");
    closeAll();
    //TODOGpsFramework & f = GpsFramework::Instance();
    //TODOf.m_config.m_input_gps = "none";
    //TODOf.m_config.m_inputPilot = "none";
    //TODOf.initOrLoadConfig();
    auto serials = getAvailablePorts();
    m_serial_searchs.clear();
    for(auto s : serials){
        INFO("##0 " << s);
        auto serial_search = new SerialSearch();
        serial_search->m_serial_s = s;
        serial_search->m_serial_port.setPortName(QString::fromStdString(s));
        serial_search->m_serial_port.setBaudRate(115200);
        if (!serial_search->m_serial_port.open(QIODevice::ReadWrite)) {
            INFO("##0 " << "failed");
             std::ostringstream oss;
            oss << "Failed to open imu port " << s << ", error:" << serial_search->m_serial_port.errorString().toUtf8().constData();
            //TODOGpsFramework::Instance().addError(oss.str());
            //standardOutput << QObject::tr("Failed to open port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;//
        }
        m_serial_searchs[s] = serial_search;
        
    }
    m_serial_searchs_i = 1;
}

void MyQTSerialPorts::ecrireRecherche(){
    INFO("##1 ecrire");
    for(auto s : m_serial_searchs){
        auto serial_search = s.second;
        
        std::string s2 = "$P;\n$P;\n";
        QByteArray b;
        b.append(s2.c_str());
        serial_search->m_serial_port.write(b);
    }
    
}
void MyQTSerialPorts::analyseRecherche(){
    INFO("##2 analyse");
    std::string gps_port = "none";
    std::string pilot_port = "none";
    for(auto s : m_serial_searchs){
        auto serial_search = s.second;
        INFO(s.first << " " << serial_search->m_serial_port.isOpen());
        QByteArray b = serial_search->m_serial_port.readAll();
        QString hex(b);
        std::string data_s = hex.toUtf8().constData();
        serial_search->m_data_s = data_s;
        if(data_s.size()>2){
            if(data_s.find("$GNGGA") !=std::string::npos){
                gps_port = serial_search->m_serial_s;
            } else if(data_s.find("$GPGGA") !=std::string::npos){
                gps_port = serial_search->m_serial_s;
            } else if(data_s.find("$P,") !=std::string::npos){
                pilot_port = serial_search->m_serial_s;
            } else {
                INFO(data_s);
            }
        }
        QByteArray b2 = serial_search->m_serial_port.readAll();
        
        INFO(data_s);
        INFO("toto " << b2.constData());
        s.second->m_serial_port.close();
    }
    
    
    INFO("##2 fin recherche");
    INFO("## gps_port " << gps_port);
    INFO("## pilot_port " << pilot_port);
    //TODOGpsFramework & f = GpsFramework::Instance();
    //TODOf.m_config.m_input_gps = gps_port;
    //TODOf.m_config.m_inputPilot = pilot_port;
    //TODOf.initOrLoadConfig();
    
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

void MyQTSerialPorts::handle100Ms(){
    //INFO("la " << m_serial_searchs_i);
    if(m_serial_searchs_i > 0){
        m_serial_searchs_i++;
        INFO(m_serial_searchs_i);
        if(m_serial_searchs_i == 10){
            ecrireRecherche();
        }
        if(m_serial_searchs_i > 20){
            analyseRecherche();
            //MainWidget::instance()->m_menuWidget.m_close = true;
            m_serial_searchs_i = 0;
            
        }
    }
    Framework & f = Framework::Instance();
    f.m_hexa_parser.handle100ms();
}

void MyQTSerialPorts::handlePilot(){
    DEBUG("begin");
    
    if(Framework::Instance().m_position){
        writeGpsSerialS("$P,*\n");
    }
    
    //INFO("coucou je suis ici");
    
    if(m_pilot_langage == 0){
        //TODOGpsFramework::Instance().m_pilotModule.handleHadrien();
        //GpsFramework::Instance().m_pilotModule.update();
    } else {
        //TODOGpsFramework::Instance().m_pilotModule.handleArduino();
    }
    DEBUG("end");
}
