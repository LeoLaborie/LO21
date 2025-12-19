#include "StrategieScoreHabitation.h"

#include "Plateau.h"
#include "Hexagone.h"

#include <algorithm>

int StrategieScoreHabitationBase::calculerPlusGrosQuartierScore(const Plateau& plateau, int &multiplicateur)
{
    std::vector<SommetHab> grapheHabitation;
    std::unordered_map<const Hexagone*, int> indexHabitation;

    multiplicateur = 0;

    plateau.pourChaqueHexagone([&](const Hexagone* h){
        if (h->getEstRecouvert()) return;
        if (h->getType() == TypeHex::Habitation)
        {
            SommetHab s;
            s.current = h;
            grapheHabitation.push_back(s);
            indexHabitation[h] = static_cast<int>(grapheHabitation.size() - 1);
        }
        else if (h->getType() == TypeHex::PHabitation)
        {
            multiplicateur += 1;
        }
    });

    for (auto& sommet : grapheHabitation)
    {
        for (auto voisin : sommet.current->getVoisins())
        {
            auto it = indexHabitation.find(voisin);
            if (it != indexHabitation.end())
                sommet.voisins.push_back(it->second);
        }
    }
    int plusGrosQuartier = 0;
    int scorePlusgrosQuartier = 0;

    for (std::size_t i = 0; i < grapheHabitation.size(); ++i)
    {
        if (grapheHabitation[i].dejaVisite) continue;

        int tailleQuartier = 0;
        int scoreQuartier = 0;

        std::vector<std::size_t> aVisiter = {i};
        grapheHabitation[i].dejaVisite = true;

        while (!aVisiter.empty())
        {
            std::size_t currentIndex = aVisiter.back();
            aVisiter.pop_back();
            tailleQuartier += 1;
            scoreQuartier += grapheHabitation[currentIndex].current->getZ() + 1;

            for (int voisinIndex : grapheHabitation[currentIndex].voisins)
            {
                std::size_t v = static_cast<std::size_t>(voisinIndex);
                if (!grapheHabitation[v].dejaVisite)
                {
                    grapheHabitation[v].dejaVisite = true;
                    aVisiter.push_back(v);
                }
            }
        }

        if (tailleQuartier > plusGrosQuartier)
        {
            plusGrosQuartier = tailleQuartier;
            scorePlusgrosQuartier = scoreQuartier;
        }
        else if (tailleQuartier == plusGrosQuartier && scorePlusgrosQuartier < scoreQuartier)
        {
            scorePlusgrosQuartier = scoreQuartier;
        }
    }

    return scorePlusgrosQuartier;
}

int StrategieScoreHabitationBase::calculer(const Plateau& plateau) const
{
    int multiplicateur = 0;
    int score = calculerPlusGrosQuartierScore(plateau, multiplicateur);
    return score * multiplicateur;
}

int StrategieScoreHabitationVariante::calculer(const Plateau& plateau) const
{
    int multiplicateur = 0;
    int score = calculerPlusGrosQuartierScore(plateau, multiplicateur);
    if (score >= 10) score *= 2;
    return score * multiplicateur;
}
