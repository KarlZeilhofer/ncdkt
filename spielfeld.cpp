#include "spielfeld.h"

Spielfeld::Spielfeld(int nummer, const std::string& name, int preis)
    : nummer(nummer), name(name), eigentuemer(""), preis(preis) {}
