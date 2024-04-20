#ifndef PILOT_TRANSLATOR_MODULE_H
#define PILOT_TRANSLATOR_MODULE_H

//#include "../decisionmodule/roboticorder/roboticorder.hpp"
#include "../gui/qt/my_qt_serial_port.hpp"
#include "../util/stat.hpp"

enum SerialEtat {
    SerialEtat_Off = 0,
    SerialEtat_Pas = 1,
    SerialEtat_Temp = 2,
};

enum TypeAlgoCommand {
    TypeAlgoCommand_Temp
    ,TypeAlgoCommand_Capteur
};


class PilotTranslatorModule
{
    TypeAlgoCommand m_algo = TypeAlgoCommand_Temp;
    
    
    int m_last_wait_order = 0;
    
    double m_temp_min_ms;
    double m_temp_max_ms;
    double m_temp_agressivite;
    
    double m_offset_mm = 0;
    double m_offset_cam12_mm = 0;
    int m_frequence;
    
    bool m_lamp_open = false;
    bool m_must_be_change = false;
    
    
    std::string m_textLeftOn;
    std::string m_textLeftOff;
    std::string m_textRightOn;
    std::string m_textRightOff;
    std::string m_text_lamp_on;
    std::string m_text_lamp_off;
public:
    bool m_engaged = false;
    TimeObserver m_pilot_time;
    
    std::string m_last_order_send;
    bool m_proportionnal = true;
    
    PilotTranslatorModule();
    virtual ~PilotTranslatorModule();
    
    void initOrLoad(const Config & config);
    void reset();
    
    //run
    void runRobot();
    void changeOrder(double mm);
    
    int m_temp_prop = 110;
    void openRelayRight(int durationMs, int prop = 110);
    void openRelayLeft(int durationMs, int prop = 110);
    void stopAll();
    void stopAllMs(int durationMs);
    void setAlive();
    
    void test_mm(double mm);
    
    bool m_relai_right = 0;
    bool m_relai_left = 0;
    
private:
    
    void openRelayLeft();
    void openRelayRight();
    void closeRelayLeft();
    void closeRelayRight();
    
    void arduinoLeftRightProp(int res);
    void arduinoLeftRightOnOff(int res);
public:
    bool m_inverse_relai = false;
    double m_command_capteur_left = 0;
    double m_command_capteur_middle = 0;
    double m_command_capteur_right = 0;
    double m_pid_p = 0;
    double m_pid_i = 0;
    double m_pid_d = 0;
    
    double m_pid_dt = 0.01;
    double m_pid_integrale = 0;
    double m_pid_pre_error = 0;
    double m_pid_p_out;
    double m_pid_i_out;
    double m_pid_d_out;
    
    double m_command_capteur_vitesse_max_mm_s = 0;
    double m_command_bineuse_debatement_mm = 0;
    
    bool m_inverse_capteur_3point = false;
    
    bool m_command_auto_0_3point = true;
    
    bool m_devers = false;
    double m_devers_l = 0;
    
    bool go3emPoint();

    int m_record = 0;
    std::vector<int> m_last_values_pas_left;
    std::vector<int> m_last_values_pas_right;
    
    double m_deplacement_mm;
    double m_deplacement_max_mm;
    void setPasDesiredMM(double distance_mm);
    void updateSerial();
    
public:
    TimeObserver m_capteur_timer;
    
    SerialEtat m_etat = SerialEtat_Off;
    bool m_stop = true;
    double m_pas_desired_mm = 0;
    double m_pas_desired2_mm = 0;
    double m_pas_mesured_mm = 0;
    double m_pas_mesured_mm_liss = 0;
    int m_pas_mesured_2 = 0;
    
    int m_old_point_3 = 0;
    std::chrono::system_clock::time_point m_time_point_3 = std::chrono::system_clock::now();
    bool m_point_3 = false;
    int m_delai_point_3_ms = false;
    
    double m_pulse_time_ms = 0;
    double m_pulse_time_ms_liss = 0;
    
    
    
    std::chrono::system_clock::time_point m_time_end = std::chrono::system_clock::now();
    int m_direction = 0;
   
    std::chrono::system_clock::time_point m_time_end_stop_all = std::chrono::system_clock::now();
    
    double getDeplacementMeasuredBineuseMm();
    double getDeplacementDesiredBineuseMm();

    int m_last_count = 0;
    double m_count_liss = 0.0;
    void setLemcaTrameV2(int pas, bool point_3, int count);
    void setLemcaTrame(int pas, bool point_3, double pulse_time_ms);
    void setLamp(bool open);
    
    std::string m_pilot_version = "";
    int m_arduino_send_v = 0;
    void writePilotSerialString(const std::string & l);
    void handleArduino();
};

#endif
