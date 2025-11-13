#ifndef JOUEUR_H
#define JOUEUR_H

#include <vector>
#include <iostream>
#include "Plateau.h"
#include "Tuile.h"
#include "Chantier.h"
#include "couleurs_console.h"

class Partie; // déclaration anticipée pour l'amitié

class Joueur
{
    friend class Partie;

private:
    int nbrPierres{};
    int nbrPoints{};
    std::string nom = "";
    Plateau plateau;
    Tuile tuileEnMain;
    explicit Joueur(); // le joueur est créé au début de la partie, par la partie (composition)

public:
    void setNom(const std::string &n) { nom = n; }
    const std::string &getNom() const { return nom; }
    void setNbrPierres(int);
    int getNbrPierres();
    void setNbrPoints();
    Plateau &getPlateau() { return plateau; }
    const Tuile &getTuileEnMain() const { return tuileEnMain; }
    int getNbrPoints();
    void setTuileEnMain(const Tuile &t) { tuileEnMain = t; }
    Tuile *piocherTuile(int id, Chantier &chantier);
    bool placerTuile(Tuile &t, Position &p);
    void afficher() const
    {
        std::cout << " ";
        texte_couleur(ROUGE);
        texte_gras_on();
        std::cout << nom;
        texte_reset();
        std::cout << " | Pierres : ";
        texte_couleur(BLEU);
        std::cout << nbrPierres;
        texte_reset();
        std::cout << ", Points : ";
        texte_couleur(JAUNE);
        std::cout << nbrPoints;
        texte_reset();
        std::cout << "\n"<<plateau<<std::endl;
    }
};

#endif
