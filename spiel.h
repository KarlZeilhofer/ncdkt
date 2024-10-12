#ifndef SPIEL_H
#define SPIEL_H

#include "spielfeld.h"
#include "spieler.h"
#include <vector>
#include <ncurses.h>

class Spiel {
private:
    std::vector<Spielfeld> spielfelder;
    std::vector<Spieler> spieler;
    int aktuellerSpieler;

    void zeichneSpielfeld();
    void zeigeSpielerAufFeld(Spielfeld &feld, int zeile, int spalte);
    void wuerfelnUndZiehen();
public:
    Spiel();
    void willkommenBildschirm();
    void start();
};

#endif
