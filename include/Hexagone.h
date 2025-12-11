#ifndef HEXAGONE_H
#define HEXAGONE_H

#include <iostream>
#include <utility>
#include <vector>

#include "couleurs_console.h"

class Tuile;

/**
 * @enum TypeHex
 * @brief Types possibles pour un hexagone
 */
enum class TypeHex
{
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
    Carriere
};

/**
 * @class Hexagone
 * @brief Représente un hexagone dans le jeu
 */
class Hexagone
{
private:
    std::vector<Hexagone *> voisins;
    int x{}, y{}, z{};
    Tuile *parent{};
    bool est_recouvert = false;
    TypeHex type_ = TypeHex::Habitation;

public:
    /**
     * @brief Constructeur de Hexagone
     * @param x_coord Coordonnée x
     * @param y_coord Coordonnée y
     * @param z_coord Coordonnée z
     * @param type Type de l'hexagone
     * @param p Pointeur vers la tuile parente (default nullptr)
     * @param v Vecteur de pointeurs vers les hexagones voisins (optionnel)
     */
    Hexagone(int x_coord, int y_coord, int z_coord, TypeHex type, Tuile *p = nullptr, std::vector<Hexagone *> v = {})
        : voisins(std::move(v)), x(x_coord), y(y_coord), z(z_coord), parent(p), type_(type) {}

    /**
     * @brief Destructeur de Hexagone
     */
    ~Hexagone() = default;

    // Getters

    /**
     * @brief Retourne le pointeur vers la tuile parente
     * @return Tuile* : pointeur vers la tuile parente
     */
    Tuile *getParent()
    {
        return parent;
    }

    /**
     * @brief Retourne le pointeur constant vers la tuile parente
     * @return const Tuile* : pointeur constant vers la tuile parente
     */
    const Tuile *getParent() const
    {
        return parent;
    }

    /**
     * @brief Retourne les voisins de l'hexagone
     * @return const std::vector<Hexagone *>& : référence constante vers le vecteur des voisins
     */
    const std::vector<Hexagone *> &getVoisins() const
    {
        return voisins;
    }

    /**
     * @brief Indique si l'hexagone est recouvert
     * @return bool : true si recouvert, false sinon
     */
    bool getEstRecouvert() const
    {
        return est_recouvert;
    }

    /**
     * @brief Retourne la coordonnée x de l'hexagone
     * @return int : coordonnée x
     */
    int getX() const
    {
        return x;
    }

    /**
     * @brief Retourne la coordonnée y de l'hexagone
     * @return int : coordonnée y
     */
    int getY() const
    {
        return y;
    }

    /**
     * @brief Retourne la coordonnée z de l'hexagone
     * @return int : coordonnée z
     */
    int getZ() const
    {
        return z;
    }

    /**
     * @brief Retourne le type de l'hexagone
     * @return TypeHex : type de l'hexagone
     */
    TypeHex getType() const
    {
        return type_;
    }

    // Setters

    /**
     * @brief Définit les voisins de l'hexagone
     * @param v Vecteur de pointeurs vers les hexagones voisins
     */
    void setVoisins(std::vector<Hexagone *> v)
    {
        voisins = std::move(v);
    }

    /**
     * @brief Définit le pointeur vers la tuile parente
     * @param p Pointeur vers la tuile parente
     */
    void setParent(Tuile *p)
    {
        parent = p;
    }

    /**
     * @brief Définit si l'hexagone est recouvert
     * @param v Booléen indiquant si l'hexagone est recouvert (default true)
     */
    void setEstRecouvert(bool v = true)
    {
        est_recouvert = v;
    }

    /**
     * @brief Définit le type de l'hexagone
     * @param t TypeHex : type de l'hexagone
     */
    void setType(TypeHex t)
    {
        type_ = t;
    }

    /**
     * @brief Définit les coordonnées de l'hexagone
     * @param x_coord Coordonnée x
     * @param y_coord Coordonnée y
     * @param z_coord Coordonnée z
     */
    void setCoord(int x_coord, int y_coord, int z_coord)
    {
        x = x_coord;
        y = y_coord;
        z = z_coord;
    }

    // Autres méthodes

    /**
     * @brief Ajoute un voisin à l'hexagone
     * @param v Pointeur vers l'hexagone voisin à ajouter
     */
    void addVoisin(Hexagone *v)
    {
        voisins.push_back(v);
    }

    /**
     * @brief Vérifie si l'hexagone est une place
     * @return bool : true si c'est une place, false sinon
     */
    bool isPlace() const
    {
        switch (type_)
        {
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

    /**
     * @brief Vérifie si l'hexagone est un quartier
     * @return bool : true si c'est un quartier, false sinon
     */
    bool isQuartier() const
    {
        switch (type_)
        {
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

    /**
     * @brief Vérifie si l'hexagone est une carrière
     * @return bool : true si c'est une carrière, false sinon
     */
    bool isCarriere() const
    {
        return type_ == TypeHex::Carriere;
    }

    /**
     * @brief Affiche les informations de l'hexagone
     * @param court Booléen indiquant si l'affichage doit être court (default false)
     */
    void afficher(bool court = false) const;
};

#endif
