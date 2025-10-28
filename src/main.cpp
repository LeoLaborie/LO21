// main.cpp — scénario de test complet
#include <iostream>
#include <vector>
#include <tuple>
#include <optional>
#include <cstdlib>
#include <ctime>

#include "Partie.h"
#include "Joueur.h"
#include "Plateau.h"
#include "Chantier.h"
#include "Tuile.h"
#include "Hexagone.h"

int main()
{
    std::cout << "===== DEMARRAGE PARTIE =====\n";

    Partie partie;

    std::cout << "Nombre de joueurs ? ";
    int nbrJoueurs;
    std::cin >> nbrJoueurs;
    partie.setNbrJoueurs(nbrJoueurs);

    // Préparation des joueurs
    std::cout << partie.getNbrJoueurs() << " joueurs dans la partie.\n";
    partie.setMaitreArchitecte(0);

    // Préparation du paquet de tuiles
    //  --> Génération de toutes les tuiles de la partie
    partie.genererTuilesParties();

    // Sortie d'une pile
    // std::vector<Tuile> paquet = partie.retirerPaquet();

    // Ajout des tuiles au chantier
    // for (Tuile tuile : paquet){
    //     partie.getChantier().ajouterTuile(tuile);
    // }

    partie.getChantier().afficher();

    while (partie.getChantier().getTaille() > 1) {
        std::cout << "\n--- Nouveau tour de jeu ---\n";
        // std::cout << "Joueur courant: " << partie.getJoueurMain().getNom() << "\n";

        // Affichage du plateau du joueur
        partie.getJoueurMain().getPlateau().afficher();

        // Affichage du chantier
        partie.getChantier().afficher();

        // Ici, on pourrait ajouter la logique pour que le joueur choisisse une tuile,
        // la place sur son plateau, etc.

        // Passage au joueur suivant
        partie.setProchainJoueur();
    }

    return 0;
}
