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
    std::vector<int> verfuegbareFarben; // Verfügbare Farben (1=Rot, 2=Blau, 3=Gruen, 4=Gelb)
    std::string eingabeHinweis;

    void setzeFarben();
    int zeigeFarbauswahl(WINDOW* win, int starty);
    void zeichneSpielfeld();
    void wuerfelnUndZiehen();

    int lastWuerfel1;
    int lastWuerfel2;
public:
    Spiel();
    void willkommenBildschirm();
    void spielLoop();
    void feldAktion(Spieler &sp);
    void animateMovement(Spieler &sp, int schritte);
    void animateDice(int startRow, int &w1, int &w2);
    void clearInteractionArea();
    void zeichneWuerfel();
};

#endif // SPIEL_H
