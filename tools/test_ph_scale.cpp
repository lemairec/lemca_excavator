//Auto-test de phBande : c'est ce qui decide la couleur de chaque pastille pH sur la
//carte (menu Infos > Echelle pH). Une erreur de borne = une carte de chaulage fausse.
//
//Compilation (MSYS2, <build> = un dossier build deja configure, pour environnement.hpp) :
//  g++ -std=c++17 -w -o test_ph_scale tools/test_ph_scale.cpp src/util/*.cpp \
//      -Isrc -I<build> -isystem /mingw64/include/QtCore -lQt5Core && ./test_ph_scale

#include "../src/util/util.hpp"
#include <cassert>

int main(){
    const double b[PH_BANDE_N] = {5.5, 6.0, 6.5, 7.0};

    //sous la premiere borne -> bande 0 (rouge)
    assert(phBande(5.49, b) == 0);
    assert(phBande(0.0,  b) == 0);
    //pH negatif (offset carte a -2 sur un sol acide) : pas d'index hors tableau
    assert(phBande(-3.0, b) == 0);

    //pile sur une borne -> bande du dessus : les intervalles sont [min, max[
    assert(phBande(5.5, b) == 1);
    assert(phBande(6.0, b) == 2);
    assert(phBande(6.5, b) == 3);
    assert(phBande(7.0, b) == 4);

    //milieu de chaque bande
    assert(phBande(5.75, b) == 1);
    assert(phBande(6.25, b) == 2);
    assert(phBande(6.75, b) == 3);

    //au-dela de la derniere borne et hors echelle -> derniere bande, pas de noir
    assert(phBande(9.0,  b) == PH_BANDE_N);
    assert(phBande(99.0, b) == PH_BANDE_N);

    //bande vide (deux bornes egales) : personne ne tombe dedans, rien ne plante
    const double collees[PH_BANDE_N] = {6.0, 6.0, 6.5, 7.0};
    assert(phBande(5.9, collees) == 0);
    assert(phBande(6.0, collees) == 2);

    //bornes non triees (ini bidouille a la main) : le premier seuil depasse gagne
    const double desordre[PH_BANDE_N] = {7.0, 5.5, 6.0, 6.5};
    int r = phBande(6.2, desordre);
    assert(r >= 0 && r <= PH_BANDE_N);

    printf("test_ph_scale ok\n");
    return 0;
}
