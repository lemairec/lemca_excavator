#include "my_qt_timer.hpp"
#include "include_qt.hpp"
#include <QMetaEnum>
#include <QSerialPortInfo>


#include "environnement.hpp"
#include "../../util/directory_manager.hpp"


#include "../../framework.hpp"

MyQTTimer::MyQTTimer(){
    connect(&m_timer_100_ms, SIGNAL(timeout()), this, SLOT(handle100Ms()));
}

MyQTTimer::~MyQTTimer(){
}

void MyQTTimer::handle100ms(){
    Framework & f = Framework::instance();
    //f.m_hexa_parser.handle100ms();
}
