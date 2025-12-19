#ifndef STRATEGIE_SCORE_CASERNE_H
#define STRATEGIE_SCORE_CASERNE_H

#include "IStrategieScore.h"

class Hexagone;

class StrategieScoreCaserneBase : public IStrategieScore
{
public:
    int calculer(const Plateau& plateau) const override;

protected:
    static int pointsPourUneCaserne_Base(const Hexagone* h);
};

class StrategieScoreCaserneVariante : public StrategieScoreCaserneBase
{
public:
    int calculer(const Plateau& plateau) const override;

private:
    static int pointsPourUneCaserne_Variante(const Hexagone* h);
};

#endif
