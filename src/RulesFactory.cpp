#include "RulesFactory.h"

#include <algorithm>
#include <vector>

#include "Partie.h"
#include "Plateau.h"
#include "Tuile.h"

void GenerateurTuilesClassique::generer(Partie &partie) const
{
    partie.genererTuilesParties(utiliserToutesLesTuiles);
}

void GenerateurTuilesFull::generer(Partie &partie) const
{
    partie.genererTuilesParties(true);
}

RulesFactoryPersonnalisee::RulesFactoryPersonnalisee(const bool variantes[5], bool fullTuiles)
{
    for (int i = 0; i < 5; ++i)
        regles.variantesScore[i] = variantes[i];
    regles.fullTuiles = fullTuiles;
}

RulesFactoryPersonnalisee::RulesFactoryPersonnalisee(Ruleset r)
    : regles(r)
{
}

Ruleset RulesFactoryPersonnalisee::creerRuleset() const
{
    return regles;
}

ScoreStrategies RulesFactoryPersonnalisee::creerStrategiesScore() const
{
    return ChoisirScoreStrategies(regles.variantesScore);
}

std::unique_ptr<ITuileSetGenerator> RulesFactoryPersonnalisee::creerGenerateurTuiles() const
{
    if (regles.fullTuiles)
        return std::make_unique<GenerateurTuilesFull>();
    return std::make_unique<GenerateurTuilesClassique>(false);
}

RulesFactoryClassique::RulesFactoryClassique()
    : RulesFactoryPersonnalisee(Ruleset{})
{
}

RulesFactoryVariantes::RulesFactoryVariantes()
    : RulesFactoryPersonnalisee([]{
          Ruleset r;
          r.variantesScore[0] = true;
          r.variantesScore[1] = true;
          r.variantesScore[3] = true;
          r.variantesScore[4] = true;
          r.fullTuiles = true;
          return r;
      }())
{
}
