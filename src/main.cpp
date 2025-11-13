#include <iostream>
#include "Partie.h"
#include "Position.h"
#include "couleurs_console.h"

int main()
{
    texte_reset();
    afficher_curseur();
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
    std::cout << partie.getNbrJoueurs() << " joueurs dans la partie.\n\n";
    partie.setMaitreArchitecte(0);

    int choixVariante = 0;
    std::cout << "Choisissez une variante de jeu : \n";
    while (choixVariante != 1 && choixVariante != 2){
        std::cout << " 1: Jouer avec les règles de base et n'avoir que 11 piles.\n";
        std::cout << " 2: Jouer avec une variante et utiliser toutes les tuiles disponibles pour avoir le maximum de piles.\n";
        std::cout << "Votre choix : ";
        std::cin >> choixVariante;
        if (choixVariante != 1 && choixVariante != 2){
            texte_couleur(ROUGE);
            texte_gras_on();
            std::cout << "Choix invalide.\n\n";
            texte_reset();
        }
    }

    // Préparation du paquet de tuiles
    partie.genererTuilesParties(choixVariante == 2);

    while (partie.pilesRestantes() || partie.getChantier().getTaille() > 1)
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
                    idTuile = -1;
                }
                else if (idTuile > joueurCourant.getNbrPierres())
                {
                    texte_couleur(ROUGE);
                    texte_gras_on();
                    std::cout << "Vous n'avez pas assez de pierres pour piocher cette tuile.\n";
                    texte_reset();
                    idTuile = -1;
                }
            }

            // Pioche de la tuile
            Tuile *tuilePiochee = joueurCourant.piocherTuile(idTuile, partie.getChantier());
            if (tuilePiochee)
            {
                std::cout << "\nTuile piochée :\n\n";
                std::cout << *tuilePiochee; // operator<< respecte les offsets
            }
            else
            {
                std::cout << "Erreur lors de la pioche de la tuile.\n";
            }

            // --- On peut pivoter autant de fois qu'on veut AVANT de placer ---
            if (tuilePiochee)
            {
                bool phaseRotation = true;
                while (phaseRotation)
                {
                    joueurCourant.getPlateau().afficherPositionsLegales(joueurCourant.getTuileEnMain());
                    std::cout << "\nActions : [o] pivoter +60°  |  [p] placer  |  [a] afficher tuile : ";
                    char rep;
                    std::cin >> rep;
                    if (rep == 'o' || rep == 'O')
                    {
                        tuilePiochee->pivoterTuile();
                        std::cout << "\nTuile après pivot :\n\n";
                        std::cout << *tuilePiochee;
                    }
                    else if (rep == 'a' || rep == 'A')
                    {
                        std::cout << "\nAperçu tuile :\n\n";
                        std::cout << *tuilePiochee;
                    }
                    else if (rep == 'p' || rep == 'P')
                    {
                        phaseRotation = false; // on passe à la phase de placement
                    }
                }
            }

            // Choix de la position pour placer la tuile 
            int x, y, z;
            bool placementTuile = false;

            while (!placementTuile)
            {
                std::cout << "\nEntrez les coordonnées (x y z) pour placer la tuile : ";
                std::cin >> x >> y >> z;
                Position pos{x, y, z};

                // Vérification avant tentative de placement
                if (!joueurCourant.getPlateau().verifierPlacementTuile(pos, *tuilePiochee))
                {
                    texte_couleur(ROUGE);
                    texte_gras_on();
                    std::cout << "Placement invalide : au sol il faut toucher un bord ; en hauteur, chaque hex doit être supporté (sur au moins 2 tuiles différentes).\n";
                    texte_reset();
                    continue;
                }

                placementTuile = joueurCourant.placerTuile(*tuilePiochee, pos);

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

        if (partie.pilesRestantes())
        {
            // Sortie d'une pile
            std::vector<Tuile> paquet = partie.retirerPaquet();

            // Ajout des tuiles au chantier
            for (Tuile tuile : paquet)
            {
                partie.getChantier().ajouterTuile(tuile);
            }
        }

        texte_reset();
        system("sleep 4");
        system("clear");
        afficher_curseur();
    }

    texte_couleur(ROUGE);
    texte_gras_on();
    texte_clignote_on();
    std::cout << "===== FIN DE PARTIE =====\n";

    texte_reset();

    texte_couleur(JAUNE);
    std::cout << "\n Scores finaux \n";
    std::cout << "---------------\n";
    for (int _i = 0; _i < partie.getNbrJoueurs(); ++_i)
    {
        Joueur &joueur = partie.getJoueurMain();
        texte_gras_on();
        std::cout << " " << joueur.getNom();
        texte_reset();
        texte_couleur(JAUNE);
        std::cout << " : " << joueur.getNbrPoints() << " points\n";
        partie.setProchainJoueur();
    }

    texte_reset();
    std::cout << std::endl;

    return 0;
}
