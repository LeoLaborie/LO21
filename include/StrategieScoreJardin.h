#ifndef STRATEGIE_SCORE_JARDIN_H
#define STRATEGIE_SCORE_JARDIN_H

#include "IStrategieScore.h"

class Hexagone;

class StrategieScoreJardinBase : public IStrategieScore
{
public:
    int calculer(const Plateau& plateau) const override;

protected:
    static int pointsPourUnJardin_Base(const Hexagone* h);
};

class StrategieScoreJardinVariante : public StrategieScoreJardinBase
{
public:
    int calculer(const Plateau& plateau) const override;

private:
    static bool estLac(const Plateau& plateau, const Hexagone* jardin);
};

#endif
