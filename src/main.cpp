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

using std::cout;
using std::endl;

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    cout << "===== DEMARRAGE PARTIE =====\n";

    // --- Initialisation de la partie et des joueurs
    Partie partie;                 // si ton type n’a pas de ctor par défaut, remplace par Partie partie(nbrJoueurs);
    partie.setNbrJoueurs(2);       // adapte si ton constructeur prend déjà ce paramètre
    partie.setMaitreArchitecte(0); // joueur 0 maître architecte au départ (si différent, adapte)

    // On récupère le joueur courant pour vérifier que tout est bien initialisé
    Joueur &j0 = partie.getJoueurMain();
    j0.setNbrPierres(2); // donne un peu de marge pour piocher des tuiles non gratuites
    j0.setNbrPoints();   // initialise le score

    // --- Générer un paquet initial et construire le chantier
    const int TAILLE_CHANTIER = 5;
    std::vector<Tuile> paquet = genererPaquet(TAILLE_CHANTIER, partie);
    Chantier chantier(paquet);

    cout << "Paquet initial: " << chantier.taille() << " tuile(s)\n\n";

    // --- Boucle de manches
    int toursMax = 6; // ajuste si tu veux plus/moins long
    for (int tour = 1; tour <= toursMax; ++tour)
    {
        cout << "========== TOUR " << tour << " ==========\n";
        cout << "Maitre architecte: J" << partie.getMaitreArchitecte() << "\n";

        // Chaque tour : deux joueurs jouent l’un après l’autre
        for (int i = 0; i < partie.getNbrJoueurs(); ++i)
        {
            Joueur &joueur = partie.getJoueurMain();
            cout << "-- Tour du joueur courant --\n";
            cout << "Pierres: " << joueur.getNbrPierres() << "\n";
            cout << "Points : " << joueur.getNbrPoints() << "\n";

            // Affiche le chantier
            cout << "Chantier (" << chantier.taille() << "): ";
            for (int k = 0; k < chantier.taille(); ++k)
                cout << "#" << k << " ";
            cout << "\n";

            if (chantier.taille() == 0)
            {
                cout << "Plus de tuiles au chantier.\n";
                break;
            }

            // Politique simple de pioche: si on a des pierres, on tente un index > 0, sinon l’index 0
            int choix = 0;
            if (joueur.getNbrPierres() >= 1 && chantier.taille() >= 2)
                choix = 1;
            Tuile *tuilePiochee = nullptr;
            if (!(tuilePiochee = chantier.piocherTuile(choix, partie)))
            {
                cout << "Echec pioche (choix=" << choix << "). On tente 0...\n";
                if (!(tuilePiochee = chantier.piocherTuile(0, partie)))
                {
                    cout << "Echec pioche même sur 0: on passe.\n";
                    partie.setProchainJoueur();
                    continue;
                }
            }
            else
            {
                cout << "Pioche OK: tuile #" << choix << " prise. Pierres restantes: "
                     << joueur.getNbrPierres() << "\n";
            }

            // Essayer de poser la tuile sur son plateau
            cout << "Essai de pose...\n";
            C *pose = joueur.getPlateau().essayerPlacerTuile(*tuilePiochee);
            if (!pose)
            {
                cout << "Aucun placement valide trouvé -> tour perdu pour ce joueur.\n";
            }
            else
            {
                // Affiche l’état du plateau et met à jour le score
                if (joueur.getPlateau().ajouterTuile(*tuilePiochee, *pose))
                {
                    joueur.setNbrPierres(joueur.getNbrPierres() + 1);
                }
                joueur.getPlateau().afficher();
                joueur.setNbrPoints();
                cout << "Points après pose: " << joueur.getNbrPoints() << "\n";
            }

            // Joueur suivant
            partie.setProchainJoueur();
        }

        // Fin du tour global
        partie.tourTermine();
        cout << "Fin du tour " << tour << ". Maitre architecte devient J"
             << partie.getMaitreArchitecte() << "\n\n";
    }

    // --- Bilan final
    cout << "===== FIN DE PARTIE =====\n";
    // Petit affichage des scores — ici on suppose 2 joueurs
    // Si tu as un getter pour accéder à tous les joueurs, remplace par une boucle.
    // Sinon, on se contente d’afficher le joueur en main (exemple illustratif).
    cout << "Score joueur en main: " << partie.getJoueurMain().getNbrPoints() << "\n";
    cout << "Taille paquet restant (si suivi par la Partie): " << partie.getTaillePaquet() << "\n";

    return 0;
}
