#include "spielfeld.h"
#include <algorithm>  // Für std::remove

Spielfeld::Spielfeld(int nummer, const std::string &name)
    : nummer(nummer), name(name) {}

void Spielfeld::fuegeSpielerHinzu(int spielerIndex) {
    spielerAufFeld.push_back(spielerIndex);
}

void Spielfeld::entferneSpieler(int spielerIndex) {
    // Spieler aus dem Vektor entfernen
    spielerAufFeld.erase(
        std::remove(spielerAufFeld.begin(), spielerAufFeld.end(), spielerIndex),
        spielerAufFeld.end()
    );
}
