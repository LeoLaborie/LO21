#include "Partie.h"
#include <map>
#include <string>
#include <vector>
#include <stdexcept>
#include <random>

Hexagone *creerHexagoneDepuisType(const std::string &type, Tuile &tuile, bool *marcheDejaPresent)
{
    if (type == "placeBleue")
        return new Place(0, 0, 0, TypePlace::Habitation, 1, &tuile);
    else if (type == "placeJaune")
    {
        *marcheDejaPresent = true;
        return new Place(0, 0, 0, TypePlace::Marche, 2, &tuile);
    }
    else if (type == "placeRouge")
        return new Place(0, 0, 0, TypePlace::Caserne, 2, &tuile);
    else if (type == "placeViolette")
        return new Place(0, 0, 0, TypePlace::Temple, 2, &tuile);
    else if (type == "placeVerte")
        return new Place(0, 0, 0, TypePlace::Jardin, 3, &tuile);
    else if (type == "quartierBleu")
        return new Quartier(0, 0, 0, TypeQuartier::Habitation, &tuile);
    else if (type == "quartierJaune")
        return new Quartier(0, 0, 0, TypeQuartier::Marche, &tuile);
    else if (type == "quartierRouge")
        return new Quartier(0, 0, 0, TypeQuartier::Caserne, &tuile);
    else if (type == "quartierViolet")
        return new Quartier(0, 0, 0, TypeQuartier::Temple, &tuile);
    else if (type == "quartierVert")
        return new Quartier(0, 0, 0, TypeQuartier::Jardin, &tuile);
    else if (type == "carriere")
        return new Carriere(0, 0, 0, &tuile);

    throw std::runtime_error("Type inconnu: " + type);
}

void Partie::addTuileInChantierFromPiles()
{
    std::vector<Tuile> pileActuel = piles.back();
    piles.pop_back();
    for (int i = 0; i < nbrJoueurs + 1; i++) // nbrJoeurs + 1 = nombre de tuiles dans une pile
    {
        chantier.ajouterTuile(pileActuel.back());
        pileActuel.pop_back();
    }
}

std::string tirerCarte(std::map<std::string, int> &stock, bool marcheDejaPresent)
{
    // Construire un vecteur pondéré selon les quantités restantes
    std::vector<std::string> pool;
    for (auto &[type, quantite] : stock)
    {
        if (quantite > 0)
        {
            if (type == "placeJaune" && marcheDejaPresent)
                continue; // Interdiction du 2e marché
            for (int i = 0; i < quantite; i++)
            {
                pool.push_back(type);
            }
        }
    }

    if (pool.empty())
        throw std::runtime_error("ya plus de cartes frr");

    // on tire une carte random
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, pool.size() - 1);

    std::string choisi = pool[distrib(gen)];
    stock[choisi]--;
    return choisi;
}

void Partie::genererTuilesParties()
{
    std::map<int, std::map<std::string, int>> cartes = {
        {2, {
                {"placeBleue", 5}, {"placeJaune", 4}, {"placeRouge", 4}, {"placeViolette", 4}, {"placeVerte", 3}, {"quartierBleu", 18}, {"quartierJaune", 12}, {"quartierRouge", 10}, {"quartierViolet", 8}, {"quartierVert", 6}, {"carriere", 37} 
            }},
        {3, {
                {"placeBleue", 6}, {"placeJaune", 5}, {"placeRouge", 5}, {"placeViolette", 5}, {"placeVerte", 4}, {"quartierBleu", 27}, {"quartierJaune", 16}, {"quartierRouge", 13}, {"quartierViolet", 10}, {"quartierVert", 7}, {"carriere", 49} 
            }},
        {4, {
                {"placeBleue", 7}, {"placeJaune", 6}, {"placeRouge", 6}, {"placeViolette", 6}, {"placeVerte", 5}, {"quartierBleu", 36}, {"quartierJaune", 20}, {"quartierRouge", 16}, {"quartierViolet", 12}, {"quartierVert", 8}, {"carriere", 61} 
            }}};

    auto &stock = cartes[getNbrJoueurs()];

    int tuilesParPile = getNbrJoueurs() + 1;
    int nombreDePiles = 12;

    for (int i = 0; i < nombreDePiles; i++)
    {
        std::vector<Tuile> pile;
        for (int j = 0; j < tuilesParPile; j++)
        {
            Tuile tuile;
            std::vector<Hexagone *> hexas;
            bool marcheDejaPresent = false;

            for (int k = 0; k < 3; k++)
            {
                std::string type = tirerCarte(stock, marcheDejaPresent);
                Hexagone *h = creerHexagoneDepuisType(type, tuile, &marcheDejaPresent);
                hexas.push_back(h);
            }

            tuile.creerTuile(hexas[0], hexas[1], hexas[2]);
            pile.push_back(tuile);
        }
        piles.push_back(pile);
    }

    // Ajouter la tuile bonus faite des 3 dernières cartes du stock

    Tuile tuileBonus;
    std::vector<Hexagone *> hexas;
    bool marcheDejaPresent = false;

    for (int i = 0; i < 3; i++)
    {
        std::string type = tirerCarte(stock, marcheDejaPresent);
        Hexagone *h = creerHexagoneDepuisType(type, tuileBonus, &marcheDejaPresent);
        hexas.push_back(h);
    }

    tuileBonus.creerTuile(hexas[0], hexas[1], hexas[2]);
    chantier.ajouterTuile(tuileBonus); // ajouté directement dans le chantier

    // Vérification : le stock doit être vide
    for (auto &[type, quantite] : stock)
    {
        if (quantite != 0)
        {
            throw std::runtime_error("Erreur: il reste " + std::to_string(quantite) +
                                     " cartes du type " + type + " non utilisées !");
        }
    }

    // Initialiser le chantier avec une pile de tuiles
    addTuileInChantierFromPiles();
}

void Partie::setNbrJoueurs(int nbr)
{
    int nbpierre = 1;
    if (nbr <= 0 || nbr > 4)
        throw std::invalid_argument("nbrJoueurs doit etre > 0 et inférieur à 4");
    nbrJoueurs = nbr;
    joueurs.assign(nbrJoueurs, Joueur{});
    for (auto &j : joueurs)
    {
        j.setNbrPierres(nbpierre);
        nbpierre++;
    }
    mainJoueur = 0;
    maitreArchitecte = 0;
    nbrTours = 0;
    taillepaquet = 1 + nbrJoueurs;
}

void Partie::tourTermine()
{
    if (nbrJoueurs > 0)
    {
        maitreArchitecte = (maitreArchitecte + 1) % nbrJoueurs;
    }
    nbrTours++;
}

std::vector<Tuile> Partie::retirerPaquet()
{
    std::vector<Tuile> paquet;

    paquet = piles.back();
    piles.pop_back();

    return paquet;
}