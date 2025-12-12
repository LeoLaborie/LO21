#ifndef SCORESTRATEGY_H
#define SCORESTRATEGY_H

#include <memory>

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

struct ScoreStrategies
{
    std::unique_ptr<HabitationScoreStrategy> habitation;
    std::unique_ptr<MarcheScoreStrategy> marche;
    std::unique_ptr<CaserneScoreStrategy> caserne;
    std::unique_ptr<TempleScoreStrategy> temple;
    std::unique_ptr<JardinScoreStrategy> jardin;
};

inline ScoreStrategies makeScoreStrategies(const bool variantes[5])
{
    ScoreStrategies strategies;
    strategies.habitation = variantes[0] ? std::make_unique<HabitationVarianteStrategy>()
                                         : std::make_unique<HabitationClassiqueStrategy>();
    strategies.marche = variantes[1] ? std::make_unique<MarcheVarianteStrategy>()
                                     : std::make_unique<MarcheClassiqueStrategy>();
    strategies.caserne = variantes[2] ? std::make_unique<CaserneVarianteStrategy>()
                                      : std::make_unique<CaserneClassiqueStrategy>();
    strategies.temple = variantes[3] ? std::make_unique<TempleVarianteStrategy>()
                                     : std::make_unique<TempleClassiqueStrategy>();
    strategies.jardin = variantes[4] ? std::make_unique<JardinVarianteStrategy>()
                                     : std::make_unique<JardinClassiqueStrategy>();
    return strategies;
}

#endif
