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

struct SommetHab
{
    const Hexagone *current;
    std::vector<size_t> voisins;
    bool dejaVisite = false;
};

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
    template <typename F>
    void pourChaqueHexagone(F f)
    {
        for (auto &tuile : listeTuiles)
            for (auto &hex : tuile.getHexagones())
                f(hex);
    }
    template <typename F>
    void pourChaqueHexagone(F f) const
    {
        for (const auto &tuile : listeTuiles)
            for (auto hex : tuile.getHexagones())
                f(hex);
    }

    // getters
    const std::vector<Tuile> &getTuiles() const { return listeTuiles; };
    const bool *getVarianteScores() const { return variantesScores; };

    // Calcul des points

    /**
     * @brief Calcule le nombre de points total
     * @return Nombre total de points total
     */
    int calculerPoints() const;
    int calculerPointsia(int &diff) const;

    /**
     * @brief Calcule le nombre de points apportés par les habitations
     * @return Nombre total de points apportés par les habitations
     */
    int calculerPointsHabitation(std::vector<SommetHab> grapheHabitation) const;

    /**
     * @brief Calcule le nombre de points donnés par le marché
     * @return Nombre total de points apportés par le marché
     */
    int calculerPointsMarche(const Hexagone *h) const;

    /**
     * @brief Calcule le nombre de points apportés par le jardin
     * @return Nombre total de points apportés par le jardin
     */
    int calculerPointsJardin(const Hexagone *h) const;
    bool conditionVarianteJardin(const Hexagone *q) const;

    /**
     * @brief Calcule le nombre de points apportés par le temple
     * @return Nombre total de points apportés par le temple
     */
    int calculerPointsTemple(const Hexagone *h) const;

    /**
     * @brief Calcule le nombre de points apportés par la caserne
     * @return Nombre de points
     */
    int calculerPointsCaserne(const Hexagone *h) const;

    // Gestion des placements

    /**
     * @brief Calcule toutes les positions légales en fonction d'une tuile (sa rotation)
     * @param t Tuile à placer
     * @return Vecteur contenant toutes les positions correctes
     */
    std::vector<Position> getPositionsLegales(const Tuile &t) const;

    /**
     * @brief Affiche toutes les positions légales possibles pour une certaine tuile
     * @param t Tuile à placer
     */
    void afficherPositionsLegales(const Tuile &t) const;

    /**
     * @brief Vérifie si le placement d'une tuile à une position donnée est valide
     * @param p Position de placement
     * @param t Tuile à placer
     * @return true si le placement est valide, false sinon
     */
    bool verifierPlacementTuile(const Position &p, const Tuile &t) const;

    /**
     * @brief Place une tuile à une position donnée sur le plateau
     * @param t Tuile à placer
     * @param p Position de placement
     * @return Nombre de carrières recouvertes
     * @throws std::invalid_argument si le placement est invalide
     */
    int placerTuile(Tuile &t, Position &p);
    int placerTuile(Tuile &t);
    /**
     * @brief Met à jour les voisins de chaque hexagone du plateau
     */
    void updateVoisins();

    /**
     * @brief Surchage l'operator << pour utiliser std::cout
     * @return une référence ostream
     */
    friend std::ostream &operator<<(std::ostream &os, const Plateau &p);
};
#endif
