#include "Partie.h"

// Constructeur privé utilisé par fromSave (factory de chargement)
Partie::Partie(int nbJoueursInit,
               int nbToursInit,
               int maitreArchitecteInit,
               int mainJoueurInit,
               Chantier chantierInit,
               std::vector<std::vector<Tuile>> pilesInit,
               std::vector<Joueur*> joueursInit,
               bool fauxJoueurPInit )
    : nbrJoueurs(nbJoueursInit),
      maitreArchitecte(maitreArchitecteInit % (nbJoueursInit ? nbJoueursInit : 1)),
      mainJoueur(mainJoueurInit % (nbJoueursInit ? nbJoueursInit : 1)),
      nbrTours(nbToursInit),
      taillepaquet(nbJoueursInit + 1),
      fauxJoueurP(fauxJoueurPInit),
      chantier(std::move(chantierInit)),
      joueurs(std::move(joueursInit)),
      piles(std::move(pilesInit))
{
}

Partie::Partie(int nbJouer, std::vector<std::string> &pseudo, const bool variantesScore[5], bool varianteFullTuile, int difficulte)
{
    if (nbJouer <= 0 || nbJouer > 4)
        throw std::invalid_argument("nbrJoueurs doit être > 0 et ≤ 4");

    nbrJoueurs = nbJouer;

    // creation des joueurs (alloués dynamiquement pour éviter dangling pointers)
    joueurs.clear();

    for (int i = 0; i < nbrJoueurs; ++i)
    {
        JoueurConfig cfg;
        cfg.nom = pseudo.at(static_cast<size_t>(i));
        cfg.variantesScore = variantesScore;
        std::unique_ptr<Joueur> joueur = getFactoryJoueur(TypeJoueurs::JoueurHumain)->creer(cfg);
        joueur->setNbrPierres(i + 1);
        joueurs.push_back(joueur.release());
    }

    if (nbrJoueurs == 1)
    {
        if (difficulte>3) difficulte=3;
        if (difficulte<1) difficulte=1;
        nbrJoueurs++;
        JoueurConfig cfg;
        cfg.nom = "Illustre Architecte";
        cfg.difficulte = difficulte;
        std::unique_ptr<Joueur> fauxJoueur = getFactoryJoueur(TypeJoueurs::IllustreArchitecte)->creer(cfg);
        fauxJoueur->setNbrPierres(2);
        joueurs.push_back(fauxJoueur.release());
        fauxJoueurP = true;
    }

    // initialisation des paramètres
    mainJoueur = 0;
    maitreArchitecte = 0;
    nbrTours = 0;
    taillepaquet = 1 + nbrJoueurs;

    genererTuilesParties(varianteFullTuile);
}


Hexagone* creerHexagone(TypeHex type, Tuile& tuile, bool* marcheDejaPresent)
{
    if (type == TypeHex::Marche)
        *marcheDejaPresent = true;

    return new Hexagone(0, 0, 0, type, &tuile);
}

void Partie::addTuileInChantierFromPiles()
{
    std::vector<Tuile> pileActuel = piles.back();
    piles.pop_back();
    for (int i = 0; i < nbrJoueurs + 1; i++)  // nbrJoeurs + 1 = nombre de tuiles dans une pile
    {
        chantier.ajouterTuile(pileActuel.back());
        pileActuel.pop_back();
    }
}

TypeHex Partie::tirerCarte(std::map<TypeHex, int> &stock, bool marcheDejaPresent)
{
    // Construire un vecteur pondéré selon les quantités restantes
    std::vector<TypeHex> pool;
    for (auto &[type, quantite] : stock)
    {
            if (quantite > 0)
            {
            if (type == TypeHex::Marche && marcheDejaPresent)
                continue;  // Interdiction du 2e marché
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

    TypeHex choisi = pool[distrib(gen)];
    stock[choisi]--;
    return choisi;
}

void Partie::genererTuilesParties(bool fullTuiles)
{
    std::map<int, std::map<TypeHex, int>> cartes = {
        {2, {{TypeHex::PHabitation, 5}, {TypeHex::PMarche, 4}, {TypeHex::PCaserne, 4}, {TypeHex::PTemple, 4}, {TypeHex::PJardin, 3}, {TypeHex::Habitation, 18}, {TypeHex::Marche, 12}, {TypeHex::Caserne, 10}, {TypeHex::Temple, 8}, {TypeHex::Jardin, 6}, {TypeHex::Carriere, 37}}},
        {3, {{TypeHex::PHabitation, 6}, {TypeHex::PMarche, 5}, {TypeHex::PCaserne, 5}, {TypeHex::PTemple, 5}, {TypeHex::PJardin, 4}, {TypeHex::Habitation, 27}, {TypeHex::Marche, 16}, {TypeHex::Caserne, 13}, {TypeHex::Temple, 10}, {TypeHex::Jardin, 7}, {TypeHex::Carriere, 49}}},
        {4, {{TypeHex::PHabitation, 7}, {TypeHex::PMarche, 6}, {TypeHex::PCaserne, 6}, {TypeHex::PTemple, 6}, {TypeHex::PJardin, 5}, {TypeHex::Habitation, 36}, {TypeHex::Marche, 20}, {TypeHex::Caserne, 16}, {TypeHex::Temple, 12}, {TypeHex::Jardin, 8}, {TypeHex::Carriere, 61}}}};

    std::map<TypeHex, int> *stock = &cartes[getNbrJoueurs()];

    int tuilesParPile = getNbrJoueurs() + 1;
    int nombreDePiles = 12;
    if (nbrJoueurs < 4)
    {  // Variante du nombre de piles en fonction du nombre de joueurs
        if (fullTuiles)
        {
            stock = &cartes[4];
            if (nbrJoueurs == 3)
            {
                nombreDePiles = 15;
            }
            else if (nbrJoueurs == 2)
            {
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
                TypeHex type = tirerCarte(*stock, marcheDejaPresent);
                Hexagone *h = creerHexagone(type, tuile, &marcheDejaPresent);
                hexas.push_back(h);
            }
            tuile = Tuile(hexas[0], hexas[1], hexas[2]);
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
        TypeHex type = tirerCarte(*stock, marcheDejaPresent);
        Hexagone *h = creerHexagone(type, tuileBonus, &marcheDejaPresent);
        hexas.push_back(h);
    }

    tuileBonus = Tuile(hexas[0], hexas[1], hexas[2]);
    chantier.ajouterTuile(tuileBonus);
    // Vérification : le stock doit être vide
    for (auto &[type, quantite] : *stock)
    {
        if (quantite != 0)
        {
            throw std::runtime_error("Erreur: il reste " + std::to_string(quantite) +
                                     " cartes du type " + std::to_string(static_cast<int>(type)) + " non utilisées !");
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

FactoryJoueurs *Partie::getFactoryJoueur(TypeJoueurs type)
{
    static FactoryJoueurHumain factoryHumain;
    static FactoryIllustreArchitecte factoryIA;
    switch (type)
    {
    case TypeJoueurs::JoueurHumain:
        return &factoryHumain;
    case TypeJoueurs::IllustreArchitecte:
        return &factoryIA;
    default:
        throw std::invalid_argument("Type de joueur inconnu");
    }
}
