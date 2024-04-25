#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#include <QString>
#include <QSettings>

#include "enum.hpp"

class Config  {
public:
    std::string m_licence_panel = "XXXX";
    std::string m_licence_boitier = "XXXX";
    std::string m_licence = "XXXX";
    
    int m_son = 2;
    int m_luminosite = 3;
    int m_user_mode = 0;
    
    int m_profondeur_mm = 0;
    
    std::string m_gps_serial;
    int m_gps_baudrate = 115200;
    std::string m_gps_file;
    
    std::string m_pilot_serial = "none";
    int m_pilot_baudrate = 115200;
    
    bool m_gga = false;
    
    bool m_correction_devers = true;
   
    bool m_3d = false;
    
    bool m_fullscreen = true;
    
    int m_pilot_frequence = 40;
    
    bool m_logNMEA = true;
    
    double m_tracteur_antenne_pont_arriere = 1;
    double m_tracteur_empatement = 2.75;
    double m_tracteur_hauteur = 2.75;
    double m_tracteur_antenne_lateral = 0;
    
    double m_outil_distance = 1.5;
    double m_outil_largeur = 3.0;
    double m_outil_record_h = 0;
    double m_outil_replay_h = 0;
    
    bool m_largeur_AB_set = false;
    double m_largeur_AB = 3.0;
    double m_offset_AB = 0;
    
    double m_resolution_calcul = 0.5;
    double m_resolution_draw = 1.0;
    
    double m_vitesse_min = 0.4;
    
    double m_translateur_p = 1;
    double m_command_translateur_debatement_mm = 400;
    
    double m_command_capteur_vitesse_max_mm_s = 20.0;
    double m_command_capteur_p = 1.0;
    double m_command_bineuse_debatement_mm = 450;
    int m_command_capteur_left = 0;
    int m_command_capteur_middle = 50;
    int m_command_capteur_right = 100;
    double m_temp_deplacement_ms = 400;
    bool m_proportionnal = false;
    bool m_balise_enable = false;
    
    int m_offset_mm = 0;
    bool m_inverse_relai = false;
    
    std::string m_company = "test";
    
    
    
    int m_offset_auto_enable = 0;
    int m_offset_auto = 0;
    int m_offset2_mm = 0;
    
    bool isDeveloppeur(){
        return m_user_mode > 1;
    }
    
    bool isTechnicien(){
        return m_user_mode > 0;
    }
    
    Config();
    
    void work(QSettings & settings, bool save);
    void validate();
    
    void save();
    void load();
    
    void saveLoadBool(QSettings & settings, const QString & name, bool & ref, bool save);
    void saveLoadDouble(QSettings & settings, const QString & name, double & ref, bool save);
    void saveLoadInt(QSettings & settings, const QString & name, int & ref, bool save);
    void saveLoadString(QSettings & settings, const QString & name, std::string & ref, bool save);
    template<typename T> void saveLoadEnum(QSettings & settings, const QString & name, T & ref, bool save);
};

//-500 =>

#endif // CONFIG_H
