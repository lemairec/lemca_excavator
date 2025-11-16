#include "main_window.hpp"
#include "../../util/directory_manager.hpp"

#include <QGridLayout>
#include <QTabWidget>
#include <QStatusBar>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QResizeEvent>

#include <QTimer>
#include <QFileDialog>
#include "environnement.hpp"
void MyWidget::setupUi(){
    m_main_widget = GpsWidget::instance();
};

void MyWidget::mousePressEvent ( QMouseEvent * event ){
    if(m_main_widget){
        int x = event->x();
        int y = event->y();
        
        m_main_widget->onMouse(x, y);
    }
}

void MyWidget::paintEvent(QPaintEvent* e)
{
    QWidget::paintEvent(e); // effectue le comportement standard

    QPainter painter(this); // construire
    //painter.setRenderHint(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    if(m_main_widget){
        m_main_widget->setPainter(&painter);
        m_main_widget->draw();
    }
}

void MyWidget::setSize(int width, int height){
    if(m_main_widget){
        m_main_widget->setSize(width, height);
    }
}



MainWindow * MainWindow::instancePtr(){
    static MainWindow gf;
    return &gf;
}

MainWindow::MainWindow(QWidget *parent)
:QMainWindow(parent)
{
    DEBUG("begin");
    m_timer = new QTimer(this);
    m_timer->start(200);
    
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimerSlot()));
    
    connect(this, SIGNAL(onValueChangeSignal()), this, SLOT(onValueChangeSlot()));
    
    setupUi();
    DEBUG("end");
    
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi(){
    if (this->objectName().isEmpty()){
        this->setObjectName(QString::fromUtf8("MainWindow"));
    }
    //this->resize(800, 480);
    this->resize(1280, 800);
    
    
    m_my_widget = new MyWidget();
    m_my_widget->setupUi();
    this->setCentralWidget(m_my_widget);
    
    
    Framework & f = Framework::instance();
    if(f.m_config.m_fullscreen){
        showFullScreen();
    } else {
        creerMenu();
    }
}


void MainWindow::resizeEvent(QResizeEvent *event){
    DEBUG("begin");
    int width = event->size().width();
    int height = event->size().height();
    m_my_widget->setSize(width, height);
    DEBUG("end");
}

void MainWindow::onNewPoint(){
    DEBUG("begin");
    onValueChangeSlot();
    DEBUG("end");
}

void MainWindow::onValueChangeSlot(){
    DEBUG("begin");
    //m_view->m_gpsWidget->draw();
    DEBUG("end");
}

void MainWindow::onTimerSlot(){
    DEBUG("begin");
    m_my_widget->update();
    DEBUG("end");
}

void MainWindow::creerMenu()
{
    QMenu *menuFichier = menuBar()->addMenu(tr("&Fichier"));
      
    menuFichier->addSeparator();
    menuFichier->addAction("open File", this, SLOT(openFile()));
    menuFichier->addAction("open Camera", this, SLOT(openCamera()));

}


void MainWindow::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_F1){
    }
}

void MainWindow::openFile(){
    DEBUG("begin");
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Address Book"), QString::fromStdString(DirectoryManager::instance().getSourceDirectory())+QString("/gps_samples"),
                                                    tr("Gps files (*.ubx)"));
    
    Framework & f = Framework::instance();
    f.m_config.m_gps_file = fileName.toUtf8().constData();
    f.m_config.m_port1_gps_serial = "file";
    f.initOrLoadConfig();
    DEBUG("end");
}

