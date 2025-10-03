#ifndef HEXAGONE_H
#define HEXAGONE_H

#include <vector>

class Tuile;

class Hexagone
{
private:
    std::vector<Hexagone *> voisins;
    int x{}, y{}, z{};
    Tuile *parent{};
    bool est_recouvert = false;

public:
    virtual ~Hexagone() = default;

    void setParent(Tuile *p) { parent = p; }
    void setVoisins(std::vector<Hexagone *> v) { voisins = std::move(v); }
    void addVoisin(Hexagone *v) { voisins.push_back(v); }
    const std::vector<Hexagone *> &getVoisins() const { return voisins; }

    bool getEstRecouvert() const { return est_recouvert; }
    void setEstRecouvert() { est_recouvert = true; }
    Tuile *getParent() { return parent; }

    int getX() const { return x; }
    int getY() const { return y; }
    int getZ() const { return z; }

    void SetCoord(int x_coord, int y_coord, int z_coord)
    {
        x = x_coord;
        y = y_coord;
        z = z_coord;
    }

    Hexagone(int x_coord, int y_coord, int z_coord,
             Tuile *p = nullptr, std::vector<Hexagone *> v = {})
        : voisins(std::move(v)), x(x_coord), y(y_coord), z(z_coord), parent(p) {}
};

class Carriere : public Hexagone
{
    using Hexagone::Hexagone;
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
             Tuile *p = nullptr, std::vector<Hexagone *> v = {})
        : Hexagone(x, y, z, p, std::move(v)), typeQuartier(type) {}

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
          Tuile *p = nullptr, std::vector<Hexagone *> v = {})
        : Hexagone(x, y, z, p, std::move(v)), typePlace(type), multiplicateur(mult) {}

    void setTypePlace(TypePlace t) { typePlace = t; }
    TypePlace getTypePlace() const { return typePlace; }
    int getMultiplicateur() const { return multiplicateur; }
};

#endif
