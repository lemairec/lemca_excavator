#include "config.hpp"
#include "environnement.hpp"
#include "../util/log.hpp"
#include "../util/directory_manager.hpp"

Config::Config(){
    m_port1_gps_serial = "none";
    
    //m_gps_serial= "file";
    //m_gps_file = DirectoryManager::instance().getSourceDirectory() + "/gps_samples/gps_travail_3m.ubx";
    
    m_resolution_draw = 3;
    m_resolution_calcul = 3;
    m_vitesse_min = 0.5;
}

void Config::work(QSettings & settings, bool save){
    saveLoadString(settings, "licence_panel", m_licence_panel, save);
    saveLoadString(settings, "licence_boitier", m_licence_boitier, save);
    saveLoadString(settings, "licence", m_licence, save);
    
    saveLoadInt(settings, "son", m_son, save);
    saveLoadBool(settings, "gga", m_gga, save);
    
    saveLoadBool(settings, "fullscreen", m_fullscreen, save);
    
    saveLoadString(settings, "port1_gps_serial", m_port1_gps_serial, save);
    //saveLoadInt(settings, "port1_gps_baudrate", m_port1_gps_baudrate, save);
    saveLoadString(settings, "gps_file", m_gps_file, save);
    
    saveLoadString(settings, "port2_mcu_serial", m_port2_mcu_serial, save);
    //saveLoadInt(settings, "port2_mcu_baudrate", m_port2_mcu_baudrate, save);
    
    saveLoadString(settings, "port3_soil_serial", m_port3_soil_serial, save);
    //saveLoadInt(settings, "port3_soil_baudrate", m_port3_soil_baudrate, save);
    
    saveLoadDouble(settings, "tracteur_antenne_pont_arriere", m_tracteur_antenne_pont_arriere, save);
    saveLoadDouble(settings, "tracteur_empatement", m_tracteur_empatement, save);
    saveLoadDouble(settings, "tracteur_hauteur", m_tracteur_hauteur, save);
    saveLoadDouble(settings, "tracteur_antenne_lateral", m_tracteur_antenne_lateral, save);
    

    saveLoadDouble(settings, "outil_largeur", m_outil_largeur, save);
    saveLoadDouble(settings, "outil_distance", m_outil_distance, save);
    saveLoadDouble(settings, "outil_record_h", m_outil_record_h, save);
    saveLoadDouble(settings, "outil_replay_h", m_outil_replay_h, save);
    
    saveLoadDouble(settings, "largeur_AB", m_largeur_AB, save);
    saveLoadBool(settings, "largeur_AB_set", m_largeur_AB_set, save);
    
    saveLoadBool(settings, "3d", m_3d, save);
    
   
    saveLoadDouble(settings, "translateur_p", m_translateur_p, save);
    
    saveLoadDouble(settings, "m_command_capteur_vitesse_max_mm_s", m_command_capteur_vitesse_max_mm_s, save);
    saveLoadDouble(settings, "command_capteur_p", m_command_capteur_p, save);
    saveLoadDouble(settings, "command_bineuse_debatement_mm", m_command_bineuse_debatement_mm, save);
    
    
    saveLoadInt(settings, "command_capteur_left", m_command_capteur_left, save);
    saveLoadInt(settings, "command_capteur_middle", m_command_capteur_middle, save);
    saveLoadInt(settings, "command_capteur_right", m_command_capteur_right, save);
    
    saveLoadString(settings, "company", m_company, save);

    saveLoadString(settings, "landmanager_url", m_landmanager_url, save);
    saveLoadString(settings, "landmanager_user", m_landmanager_user, save);
    saveLoadString(settings, "landmanager_password", m_landmanager_password, save);
    saveLoadString(settings, "landmanager_token", m_landmanager_token, save);
    saveLoadString(settings, "landmanager_client", m_landmanager_client, save);
    saveLoadString(settings, "landmanager_field", m_landmanager_field, save);

    saveLoadBool(settings, "proportionnal", m_proportionnal, save);
    saveLoadInt(settings, "offset_mm", m_offset_mm, save);
    saveLoadBool(settings, "inverse_relai", m_inverse_relai, save);
    
    saveLoadBool(settings, "balise_enable", m_balise_enable, save);
     
    saveLoadInt(settings, "etat", m_etat, save);
    
    saveLoadInt(settings, "soil_capteur", m_soil_capteur, save);
    saveLoadBool(settings, "soil_auto_dist", m_soil_auto_dist, save);
    saveLoadDouble(settings, "soil_maille_m", m_soil_maille_m, save);
    saveLoadDouble(settings, "soil_tp_start_delay_s", m_soil_tp_start_delay_s, save);
    saveLoadDouble(settings, "soil_tp_down_s", m_soil_tp_down_s, save);
    saveLoadDouble(settings, "soil_tp_down_wait_s", m_soil_tp_down_wait_s, save);
    saveLoadDouble(settings, "soil_tp_up_s", m_soil_tp_up_s, save);
    saveLoadDouble(settings, "soil_tp_wait_s", m_soil_tp_wait_s, save);
    
    saveLoadDouble(settings, "m_soil_ph_bas", m_soil_ph_bas, save);
    saveLoadDouble(settings, "m_soil_ph_bas_m", m_soil_ph_bas_m, save);
    saveLoadDouble(settings, "m_soil_ph_haut", m_soil_ph_haut, save);
    saveLoadDouble(settings, "m_soil_ph_haut_m", m_soil_ph_haut_m, save);
    saveLoadDouble(settings, "m_soil_temp_ambiante", m_soil_temp_ambiante, save);
    saveLoadDouble(settings, "m_soil_ph_offset", m_soil_ph_offset, save);
    saveLoadInt(settings, "m_soil_filter_window", m_soil_filter_window, save);
    saveLoadDouble(settings, "m_soil_slope_max_mv_s", m_soil_slope_max_mv_s, save);
    saveLoadInt(settings, "m_soil_pente_empirique", m_soil_pente_empirique, save);
    saveLoadDouble(settings, "m_soil_temp_cal", m_soil_temp_cal, save);
    saveLoadInt(settings, "m_soil_stale_ms", m_soil_stale_ms, save);
    saveLoadBool(settings, "m_map_enable", m_map_enable, save);

}

void Config::validate(){
}

/**
 COMMON
 */

void Config::save(){
    std::string s = DirectoryManager::instance().m_config_file;
    QString path = QString::fromStdString(s);
    QSettings settings(path, QSettings::IniFormat);

    work(settings, true);
}

void Config::load(){
    std::string s = DirectoryManager::instance().m_config_file;
    QString path = QString::fromStdString(s);
    QSettings settings(path, QSettings::IniFormat);
    
    work(settings, false);
}


void Config::saveLoadDouble(QSettings & settings, const QString & name, double & ref, bool save){
    if(save){
        settings.setValue(name, ref);
    } else {
        if(settings.contains(name)){
            ref = settings.value(name).toDouble();
        }
    }
}

void Config::saveLoadInt(QSettings & settings, const QString & name, int & ref, bool save){
    if(save){
        settings.setValue(name, ref);
    } else {
        if(settings.contains(name)){
            ref = settings.value(name).toInt();
        }
    }
}

void Config::saveLoadBool(QSettings & settings, const QString & name, bool & ref, bool save){
    if(save){
        settings.setValue(name, ref);
    } else {
        if(settings.contains(name)){
            ref = settings.value(name).toBool();
        }
    }
}

void Config::saveLoadString(QSettings &settings, const QString &name, std::string &ref, bool save){
    if(save){
        QString ref2 = QString::fromStdString(ref);
        settings.setValue(name, ref2);
    } else {
        if(settings.contains(name)){
            ref = settings.value(name).toString().toUtf8().constData();
        }
    }
}

template<typename T> void Config::saveLoadEnum(QSettings &settings, const QString &name, T &ref, bool save){
    if(save){
        settings.setValue(name, ref);
    } else {
        if(settings.contains(name)){
            ref = (T)settings.value(name).toInt();
        }
    }
}
