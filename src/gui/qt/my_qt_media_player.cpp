#include "my_qt_media_player.hpp"
#include "include_qt.hpp"
#include "environnement.hpp"
#include "../../framework.hpp"
#include "../../util/directory_manager.hpp"

MyQtMediaPlayer::MyQtMediaPlayer(){
    //connect(m_media_player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    
    {
        QString s = QString::fromStdString(DirectoryManager::instance().getSourceDirectory());
        QString s2 = s+"/sound/beep-01a.wav";
        m_media_player_beep.setMedia(QUrl::fromLocalFile(s2));
    }
    {
        QString s = QString::fromStdString(DirectoryManager::instance().getSourceDirectory());
        QString s2 = s+"/sound/autoOn.wav";
        m_media_player_on.setMedia(QUrl::fromLocalFile(s2));
    }
    {
        QString s = QString::fromStdString(DirectoryManager::instance().getSourceDirectory());
        QString s2 = s+"/sound/autoOff.wav";
        m_media_player_off.setMedia(QUrl::fromLocalFile(s2));
    }
    
}

MyQtMediaPlayer::~MyQtMediaPlayer(){
}

void MyQtMediaPlayer::setVolume(int i){
    m_media_player_beep.setVolume(i);
    m_media_player_on.setVolume(i);
    m_media_player_off.setVolume(i);
}

void MyQtMediaPlayer::play(int i){
    if(i == 0){
        if(m_media_player_beep.state() != QMediaPlayer::PlayingState && m_media_player_on.state() != QMediaPlayer::PlayingState && m_media_player_off.state() != QMediaPlayer::PlayingState){
            m_media_player_beep.play();
        }
    } else if(i == 1){
        m_media_player_beep.stop();
        m_media_player_on.play();
    } else if(i == 2){
        m_media_player_beep.stop();
        m_media_player_off.play();
    }
}

void MyQtMediaPlayer::initOrLoad(const Config &config){
    m_son = config.m_son;
    //m_son_image = config.m_son_image;
    //m_son_3eme_point = config.m_son_3point;
    if(m_son == 0){
        setVolume(0);
    }else if(m_son == 1){
        setVolume(30);
    }else if(m_son == 2){
        setVolume(60);
    }else if(m_son == 3){
        setVolume(90);
    }
}

bool is_engaged = false;
void MyQtMediaPlayer::work(){
    if(m_son == 0){
        return;
    }
    /*FrameworkVision & f = FrameworkVision::instance();
    
    bool point_3em = f.m_pilot_module.go3emPoint();
    if(f.m_playRobot){
        if(!point_3em){
            if(m_old_3eme_point){
                m_old_3eme_point = point_3em;
                play(2);
            }
        } else {
            if(!m_old_3eme_point){
                m_old_3eme_point = point_3em;
                play(1);
            }
            return;
        }
        
        for(auto e : f.getEtats()){
            if(e == Etat_score_low){
                if(m_son_image){
                    play(0);
                }
                return;
            }
            if(e == Etat_Butee_Droite){
                if(m_son_image){
                    play(0);
                }
                return;
            }
            if(e == Etat_Butee_Gauche){
                if(m_son_image){
                    play(0);
                }
                return;
            }
        }
    }*/
}
