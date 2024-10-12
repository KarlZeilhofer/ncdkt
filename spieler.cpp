#include "spieler.h"

Spieler::Spieler(const std::string &name, int startPosition, int startGeld)
    : name(name), position(startPosition), geld(startGeld) {}

void Spieler::bewegeVorwaerts(int felder) {
    position = (position + felder) % 40;  // Rundlauf über 40 Felder
}
