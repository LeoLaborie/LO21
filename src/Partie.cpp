#include "Partie.h"

Partie::Partie(int nbJouer, std::vector<std::string>& pseudo,bool varianteScore, bool varianteFullTuile)
{
    if (nbJouer <= 0 || nbJouer > 4)
        throw std::invalid_argument("nbrJoueurs doit être > 0 et ≤ 4");

    nbrJoueurs = nbJouer;

    // creation des joueurs
    joueurs.clear();
    joueurs.reserve(nbrJoueurs);

    for (int i = 0; i < nbrJoueurs; ++i)
    {
        // ajout de chaque joueur dans la liste
        Joueur j(varianteScore, pseudo[i]);
        j.setNbrPierres(i+1);
        joueurs.push_back(j); 
    }

    //initialisation des paramètres
    mainJoueur = 0;
    maitreArchitecte = 0;
    nbrTours = 0;
    taillepaquet = 1 + nbrJoueurs;

    genererTuilesParties(varianteFullTuile);

}


Hexagone *creerHexagoneDepuisType(const std::string &type, Tuile &tuile, bool *marcheDejaPresent)
{
    if (type == "placeBleue")
        return new Hexagone(0, 0, 0, TypeHex::PHabitation, 1, &tuile);
    else if (type == "placeJaune")
    {
        *marcheDejaPresent = true;
        return new Hexagone(0, 0, 0, TypeHex::PMarche, 2, &tuile);
    }
    else if (type == "placeRouge")
        return new Hexagone(0, 0, 0, TypeHex::PCaserne, 2, &tuile);
    else if (type == "placeViolette")
        return new Hexagone(0, 0, 0, TypeHex::PTemple, 2, &tuile);
    else if (type == "placeVerte")
        return new Hexagone(0, 0, 0, TypeHex::PJardin, 3, &tuile);
    else if (type == "quartierBleu")
        return new Hexagone(0, 0, 0, TypeHex::Habitation, 1, &tuile);
    else if (type == "quartierJaune")
        return new Hexagone(0, 0, 0, TypeHex::Marche, 1, &tuile);
    else if (type == "quartierRouge")
        return new Hexagone(0, 0, 0, TypeHex::Caserne, 1, &tuile);
    else if (type == "quartierViolet")
        return new Hexagone(0, 0, 0, TypeHex::Temple, 1, &tuile);
    else if (type == "quartierVert")
        return new Hexagone(0, 0, 0, TypeHex::Jardin, 1, &tuile);
    else if (type == "carriere")
        return new Hexagone(0, 0, 0, TypeHex::Carriere, 1, &tuile);

    throw std::runtime_error("Type inconnu: " + type);
}

void Partie::addTuileInChantierFromPiles()
{
    std::vector<Tuile> pileActuel = piles.back();
    piles.pop_back();
    while (this->chantier.getTaille()<nbrJoueurs+1) // nbrJoeurs + 1 = nombre de tuiles dans une pile
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

void Partie::genererTuilesParties(bool fullTuiles)
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

    std::map<std::string, int> *stock = &cartes[getNbrJoueurs()];

    int tuilesParPile = getNbrJoueurs() + 1;
    int nombreDePiles = 12;
    if(nbrJoueurs < 4){//Variante du nombre de piles en fonction du nombre de joueurs
        if(fullTuiles){
            stock = &cartes[4];
            if(nbrJoueurs == 3){
                nombreDePiles = 15;
            }else if(nbrJoueurs == 2){
                nombreDePiles = 20;
            }
        }
    }

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
                std::string type = tirerCarte(*stock, marcheDejaPresent);
                Hexagone *h = creerHexagoneDepuisType(type, tuile, &marcheDejaPresent);
                hexas.push_back(h);
            }
            tuile= Tuile(hexas[0], hexas[1], hexas[2]);
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
        std::string type = tirerCarte(*stock, marcheDejaPresent);
        Hexagone *h = creerHexagoneDepuisType(type, tuileBonus, &marcheDejaPresent);
        hexas.push_back(h);
    }

    tuileBonus=Tuile(hexas[0], hexas[1], hexas[2]);

    // Vérification : le stock doit être vide
    for (auto &[type, quantite] : *stock)
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