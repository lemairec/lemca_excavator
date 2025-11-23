#include "my_qt_timer.hpp"
#include "include_qt.hpp"
#include <QMetaEnum>
#include <QSerialPortInfo>


#include "environnement.hpp"
#include "../../util/directory_manager.hpp"


#include "../../framework.hpp"

MyQTTimer::MyQTTimer(){
    connect(&m_timer_100_ms, SIGNAL(timeout()), this, SLOT(handle100ms()));
    connect(&m_timer_25_ms, SIGNAL(timeout()), this, SLOT(handle25ms()));
}

MyQTTimer::~MyQTTimer(){
}

void MyQTTimer::init(){
    m_timer_100_ms.stop();
    m_timer_100_ms.start(1000/10);
    m_timer_25_ms.stop();
    m_timer_25_ms.start(1000/40);
}

void MyQTTimer::handle100ms(){
    Framework & f = Framework::instance();
    f.m_hexa_parser.handle100ms();
}

void MyQTTimer::handle25ms(){
    Framework & f = Framework::instance();
    f.m_pilot_translator_module.handleArduino();
}
