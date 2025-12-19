#ifndef STRATEGIE_SCORE_HABITATION_H
#define STRATEGIE_SCORE_HABITATION_H

#include "IStrategieScore.h"
#include <unordered_map>
#include <vector>

class Hexagone;

struct SommetHab
{
    const Hexagone* current = nullptr;
    bool dejaVisite = false;
    std::vector<int> voisins;
};

class StrategieScoreHabitationBase : public IStrategieScore
{
public:
    int calculer(const Plateau& plateau) const override;

protected:
    static int calculerPlusGrosQuartierScore(const Plateau& plateau, int &multiplicateur);
};

class StrategieScoreHabitationVariante : public StrategieScoreHabitationBase
{
public:
    int calculer(const Plateau& plateau) const override;
};

#endif
