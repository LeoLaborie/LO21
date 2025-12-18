#ifndef JOUEUR_FACTORY_H
#define JOUEUR_FACTORY_H

#include <memory>
#include <string>

class Joueur;

/**
 * @enum TypeJoueurs
 * @brief Types logiques de joueurs disponibles dans l'application.
 *
 * Permet à la classe Partie de choisir un type de joueur
 * sans connaître la classe concrète instanciée.
 */
enum class TypeJoueurs {
    JoueurHumain,
    IllustreArchitecte
};

/**
 * @struct JoueurConfig
 * @brief Regroupe tous les paramètres nécessaires à la création d’un joueur.
 *
 * Cette structure permet de faire évoluer facilement la création
 * des joueurs (ajout de variantes, nouvelles règles, etc.)
 * sans modifier l’interface des factories.
 */
struct JoueurConfig {
    std::string nom;
    const bool* variantesScore = nullptr;
    int difficulte = 0;
};

/**
 * @class FactoryJoueurs
 * @brief Interface abstraite de la Factory.
 *
 */
class FactoryJoueurs {
public:
    virtual ~FactoryJoueurs() = default;

    /**
     * @brief Crée un joueur à partir d’une configuration.
     * @param cfg Configuration du joueur.
     * @return std::unique_ptr<Joueur> joueur nouvellement créé.
     */
    virtual std::unique_ptr<Joueur> creer(const JoueurConfig& cfg) const = 0;
};

/**
 * @class FactoryJoueurHumain
 * @brief Factory concrète chargée de créer des joueurs humains.
 */
class FactoryJoueurHumain final : public FactoryJoueurs {
public:
    std::unique_ptr<Joueur> creer(const JoueurConfig& cfg) const override;
};

/**
 * @class FactoryIllustreArchitecte
 * @brief Factory concrète chargée de créer l’IA (Illustre Architecte).
 */
class FactoryIllustreArchitecte final : public FactoryJoueurs {
public:
    std::unique_ptr<Joueur> creer(const JoueurConfig& cfg) const override;
};

#endif // JOUEUR_FACTORY_H