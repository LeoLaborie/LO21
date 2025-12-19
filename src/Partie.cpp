#include "Partie.h"
#include "TuileGeneratorFactory.h"

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

void Partie::genererTuilesParties(bool fullTuiles, const TuileGeneratorFactory *factory)
{
    static DefaultTuileGeneratorFactory defaultFactory;
    const TuileGeneratorFactory &usedFactory = factory ? *factory : defaultFactory; //on vérfie que la factory n'est pas nul sinon on met celle par default qui est la seule à exister
    auto generator = usedFactory.creer(nbrJoueurs, fullTuiles);
    piles = generator->genererPiles();
    chantier.ajouterTuile(generator->genererTuileBonus());
    generator->verifierStockVide();
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
