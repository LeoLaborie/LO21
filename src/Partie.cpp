#include "Partie.h"
#include <map>
#include <string>
#include <vector>
#include <stdexcept>
#include <random>

Hexagone *creerHexagoneDepuisType(const std::string &type, Tuile &tuile, bool *marcheDejaPresent)
{
    if (type == "HexagoneBleue")
        return new Hexagone(0, 0, 0, TypeHex::PHabitation, 1, &tuile);
    else if (type == "HexagoneJaune")
    {
        *marcheDejaPresent = true;
        return new Hexagone(0, 0, 0, TypeHex::PMarche, 2, &tuile);
    }
    else if (type == "HexagoneRouge")
        return new Hexagone(0, 0, 0, TypeHex::PCaserne, 2, &tuile);
    else if (type == "HexagoneViolette")
        return new Hexagone(0, 0, 0, TypeHex::PTemple, 2, &tuile);
    else if (type == "HexagoneVerte")
        return new Hexagone(0, 0, 0, TypeHex::PJardin, 3, &tuile);
    else if (type == "HexagoneBleu")
        return new Hexagone(0, 0, 0, TypeHex::Habitation, 1, &tuile);
    else if (type == "HexagoneJaune")
        return new Hexagone(0, 0, 0, TypeHex::Marche, 1, &tuile);
    else if (type == "HexagoneRouge")
        return new Hexagone(0, 0, 0, TypeHex::Caserne, 1, &tuile);
    else if (type == "HexagoneViolet")
        return new Hexagone(0, 0, 0, TypeHex::Temple, 1,&tuile);
    else if (type == "HexagoneVert")
        return new Hexagone(0, 0, 0, TypeHex::Jardin, 1, &tuile);
    else if (type == "carriere")
        return new Hexagone(0, 0, 0, TypeHex::Carriere, 1, &tuile);

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
            if (type == "HexagoneJaune" && marcheDejaPresent)
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
                {"HexagoneBleue", 5}, {"HexagoneJaune", 4}, {"HexagoneRouge", 4}, {"HexagoneViolette", 4}, {"HexagoneVerte", 3}, {"HexagoneBleu", 18}, {"HexagoneJaune", 12}, {"HexagoneRouge", 10}, {"HexagoneViolet", 8}, {"HexagoneVert", 6}, {"carriere", 37} 
            }},
        {3, {
                {"HexagoneBleue", 6}, {"HexagoneJaune", 5}, {"HexagoneRouge", 5}, {"HexagoneViolette", 5}, {"HexagoneVerte", 4}, {"HexagoneBleu", 27}, {"HexagoneJaune", 16}, {"HexagoneRouge", 13}, {"HexagoneViolet", 10}, {"HexagoneVert", 7}, {"carriere", 49} 
            }},
        {4, {
                {"HexagoneBleue", 7}, {"HexagoneJaune", 6}, {"HexagoneRouge", 6}, {"HexagoneViolette", 6}, {"HexagoneVerte", 5}, {"HexagoneBleu", 36}, {"HexagoneJaune", 20}, {"HexagoneRouge", 16}, {"HexagoneViolet", 12}, {"HexagoneVert", 8}, {"carriere", 61} 
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