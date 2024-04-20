#include "gui/qt/main_window.hpp"
#include <QApplication>
#include <iostream>
#include "framework.hpp"
#include "util/directory_manager.hpp"

#include <inttypes.h>

int make_stats(){
    //Framework & f = Framework::instance();
    std::string path = DirectoryManager::instance().getDataDirectory()+"/stat.txt";
    INFO(path);
    
    std::ostringstream oss;
    oss << "{\n";
    //oss << "\"nbr_h\":" << f.m_qt_compteur_heure.m_heure_total << ",\n";
    oss << "\"version\":\"" << ProjectVersion << "\",\n";
    
    /*QSettings settings;
    f.m_config.work(settings, true);
    
    
    for(auto s : settings.allKeys()){
        std::string value = settings.value(s).toString().toUtf8().constData();
        oss << "\"" << s.toUtf8().constData() << "\":\"" << value << "\",\n";
    }*/
    oss << "\"fin\":\"fin\"}\n";
    
    QString s3 = QString::fromStdString(path);
    QFile file(s3);
    file.open(QIODevice::WriteOnly);
    QTextStream s(&file);
    s << QString::fromStdString(oss.str());
    
    std::cout << oss.str();
    return 0;
}

int main(int argc, char *argv[])
{
    std::string gui = "complete";
    for(int i = 1; i < argc; ++i){
        std::string s(argv[i]);
        if(s == "--version"){
            std::cout << ProjectVersionWeb << std::endl;
            return 0;
        } else if(s == "--stats"){
            return make_stats();
        }

    }
    
    QApplication app(argc, argv);
    Framework & framework = Framework::instance();
    //framework.test();
    MainWindow * m = MainWindow::instancePtr();
    DEBUG("init");
    framework.initOrLoadConfig();
    DEBUG("fin init");
    m->show();
    DEBUG("exec");
    //INFO(QThread::currentThreadId());
    //GpsThread * gpsThread = new GpsThread(framework);
    //gpsThread->start();
    return app.exec();
}
