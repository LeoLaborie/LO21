#ifndef JOUEUR_H
#define JOUEUR_H

#include <vector>
#include <iostream>
#include "Plateau.h"
#include "Tuile.h"
#include "Chantier.h"
#include "couleurs_console.h"

class Partie; 
class IllustreArchitecte;

class Joueur
{
    friend class Partie;

protected:
    int nbrPierres{};
    int nbrPoints{};
    std::string nom = "";
    Plateau plateau;
    Tuile tuileEnMain;
    explicit Joueur(const bool variantesScore[5],std::string nom); // le joueur est créé au début de la partie, par la partie (composition)
    explicit Joueur(std::string nom);

public:
    virtual ~Joueur() = default;

    //getters
    const std::string &getNom() const { return nom; }
    int getNbrPierres() const;
    Plateau &getPlateau() { return plateau; }
    const Plateau &getPlateau() const { return plateau; }
    const Tuile &getTuileEnMain() const { return tuileEnMain; }
    int getNbrPoints() const;

    //setters
    void setNbrPierres(int);
    virtual void setNbrPoints();
    void setTuileEnMain(const Tuile &t) { tuileEnMain = t; }

    //autres méthodes
    virtual Tuile *piocherTuile(int id, Chantier &chantier, IllustreArchitecte* fauxJoueur);
    bool placerTuile(Tuile &t, Position &p);
    
    //surchages
    friend std::ostream& operator<<(std::ostream &os,const Joueur& j);
    
    virtual int choixTuile(const Chantier& chantier);
};



class IllustreArchitecte: public Joueur{
    friend class Partie;
private:
    int difficulte{}; // Si c'est le faux joueur, c'est etre 1 et 3
    ~IllustreArchitecte()= default;
    explicit IllustreArchitecte(int diff);

public:
    void setNbrPoints() override;
    using Joueur::piocherTuile;
    Tuile *piocherTuile(int id, Chantier &chantier);
    int choixTuile(const Chantier& chantier)override;
    bool placerTuile(Tuile &t);

    int getdifficulte(){return difficulte;}

};

#endif
