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

/**
 * @class Plateau
 * @brief Représente le plateau d'un joueur, contenant ses tuiles et les règles de score
 */
class Plateau
{
    friend class Partie;
    friend class Joueur;

private:
    std::vector<Tuile> listeTuiles;
    bool variantesScores[5];
    /**
     * @brief Constructeur de Plateau
     * @param variantesScore Tableau des variantes de score
     */
    explicit Plateau(const bool variantesScore[5]); // Les plateaux sont définis par le constructeur de Joueur

    /**
     * @brief Constructeur par défaut de Plateau
     */
    explicit Plateau();

public:
    /**
     * @brief Applique une fonction à chaque hexagone du plateau
     * @param f Fonction à appliquer
     */
    template <typename F>
    void pourChaqueHexagone(F f)
    {
        for (Tuile &tuile : listeTuiles)
            for(Tuile::Iterator it = tuile.getIterator(); !it.isDone(); it.next())
                f(&it.currentItem());
    }

    /**
     * @brief Applique une fonction à chaque hexagone du plateau (version constante)
     * @param f Fonction à appliquer
     */
    template <typename F>
    void pourChaqueHexagone(F f) const
    {
        for (const Tuile &tuile : listeTuiles)
            for(Tuile::ConstIterator it = tuile.getConstIterator(); !it.isDone(); it.next())
                f(&it.currentItem());
    }

    // Getters
    /**
     * @brief Retourne les tuiles du plateau
     * @return const std::vector<Tuile>& : référence constante vers le vecteur de tuiles
     */
    const std::vector<Tuile> &getTuiles() const { return listeTuiles; };

    // TODO: Méthode jamais utilisée
    /**
     * @brief Retourne les variantes de score du plateau
     * @return const bool* : pointeur constant vers le tableau des variantes de score
     */
    const bool *getVarianteScores() const { return variantesScores; };

    // Calcul des points

    /**
     * @brief Calcule le nombre de points total
     * @return Nombre total de points total
     */
    int calculerPoints() const;

    /**
     * @brief Calcule le nombre de points gagnés par l'IA
     * @param diff
     * @return Nombre de points gagnés
     */
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

    /**
     * @brief Vérifie la condition de la variante de score "Jardin"
     * @param q Pointeur vers l'hexagone jardin
     * @return true si la condition est remplie, false sinon
     */
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

    /**
     * @brief Ajoute une tuile au plateau sans vérification de position
     * @param t Tuile à placer
     * @return 1
     */
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

    /**
     * @brief Reconstruit un plateau à partir des tuiles et variantes sauvegardées.
     * @param variantes Variantes de score actives.
     * @param tuiles Ensemble des tuiles déjà posées.
     * @return Plateau réhydraté.
     */
    static Plateau fromSave(const bool variantes[5], std::vector<Tuile> tuiles);
};
#endif
