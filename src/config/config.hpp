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
    
    std::string m_port1_gps_serial;
    int m_port1_gps_baudrate = 115200;
    std::string m_gps_file;
    
    std::string m_port2_mcu_serial = "none";
    int m_port2_mcu_baudrate = 115200;
    
    std::string m_port3_soil_serial = "none";
    int m_port3_soil_baudrate = 4800;
    
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
    
    bool m_soil_loop = false;
    
    std::string m_company = "test";

    // Land Manager (envoi des analyses de sol)
    std::string m_landmanager_url = "https://landmanager.be";
    std::string m_landmanager_user;      // login Land Manager
    std::string m_landmanager_password;  // mot de passe Land Manager
    std::string m_landmanager_token;     // alternative au login/mdp
    std::string m_landmanager_client;
    std::string m_landmanager_field;

    
    
    int m_offset_auto_enable = 0;
    int m_offset_auto = 0;
    int m_offset2_mm = 0;
    
    int m_etat = 0; //0 excavator, 1 arpentage, 2 soil
    
    int m_soil_capteur = 0; //0 analogique, 1 numerique
    //mode auto : true = un point tous les m_soil_maille_m metres,
    //false = enchainement immediat des cycles (ancien comportement "temps")
    bool m_soil_auto_dist = true;
    //maille visee : distance entre 2 points ET diametre du cercle dessine
    double m_soil_maille_m = 20;
    double m_soil_tp_start_delay_s = 0;
    double m_soil_tp_down_s = 1;
    double m_soil_tp_down_wait_s = 2;
    double m_soil_tp_up_s = 0.5;
    double m_soil_tp_wait_s = 12;
    
    double m_soil_ph_bas = 4.0;        // consigne tampon standard (independante de la sonde)
    double m_soil_ph_bas_m = 0.0;      // 0.0 = non calibre ; l'operateur calibre sur tampons pour chaque sonde
    double m_soil_ph_haut = 7.0;       // consigne tampon standard (independante de la sonde)
    double m_soil_ph_haut_m = 0.0;     // 0.0 = non calibre ; l'operateur calibre sur tampons pour chaque sonde
    double m_soil_temp_ambiante = 20.0;
    // decalage AFFICHAGE carte uniquement (soil.txt garde le pH brut)
    double m_soil_ph_offset = 0.0;

    // --- stabilisation du signal pH (electrode antimoine, lente/bruitee) ---
    int    m_soil_filter_window  = 24;   // taille fenetre glissante (echantillons) - divise le bruit blanc ~+/-15 counts par ~4.9
    double m_soil_slope_max_mv_s = 2.0;  // seuil de pente pour "stabilise" (mV/s)
    // pente 2 points empirique (mesuree sur tampons) au lieu de la pente constructeur K(T).
    // 1 = DEFAUT : pente empirique SIGNEE (electrode de verre reelle, pente negative).
    // 0 = comportement historique (pente theorique positive) - CASSE pour DFRobot pH Pro V2.
    int    m_soil_pente_empirique = 1;
    // temperature des tampons AU MOMENT de la capture (ref de la pente empirique).
    // Ecrite en meme temps que bas_m/haut_m ; defaut = m_soil_temp_ambiante.
    double m_soil_temp_cal = 20.0;
    // timeout de "staleness" : si le dernier echantillon date de plus de N ms,
    // la lecture n'est plus consideree comme stabilisee (capteur mort/debranche).
    int    m_soil_stale_ms = 2000;

    bool m_map_enable = true;

    bool isDeveloppeur(){
        return true;//m_user_mode > 1;
    }
    
    bool isTechnicien(){
        return true;//m_user_mode > 0;
    }
    
    bool isBaliseEnable(){
        return (m_balise_enable || (m_etat == 1));
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
