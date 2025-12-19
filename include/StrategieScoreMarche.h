#ifndef STRATEGIE_SCORE_MARCHE_H
#define STRATEGIE_SCORE_MARCHE_H

#include "IStrategieScore.h"

class Hexagone;

class StrategieScoreMarcheBase : public IStrategieScore
{
public:
    int calculer(const Plateau& plateau) const override;

protected:
    static int pointsPourUnMarche_Base(const Hexagone* h);
};

class StrategieScoreMarcheVariante : public StrategieScoreMarcheBase
{
public:
    int calculer(const Plateau& plateau) const override;

private:
    static int pointsPourUnMarche_Variante(const Hexagone* h);
};

#endif
