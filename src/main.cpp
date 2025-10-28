// main.cpp — scénario de test complet
#include <iostream>
#include "Partie.h"
#include "Position.h"
#include "couleurs_console.h"

int main()
{
    texte_couleur(ROUGE);
    texte_gras_on();
    std::cout << "===== DEMARRAGE PARTIE =====\n";
    texte_reset();

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

    while (partie.pilesRestantes())
    {
        std::cout << "--- Nouveau tour de jeu : Tour " << partie.getNbrTours() + 1 << " ---\n";
        std::cout << "Il reste " << partie.getNbrPiles() << " piles de tuiles.\n\n";
        // Tant qu'il reste plus d'une tuile dans le chantier
        while (partie.getChantier().getTaille() > 1)
        {
            Joueur &joueurCourant = partie.getJoueurMain();
            if (joueurCourant.getNom().empty())
            {
                std::string nomJoueur;
                std::cout << "Nom du joueur " << (partie.getMainJoueur() + 1) << " : ";
                std::cin >> nomJoueur;
                joueurCourant.setNom(nomJoueur);
                system("clear");
            }
            std::cout << "Au tour de :\n";

            // Affichage du plateau du joueur
            joueurCourant.afficher();

            // Affichage du chantier
            partie.getChantier().afficher();

            // Choix de la tuile à piocher
            int idTuile = -1;
            while (partie.getChantier().getTaille() <= idTuile || idTuile < 0)
            {
                std::cout << "Entrez l'ID de la tuile à piocher : ";
                std::cin >> idTuile;

                if (idTuile < 0 || idTuile >= partie.getChantier().getTaille())
                {
                    texte_couleur(ROUGE);
                    texte_gras_on();
                    std::cout << "ID invalide. Veuillez réessayer.\n";
                    texte_reset();
                    idTuile = -1; // Réinitialiser pour redemander
                }
                else if (idTuile > joueurCourant.getNbrPierres())
                {
                    texte_couleur(ROUGE);
                    texte_gras_on();
                    std::cout << "Vous n'avez pas assez de pierres pour piocher cette tuile.\n";
                    texte_reset();
                    idTuile = -1; // Réinitialiser pour redemander
                }
            }

            // Pioche de la tuile
            Tuile *tuilePiochee = joueurCourant.piocherTuile(idTuile, partie.getChantier());
            if (tuilePiochee)
            {
                std::cout << "\nTuile piochée :\n";
                tuilePiochee->afficher();
            }
            else
            {
                std::cout << "Erreur lors de la pioche de la tuile.\n";
            }

            // Choix de la position pour placer la tuile
            int x, y, z;
            bool placementTuile = false;

            while (!placementTuile)
            {
                std::cout << "Entrez les coordonnées (x y z) pour placer la tuile : ";
                std::cin >> x >> y >> z;
                Position pos{x, y, z};
                placementTuile = joueurCourant.placerTuile(*tuilePiochee, pos);
            }

            if (placementTuile)
            {
                texte_couleur(JAUNE);
                texte_gras_on();
                std::cout << "\n Tuile placée avec succès.\n";
                texte_reset();
            }
            else
            {
                texte_couleur(ROUGE);
                texte_gras_on();
                std::cout << "\n La tuile n'a pas été placée.\n";
                texte_reset();
            }

            system("sleep 2");

            // Passage au joueur suivant
            partie.setProchainJoueur();
            system("clear");
        }

        std::cout << "\n ";

        texte_couleur(JAUNE);
        texte_gras_on();
        texte_souligne_on();
        cacher_curseur();

        std::cout << "Il reste une seule tuile dans le chantier. Le tour est terminé.\n";
        partie.tourTermine();

        // Sortie d'une pile
        std::vector<Tuile> paquet = partie.retirerPaquet();

        // Ajout des tuiles au chantier
        for (Tuile tuile : paquet){
            partie.getChantier().ajouterTuile(tuile);
        }

        afficher_curseur();
        texte_reset();
        system("sleep 4");
        system("clear");
    }

    texte_couleur(ROUGE);
    texte_gras_on();
    std::cout << "===== FIN DE PARTIE =====\n";

    texte_reset();
    return 0;
}
