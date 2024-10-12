#ifndef SPIELFELD_H
#define SPIELFELD_H

#include <string>
#include <vector>

class Spielfeld {
public:
    int nummer;
    std::string name;
    int preis;                 // Kaufpreis des Grundstücks
    int investitionsKosten;    // Kosten für den Bau eines Hauses
    std::vector<int> miete;    // Mieten für unterschiedliche Entwicklungsstufen
    std::string eigentuemer;
    int haeuser;               // Anzahl der Häuser (0-4), 5 für Hotel
    bool erwerblich;
    bool medienVerlag;

    Spielfeld(int nr, const std::string& n, int p, int investKosten, const std::vector<int>& m);
};

#endif // SPIELFELD_H
