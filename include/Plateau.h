#ifndef PLATEAU_H
#define PLATEAU_H

#include <vector>
#include <algorithm>
#include <iostream>
#include "Tuile.h"
#include "Hexagone.h"
#include "Position.h"
#include "couleurs_console.h"

// Déclarations anticipées pour l'amitié
class Partie;
class Joueur;





class Plateau
{
    friend class Partie;
    friend class Joueur;

private:
    std::vector<Tuile> listeTuiles;
    bool variantesScores[5];
    explicit Plateau(const bool variantesScore[5]); // Les plateaux sont définis par le constructeur de Joueur
    explicit Plateau();

public:
    template<typename F>
    void pourChaqueHexagone(F f)
    {
        for (auto& tuile : listeTuiles)
            for (auto& hex : tuile.getHexagones())
                f(hex);
    }
    template<typename F>
    void pourChaqueHexagone(F f) const
    {
        for (const auto& tuile : listeTuiles)
            for (auto hex : tuile.getHexagones())
                f(hex);
    }
    //getters
    const std::vector<Tuile> &getTuiles() const { return listeTuiles; };
    
    const bool* getVarianteScores() const {return variantesScores;};

   
    
    //calcul des points
    int calculerPoints() const;
    int calculerPointsia(int& diff) const;
    int calculerPointsHabitation() const;
    int calculerPointsMarche() const;
    int calculerPointsJardin() const;
    bool conditionVarianteJardin(const Hexagone* q) const;
    int calculerPointsTemple() const;
    int calculerPointsCaserne() const;

    //gestion placement
    std::vector<Position>getPositionsLegales(const Tuile &t) const;
    void afficherPositionsLegales(const Tuile &t) const;
    bool verifierPlacementTuile(const Position &p,const Tuile &t) const;
    int placerTuile(Tuile &t, Position &p);
    int placerTuile(Tuile &t);
    void updateVoisins();
    
    //surchage operateurs
    friend std::ostream& operator<<(std::ostream& os, const Plateau& p) ;
};
#endif
