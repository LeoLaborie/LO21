#include "JeuController.h"

#include <iostream>
#include <limits>

#include "Position.h"
#include "Sauvegarde.h"
#include "couleurs_console.h"

namespace
{
    void afficherMessageErreurSaisie(const std::string &message)
    {
        texte_couleur(ROUGE);
        texte_gras_on();
        std::cout << message << '\n';
        texte_reset();
    }
}  // namespace

void JeuController::executerConsole()
{
    bool quitter = false;
    while (!quitter)
    {
        texte_reset();
        afficher_curseur();
        texte_couleur(ROUGE);
        texte_gras_on();

        std::cout << "=== Menu principal ===\n";
        std::cout << " 1: Jouer une nouvelle partie\n";
        std::cout << " 2: Charger une sauvegarde\n";
        std::cout << " 0: Quitter\n";
        std::cout << "Votre choix : ";

        int choix = -1;
        if (!(std::cin >> choix))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choix)
        {
            case 1:
            {
                Partie partie = creerNouvellePartie();
                boucleDeJeu(partie);
                break;
            }
            case 2:
            {
                Partie partie;
                if (chargerDepuisSauvegarde(partie))
                {
                    boucleDeJeu(partie);
                }
                break;
            }
            case 0:
                quitter = true;
                break;
            default:
                afficherMessageErreurSaisie("Choix invalide, veuillez réessayer.");
                break;
        }
    }
}

Partie JeuController::creerNouvellePartie()
{
    std::cout << "===== CREATION PARTIE =====\n";
    texte_reset();

    int nbrJoueurs = 0;
    while (nbrJoueurs <= 0)
    {
        std::cout << "Nombre de joueurs ? ";
        if ((std::cin >> nbrJoueurs) && nbrJoueurs > 0)
            break;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        afficherMessageErreurSaisie("Veuillez saisir un nombre strictement positif.");
    }
    std::cout << nbrJoueurs << " joueurs dans la partie.\n\n";

    int choixVarianteTuiles = 0;
    while (choixVarianteTuiles != 1 && choixVarianteTuiles != 2)
    {
        std::cout << "Variante des tuiles :\n";
        std::cout << " 1: Règles de base (11 piles)\n";
        std::cout << " 2: Variante (toutes les tuiles)\n";
        std::cout << "Votre choix : ";
        if (!(std::cin >> choixVarianteTuiles))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choixVarianteTuiles = 0;
        }
        if (choixVarianteTuiles != 1 && choixVarianteTuiles != 2)
            afficherMessageErreurSaisie("Choix invalide.");
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
            std::cout << " 1: Règles classiques\n";
            std::cout << " 2: Variante de score\n";
            std::cout << "Votre choix : ";
            if (!(std::cin >> choixVarianteScore))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                choixVarianteScore = 0;
            }
            if (choixVarianteScore != 1 && choixVarianteScore != 2)
                afficherMessageErreurSaisie("Choix invalide.");
        }
        variantesScore[i] = (choixVarianteScore == 2);
    }

    system("clear");
    std::vector<std::string> pseudos;
    pseudos.reserve(nbrJoueurs);
    for (int i = 0; i < nbrJoueurs; ++i)
    {
        std::string nom;
        std::cout << "Nom du joueur " << i + 1 << " : ";
        std::cin >> nom;
        system("clear");
        pseudos.push_back(nom);
    }

    return Partie(nbrJoueurs, pseudos, variantesScore, utiliserToutesLesTuiles);
}

bool JeuController::chargerDepuisSauvegarde(Partie &partie)
{
    const auto sauvegardes = getSauvegardes();
    if (sauvegardes.empty())
    {
        afficherMessageErreurSaisie("Aucune sauvegarde disponible.");
        return false;
    }

    std::cout << "Liste des sauvegardes :\n";
    for (size_t i = 0; i < sauvegardes.size(); ++i)
        std::cout << " [" << i << "] " << sauvegardes[i] << '\n';

    int choix = -1;
    while (choix < 0 || choix >= static_cast<int>(sauvegardes.size()))
    {
        std::cout << "Numéro de sauvegarde : ";
        if (!(std::cin >> choix))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choix = -1;
        }
        if (choix < 0 || choix >= static_cast<int>(sauvegardes.size()))
            afficherMessageErreurSaisie("Choix invalide.");
    }

    partie = Partie::FromSave("saves/" + sauvegardes[choix]);
    return true;
}

bool JeuController::boucleDeJeu(Partie &partie)
{
    while (partie.pilesRestantes() || partie.getChantier().getTaille() > 1)
    {
        std::cout << "--- Nouveau tour : " << partie.getNbrTours() + 1 << " ---\n";
        std::cout << "Il reste " << partie.getNbrPiles() << " piles de tuiles.\n\n";

        while (partie.getChantier().getTaille() > 1)
        {
            traiterTourHumain(partie);
            if (partie.fauxJoueurPresent())
                traiterTourIA(partie);
            partie.setProchainJoueur();
            system("clear");
        }

        std::cout << "\n";
        texte_couleur(JAUNE);
        texte_gras_on();
        texte_souligne_on();
        cacher_curseur();
        std::cout << "Il reste une seule tuile dans le chantier. Fin du tour.\n";
        texte_reset();

        partie.tourTermine();
        if (partie.pilesRestantes())
            partie.addTuileInChantierFromPiles();

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
    std::cout << "\nScores finaux\n";
    std::cout << "-------------\n";
    for (int i = 0; i < partie.getNbrJoueurs(); ++i)
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
    std::cout << '\n';
    return true;
}

void JeuController::traiterTourHumain(Partie &partie)
{
    Joueur &joueur = partie.getJoueurMain();
    std::cout << "Tour de :\n"
              << joueur;
    std::cout << partie.getChantier();

    Tuile tuilePiochee;
    const bool tuileDejaEnMain = joueur.getTuileEnMain().getNbHexa() > 0;
    if (!tuileDejaEnMain)
    {
        int idTuile = joueur.choixTuile(partie.getChantier());
        bool piocheOK = false;
        while (!piocheOK)
        {
            try
            {
                tuilePiochee = joueur.piocherTuile(idTuile, partie.getChantier(), partie.getFauxJoueur());
                std::cout << "\nTuile piochée :\n\n"
                          << tuilePiochee;
                piocheOK = true;
            }
            catch (const std::exception &e)
            {
                afficherMessageErreurSaisie(e.what());
            }
        }
    }
    else
    {
        tuilePiochee = joueur.getTuileEnMain();
        std::cout << "\nTuile déjà en main.\n";
    }

    if (!phaseManipulationTuile(joueur, partie, tuilePiochee))
        return;

    phasePlacementTuile(joueur);
}

void JeuController::traiterTourIA(Partie &partie)
{
    IllustreArchitecte *ia = partie.getFauxJoueur();
    if (!ia)
        return;

    std::cout << ia;
    std::cout << partie.getChantier();

    int idTuile = ia->choixTuile(partie.getChantier());
    Tuile tuilePiochee;
    try
    {
        tuilePiochee = ia->piocherTuile(idTuile, partie.getChantier());
        std::cout << "\nTuile piochée par l'IA :\n\n"
                  << tuilePiochee;
    }
    catch (const std::exception &e)
    {
        afficherMessageErreurSaisie(e.what());
    }

    ia->placerTuile(tuilePiochee);
}

bool JeuController::phaseManipulationTuile(Joueur &joueur, Partie &partie, Tuile &tuilePiochee)
{
    bool phaseRotation = true;
    while (phaseRotation)
    {
        joueur.getPlateau().afficherPositionsLegales(joueur.getTuileEnMain());
        std::cout << "\nActions : [o] pivoter  |  [p] placer  |  [a] afficher  |  [s] sauvegarder  |  [q] quitter : ";
        char rep;
        std::cin >> rep;
        switch (rep)
        {
            case 'o':
            case 'O':
                tuilePiochee.pivoterTuile();
                std::cout << "\nTuile après pivot :\n\n"
                          << tuilePiochee;
                break;
            case 'a':
            case 'A':
                std::cout << "\nAperçu tuile :\n\n"
                          << tuilePiochee;
                break;
            case 'p':
            case 'P':
                phaseRotation = false;
                break;
            case 's':
            case 'S':
                sauvegarderPartie(partie);
                break;
            case 'q':
            case 'Q':
                if (confirmerQuitter(partie))
                    return false;
                break;
            default:
                afficherMessageErreurSaisie("Choix invalide.");
                break;
        }
    }
    return true;
}

void JeuController::phasePlacementTuile(Joueur &joueur)
{
    bool placementOK = false;
    Tuile tuile = joueur.getTuileEnMain();
    while (!placementOK)
    {
        try
        {
            Position pos = demanderPosition();
            joueur.placerTuile(tuile, pos);
            texte_couleur(JAUNE);
            texte_gras_on();
            std::cout << "\nTuile placée avec succès.\n";
            texte_reset();
            placementOK = true;
        }
        catch (const std::invalid_argument &e)
        {
            afficherMessageErreurSaisie(e.what());
        }
    }
    system("sleep 2");
}

Position JeuController::demanderPosition() const
{
    int x, y, z;
    while (true)
    {
        std::cout << "\nCoordonnées (x y z) : ";
        if (std::cin >> x >> y >> z)
            return Position{x, y, z};

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        afficherMessageErreurSaisie("Coordonnées invalides. Entrez trois entiers.");
    }
}

bool JeuController::confirmerQuitter(Partie &partie)
{
    char choix;
    std::cout << "Voulez-vous sauvegarder avant de quitter ? (o/n) : ";
    std::cin >> choix;
    if (choix == 'o' || choix == 'O')
        sauvegarderPartie(partie);
    return true;
}
