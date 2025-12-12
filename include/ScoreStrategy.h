#ifndef SCORESTRATEGY_H
#define SCORESTRATEGY_H

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

class ScoreHabitationStrategy
{
public:
    virtual ~ScoreHabitationStrategy() = default;
    virtual int calculer(int scorePlusGrandQuartier) const = 0;
};

class SHClassique : public ScoreHabitationStrategy
{
public:
    int calculer(int scorePlusGrandQuartier) const override
    {
        return scorePlusGrandQuartier;
    }
};

class SHVariante : public ScoreHabitationStrategy
{
public:
    int calculer(int scorePlusGrandQuartier) const override
    {
        if (scorePlusGrandQuartier >= 10)
            return scorePlusGrandQuartier * 2;
        return scorePlusGrandQuartier;
    }
};

class ScoreMarcheStrategy
{
public:
    virtual ~ScoreMarcheStrategy() = default;
    virtual int calculer(const MarcheContext &ctx) const = 0;
};

class SMClassique : public ScoreMarcheStrategy
{
public:
    int calculer(const MarcheContext &ctx) const override
    {
        if (ctx.aVoisinMarche)
            return 0;
        return ctx.hauteur;
    }
};

class SMVariante : public ScoreMarcheStrategy
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

class ScoreCaserneStrategy
{
public:
    virtual ~ScoreCaserneStrategy() = default;
    virtual int calculer(const CaserneContext &ctx) const = 0;
};

class SCClassique : public ScoreCaserneStrategy
{
public:
    int calculer(const CaserneContext &ctx) const override
    {
        if (!ctx.placementValide)
            return 0;
        return ctx.baseScore;
    }
};

class SCVariante : public ScoreCaserneStrategy
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

class ScoreTempleStrategy
{
public:
    virtual ~ScoreTempleStrategy() = default;
    virtual int calculer(const TempleContext &ctx) const = 0;
};

class STClassique : public ScoreTempleStrategy
{
public:
    int calculer(const TempleContext &ctx) const override
    {
        if (!ctx.estEntoure)
            return 0;
        return ctx.baseScore;
    }
};

class STVariante : public ScoreTempleStrategy
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

class ScoreJardinStrategy
{
public:
    virtual ~ScoreJardinStrategy() = default;
    virtual int calculer(const JardinContext &ctx) const = 0;
};

class SJClassique : public ScoreJardinStrategy
{
public:
    int calculer(const JardinContext &ctx) const override
    {
        return ctx.baseScore;
    }
};

class SJVariante : public ScoreJardinStrategy
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
inline ScoreStrategies ChoisirScoreStrategies(const bool variantes[5])
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
