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
    explicit Joueur(bool varianteScore,std::string nom); // le joueur est créé au début de la partie, par la partie (composition)

public:
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
    friend std::ostream& operator<<(std::ostream &os,const Joueur& j);
    
};

#endif
