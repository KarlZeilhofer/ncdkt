#ifndef SPIELFELD_H
#define SPIELFELD_H

#include <string>
#include <vector>

class Spielfeld {
public:
    int nummer;
    std::string name;
    std::string eigentuemer;
    std::vector<int> spielerAufFeld;

    Spielfeld(int nummer, const std::string &name);
    void fuegeSpielerHinzu(int spielerIndex);
    void entferneSpieler(int spielerIndex);
};

#endif
