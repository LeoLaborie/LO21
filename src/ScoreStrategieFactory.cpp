#include "ScoreStrategieFactory.h"

#include "StrategieScoreCaserne.h"
#include "StrategieScoreHabitation.h"
#include "StrategieScoreJardin.h"
#include "StrategieScoreMarche.h"
#include "StrategieScoreTemple.h"

std::array<std::unique_ptr<IStrategieScore>, 5>
ScoreStrategieFactory::construireStrategie(const bool variantes[5])
{
    std::array<std::unique_ptr<IStrategieScore>, 5> strategies{};

    strategies[0] = creerStrategie<StrategieScoreHabitationBase, StrategieScoreHabitationVariante>(variantes[0]);
    strategies[1] = creerStrategie<StrategieScoreMarcheBase, StrategieScoreMarcheVariante>(variantes[1]);
    strategies[2] = creerStrategie<StrategieScoreCaserneBase, StrategieScoreCaserneVariante>(variantes[2]);
    strategies[3] = creerStrategie<StrategieScoreTempleBase, StrategieScoreTempleVariante>(variantes[3]);
    strategies[4] = creerStrategie<StrategieScoreJardinBase, StrategieScoreJardinVariante>(variantes[4]);

    return strategies;
}

template <typename BaseStrategy, typename VarianteStrategy>
std::unique_ptr<IStrategieScore> ScoreStrategieFactory::creerStrategie(bool variante)
{
    if (variante)
        return std::make_unique<VarianteStrategy>();
    return std::make_unique<BaseStrategy>();
}
