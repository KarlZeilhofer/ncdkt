#ifndef SPIEL_H
#define SPIEL_H

#include "spieler.h"
#include "spielfeld.h"
#include <vector>
#include <ncurses.h>

class Spiel {
private:
    std::vector<Spieler> spieler;
    int aktuellerSpieler;
    std::vector<Spielfeld> spielfelder;
    std::vector<int> verfuegbareFarben; // Verfügbare Farben (1=Rot, 2=Blau, 3=Grün, 4=Gelb)

    void setzeFarben();
    int zeigeFarbauswahl(WINDOW* win, int starty);
    void zeichneSpielfeld();
    void wuerfelnUndZiehen();

public:
    Spiel();
    void willkommenBildschirm();
    void spielLoop();
};

#endif // SPIEL_H
