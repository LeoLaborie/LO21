#ifndef TUILEGENERATORFACTORY_H
#define TUILEGENERATORFACTORY_H

#include <memory>

#include "TuileGenerator.h"

/**
 * @class TuileGeneratorFactory
 * @brief Factory abstraite permettant d’obtenir un générateur de tuiles.
 *
 * Pour ajouter un nouveau générateur :
 * on créez une classe dérivée de ITuileGenerator.
 * on implémente genererPiles, genererTuileBonus et verifierStockVide.
 * fournir une factory dédiée qui retourne cette implémentation et définir dans partie une fonction qui get la bonne factory.
 */
class TuileGeneratorFactory
{
public:
    virtual ~TuileGeneratorFactory() = default;
    virtual std::unique_ptr<ITuileGenerator> creer(int nbJoueurs, bool varianteFullTuile) const = 0;
};

/**
 * @class DefaultTuileGeneratorFactory
 * @brief Implémentation de la factory qui utilise le générateur standard.
 */
class DefaultTuileGeneratorFactory : public TuileGeneratorFactory
{
public:
    std::unique_ptr<ITuileGenerator> creer(int nbJoueurs, bool varianteFullTuile) const override;
};

#endif // TUILEGENERATORFACTORY_H
