#include "langage.hpp"
#include <iostream>
#include <fstream>

Langage & Langage::instance(){
    static Langage obj;
    return obj;
}


const std::string & Langage::getKey(const std::string & ref){
    Langage & l = instance();
    if(l.m_l == MyLangage_fr){
        auto i = l.m_fr.find(ref);
        if(i != l.m_fr.end()){
            return i->second;
        }
    }
    if(l.m_l == MyLangage_en){
        auto i = l.m_en.find(ref);
        if(i != l.m_en.end()){
            return i->second;
        }
    }
    if(l.m_l == MyLangage_de){
        auto i = l.m_de.find(ref);
        if(i != l.m_de.end()){
            return i->second;
        }
    }
    l.m_fr["ref"] = "$" + ref;
    return l.m_fr["ref"];
}

void Langage::setLangage(int i){
    Langage & l = instance();
    l.m_l = (MyLangage)i;
}

void Langage::setLangage(const std::string & s){
    Langage & l = instance();
    if(s == "FR"){
        l.m_l = MyLangage_fr;
    } else if(s == "EN"){
        l.m_l = MyLangage_en;
    } else if(s == "DE"){
        l.m_l = MyLangage_de;
    }
}


Langage::Langage(){
    add("FR", "HOME_L1", "AVERTISSEMENT");
    add("FR", "HOME_L2", "- L'activation du système de guidage peut causer des mouvements inattendus.");
    add("FR", "HOME_L3", "- Désactiver le guidage et l'hydraulique pour toutes intervention sur la machine.");
    add("FR", "HOME_L4", "- Tout dégat matériel et humain est sous la responsabilité de l'opérateur.");
    add("FR", "ACCEPT", "Accepter");
    add("FR", "HOME_VERSION", "version ");
    
    add("FR", "TRANSLATOR_TITLE", "Menu du translateur");
    add("FR", "TRANSLATOR_SAVE", "Sauvegarder");
    add("FR", "TRANSLATOR_LOAD", "Charger");
    add("FR", "TRANSLATOR_DELETE", "Supprimer");
    add("FR", "TRANSLATOR_TRAJET", "un trajet");
    
    add("FR", "TRANSLATOR_NAME", "Nom du trajet");
    
    add("FR", "TRANSLATOR_WANT_LOAD", "Voulez-vous charger le trajet ?");
    add("FR", "TRANSLATOR_WANT_DELETE", "Voulez-vous supprimer le trajet ?");
    add("FR", "TRANSLATOR_IS_DELETE", "Le trajet est supprimé");
    add("FR", "TRANSLATOR_IS_LOAD", "Le trajet est chargé");
    
    
    
    
    add("FR", "TRANSLATOR_OPEN", "Ouvrir un trajet");
    add("FR", "TRANSLATOR_DELETE", "Supprimer un trajet");
    
    add("FR", "LOGO_OFFSET", "Décalage");
    add("FR", "LOGO_BALISE", "Balise");
    add("FR", "LOGO_INFOS", "Infos");
    add("FR", "LOGO_RETURN", "Retour");
    add("FR", "LOGO_CLOSE", "Quitter");
    add("FR", "LOGO_ENGAGE", "Engager");
    add("FR", "LEFT", "Gauche");
    add("FR", "RIGHT", "Milieu");
    add("FR", "MIDDLE", "Droite");
    add("FR", "QUIT", "Quitter");
   
    add("FR", "INFOS", "Infos");
    add("FR", "CLOSE", "Quitter");
    add("FR", "DIAGNOSTIC", "Diagnostique");
   
    
    add("FR", "OK", "OK");
    add("FR", "CANCEL", "Annuler");
    
    add("FR", "YES", "Oui");
    add("FR", "NO", "Non");

    add("FR", "OPT_SOIL", "Sol");
    add("FR", "SOIL_CYCLE", "Cycle");
    add("FR", "SOIL_TP_START_DELAY", "Delai depart");
    add("FR", "SOIL_TP_DOWN", "Bas");
    add("FR", "SOIL_TP_DOWN_WAIT", "Bas attend");
    add("FR", "SOIL_TP_UP", "Haut");
    add("FR", "SOIL_TP_WAIT", "Haut attend");

    // ---- Cles ajoutees (a traduire / ajuster) ----
    add("FR", "FR", "FR");
    add("FR", "EN", "EN");
    add("FR", "DE", "DE");
    add("FR", "NEXT", "Suivant");
    add("FR", "END", "Fin");
    add("FR", "INVERSE", "Inverser");
    add("FR", "KM_H", "km/h");
    add("FR", "QUALITY", "Qualite");
    add("FR", "LICENCE_NOT_VALID", "Licence invalide");

    add("FR", "PHBAS", "pH bas");
    add("FR", "PHHAUT", "pH haut");

    add("FR", "HYDR_V_MAX", "Vitesse max");
    add("FR", "HYDR_AGRESS", "Agressivite");

    // Ecran principal
    add("FR", "MAIN_3POINT", "3 points");
    add("FR", "MAIN_CONTRAINT", "Contrainte");
    add("FR", "MAIN_JOYSTICK_AUTO", "Joystick auto");
    add("FR", "MAIN_JOYSTICK_LEFT", "Joystick gauche");
    add("FR", "MAIN_JOYSTICK_RIGHT", "Joystick droite");
    add("FR", "MAIN_MAX_LEFT", "Butee gauche");
    add("FR", "MAIN_MAX_RIGHT", "Butee droite");
    add("FR", "MAIN_NOT_CONNECTED", "Non connecte");
    add("FR", "MAIN_NOT_ENGAGED", "Non engage");
    add("FR", "MAIN_PAS_MESURED_0", "Pas mesure");
    add("FR", "MAIN_SCORE_LOW", "Score faible");
    add("FR", "MAIN_TEMP", "Temperature");
    add("FR", "MAIN_UNKNOWN_ERROR", "Erreur inconnue");
    add("FR", "MAIN_VITESSE_LOW", "Vitesse faible");
    add("FR", "MAIN_VITESSE_SAT", "Vitesse satellite");

    // Options
    add("FR", "OPT_GEN_TITLE", "General");
    add("FR", "OPT_LUM", "Luminosite");
    add("FR", "OPT_SON", "Son");
    add("FR", "OPT_OUTIL", "Outil");
    add("FR", "OPT_OUTIL_L", "Largeur");
    add("FR", "OPT_OUTIL_LG", "Longueur");
    add("FR", "OPT_OUTIL_RECORD_H", "Hauteur enreg.");
    add("FR", "OPT_OUTIL_REPLAY_H", "Hauteur rejeu");
    add("FR", "OPT_HYDRAULIQUE", "Hydraulique");
    add("FR", "OPT_DEPLACEMENT", "Deplacement");
    add("FR", "OPT_BALISE", "Balise");
    add("FR", "OPT_PARAM_TRANSLATEUR", "Parametres translateur");
    add("FR", "OPT_TRANSLATEUR", "Translateur");
    add("FR", "OPT_TRANSLATEUR_PARAM", "Parametres");
    add("FR", "OPT_TRANSLATEUR_CARD", "Carte translateur");
    add("FR", "OPT_TEST_TRANSLATEUR", "Test translateur");
    add("FR", "OPT_TEST_TRANSLATEUR2", "Test translateur 2");

    // Options avancees / licence
    add("FR", "OPT_AV_TITLE", "Avance");
    add("FR", "OPT_AV_TECHNICIEN", "Technicien");
    add("FR", "OPT_LICENCE_TITLE", "Licence");
    add("FR", "OPT_LIC_PANEL", "Panneau");
    add("FR", "OPT_LIC_MCU", "MCU");
    add("FR", "OPT_LIC_LICENCE", "Licence");
    add("FR", "OPT_LIC_OK", "Licence OK");
    add("FR", "OPT_LIC_ERR_L1", "Erreur licence (longueur)");
    add("FR", "OPT_LIC_ERR_B1", "Erreur licence (debut)");
    add("FR", "OPT_LIC_ERR_F", "Erreur licence");

    // Parametrage translateur
    add("FR", "PARAMS_TITLE", "Parametres");
    add("FR", "PARAMS_PAS", "Pas");
    add("FR", "PARAMS_P1_BEGIN", "Debut");
    add("FR", "PARAMS_P1_SECURITY", "Securite");
    add("FR", "PARAMS_P2_TEST", "Test");
    add("FR", "PARAMS_P2_INVERSE_QUESTION", "Inverser ?");
    add("FR", "PARAMS_P3_MAX_LEFT", "Max gauche");
    add("FR", "PARAMS_P4_MAX_RIGHT", "Max droite");
    add("FR", "PARAMS_P5_CENTER", "Centre");
    add("FR", "PARAMS_P6_DEBATTEMENT_CM", "Debattement (cm)");
    add("FR", "PARAMS_P6_MESURE", "Mesure");
    add("FR", "PARAMS_P7_AGRESS", "Agressivite");
    add("FR", "PARAMS_P7_AGRESSIVITY", "Agressivite");
    add("FR", "PARAMS_P7_PROPORTIONNAL", "Proportionnel");
    add("FR", "PARAMS_P7_PROPORTIONNAL2", "Proportionnel 2");
    add("FR", "PARAMS_P7_PUT_MAX", "Mettre au max");

    // Rapide / compteurs
    add("FR", "RAPIDE_TOTAL", "Total");
    add("FR", "RAPIDE_PARTIAL", "Partiel");
    add("FR", "RAPIDE_SESSION", "Session");
    add("FR", "RAPIDE_GPS", "GPS");
    add("FR", "RAPIDE_SATELLITES", "satellites");
    add("FR", "RAPIDE_RESET", "Reinitialiser");
}

void Langage::add(std::string langage, std::string key, std::string trad){
    if(langage == "FR"){
        m_fr[key] = trad;
    } else if(langage == "EN"){
        m_en[key] = trad;
    } else if(langage == "DE"){
        m_de[key] = trad;
    }
    for(auto e : m_all){
        if(e == key){
            return;
        }
    }
    m_all.push_back(key);
}


void Langage::verifyAll(){

    std::ofstream myfile;
    myfile.open ("/Users/lemairec/workspace/bineuse/all.txt");
    for(auto s : m_all){
        myfile << s << "\n";
        
    }
    myfile.close();
    
    std::ofstream myfile2;
    myfile2.open ("/Users/lemairec/workspace/bineuse/all2.txt");
    for(auto s : m_all){
        std::string trad = "*****";
        std::string s2 = s;
        auto i = m_fr.find(s2);
        if(i != m_fr.end()){
            trad = i->second;
        }
        myfile2 << "add(\"FR\", \"" << s << "\", \"" << trad << "\");\n";
    }
    myfile2 << "\n";
    myfile2 << "\n";
    
    for(auto s : m_all){
        std::string trad = "*****";
        std::string s2 = s;
        auto i = m_en.find(s2);
        if(i != m_en.end()){
            trad = i->second;
            myfile2 << "add(\"EN\", \"" << s << "\", \"" << trad << "\");\n";
        } else {
            myfile2 << "add(\"EN\", \"" << s << "\", \"" << trad << "\");//" << m_fr[s]<<"\n";
        }
    }
    
    myfile2 << "\n";
    myfile2 << "\n";
    for(auto s : m_all){
        std::string trad = "*****";
        std::string s2 = s;
        auto i = m_de.find(s2);
        if(i != m_de.end()){
            trad = i->second;
            myfile2 << "add(\"DE\", \"" << s << "\", \"" << trad << "\");\n";
        } else {
            myfile2 << "add(\"DE\", \"" << s << "\", \"" << trad << "\");\n";
        }
        
    }
    
    for(auto s : m_all){
        std::string trad = "*****";
        std::string s2 = s;
        myfile2 << "add(\"NL\", \"" << s << "\", \"" << m_fr[s] << "\");" <<"\n";
        
    }
    myfile2.close();
        
}
