#ifndef TUILEGENERATOR_H
#define TUILEGENERATOR_H

#include <map>
#include <memory>
#include <random>
#include <vector>

#include "Hexagone.h"
#include "Tuile.h"

/**
 * @class ITuileGenerator
 * @brief Interface abstraite représentant un générateur de tuiles.
 */
class ITuileGenerator
{
public:
    virtual ~ITuileGenerator() = default;
    virtual std::vector<std::vector<Tuile>> genererPiles() = 0;
    virtual Tuile genererTuileBonus() = 0;
    virtual void verifierStockVide() const = 0;
};

/**
 * @class TuileGenerator
 * @brief Génère les tuiles du jeu en fonction du nombre de joueurs et des variantes actives.
 */
class TuileGenerator : public ITuileGenerator
{
public:
    /**
     * @brief Constructeur à partir du nombre de joueurs et de la variante full-tuile.
     */
    TuileGenerator(int nbJoueurs, bool varianteFullTuile);

    std::vector<std::vector<Tuile>> genererPiles() override;
    Tuile genererTuileBonus() override;
    void verifierStockVide() const override;

private:
    Tuile creerTuile();
    std::unique_ptr<Hexagone> creerHexagone(bool &marcheDejaPresent);
    TypeHex tirerCarte(bool marcheDejaPresent);
    TypeHex tirerCarte(bool marcheDejaPresent, bool interdireType, TypeHex typeInterdit);
    static const std::map<int, std::map<TypeHex, int>> &cartesBase();

    std::map<TypeHex, int> stock;
    int nombreDePiles = 0;
    int tuilesParPile = 0;
    mutable std::mt19937 rng;
};

#endif // TUILEGENERATOR_H
