#ifndef SPIELER_H
#define SPIELER_H

#include <string>

class Spieler {
public:
    std::string name;
    int farbe;      // Spielerfarbe (1=Rot, 2=Blau, 3=Grün, 4=Gelb)
    int position;
    int geld;

    int rundenInSchulung;
    int medienVerlage;

    Spieler(const std::string& name, int farbe, int startPosition, int startGeld);
    void bewegeVorwaerts(int felder);
};

#endif // SPIELER_H
