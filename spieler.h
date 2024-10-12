#ifndef SPIELER_H
#define SPIELER_H

#include <string>

class Spieler {
public:
    std::string name;
    int position;
    int geld;

    Spieler(const std::string &name, int startPosition, int startGeld);
    void bewegeVorwaerts(int felder);
};

#endif
