#ifndef PLATEAU_H
#define PLATEAU_H

#include <vector>
#include <algorithm>
#include <iostream>
#include "Tuile.h"
#include "Hexagone.h"
#include "Position.h"
#include "utils.h"
#include "Partie.h"

class Plateau
{
friend class Partie;
private:
    std::vector<Tuile> listeTuiles;
    std::vector<Hexagone *> listeHexagones;

    bool varianteHabitation{0};
    bool varianteMarche{0};
    bool varianteTemple{0};
    bool varianteCaserne{0};
    bool varianteJardin{0};

    explicit Plateau();// Les plateaux sont définis par le constructeur de Joueur
    
public:

    bool verifierPlacementTuile(Position &p) const;
    Position *essayerPlacerTuile(Tuile &t);
    bool ajouterTuile(Tuile &t, Position &p);
    void updateVoisins();
    const std::vector<Tuile> &getTuiles() const { return listeTuiles; }
    const std::vector<Hexagone *> &getHexagones() const { return listeHexagones; }
    int calculerPoints() const;
    int calculerPointsHabitation() const;
    int calculerPointsMarche() const;
    int calculerPointsJardin() const;
    bool conditionVarianteJardin (const Quartier* q) const;
    int calculerPointsTemple() const;
    int calculerPointsCaserne() const;
    void afficher() const;
};

#endif
