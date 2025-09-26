#ifndef HEXAGONE_H
#define HEXAGONE_H

#include <iostream>
#include <vector>
#include <string.h>

using namespace std;

class Tuile;

class Hexagone
{

private:
    vector<Hexagone *> voisins;
    int x;
    int y;
    int z;
    Tuile *parent;

public:
    void setParent(Tuile *p) { parent = p; }
    void setVoisins(vector<Hexagone *> v) { voisins = v; }
    void addVoisin(Hexagone *v) { voisins.push_back(v); }
    std::vector<Hexagone *> &getVoisins() { return voisins; }

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
};

class Carriere : public Hexagone
{
    using Hexagone::Hexagone; // hérite du constructeur d'hexagone
};

enum class TypeQuartier
{
    Habitation,
    Marche,
    Temple,
    Caserne,
    Jardin

};

class Quartier : public Hexagone
{
private:
    TypeQuartier typeQuartier;

public:
    Quartier(int x, int y, int z, TypeQuartier type,
             Tuile *p = nullptr, vector<Hexagone *> v = {})
        : Hexagone(x, y, z, p, v), typeQuartier(type) {}
    void setTypeQuartier(TypeQuartier t) { typeQuartier = t; }
    TypeQuartier getTypeQuartier() const { return typeQuartier; }
};

enum class TypePlace
{
    Habitation,
    Marche,
    Temple,
    Caserne,
    Jardin
};

class Place : public Hexagone
{
private:
    TypePlace typePlace;
    int multiplicateur;

public:
    Place(int x, int y, int z, TypePlace type, int mult,
          Tuile *p = nullptr, vector<Hexagone *> v = {})
        : Hexagone(x, y, z, p, v), typePlace(type), multiplicateur(mult) {}

    void setTypePlace(TypePlace t) { typePlace = t; }

    TypePlace getTypePlace() const { return typePlace; }
};

#endif
