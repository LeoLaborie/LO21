#ifndef JOUEUR_H
#define JOUEUR_H

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "Chantier.h"
#include "Plateau.h"
#include "Tuile.h"
#include "couleurs_console.h"

class Partie;
class IllustreArchitecte;

/**
 * @class Joueur
 * @brief Classe représentant un joueur humain
 */
class Joueur
{
    friend class Partie;

protected:
    int nbrPierres{};
    int nbrPoints{};
    std::string nom = "";
    Plateau plateau;
    Tuile tuileEnMain;

    /**
     * @brief Constructeur de Joueur
     * @param variantesScore Tableau des variantes de score
     * @param nom Nom du joueur
     */
    explicit Joueur(const bool variantesScore[5], std::string nom);  // le joueur est créé au début de la partie, par la partie (composition)

    /**
     * @brief Constructeur de Joueur
     * @param nom Nom du joueur
     */
    explicit Joueur(std::string nom);

    /**
     * @brief Constructeur utilisé pour la restauration depuis une sauvegarde.
     */
    Joueur(const bool variantes[5], std::string nom, int pierres, int points, Tuile tuileMain, std::vector<Tuile> plateau);

public:
    /**
     * @brief Destructeur de Joueur
     */
    virtual ~Joueur() = default;

    // Getters

    /**
     * @brief Retourne le nom du joueur
     * @return const std::string& : référence constante vers le nom du joueur
     */
    const std::string &getNom() const
    {
        return nom;
    }

    /**
     * @brief Retourne le nombre de pierres du joueur
     * @return int : nombre de pierres du joueur
     */
    int getNbrPierres() const;

    /**
     * @brief Retourne le plateau du joueur
     * @return Plateau& : référence vers le plateau du joueur
     */
    Plateau &getPlateau()
    {
        return plateau;
    }

    /**
     * @brief Retourne le plateau constant du joueur
     * @return const Plateau& : référence constante vers le plateau du joueur
     */
    const Plateau &getPlateau() const
    {
        return plateau;
    }

    /**
     * @brief Retourne la tuile en main du joueur
     * @return const Tuile& : référence constante vers la tuile en main du joueur
     */
    const Tuile &getTuileEnMain() const
    {
        return tuileEnMain;
    }

    /**
     * @brief Retourne le nombre de points du joueur
     * @return int : nombre de points du joueur
     */
    int getNbrPoints() const;

    // Setters

    /**
     * @brief Définit le nombre de pierres du joueur
     * @param n Nombre de pierres
     */
    void setNbrPierres(int);

    /**
     * @brief Définit le nombre de points du joueur
     */
    virtual void setNbrPoints();

    /**
     * @brief Définit la tuile en main du joueur
     * @param t Tuile à définir comme tuile en main
     */
    void setTuileEnMain(const Tuile &t)
    {
        tuileEnMain = t;
    }

    // Autres méthodes

    bool tuileDejaEnMain(){return getTuileEnMain().getNbHexa() > 0;}

    /**
     * @brief Permet au joueur de piocher une tuile dans le chantier
     * @param id Identifiant de la tuile à piocher
     * @param chantier Chantier de la partie
     * @param fauxJoueur Pointeur vers le faux joueur (Illustre Architecte), nullptr si pas utilisé
     * @return Référence vers la tuile piochée
     * @throws std::out_of_range si l'ID est invalide
     * @throws std::invalid_argument si le nombre de pierres est insuffisant
     */
    virtual Tuile &piocherTuile(int id, Chantier &chantier, Joueur *fauxJoueur = nullptr);

    /**
     * @brief Permet au joueur de placer une tuile sur son plateau
     * @param t Tuile à placer
     * @param p Position de placement
     */
    void placerTuile(Tuile &t, Position &p);

    /**
     * @brief Surcharge de l'opérateur d'affichage pour le Joueur
     * @param os Flux de sortie
     * @param j Joueur à afficher
     * @return Flux de sortie modifié
     */
    friend std::ostream &operator<<(std::ostream &os, const Joueur &j);

    /**
     * @brief Permet au joueur de choisir une tuile dans le chantier
     * @param chantier le chantier de la partie
     * @return id de la Tuile choisie par le joueur
     */
    virtual int choixTuile(const Chantier &chantier);

    /** 
     * @brief Permet de savoir si je joueur actuel est un IlllustreArchitecte
     * @return un booléen
    */
    virtual bool isIA() const { return false; }

    /**
     * @brief Reconstruit un joueur depuis les données de sauvegarde.
     * @param variantes Variantes de score actives.
     * @param nom Nom du joueur.
     * @param pierres Nombre de pierres.
     * @param points Nombre de points.
     * @param tuileMain Tuile en main.
     * @param plateau Tuiles présentes sur le plateau.
     */
    static Joueur fromSave(const bool variantes[5], std::string nom, int pierres, int points, Tuile tuileMain, std::vector<Tuile> plateau);
};

/**
 * @brief Classe représentant un joueur IA
 */
class IllustreArchitecte : public Joueur
{
    friend class Partie;
    friend struct std::default_delete<IllustreArchitecte>;

private:
    int difficulte{};  // Si c'est le faux joueur, c'est etre 1 et 3
    ~IllustreArchitecte() = default;

    /**
     * @brief Constructeur de IllustreArchitecte
     * @param diff Difficulté de l'IA (1 à 3)
     */
    explicit IllustreArchitecte(int diff);

public:
    /**
     * @brief Définit le nombre de points du joueur IA
     */
    void setNbrPoints() override;

    using Joueur::piocherTuile;
    /**
     * @brief Permet au joueur IA de piocher une tuile dans le chantier
     * @param id Identifiant de la tuile à piocher
     * @param chantier Chantier de la partie
     * @return Référence vers la tuile piochée
     */
    Tuile &piocherTuile(int id, Chantier &chantier, Joueur *fauxJoueur) override;

    /**
     * @brief Reconstruit un Illustre Architecte depuis une sauvegarde.
     * @param diff Difficulté (1 à 3).
     * @param pierres Nombre de pierres.
     * @param points Nombre de points.
     * @param variantes Variantes de score actives.
     * @param plateau Tuiles du plateau.
     */
    static IllustreArchitecte *fromSave(int diff, int pierres, int points, const bool variantes[5], std::vector<Tuile> plateau);

    /**
     * @brief Permet au joueur IA de choisir une tuile dans le chantier
     * @param chantier le chantier de la partie
     * @return id de la Tuile choisie par le joueur IA
     */
    int choixTuile(const Chantier &chantier) override;

    /**
     * @brief Permet au joueur IA de placer une tuile sur son plateau
     * @param t Tuile à placer
     */
    void placerTuile(Tuile &t);

    /**
     * @brief Retourne la difficulté de l'IA
     * @return int : difficulté de l'IA
     */
    int getdifficulte()
    {
        return difficulte;
    }

    /** 
     * @brief Permet de savoir si je joueur actuel est un IlllustreArchitecte
     * @return un booléen
    */
    bool isIA() const override { return true; } 
};

#endif
