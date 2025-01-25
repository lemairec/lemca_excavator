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
