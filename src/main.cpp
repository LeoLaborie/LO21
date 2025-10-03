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

struct C
{
    int x, y, z;
};

static std::vector<C> candidatsVoisinsAutour(const C &c, int rayon = 2)
{
    std::vector<C> res;
    for (int dx = -rayon; dx <= rayon; ++dx)
    {
        for (int dy = std::max(-rayon, -dx - rayon); dy <= std::min(rayon, -dx + rayon); ++dy)
        {
            int dz = -dx - dy;
            if (dx == 0 && dy == 0 && dz == 0)
                continue;
            res.push_back({c.x + dx, c.y + dy, c.z + dz});
        }
    }
    return res;
}

static std::vector<C> grillePetite(int r = 3)
{
    std::vector<C> res;
    for (int x = -r; x <= r; ++x)
    {
        for (int y = -r; y <= r; ++y)
        {
            int z = -x - y;
            if (std::abs(z) > r)
                continue;
            res.push_back({x, y, z});
        }
    }
    return res;
}

static void afficherEtatPlateau(const Plateau &p)
{
    cout << "  Hexagones sur plateau: " << p.getHexagones().size() << "\n";
    for (auto *h : p.getHexagones())
    {
        cout << "    (" << h->getX() << "," << h->getY() << "," << h->getZ() << ")"
             << " voisins=" << h->getVoisins().size()
             << (h->getEstRecouvert() ? " [recouvert]" : "") << "\n";
    }
}

static bool essayerPlacerTuile(Joueur &j)
{
    Plateau &p = j.getPlateau();
    const Tuile &t = j.getTuileEnMain();

    // 1) on balaie une grille autour de (0,0,0)
    auto grille = grillePetite(3);

    // 2) on teste plusieurs orientations (0,1,2 rotations)
    Tuile tuileMutable = t; // on copie pour pouvoir pivoter
    for (int rot = 0; rot < 3; ++rot)
    {
        if (rot > 0)
            tuileMutable.pivoterTuile();

        for (const auto &c : grille)
        {
            if (p.verifierPlacementTuile(c.x, c.y, c.z))
            {
                cout << "    -> placement OK en (" << c.x << "," << c.y << "," << c.z
                     << ") avec rotation=" << rot << "\n";
                Tuile tuileAPlacer = tuileMutable; // copie de l’orientation courante
                p.ajouterTuile(tuileAPlacer, c.x, c.y, c.z);
                return true;
            }
        }
    }
    return false;
}

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
            if (!chantier.piocherTuile(choix, partie))
            {
                cout << "Echec pioche (choix=" << choix << "). On tente 0...\n";
                if (!chantier.piocherTuile(0, partie))
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
            bool pose = essayerPlacerTuile(joueur);
            if (!pose)
            {
                cout << "Aucun placement valide trouvé -> tour perdu pour ce joueur.\n";
            }
            else
            {
                // Affiche l’état du plateau et met à jour le score
                afficherEtatPlateau(joueur.getPlateau());
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
