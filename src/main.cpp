// main.cpp — scénario de test complet
#include <iostream>

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

    while (partie.getChantier().getTaille() > 1)
    {
        std::cout << "\n--- Nouveau tour de jeu ---\n";
        Joueur &joueurCourant = partie.getJoueurMain();
        if (joueurCourant.getNom().empty())
        {
            std::string nomJoueur;
            std::cout << "Entrez le nom du joueur " << (partie.getMainJoueur() + 1) << ": ";
            std::cin >> nomJoueur;
            joueurCourant.setNom(nomJoueur);
        }
        std::cout << "Joueur courant: " << joueurCourant.getNom() << "\n";

        // Affichage du plateau du joueur
        joueurCourant.getPlateau().afficher();

        // Affichage du chantier
        partie.getChantier().afficher();

        // Choix de la tuile à piocher
        int idTuile = -1;
        while (partie.getChantier().getTaille() <= idTuile || idTuile < 0)
        {
            std::cout << "Entrez l'ID de la tuile à piocher : ";
            std::cin >> idTuile;

            if (idTuile > joueurCourant.getNbrPierres())
            {
                std::cout << "Vous n'avez pas assez de pierres pour piocher cette tuile.\n";
                idTuile = -1; // Réinitialiser pour redemander
            }
        }

        Tuile *tuilePiochee = joueurCourant.piocherTuile(idTuile, partie.getChantier());
        if (tuilePiochee)
        {
            std::cout << "Tuile piochée :\n";
            tuilePiochee->afficher();
        }
        else
        {
            std::cout << "Erreur lors de la pioche de la tuile.\n";
        }

        // Passage au joueur suivant
        partie.setProchainJoueur();
    }

    return 0;
}
