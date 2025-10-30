#ifndef HEXAGONE_H
#define HEXAGONE_H

#include <vector>
#include <utility>
#include <iostream>
#include "couleurs_console.h"

class Tuile;

enum class TypeHex {
    Habitation,
    Marche,
    Temple,
    Caserne,
    Jardin,
    PHabitation,
    PMarche,
    PTemple,
    PCaserne,
    PJardin,
    Depart,
    Carriere
};

class Hexagone
{
private:
    std::vector<Hexagone*> voisins;
    int x{}, y{}, z{};
    Tuile* parent{};
    bool est_recouvert = false;

    TypeHex type_ = TypeHex::Habitation;
    int multiplicateur_ = 1;

public:
    Hexagone(int x_coord, int y_coord, int z_coord,
             TypeHex type,
             int multiplicateur = 1,
             Tuile* p = nullptr,
             std::vector<Hexagone*> v = {})
        : voisins(std::move(v)),
        x(x_coord), y(y_coord), z(z_coord),
        parent(p),
        type_(type),
        multiplicateur_(multiplicateur) {}

    void setParent(Tuile* p) { parent = p; }
    Tuile* getParent() { return parent; }

    void setVoisins(std::vector<Hexagone*> v) { voisins = std::move(v); }
    void addVoisin(Hexagone* v) { voisins.push_back(v); }
    const std::vector<Hexagone*>& getVoisins() const { return voisins; }

    bool getEstRecouvert() const { return est_recouvert; }
    void setEstRecouvert(bool v = true) { est_recouvert = v; }


    int getX() const { return x; }
    int getY() const { return y; }
    int getZ() const { return z; }
    void setCoord(int x_coord, int y_coord, int z_coord) {
        x = x_coord; y = y_coord; z = z_coord;
    }


    TypeHex getType() const { return type_; }
    void setType(TypeHex t) { type_ = t; }

    bool isPlace() const {
        switch (type_) {
        case TypeHex::PHabitation:
        case TypeHex::PMarche:
        case TypeHex::PTemple:
        case TypeHex::PCaserne:
        case TypeHex::PJardin:
            return true;
        default:
            return false;
        }
    }
    bool isQuartier() const {
        switch (type_) {
        case TypeHex::Habitation:
        case TypeHex::Marche:
        case TypeHex::Temple:
        case TypeHex::Caserne:
        case TypeHex::Jardin:
            return true;
        default:
            return false;
        }
    }
    bool isCarriere() const { return type_ == TypeHex::Carriere; }
    bool isDepart()   const { return type_ == TypeHex::Depart; }

    int  getMultiplicateur() const { return multiplicateur_; }
    void setMultiplicateur(int m)  { multiplicateur_ = m; }

    void afficher() const;
};

#endif
