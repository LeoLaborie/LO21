#ifndef SCORE_STRATEGIE_FACTORY_H
#define SCORE_STRATEGIE_FACTORY_H

#include <array>
#include <memory>

#include "IStrategieScore.h"

class IStrategieScore;

class ScoreStrategieFactory
{
public:
    static std::array<std::unique_ptr<IStrategieScore>, 5> construireStrategie(const bool variantes[5]);

private:
    template <typename BaseStrategy, typename VarianteStrategy>
    static std::unique_ptr<IStrategieScore> creerStrategie(bool variante);
};

#endif
