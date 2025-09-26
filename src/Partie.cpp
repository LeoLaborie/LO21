#include "Partie.h"
#include <vector>

using namespace std;

void Partie::setNbrJoueurs(int nbr)
{
    nbrJoueurs = nbr;
}

void Partie::addTuileInChantierFromPiles()
{
    vector<Tuile> pileActuel = piles.back();
    piles.pop_back();
    for (int i = 0; i < nbrJoueurs + 1; i++) // nbrJoeurs + 1 = nombre de tuiles dans une pile
    {
        chantier.push_back(pileActuel.back());
        pileActuel.pop_back();
    }
}
void Partie::tourTermine()
{
    if (maitreArchitecte < nbrJoueurs - 1)
        maitreArchitecte++;
    else
        maitreArchitecte = 0;

    nbrTours++;
}

void Partie::addJoueur(Joueur j)
{
    if (joueurs.size() < nbrJoueurs)
        joueurs.push_back(j);
    else
        cout << "Nombre de joueurs maximum atteint" << endl;
}