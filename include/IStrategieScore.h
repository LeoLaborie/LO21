#ifndef ISTRATEGIE_SCORE_H
#define ISTRATEGIE_SCORE_H

class Plateau;

class IStrategieScore
{
public:
    virtual ~IStrategieScore() = default;
    virtual int calculer(const Plateau& plateau) const = 0;

protected:
    IStrategieScore() = default;
    IStrategieScore(const IStrategieScore&) = default;
    IStrategieScore& operator=(const IStrategieScore&) = default;
};

#endif 
