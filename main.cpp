#include "spiel.h"

/*
 * CHANGELOG
 *
 *
 *
 * TODOs
 * Pfeil-Indikator, der aktuellen Spieler anzeigt
 * Spieler-Status eine Zeile nach oben schieben
 *
 * Zahlungshistorie für jeden Spieler
 *
 * Kaufbare Karten vollständig anzeigen
 *
 * Handel:
 * Grundstück verkaufen an Bank
 * Grundstück versteigern an Mitspieler
 * Grundstück verkaufen an Mitspieler
 * Investition verkaufen an Bank
 * Investition verkaufen an Mitspieler
 * Investition versteigern an Mitspieler
 *
 */




int main() {
    Spiel meinSpiel;
    meinSpiel.willkommenBildschirm();
    meinSpiel.spielLoop();
    return 0;
}
