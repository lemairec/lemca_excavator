#ifndef MY_QT_MEDIA_PLAYER_H
#define MY_QT_MEDIA_PLAYER_H

#include "../../config/config.hpp"
#include <QMediaPlayer>

QT_USE_NAMESPACE

class MyQtMediaPlayer : public QObject{
    Q_OBJECT
    
    QMediaPlayer m_media_player_beep;
    QMediaPlayer m_media_player_on;
    QMediaPlayer m_media_player_off;
    int m_son = 0;
    bool m_son_image = false;
    bool m_son_3eme_point = false;
    bool m_old_3eme_point = false;
public:
    MyQtMediaPlayer();
    ~MyQtMediaPlayer();
    
    void initOrLoad(const Config & config);
    void setVolume(int i);
    void play(int i);
    void work();
};



#endif
