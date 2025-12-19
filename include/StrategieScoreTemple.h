#ifndef STRATEGIE_SCORE_TEMPLE_H
#define STRATEGIE_SCORE_TEMPLE_H

#include "IStrategieScore.h"

class Hexagone;

class StrategieScoreTempleBase : public IStrategieScore
{
public:
    int calculer(const Plateau& plateau) const override;

protected:
    static int pointsPourUnTemple_Base(const Hexagone* h);
};

class StrategieScoreTempleVariante : public StrategieScoreTempleBase
{
public:
    int calculer(const Plateau& plateau) const override;

private:
    static int pointsPourUnTemple_Variante(const Hexagone* h);
};

#endif
