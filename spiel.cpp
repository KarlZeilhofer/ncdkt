#include "spiel.h"
#include <algorithm>

Spiel::Spiel() : aktuellerSpieler(0) {
    // Initialisierung der Spielfelder (Beispielhafte Namen)
    for (int i = 1; i <= 40; ++i) {
        spielfelder.push_back(Spielfeld(i, "Spielfeld " + std::to_string(i)));
    }
}

void Spiel::willkommenBildschirm() {
    initscr();
    curs_set(1);
    echo();

    int anzahlSpieler;
    mvprintw(0, 0, "Willkommen bei DKT! Wie viele Spieler nehmen teil (2-4)? ");
    scanw("%d", &anzahlSpieler);

    while (anzahlSpieler < 2 || anzahlSpieler > 4) {
        mvprintw(1, 0, "Ungültige Anzahl von Spielern. Bitte wähle zwischen 2 und 4: ");
        scanw("%d", &anzahlSpieler);
    }

    for (int i = 0; i < anzahlSpieler; i++) {
        char name[50];
        mvprintw(2 + i, 0, "Name des Spielers %d: ", i + 1);
        getstr(name);
        spieler.push_back(Spieler(name, 1, 1500));  // Jeder Spieler startet auf Feld 1 mit 1500 Euro
    }

    clear();
    mvprintw(0, 0, "Alle Spieler sind bereit! Drücke eine Taste, um zu beginnen...");
    refresh();
    getch();
    noecho();
    curs_set(0);
    endwin();
}

void Spiel::wuerfelnUndZiehen() {
    clear();
    Spieler &aktSpieler = spieler[aktuellerSpieler];
    mvprintw(0, 0, "Spieler %s ist an der Reihe. Drücke eine Taste zum Würfeln.", aktSpieler.name.c_str());
    getch();

    int wuerfel1 = (rand() % 6) + 1;
    int wuerfel2 = (rand() % 6) + 1;
    int schritte = wuerfel1 + wuerfel2;

    mvprintw(2, 0, "Spieler %s würfelt: %d + %d = %d", aktSpieler.name.c_str(), wuerfel1, wuerfel2, schritte);
    aktSpieler.bewegeVorwaerts(schritte);
    spielfelder[aktSpieler.position].fuegeSpielerHinzu(aktuellerSpieler);
    refresh();
    getch();
}

void Spiel::start() {
    willkommenBildschirm();
    initscr();
    curs_set(0);
    while (true) {
        zeichneSpielfeld();
        wuerfelnUndZiehen();
        aktuellerSpieler = (aktuellerSpieler + 1) % spieler.size();
    }
    endwin();
}

void Spiel::zeichneSpielfeld() {
    clear();

    // Beispiel: Zeichnen der oberen Reihe von Spielfeldern (1 bis 10)
    for (int i = 0; i < 10; ++i) {
        mvprintw(1, i * 12, "[%2d] %-10s", spielfelder[i].nummer, spielfelder[i].name.c_str());
        zeigeSpielerAufFeld(spielfelder[i], 2, i * 12);
    }

    refresh();
}

void Spiel::zeigeSpielerAufFeld(Spielfeld &feld, int zeile, int spalte) {
    for (int spielerIndex : feld.spielerAufFeld) {
        mvprintw(zeile, spalte, "P%d", spielerIndex + 1);
    }
}
