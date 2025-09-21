#ifndef TUILE_H
#define TUILE_H

#include <iostream>
#include <vector>
#include "Hexagone.h"

class Tuile
{
private:
    std::vector<Hexagone *> hex;

public:
    void creerTuile(Hexagone *hex1, Hexagone *hex2, Hexagone *hex3);
};

#endif
