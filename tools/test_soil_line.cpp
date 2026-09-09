//Auto-test de splitSoilLine : soil.txt a change de format le 2025-12-15 (virgule ->
//point-virgule), le chargement d'un vieux job depend de ce decoupage.
//
//Compilation (MSYS2, <build> = un dossier build deja configure, pour environnement.hpp) :
//  g++ -std=c++17 -w -o test_soil_line tools/test_soil_line.cpp src/util/*.cpp \
//      -Isrc -I<build> -isystem /mingw64/include/QtCore -lQt5Core && ./test_soil_line

#include "../src/util/csv.hpp"
#include <cassert>
#include <cmath>
#include <clocale>

int main(){
    std::vector<std::string> w;

    //Le tracteur tourne sous locale FR : c'est pour ca que soil.txt a des decimales
    //a la virgule, et c'est ce qui faisait lire "50.6552792" comme 50 par std::stod.
    //Si setlocale echoue (locale absente) le test reste valable, juste moins severe.
    setlocale(LC_ALL, "fr_FR.UTF-8") || setlocale(LC_ALL, "French_France.1252");
    assert(std::fabs(toDouble("50,6552792") - 50.6552792) < 1e-9);
    assert(std::fabs(toDouble("50.6552792") - 50.6552792) < 1e-9);
    assert(std::fabs(toDouble("-4,25") + 4.25) < 1e-9);

    //format actuel : ';' 10 colonnes, la 6e (index 5) est le pH corrige
    assert(splitSoilLine("50.6100000;4.5200000;25.0;18.0;0.5;6.8;0.0;0.0;0.0;7.1", w));
    assert(w.size() == 10 && std::fabs(toDouble(w[5]) - 6.8) < 1e-9);

    //format ';' a 9 colonnes : entre "Fix ;" et l'ajout du pH corrige
    assert(splitSoilLine("50.61;4.52;25.0;18.0;0.5;6.8;0.0;0.0;0.0", w) && w.size() == 9);

    //ancien format ',' a 9 colonnes, decimales en '.'
    assert(splitSoilLine("50.6100000,4.5200000,25.0,18.0,0.5,6.8,0.0,0.0,0.0", w));
    assert(w.size() == 9 && std::fabs(toDouble(w[0]) - 50.61) < 1e-9);

    //decimales a la virgule avec separateur ';' : les champs restent lisibles
    assert(splitSoilLine("50,61;4,52;25,0;18,0;0,5;6,8;0,0;0,0;0,0", w));
    assert(std::fabs(toDouble(w[1]) - 4.52) < 1e-9);

    //ambigu : ',' separateur ET ',' decimal -> rejet, plutot que des coordonnees fausses
    assert(!splitSoilLine("50,6100000,4,5200000,25,0,18,0,0,5,6,8,0,0,0,0,0,0", w));

    assert(!splitSoilLine("", w));
    assert(!splitSoilLine("PROG;EXCAVATOR", w));

    printf("test_soil_line ok\n");
    return 0;
}
