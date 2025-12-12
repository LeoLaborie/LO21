#ifndef RULESFACTORY_H
#define RULESFACTORY_H

#include <memory>

#include "ScoreStrategy.h"

class Partie;
class Plateau;
class Tuile;
struct Position;

/**
 * @brief Pack de paramètres décrivant un mode de jeu (variantes, distribution des tuiles…).
 */
struct Ruleset
{
    bool variantesScore[5] = {};
    bool fullTuiles = false;
};

/**
 * @brief Permet de générer les piles de tuiles adaptées à un mode de jeu.
 */
class ITuileSetGenerator
{
public:
    virtual ~ITuileSetGenerator() = default;
    virtual void generer(Partie &partie) const = 0;
};

/**
 * @brief Interface de fabrique regroupant toutes les règles d'un mode de jeu.
 */
class IRulesFactory
{
public:
    virtual ~IRulesFactory() = default;
    
    virtual Ruleset creerRuleset() const = 0;
    virtual ScoreStrategies creerStrategiesScore() const = 0;
    virtual std::unique_ptr<ITuileSetGenerator> creerGenerateurTuiles() const = 0;
    virtual ScoreStrategies creerStrategiesScore() const = 0;
    virtual std::unique_ptr<ITuileSetGenerator> creerGenerateurTuiles() const = 0;
};

/**
 * @brief Générateur de piles pour le mode classique (11 piles maximum).
 */
class GenerateurTuilesClassique : public ITuileSetGenerator
{
public:
    explicit GenerateurTuilesClassique(bool full = false)
        : utiliserToutesLesTuiles(full) {}
    void generer(Partie &partie) const override;

private:
    bool utiliserToutesLesTuiles = false;
};

/**
 * @brief Générateur qui active systématiquement la variante « toutes les tuiles ».
 */
class GenerateurTuilesFull : public ITuileSetGenerator
{
public:
    void generer(Partie &partie) const override;
};

/**
 * @brief Fabrique utilisée pour les modes préconfigurés (classique, variantes…).
 */
class RulesFactoryPersonnalisee : public IRulesFactory
{
public:
    RulesFactoryPersonnalisee(const bool variantes[5], bool fullTuiles);
    explicit RulesFactoryPersonnalisee(Ruleset regles);

    Ruleset creerRuleset() const override;
    ScoreStrategies creerStrategiesScore() const override;
    std::unique_ptr<ITuileSetGenerator> creerGenerateurTuiles() const override;

private:
    Ruleset regles;
};

/**
 * @brief Mode classique par défaut.
 */
class RulesFactoryClassique : public RulesFactoryPersonnalisee
{
public:
    RulesFactoryClassique();
};

/**
 * @brief Exemple de mode basé sur toutes les variantes officielles.
 */
class RulesFactoryVariantes : public RulesFactoryPersonnalisee
{
public:
    RulesFactoryVariantes();
};

#endif
