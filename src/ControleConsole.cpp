#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "ControleConsole.h"
#include "Position.h"
#include "couleurs_console.h"
#include "Sauvegarde.h"

Partie CreerNouvellePartie()
{
    std::cout << "===== DEMARRAGE PARTIE =====\n";
    texte_reset();

    int nbrJoueurs;
    std::cout << "Nombre de joueurs ? ";
    while (!(std::cin >> nbrJoueurs) || nbrJoueurs <= 0)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        texte_couleur(ROUGE);
        texte_gras_on();
        std::cout << "Entrée invalide. Veuillez saisir un nombre de joueurs positif : ";
        texte_reset();
    }

    std::cout << nbrJoueurs << " joueurs dans la partie.\n\n";

    int choixVarianteTuiles = 0;
    std::cout << "Choisissez une variante de jeu : \n";
    while (choixVarianteTuiles != 1 && choixVarianteTuiles != 2)
    {
        std::cout << " 1: Jouer avec les règles de base et n'avoir que 11 piles.\n";
        std::cout << " 2: Jouer avec une variante et utiliser toutes les tuiles disponibles pour avoir le maximum de piles.\n";
        std::cout << "Votre choix : ";

        if (!(std::cin >> choixVarianteTuiles))
        {
            // saisie non entière (ex: 't')
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choixVarianteTuiles = 0; // force à rester dans la boucle
        }

        if (choixVarianteTuiles != 1 && choixVarianteTuiles != 2)
        {
            texte_couleur(ROUGE);
            texte_gras_on();
            std::cout << "Choix invalide.\n\n";
            texte_reset();
        }
    }
    const bool utiliserToutesLesTuiles = (choixVarianteTuiles == 2);

    bool variantesScore[5] = {};
    const char *nomsVariantes[5] = {"habitation", "marché", "caserne", "temple", "jardin"};
    for (int i = 0; i < 5; ++i)
    {
        int choixVarianteScore = 0;
        while (choixVarianteScore != 1 && choixVarianteScore != 2)
        {
            std::cout << "Variante pour les " << nomsVariantes[i] << " :\n";
            std::cout << " 1: Jouer avec les règles de score classiques.\n";
            std::cout << " 2: Jouer avec la variante de score.\n";
            std::cout << "Votre choix : ";

            if (!(std::cin >> choixVarianteScore))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                choixVarianteScore = 0;
            }

            if (choixVarianteScore != 1 && choixVarianteScore != 2)
            {
                texte_couleur(ROUGE);
                texte_gras_on();
                std::cout << "Choix invalide.\n\n";
                texte_reset();
            }
        }
        variantesScore[i] = (choixVarianteScore == 2);
    }

    system("clear");
    std::vector<std::string> listePseudo;
    for (int i = 0; i < nbrJoueurs; i++)
    {
        std::string nomJoueur;
        std::cout << "Nom du joueur " << i + 1 << " : ";
        std::cin >> nomJoueur;
        system("clear");
        listePseudo.push_back(nomJoueur);
    }

    return Partie(nbrJoueurs, listePseudo, variantesScore, utiliserToutesLesTuiles);
}

void controleConsole()
{
    texte_reset();
    afficher_curseur();
    texte_couleur(ROUGE);
    texte_gras_on();
    Partie partie;
    while (true)
    {
        int choix;
        int Choixsauvegarde = -1;
        int indiceSauvegarde = 0;
        std::cout << " 1: Jouer une nouvelle partie.\n";
        std::cout << " 2: charger une partie.\n";
        std::cout << "0: quiter le akropolis.\n";
        std::cout << "Votre choix : ";
        std::cin >> choix;
        std::cin.clear();
        switch (choix)
        {
        case 1:
            partie = CreerNouvellePartie();
            lancerPartie(partie);
            break;
        case 2:
        {
            const auto sauvegardes = getSauvegardes();
            if (sauvegardes.empty())
            {
                texte_couleur(ROUGE);
                texte_gras_on();
                std::cout << "\nAucune sauvegarde disponible.\n\n";
                texte_reset();
                break;
            }

            std::cout << "Listes des sauvegardes :" << std::endl;
            for (const auto &fichier : sauvegardes)
            {
                std::cout << "[" << indiceSauvegarde << "]" << fichier << std::endl;
                indiceSauvegarde++;
            }
            while (Choixsauvegarde < 0 || Choixsauvegarde >= indiceSauvegarde)
            {
                std::cout << " Choisissez le numéro de la sauvegarde: ";
                std::cin >> Choixsauvegarde;
                std::cin.clear();
                if (Choixsauvegarde < 0 || Choixsauvegarde >= indiceSauvegarde)
                {
                    std::cout << "\n choix invalide \n";
                }
            }
            try
            {
                partie = Partie::FromSave("saves/" + sauvegardes[Choixsauvegarde]);
                texte_reset();
                lancerPartie(partie);
                
            }
            catch (const std::exception &e)
            {
                texte_couleur(ROUGE);
                texte_gras_on();
                std::cout << "\nErreur lors du chargement de la sauvegarde : " << e.what() << "\n\n";
                texte_reset();
            }
            break;
        }
        case 0:
            return;
        default:
            break;
        }
    }
}

void lancerPartie(Partie& partie){
    while (partie.pilesRestantes() || partie.getChantier().getTaille() > 1)
    {
        std::cout << "--- Nouveau tour de jeu : Tour " << partie.getNbrTours() + 1 << " ---\n";
        std::cout << "Il reste " << partie.getNbrPiles() << " piles de tuiles.\n\n";

        // Tant qu'il reste plus d'une tuile dans le chantier
        while (partie.getChantier().getTaille() > 1)
        {
            Joueur &joueurCourant = partie.getJoueurMain();
            std::cout << "Au tour de :\n";

            // Affichage du plateau du joueur
            std::cout << joueurCourant;

            // Affichage du chantier
            std::cout << partie.getChantier();

            Tuile tuilePiochee;
            const bool tuileDejaEnMain = joueurCourant.getTuileEnMain().getNbHexa() > 0;
            if (!tuileDejaEnMain)
            {
                // Choix de la tuile à piocher
                int idTuile = -1;
                idTuile = joueurCourant.choixTuile(partie.getChantier());

                // Pioche de la tuile
                bool piocheReussie = false;
                while (!piocheReussie)
                {
                    try
                    {
                        tuilePiochee = joueurCourant.piocherTuile(idTuile, partie.getChantier(), partie.getFauxJoueur());
                        std::cout << "\nTuile piochée :\n\n";
                        std::cout << tuilePiochee; // operator<< respecte les offsets
                        piocheReussie = true;
                    }
                    catch (const std::exception &e)
                    {
                        texte_couleur(ROUGE);
                        texte_gras_on();
                        std::cout << e.what() << "\n";
                        texte_reset();
                    }
                }
            }
            else
            {
                tuilePiochee = joueurCourant.getTuileEnMain();
                std::cout << "\nTuile déjà en main. Accès direct aux actions de placement.\n";
            }

            bool phaseRotation = true;
            while (phaseRotation)
            {
                joueurCourant.getPlateau().afficherPositionsLegales(joueurCourant.getTuileEnMain());
                std::cout << "\nActions : [o] pivoter +60°  |  [p] placer  |  [a] afficher tuile [a] | sauvegarder [s] | quitter [q]: ";
                char rep;
                std::cin >> rep;
                switch (rep)
                {
                case 'o':
                case 'O':
                {
                    tuilePiochee.pivoterTuile();
                    std::cout << "\nTuile après pivot :\n\n";
                    std::cout << tuilePiochee;
                    break;
                }
                case 'a':
                case 'A':
                {
                    std::cout << "\nAperçu tuile :\n\n";
                    std::cout << tuilePiochee;
                    break;
                }
                case 'p':
                case 'P':
                {
                    phaseRotation = false; // on passe à la phase de placement
                    break;
                }
                case 's':
                case 'S':
                {
                    sauvegarderPartie(partie);
                    break;
                }
                case 'q':
                case 'Q':
                {
                    char sauvegarderAvantQuitter;
                    std::cout << "Voulez-vous sauvegarder avant de quitter ? (o/n) : ";
                    std::cin >> sauvegarderAvantQuitter;

                    if (sauvegarderAvantQuitter == 'o' || sauvegarderAvantQuitter == 'O')
                    {
                        try
                        {
                            if (sauvegarderPartie(partie))
                            {
                                texte_couleur(JAUNE);
                                texte_gras_on();
                                std::cout << "\nSauvegarde effectuée avec succès.\n";
                                texte_reset();
                            }
                            else
                            {
                                texte_couleur(ROUGE);
                                texte_gras_on();
                                std::cout << "\nErreur : la sauvegarde a échoué.\n";
                                texte_reset();
                            }
                        }
                        catch (const std::exception &e)
                        {
                            texte_couleur(ROUGE);
                            texte_gras_on();
                            std::cout << "\nException lors de la sauvegarde : " << e.what() << "\n";
                            texte_reset();
                        }

                        // Après sauvegarde → quitter la partie et revenir au menu
                        return;
                    }

                    if (sauvegarderAvantQuitter == 'n' || sauvegarderAvantQuitter == 'N')
                    {
                        // Quitter sans sauvegarder
                        return;
                    }

                    // Si l’utilisateur a tapé autre chose
                    texte_couleur(ROUGE);
                    texte_gras_on();
                    std::cout << "\nRéponse invalide, retour au jeu.\n";
                    texte_reset();
                    break;
                }
                default:
                    texte_reset();
                    std::cout << "\n Choix invalide veuillez réessayer" << std::endl;
                    break;
                }
            }

            // Choix de la position pour placer la tuile
            int x, y, z;
            bool placementTuile = false;

            while (!placementTuile)
            {
                std::cout << "\nEntrez les coordonnées (x y z) pour placer la tuile : ";
                if (!(std::cin >> x >> y >> z))
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    texte_couleur(ROUGE);
                    texte_gras_on();
                    std::cout << "Coordonnées invalides. Veuillez entrer trois entiers.\n";
                    texte_reset();
                    continue;
                }

                Position pos{x, y, z};

                try
                {
                    joueurCourant.placerTuile(tuilePiochee, pos);
                    texte_couleur(JAUNE);
                    texte_gras_on();
                    std::cout << "\n Tuile placée avec succès.\n";
                    texte_reset();
                    placementTuile = true;
                }

                catch (const std::invalid_argument &e)
                {
                    texte_couleur(ROUGE);
                    texte_gras_on();
                    std::cout << e.what() << "\n";
                    texte_reset();
                }
            }

            system("sleep 2");

            if (partie.fauxJoueurPresent())
            {
                IllustreArchitecte *ia = partie.getFauxJoueur();

                // Affichage du plateau du joueur
                std::cout << &ia;

                // Affichage du chantier
                std::cout << partie.getChantier();

                // Choix de la tuile à piocher
                int idTuile = ia->choixTuile(partie.getChantier());

                Tuile tuilePiochee;

                try
                {
                    tuilePiochee = ia->piocherTuile(idTuile, partie.getChantier());
                    std::cout << "\nTuile piochée :\n\n";
                    std::cout << tuilePiochee;
                }
                catch (const std::exception &e)
                {
                    texte_couleur(ROUGE);
                    texte_gras_on();
                    std::cout << e.what() << "\n";
                    texte_reset();
                }

                ia->placerTuile(tuilePiochee);
            }

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
            partie.addTuileInChantierFromPiles();
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
}