#include "spieler.h"

Spieler::Spieler(const std::string& name, int farbe, int startPosition, int startGeld)
    : name(name), farbe(farbe), position(startPosition), geld(startGeld), rundenInSchulung(0)
{
    medienVerlage = 0;
}

void Spieler::bewegeVorwaerts(int felder) {
    position = (position + felder) % 40; // Annahme: 40 Spielfelder
}
