#ifndef PARTIE_H
#define PARTIE_H

#include <vector>
#include <math.h>
#include <stdexcept>
#include <random>
#include <map>
#include <string>
#include "Tuile.h"
#include "Chantier.h"
#include "Joueur.h"

class Partie
{
private:
    int nbrJoueurs = 0;
    int maitreArchitecte = 0; // indice du joueur maître architecte
    int mainJoueur = 0;       // indice du joueur dont c’est le tour
    int nbrTours = 0;
    int taillepaquet = 0;

    IllustreArchitecte* fauxJoueur;

    Chantier chantier;
    std::vector<Joueur> joueurs;
    std::vector<std::vector<Tuile>> piles;


public:
    //constructeur et destructeur
    Partie(int nbJouer,std::vector<std::string>& pseudo,const bool variantesScore[5],bool varianteFullTuile);
    ~Partie() = default;


    //getters
    int getNbrJoueurs() const { return nbrJoueurs; }
    int getMaitreArchitecte() const { return maitreArchitecte; }
    Joueur &getJoueurMain() { return joueurs.at(mainJoueur); }
    int getMainJoueur() const { return mainJoueur; }
    int getTaillePaquet() const { return taillepaquet; }
    Chantier& getChantier() { return chantier; }
    const Chantier& getChantier() const { return chantier; }
    const std::vector<Joueur>& getJoueurs() const { return joueurs; }
    const std::vector<std::vector<Tuile>>& getPile() const {return piles;};
    int getNbrTours() const { return nbrTours; }
    int getNbrPiles() const { return piles.size(); }

    //setters
    void setMaitreArchitecte(int i) { maitreArchitecte = i % (nbrJoueurs > 0 ? nbrJoueurs : 1); }    
    void setProchainJoueur()
    {
        if (nbrJoueurs)
            mainJoueur = (mainJoueur + 1) % nbrJoueurs;
    }

    
    
    
    //autres méthodes
    std::vector<Tuile> retirerPaquet();
    bool pilesRestantes() const { return !piles.empty(); }   
    void genererTuilesParties(bool fullTuiles = false);
    void addTuileInChantierFromPiles();
    void tourTermine();

    bool fauxJoueurPresent()const {return fauxJoueur != nullptr;}
    void creerFauxJoueur();
    IllustreArchitecte* getFauxJoueur()const {return fauxJoueur;}
};

#endif
