#ifndef SPIELFELD_H
#define SPIELFELD_H

#include <string>

class Spielfeld {
public:
    int nummer;
    std::string name;
    std::string eigentuemer;
    int preis;

    Spielfeld(int nummer, const std::string& name, int preis);
};

#endif // SPIELFELD_H
