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
    //getters
    const std::string &getNom() const { return nom; }
    int getNbrPierres() const;
    Plateau &getPlateau() { return plateau; }
    const Plateau &getPlateau() const { return plateau; }
    const Tuile &getTuileEnMain() const { return tuileEnMain; }
    int getNbrPoints() const;

    //setters
    void setNbrPierres(int);
    void setNbrPoints();
    void setTuileEnMain(const Tuile &t) { tuileEnMain = t; }

    //autres méthodes
    Tuile *piocherTuile(int id, Chantier &chantier);
    bool placerTuile(Tuile &t, Position &p);
    
    //surchages
    friend std::ostream& operator<<(std::ostream &os,const Joueur& j);
    
};

#endif
