#include "pilot_translator_module.hpp"
#include "../util/util.hpp"
#include "../framework.hpp"

PilotTranslatorModule::PilotTranslatorModule()
{
};


PilotTranslatorModule::~PilotTranslatorModule()
{
    closeRelayRight();
    closeRelayLeft();
    
    stopAll();
}

void PilotTranslatorModule::initOrLoad(const Config& config)
{
    m_offset_mm = config.m_offset_mm;
    
    m_frequence = config.m_pilot_frequence;
    
    m_text_lamp_on = "$ON;5\n$ON;2\n";
    m_text_lamp_off = "$OFF;5\n$OFF;2\n";
    
    if(config.m_inverse_relai == 0){
        m_textLeftOn = "$ON;R\n";
        m_textLeftOff = "$OFF;R\n";
        m_textRightOn = "$ON;L\n";
        m_textRightOff = "$OFF;L\n";
    } else {
        m_textLeftOn = "$ON;L\n";
        m_textLeftOff = "$OFF;L\n";
        m_textRightOn = "$ON;R\n";
        m_textRightOff = "$OFF;R\n";
    }
    
    m_proportionnal = config.m_proportionnal;
    m_command_capteur_left = config.m_command_capteur_left;
    m_command_capteur_middle = config.m_command_capteur_middle;
    m_command_capteur_right = config.m_command_capteur_right;
    m_pas_desired_mm = 0;
    m_pas_mesured_2 = config.m_command_capteur_middle;
    m_pas_mesured_mm = 0;
    m_inverse_relai = config.m_inverse_relai;
    m_pid_p = config.m_command_capteur_p/3.0;
    m_command_capteur_vitesse_max_mm_s = config.m_command_capteur_vitesse_max_mm_s;
    m_command_bineuse_debatement_mm = config.m_command_bineuse_debatement_mm;

}


bool m_old_3eme_pt = true;
bool PilotTranslatorModule::go3emPoint(){
    bool res = false;
    if(true){
        res = m_point_3;
    }
    if(m_old_3eme_pt != res){
        setAlive();
        m_old_3eme_pt = res;
    }
    return res;
}


void PilotTranslatorModule::reset()
{
    stopAll();
}


void PilotTranslatorModule::changeOrder(double mm){
    if(true){
        if(m_point_3){
            Framework::instance().m_debug_str_1 = "3p";
            setPasDesiredMM(0);
        } else {
            double offset_mm = Framework::instance().m_config.m_offset_AB*1000.0;
            Framework::instance().m_debug_str_1 = strprintf("%.1f changeOrder (offset %.1f)", mm, offset_mm);
            setPasDesiredMM(m_pas_mesured_mm+mm+offset_mm);
        }
    }
}

void PilotTranslatorModule::openRelayRight(int durationMs, int prop){
    m_etat = SerialEtat_Temp;
    m_direction = 1;
    m_temp_prop = prop;
    m_time_end = std::chrono::system_clock::now();
    m_time_end += std::chrono::milliseconds(durationMs);
    m_time_end_stop_all = m_time_end;
}
void PilotTranslatorModule::openRelayLeft(int durationMs, int prop){
    m_etat = SerialEtat_Temp;
    m_direction = -1;
    m_temp_prop = prop;
    m_time_end = std::chrono::system_clock::now();
    m_time_end += std::chrono::milliseconds(durationMs);
    m_time_end_stop_all = m_time_end;
}

void PilotTranslatorModule::stopAll(){
    m_etat = SerialEtat_Off;
}

void PilotTranslatorModule::stopAllMs(int durationMs){
    m_time_end_stop_all = std::chrono::system_clock::now();
    m_time_end_stop_all += std::chrono::milliseconds(durationMs);
}


void PilotTranslatorModule::setAlive(){
    stopAllMs(5000);
}

void PilotTranslatorModule::openRelayLeft(){
    m_relai_left = true;
    writePilotSerialString(m_textLeftOn);
}
void PilotTranslatorModule::closeRelayLeft(){
    m_relai_left = false;
    writePilotSerialString(m_textLeftOff);
}
void PilotTranslatorModule::openRelayRight(){
    m_relai_right = true;
    writePilotSerialString(m_textRightOn);
}
void PilotTranslatorModule::closeRelayRight(){
    m_relai_right = false;
    writePilotSerialString(m_textRightOff);
}


void PilotTranslatorModule::arduinoLeftRightProp(int res){
    std::ostringstream out;
    int res2 = -res;
    if(res2 < -255){
        res2 = -255;
    }
    if(res2 > 255){
        res2 = 255;
    }
    if(m_inverse_relai){
        res2 = -res2;
    }
    out << "$R," << res2 << ",*\n";
    writePilotSerialString(out.str());
}

void PilotTranslatorModule::arduinoLeftRightOnOff(int value){
    if(value>122){
        closeRelayLeft();
        openRelayRight();
    } else if(value<-122){
        closeRelayRight();
        openRelayLeft();
    } else {
        closeRelayRight();
        closeRelayLeft();
    }
}

void PilotTranslatorModule::setLamp(bool open){
    m_lamp_open = open;
    m_must_be_change = true;
}

void PilotTranslatorModule::inverseLamp(){
    m_lamp_open = !m_lamp_open;
    m_must_be_change = true;
}

void PilotTranslatorModule::setLemcaTrameV2(int pas, bool point_3, int count){
    int nb_count = count - m_last_count;
    if(nb_count > 10000){
        nb_count = 0;
    }
    if(nb_count < 0){
        nb_count = 0;
    }
    m_last_count = count;
    double lissage = 0.8;
    m_count_liss = lissage*m_count_liss+(1.0-lissage)*nb_count;
    double pulse_time_ms = 0;
    if(m_count_liss > 0){
        pulse_time_ms = 25/m_count_liss;
    }
    setLemcaTrame(std::round(pas/4.0), point_3, pulse_time_ms);
}

void PilotTranslatorModule::setLemcaTrame(int pas, bool point_3, double pulse_time_ms){
    LOG_FUNCTION();
    m_capteur_timer.setNewTime();
    
    m_pas_mesured_2 = pas;
    int point_3_2 = point_3;
    if(m_inverse_capteur_3point){
        point_3_2 = !point_3;
    }
    if(m_delai_point_3_ms){
        if(point_3_2){
            auto now = std::chrono::system_clock::now();
            if(!m_old_point_3 && point_3_2){
                m_time_point_3 = std::chrono::system_clock::now();
                m_time_point_3 += std::chrono::milliseconds(m_delai_point_3_ms);
            }
            if(now > m_time_point_3){
                m_point_3 = true;
            }
        } else {
            m_point_3 = point_3_2;
        }
    } else {
        m_point_3 = point_3_2;
    }
    m_old_point_3 = point_3_2;
    
    
    m_pulse_time_ms = pulse_time_ms;
    if(m_pulse_time_ms > 2100){
        m_pulse_time_ms_liss = 2100;
    } else {
        double lissage = 0.9;
        m_pulse_time_ms_liss = lissage*m_pulse_time_ms_liss+(1.0-lissage)*m_pulse_time_ms;
    }
    
    if(m_record == -1){
        if(m_last_values_pas_left.size() < 40*10){
            m_last_values_pas_left.push_back(m_pas_mesured_2);
        }
    } else if(m_record == 1){
        if(m_last_values_pas_right.size() < 40*10){
            m_last_values_pas_right.push_back(m_pas_mesured_2);
        }
    }
    updateSerial();
}

void PilotTranslatorModule::test_mm(double mm){
    setAlive();
    setPasDesiredMM(mm);
}

void PilotTranslatorModule::setPasDesiredMM(double distance_mm){
    Framework::instance().m_debug_str_2 = strprintf("%f setPasDesiredMM", distance_mm);
    m_etat = SerialEtat_Pas;
    m_pas_desired_mm = distance_mm;
    if(distance_mm == 0){
        m_pas_desired2_mm = 0;
    }
}


void PilotTranslatorModule::updateSerial(){
    m_pilot_time.setNewTime();
    auto time = std::chrono::system_clock::now();
    m_stop = false;
    if(time > m_time_end_stop_all){
        m_stop = true;;
    }
    
    m_pas_mesured_mm = 0;
    if(m_command_capteur_left < m_command_capteur_right){
        if(m_pas_mesured_2 < m_command_capteur_middle){
            m_pas_mesured_mm = my_map(m_pas_mesured_2, m_command_capteur_left, m_command_capteur_middle, -m_command_bineuse_debatement_mm/2, 0);
        } else {
            m_pas_mesured_mm = my_map(m_pas_mesured_2, m_command_capteur_middle, m_command_capteur_right, 0, m_command_bineuse_debatement_mm/2);
        }
    } else {
        if(m_command_capteur_middle < m_pas_mesured_2){
            m_pas_mesured_mm = my_map(m_pas_mesured_2, m_command_capteur_left, m_command_capteur_middle, -m_command_bineuse_debatement_mm/2, 0);
        } else{
            m_pas_mesured_mm = my_map(m_pas_mesured_2, m_command_capteur_middle, m_command_capteur_right, 0, m_command_bineuse_debatement_mm/2);
        }
    }
    
    if(m_pas_mesured_mm < -m_command_bineuse_debatement_mm/2){
        m_pas_mesured_mm = -m_command_bineuse_debatement_mm/2;
    }
    if(m_pas_mesured_mm > m_command_bineuse_debatement_mm/2){
        m_pas_mesured_mm = m_command_bineuse_debatement_mm/2;
    }
    double lissage = 0.9;
    m_pas_mesured_mm_liss = lissage*m_pas_mesured_mm_liss + (1.0-lissage)*m_pas_mesured_mm;
    if(m_pas_mesured_mm_liss < -m_command_bineuse_debatement_mm/2){
        m_pas_mesured_mm_liss = -m_command_bineuse_debatement_mm/2;
    }
    if(m_pas_mesured_mm_liss > m_command_bineuse_debatement_mm/2){
        m_pas_mesured_mm_liss = m_command_bineuse_debatement_mm/2;
    }
    
    if(m_must_be_change){
        if(m_lamp_open){
            writePilotSerialString(m_text_lamp_on);
        } else {
            writePilotSerialString(m_text_lamp_off);
        }
        m_must_be_change = false;
    }
    if(!m_stop && m_etat == SerialEtat_Pas){
        Framework::instance().m_debug_str_3 = "pas";
        double error_mm = (m_pas_desired_mm - m_pas_desired2_mm);
        double error_mm_max = m_command_capteur_vitesse_max_mm_s/m_frequence;
        if(error_mm > error_mm_max){
            error_mm = error_mm_max;
        }
        if(error_mm < -error_mm_max){
            error_mm = -error_mm_max;
        }
        m_pas_desired2_mm += error_mm;
        
        double error = (m_pas_desired2_mm - m_pas_mesured_mm);
            
        m_pid_p_out = m_pid_p * error;

        /*double error_i = error;
        m_pid_integrale += error_i * m_pid_dt;
        m_pid_i_out = m_pid_i * m_pid_integrale;

        // Derivative term
        double derivative = (error - m_pid_pre_error) / m_pid_dt;
        m_pid_d_out = m_pid_d * derivative;*/

        m_pid_pre_error = error;
        
        // Calculate total output
        double output = m_pid_p_out;// + m_pid_i_out + m_pid_d_out;
        //INFO(error << " => " << output << " ( " << Pout << " " << Iout << " " << Dout);
        
        if(m_proportionnal){
            Framework::instance().m_debug_str_3 = "pas prop " + std::to_string(output);
            arduinoLeftRightProp(output);
        } else {
            int output2 = std::round(output);
            Framework::instance().m_debug_str_3 = "pas rl " + std::to_string(output2);
            arduinoLeftRightOnOff(output2);
        }
     } else if(!m_stop && m_etat == SerialEtat_Temp){
         Framework::instance().m_debug_str_3 = "temp";
         
         if(m_direction != 0){
             auto time = std::chrono::system_clock::now();
             if(time < m_time_end){
                 INFO("la " << m_temp_prop);
                 if(m_temp_prop < 101){
                     if(m_direction < 0){
                         arduinoLeftRightProp(-m_temp_prop*255/100);
                     } else {
                         arduinoLeftRightProp(m_temp_prop*255/100);
                     }
                 } else {
                     if(m_direction>0){
                         closeRelayLeft();
                         openRelayRight();
                     } else {
                         closeRelayRight();
                         openRelayLeft();
                     }
                 }
             } else {
                 closeRelayLeft();
                 closeRelayRight();
                 m_direction = 0;
             }
         }
     } else if(m_etat == SerialEtat_Cycle){
         Framework::instance().m_debug_str_3 = "cycle";
         
         if(m_cycle_up){
             openRelayLeft();
         } else {
             closeRelayLeft();
         }
         
         if(m_cycle_down){
             openRelayRight();
         } else {
             closeRelayRight();
         }
         
         if(m_cycle_lamp){
             setLamp(1);
         } else {
             setLamp(0);
         }
     } else {
        closeRelayRight();
        closeRelayLeft();
    }
    
    //INFO(m_etat);
}

double PilotTranslatorModule::getDeplacementMeasuredBineuseMm(){
    return m_pas_mesured_mm;
}

double PilotTranslatorModule::getDeplacementDesiredBineuseMm(){
    return m_pas_desired_mm;
}


void PilotTranslatorModule::writePilotSerialString(const std::string & l){
    m_last_order_send = l;
    Framework::instance().m_serialModule.writePort2McuStr(l);
}

void PilotTranslatorModule::resetCycle(){
    Framework & f = Framework::instance();
    f.m_record_lat = 0;
    f.m_record_lon = 0;
    f.m_record_soil_temp = 0;
    f.m_record_soil_hum = 0;
    f.m_record_soil_cond = 0;
    f.m_record_soil_ph = 0;
    f.m_record_soil_n = 0;
    f.m_record_soil_p = 0;
    f.m_record_soil_k = 0;
}

void PilotTranslatorModule::startCycle(){
    if(m_etat != SerialEtat_Cycle){
        m_etat = SerialEtat_Cycle;
        m_begin_cycle = getMillis();
        
        resetCycle();
    }
}


void PilotTranslatorModule::updateCycle(){
    m_cycle_up = 0;
    m_cycle_down = 0;
    m_cycle_lamp = 0;
    Framework & f = Framework::instance();
    
    int millis = getMillis() - m_begin_cycle;
    double s = millis*0.001;
    if( s < 0 || s > 100){
        m_cycle_m = strprintf("%.1f ---off", s);
        return;
    }
    
    if(s < 1){
        m_cycle_down = 1;
        m_cycle_lamp = 1;
        m_cycle_m = strprintf("%.1f down", s);
    } else if(s < 3){
        m_cycle_lamp = 1;
        m_cycle_m = strprintf("%.1f down wait", s);
    } else if(s < 3+1){
        if(f.m_position_module.m_last_gga){
            f.m_record_lat = f.m_position_module.m_last_gga->m_latitude;
            f.m_record_lon = f.m_position_module.m_last_gga->m_longitude;
        }
        m_cycle_up = 1;
        m_cycle_lamp = 1;
        m_cycle_m = strprintf("%.1f up", s);
    } else if(s < 12){
        m_cycle_m = strprintf("%.1f data wait", s);
    } else if(s < 13){
        m_cycle_m = strprintf("%.1f record data", s);
        if(f.m_record_lat > 0){
            std::string s = strprintf("%.7f,%.7f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f", f.m_record_lat, f.m_record_lon, f.m_last_soil_hum, f.m_last_soil_temp, f.m_last_soil_cond , f.m_last_soil_ph, f.m_last_soil_n, f.m_last_soil_p, f.m_last_soil_k);
            f.m_job_manager.addData(s);
            resetCycle();
        }
        
    } else {
        m_etat = SerialEtat_Temp;
        m_cycle_m = strprintf("%.1f --off", s);
    }
    
}
int last_millis = 0;
std::string m_cycle_m = "";



void PilotTranslatorModule::handle25ms(){
    if(m_pilot_version.size() < 2){
        m_arduino_send_v++;
        if(m_arduino_send_v > m_frequence*5){
            m_arduino_send_v = 0;
            Framework::instance().m_serialModule.writePort2McuStr("$V,*\n");
        }
    }
    updateCycle();
    Framework::instance().m_serialModule.writePort2McuStr("$P,*\n");
}
