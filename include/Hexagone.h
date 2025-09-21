#ifndef HEXAGONE_H
#define HEXAGONE_H

#include <iostream>
#include <vector>
#include <string.h>
#include "Tuile.h"

class Hexagone
{
public:
    void setParent(Tuile *parent) { Parent = parent; }
    void setVoisins(std::vector<Hexagone *> v) { voisins = v; };

private:
    std::vector<Hexagone *> voisins;
    int x;
    int y;
    int z;
    Tuile *Parent;
};

class Carriere : public Hexagone
{
};

class Quartier : public Hexagone
{
private:
    enum class TypeQuartier
    {
        Marche,
        Temple,
        Caserne,
        Jardin
    };
    TypeQuartier typeQuartier;

public:
    void setTypeQuartier(TypeQuartier t) { typeQuartier = t; }
    TypeQuartier getTypeQuartier() const { return typeQuartier; }
};

class Place : public Hexagone
{
private:
    enum class TypePlace
    {
        Marche,
        Temple,
        Caserne,
        Jardin
    };
    TypePlace typePlace;
    int multiplicateur;

public:
    void setTypePlace(TypePlace t) { typePlace = t; }
    TypePlace getTypePlace() const { return typePlace; }
};

#endif
