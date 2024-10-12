#include "spielfeld.h"

Spielfeld::Spielfeld(int nr, const std::string& n, int p, int investKosten, const std::vector<int>& m)
    : nummer(nr), name(n), preis(p), investitionsKosten(investKosten), miete(m), eigentuemer(""), haeuser(0)
{
    if(p == 0){
        erwerblich = false;
    }else{
        erwerblich = true;
    }
    medienVerlag = false;
}
