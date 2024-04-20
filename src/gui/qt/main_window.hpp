#ifndef FEN_PRINCIPALE
#define FEN_PRINCIPALE

#include "include_qt.hpp"
#include "../gps_widget.hpp"

class MyWidget : public QWidget
{
  public:
    GpsWidget * m_main_widget = NULL;
    MyWidget( QWidget *parent = 0 ) : QWidget( parent ) {}

    void paintEvent(QPaintEvent* e);
    
    void setupUi();
    void setSize(int width, int height);
    void mousePressEvent ( QMouseEvent * event );
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    QTimer *m_timer;
    QWidget *centralWidget;
    explicit MainWindow(QWidget *parent = 0);
    
public:
    static MainWindow * instancePtr();
    MyWidget * m_my_widget;

    ~MainWindow();
    
    void test();
    void resizeEvent(QResizeEvent* event);

private:
    void setupUi();
    void onNewPoint();
    void creerMenu();

signals:
    void onValueChangeSignal();
private:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void onValueChangeSlot();
    void onTimerSlot();
    
    void openFile();

};

#endif
