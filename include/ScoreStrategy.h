#ifndef SCORESTRATEGY_H
#define SCORESTRATEGY_H

#include <functional>

struct MarcheContext
{
    int hauteur = 0;
    bool aVoisinMarche = false;
    bool voisinPlaceMarche = false;
};

struct CaserneContext
{
    int baseScore = 0;
    int nbVoisins = 0;
    bool placementValide = false;
};

struct TempleContext
{
    int baseScore = 0;
    bool estEntoure = false;
    bool estEnHauteur = false;
};

struct JardinContext
{
    int baseScore = 0;
    bool conditionVariante = false;
};

class HabitationScoreStrategy
{
public:
    virtual ~HabitationScoreStrategy() = default;
    virtual int calculer(int scorePlusGrandQuartier) const = 0;
};

class HabitationClassiqueStrategy : public HabitationScoreStrategy
{
public:
    int calculer(int scorePlusGrandQuartier) const override
    {
        return scorePlusGrandQuartier;
    }
};

class HabitationVarianteStrategy : public HabitationScoreStrategy
{
public:
    int calculer(int scorePlusGrandQuartier) const override
    {
        if (scorePlusGrandQuartier >= 10)
            return scorePlusGrandQuartier * 2;
        return scorePlusGrandQuartier;
    }
};

class MarcheScoreStrategy
{
public:
    virtual ~MarcheScoreStrategy() = default;
    virtual int calculer(const MarcheContext &ctx) const = 0;
};

class MarcheClassiqueStrategy : public MarcheScoreStrategy
{
public:
    int calculer(const MarcheContext &ctx) const override
    {
        if (ctx.aVoisinMarche)
            return 0;
        return ctx.hauteur;
    }
};

class MarcheVarianteStrategy : public MarcheScoreStrategy
{
public:
    int calculer(const MarcheContext &ctx) const override
    {
        if (ctx.aVoisinMarche)
            return 0;
        int points = ctx.hauteur;
        if (ctx.voisinPlaceMarche)
            points += ctx.hauteur;
        return points;
    }
};

class CaserneScoreStrategy
{
public:
    virtual ~CaserneScoreStrategy() = default;
    virtual int calculer(const CaserneContext &ctx) const = 0;
};

class CaserneClassiqueStrategy : public CaserneScoreStrategy
{
public:
    int calculer(const CaserneContext &ctx) const override
    {
        if (!ctx.placementValide)
            return 0;
        return ctx.baseScore;
    }
};

class CaserneVarianteStrategy : public CaserneScoreStrategy
{
public:
    int calculer(const CaserneContext &ctx) const override
    {
        if (!ctx.placementValide)
            return 0;
        if (ctx.nbVoisins <= 3)
            return ctx.baseScore * 2;
        return ctx.baseScore;
    }
};

class TempleScoreStrategy
{
public:
    virtual ~TempleScoreStrategy() = default;
    virtual int calculer(const TempleContext &ctx) const = 0;
};

class TempleClassiqueStrategy : public TempleScoreStrategy
{
public:
    int calculer(const TempleContext &ctx) const override
    {
        if (!ctx.estEntoure)
            return 0;
        return ctx.baseScore;
    }
};

class TempleVarianteStrategy : public TempleScoreStrategy
{
public:
    int calculer(const TempleContext &ctx) const override
    {
        if (!ctx.estEntoure)
            return 0;
        if (ctx.estEnHauteur)
            return ctx.baseScore * 2;
        return ctx.baseScore;
    }
};

class JardinScoreStrategy
{
public:
    virtual ~JardinScoreStrategy() = default;
    virtual int calculer(const JardinContext &ctx) const = 0;
};

class JardinClassiqueStrategy : public JardinScoreStrategy
{
public:
    int calculer(const JardinContext &ctx) const override
    {
        return ctx.baseScore;
    }
};

class JardinVarianteStrategy : public JardinScoreStrategy
{
public:
    int calculer(const JardinContext &ctx) const override
    {
        int points = ctx.baseScore;
        if (ctx.conditionVariante)
            points += ctx.baseScore;
        return points;
    }
};

/**
 * @brief Représente l'ensemble des fonctions de calcul utilisées pour chaque type d'hexagone.
 * Chaque pointeur est configuré en fonction de la variante activée ou non.
 */
struct ScoreStrategies
{
    int (*habitation)(int scorePlusGrandQuartier) = nullptr;
    int (*marche)(const MarcheContext &) = nullptr;
    int (*caserne)(const CaserneContext &) = nullptr;
    int (*temple)(const TempleContext &) = nullptr;
    int (*jardin)(const JardinContext &) = nullptr;
};

/**
 * @brief Règle classique : on ne modifie pas le score des quartiers.
 */
inline int calculHabitationClassique(int score)
{
    return score;
}

/**
 * @brief Variante habitation : double le score si le quartier atteint 10 points.
 */
inline int calculHabitationVariante(int score)
{
    return (score >= 10) ? score * 2 : score;
}

/**
 * @brief Marché classique : rapporter la hauteur si aucun marché adjacent.
 */
inline int calculMarcheClassique(const MarcheContext &ctx)
{
    if (ctx.aVoisinMarche)
        return 0;
    return ctx.hauteur;
}

/**
 * @brief Variante marché : bonus supplémentaire si une place de marché est adjacente.
 */
inline int calculMarcheVariante(const MarcheContext &ctx)
{
    if (ctx.aVoisinMarche)
        return 0;
    int points = ctx.hauteur;
    if (ctx.voisinPlaceMarche)
        points += ctx.hauteur;
    return points;
}

/**
 * @brief Caserne classique : score uniquement si la caserne a au plus 5 voisins.
 */
inline int calculCaserneClassique(const CaserneContext &ctx)
{
    if (!ctx.placementValide)
        return 0;
    return ctx.baseScore;
}

/**
 * @brief Variante caserne : double si la caserne est exposée (≤ 3 voisins).
 */
inline int calculCaserneVariante(const CaserneContext &ctx)
{
    if (!ctx.placementValide)
        return 0;
    if (ctx.nbVoisins <= 3)
        return ctx.baseScore * 2;
    return ctx.baseScore;
}

/**
 * @brief Temple classique : score uniquement si le temple est complètement entouré.
 */
inline int calculTempleClassique(const TempleContext &ctx)
{
    if (!ctx.estEntoure)
        return 0;
    return ctx.baseScore;
}

/**
 * @brief Variante temple : double lorsque le temple est en hauteur.
 */
inline int calculTempleVariante(const TempleContext &ctx)
{
    if (!ctx.estEntoure)
        return 0;
    int points = ctx.baseScore;
    if (ctx.estEnHauteur)
        points *= 2;
    return points;
}

/**
 * @brief Jardin classique : valeur brute (hauteur).
 */
inline int calculJardinClassique(const JardinContext &ctx)
{
    return ctx.baseScore;
}

/**
 * @brief Variante jardin : bonus lorsque le jardin forme un lac.
 */
inline int calculJardinVariante(const JardinContext &ctx)
{
    int points = ctx.baseScore;
    if (ctx.conditionVariante)
        points += ctx.baseScore;
    return points;
}

/**
 * @brief Crée et configure toutes les stratégies en fonction des variantes actives.
 */
inline ScoreStrategies makeScoreStrategies(const bool variantes[5])
{
    ScoreStrategies strategies;
    strategies.habitation = variantes[0] ? &calculHabitationVariante : &calculHabitationClassique;
    strategies.marche = variantes[1] ? &calculMarcheVariante : &calculMarcheClassique;
    strategies.caserne = variantes[2] ? &calculCaserneVariante : &calculCaserneClassique;
    strategies.temple = variantes[3] ? &calculTempleVariante : &calculTempleClassique;
    strategies.jardin = variantes[4] ? &calculJardinVariante : &calculJardinClassique;
    return strategies;
}

#endif
