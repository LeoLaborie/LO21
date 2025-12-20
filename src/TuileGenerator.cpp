#include "TuileGenerator.h"

#include <stdexcept>
#include <string>

namespace
{
    const std::map<int, std::map<TypeHex, int>> &stockCartes()
    {
        static const std::map<int, std::map<TypeHex, int>> cartes = {
            {2, {{TypeHex::PHabitation, 5}, {TypeHex::PMarche, 4}, {TypeHex::PCaserne, 4}, {TypeHex::PTemple, 4}, {TypeHex::PJardin, 3}, {TypeHex::Habitation, 18}, {TypeHex::Marche, 12}, {TypeHex::Caserne, 10}, {TypeHex::Temple, 8}, {TypeHex::Jardin, 6}, {TypeHex::Carriere, 37}}},
            {3, {{TypeHex::PHabitation, 6}, {TypeHex::PMarche, 5}, {TypeHex::PCaserne, 5}, {TypeHex::PTemple, 5}, {TypeHex::PJardin, 4}, {TypeHex::Habitation, 27}, {TypeHex::Marche, 16}, {TypeHex::Caserne, 13}, {TypeHex::Temple, 10}, {TypeHex::Jardin, 7}, {TypeHex::Carriere, 49}}},
            {4, {{TypeHex::PHabitation, 7}, {TypeHex::PMarche, 6}, {TypeHex::PCaserne, 6}, {TypeHex::PTemple, 6}, {TypeHex::PJardin, 5}, {TypeHex::Habitation, 36}, {TypeHex::Marche, 20}, {TypeHex::Caserne, 16}, {TypeHex::Temple, 12}, {TypeHex::Jardin, 8}, {TypeHex::Carriere, 61}}}};
        return cartes;
    }
    bool estMarche(TypeHex t)
    {
        return t == TypeHex::Marche || t == TypeHex::PMarche;
    }
} // namespace

const std::map<int, std::map<TypeHex, int>> &TuileGenerator::cartesBase()
{
    return stockCartes();
}

TuileGenerator::TuileGenerator(int nbJoueurs, bool varianteFullTuile)
    : rng(std::random_device{}())
{
    if (nbJoueurs < 2)
        throw std::invalid_argument("Il faut au moins deux joueurs pour générer des tuiles.");

    tuilesParPile = nbJoueurs + 1;
    nombreDePiles = 12;
    if (nbJoueurs < 4 && varianteFullTuile)
    {
        if (nbJoueurs == 3)
            nombreDePiles = 15;
        else if (nbJoueurs == 2)
            nombreDePiles = 20;
    }

    const int cleCartes = (varianteFullTuile && nbJoueurs < 4) ? 4 : nbJoueurs;
    const auto &cartes = cartesBase();
    const auto it = cartes.find(cleCartes);
    if (it == cartes.end())
        throw std::invalid_argument("Configuration de cartes invalide.");
    stock = it->second;
}

std::vector<std::vector<Tuile>> TuileGenerator::genererPiles()
{
    std::vector<std::vector<Tuile>> piles;
    piles.reserve(nombreDePiles);

    for (int i = 0; i < nombreDePiles; ++i)
    {
        std::vector<Tuile> pile;
        pile.reserve(tuilesParPile);
        for (int j = 0; j < tuilesParPile; ++j)
        {
            pile.emplace_back(creerTuile());
        }
        piles.push_back(std::move(pile));
    }
    return piles;
}

Tuile TuileGenerator::genererTuileBonus()
{
    return creerTuile();
}

void TuileGenerator::verifierStockVide() const
{
    for (const auto &[type, quantite] : stock)
    {
        if (quantite != 0)
            throw std::runtime_error("Erreur: il reste " + std::to_string(quantite) +
                                     " cartes du type " + std::to_string(static_cast<int>(type)) + " non utilisées !");
    }
}

Tuile TuileGenerator::creerTuile()
{
    std::vector<std::unique_ptr<Hexagone>> hexas;
    hexas.reserve(3);

    bool marcheDejaPresent = false;

    // Force les marché au début pour éviter qu'il s'accumule
    if (stock[TypeHex::Marche] > 0)
    {
        stock[TypeHex::Marche]--;
        marcheDejaPresent = true;
        hexas.push_back(std::make_unique<Hexagone>(0, 0, 0, TypeHex::Marche));
    }

    while (static_cast<int>(hexas.size()) < 3)
    {
        // Empêcher 3 fois le même type
        bool interdireType = false;
        TypeHex typeInterdit = TypeHex::Carriere;

        if (hexas.size() == 2)
        {
            const TypeHex t0 = hexas[0]->getType();
            const TypeHex t1 = hexas[1]->getType();
            if (t0 == t1)
            {
                interdireType = true;
                typeInterdit = t0;
            }
        }

        TypeHex type = tirerCarte(marcheDejaPresent, interdireType, typeInterdit);
        if (type == TypeHex::Marche)
            marcheDejaPresent = true;

        hexas.push_back(std::make_unique<Hexagone>(0, 0, 0, type));
    }

    return Tuile(hexas[0].release(), hexas[1].release(), hexas[2].release());
}
std::unique_ptr<Hexagone> TuileGenerator::creerHexagone(bool &marcheDejaPresent)
{
    TypeHex type = tirerCarte(marcheDejaPresent);
    if (type == TypeHex::Marche)
        marcheDejaPresent = true;
    return std::make_unique<Hexagone>(0, 0, 0, type);
}

TypeHex TuileGenerator::tirerCarte(bool marcheDejaPresent)
{
    return tirerCarte(marcheDejaPresent, false, TypeHex::Carriere);
}

TypeHex TuileGenerator::tirerCarte(bool marcheDejaPresent, bool interdireType, TypeHex typeInterdit)
{
    std::vector<TypeHex> pool;
    pool.reserve(stock.size());

    for (const auto &[type, quantite] : stock)
    {
        if (quantite <= 0)
            continue;
        if (type == TypeHex::Marche && marcheDejaPresent)
            continue;
        if (interdireType && type == typeInterdit)
            continue;

        for (int i = 0; i < quantite; ++i)
            pool.push_back(type);
    }
    // si on a pas de chance et qu'on a comme même 3 fois le même type on le créer comme même pour éviter les erreurs
    if (pool.empty() && interdireType)
        return tirerCarte(marcheDejaPresent, false, typeInterdit);

    if (pool.empty())
        throw std::runtime_error("ya plus de cartes frr");

    std::uniform_int_distribution<> distrib(0, static_cast<int>(pool.size()) - 1);
    TypeHex choisi = pool[distrib(rng)];
    stock[choisi]--;
    return choisi;
}
