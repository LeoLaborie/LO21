#ifndef HEXAGONE_H
#define HEXAGONE_H

#include <iostream>
#include <vector>
#include <string.h>
#include "Tuile.h"

using namespace std;

class Hexagone
{
public:
    void setParent(Tuile *p) { parent = p; }
    void setVoisins(vector<Hexagone *> v) { voisins = v; }
    void addVoisin(Hexagone * v) { voisins.push_back(v); }
    vector<Hexagone *> getVoisins() { return voisins; }

    int getX() { return x; }
    int getY() { return y; }

    Hexagone(int x_coord, int y_coord, int z_coord, Tuile *p = nullptr, vector<Hexagone *> v = {})
    {
        voisins = v;
        x = x_coord;
        y = y_coord;
        z = z_coord;
        parent = p;
    }


private:
    vector<Hexagone *> voisins;
    int x;
    int y;
    int z;
    Tuile *parent;
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
