#ifndef ENUM_H
#define ENUM_H

enum Etat {
    Etat_Reset = 0,
    Etat_PointASaved = 1,
    Etat_OK = 2,
    Etat_ParcelleAdd = 3,
    Etat_ParcellePause = 4,
    Etat_Points = 5
};

enum ErrorOrWarning {
    Error_TranslatorNotConnected,
    Error_GPSNotConnected,
    Error_GPSNotRTK,
    Warning_VitesseLow,
    Warning_3PointHitch,
    Warning_NotEngaged
};

enum SynchroState {
    SynchroState_None,
    SynchroState_Begin,
    SynchroState_Continue,
    SynchroState_Fail,
    SynchroState_Complete
};


#endif
