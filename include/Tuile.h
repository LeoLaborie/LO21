#ifndef TUILE_H
#define TUILE_H

#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include "Hexagone.h"

struct OffsetQR
{
    int q;
    int r;
};

inline void rotation60deg(OffsetQR &o)
{
    int q = o.q, r = o.r;
    o.q = -r;
    o.r = q + r;
}

/**
 * @class Tuile
 * @brief Représente une tuile composée de plusieurs hexagones
 */
class Tuile
{
private:
    std::vector<Hexagone *> hex;
    std::vector<OffsetQR> offsets;

public:
    /**
     * @class Iterator
     * @brief Itérateur pour parcourir les hexagones d'une tuile
     */
    class Iterator
    {
        std::vector<Hexagone*> &listeHex;        // NON const
        Hexagone *hexCourant;                    // NON const
        int nb;
        int idx = 0;

    public:
        Iterator(std::vector<Hexagone*> &h, int n) 
            : listeHex(h), nb(n)
        {
            if (nb > 0)
                hexCourant = listeHex[idx];
        }

        bool isDone() const { return nb == 0; }

        void next()
        {
            nb--;
            if (nb > 0)
                hexCourant = listeHex[++idx];
        }

        int currentIndex() const
        {
            return idx;
        }

        Hexagone &currentItem()
        {
            return *hexCourant;
        }
    };

    class ConstIterator
    {
        const std::vector<Hexagone*> &listeHex;
        const Hexagone *hexCourant;
        int nb;
        int idx = 0;

    public:
        ConstIterator(const std::vector<Hexagone*> &h, int n)
            : listeHex(h), nb(n)
        {
            if (nb > 0)
                hexCourant = listeHex[idx];
        }

        bool isDone() const { return nb == 0; }

        void next()
        {
            nb--;
            if (nb > 0)
                hexCourant = listeHex[++idx];
        }

        int currentIndex() const
        {
            return idx;
        }

        const Hexagone &currentItem() const
        {
            return *hexCourant;
        }
    };

    /**
     * @brief Retourne un itérateur pour parcourir les hexagones de la tuile
     * @return Iterator : itérateur pour les hexagones
     */
    Iterator getIterator()
    {
        return Iterator(hex, getNbHexa());
    }

    ConstIterator getConstIterator() const
    {
        return ConstIterator(hex, getNbHexa());
    }

    /**
     * @brief Constructeur par défaut de Tuile
     */
    Tuile() = default;

    /**
     * @brief Destructeur de Tuile
     */
    ~Tuile() = default;

    /**
     * @brief Constructeur de Tuile avec trois hexagones
     * @param hex1 Pointeur vers le premier hexagone
     * @param hex2 Pointeur vers le deuxième hexagone
     * @param hex3 Pointeur vers le troisième hexagone
     */
    Tuile(Hexagone *hex1, Hexagone *hex2, Hexagone *hex3);

    /**
     * @brief Constructeur de Tuile avec quatre hexagones
     * @param hex1 Pointeur vers le premier hexagone
     * @param hex2 Pointeur vers le deuxième hexagone
     * @param hex3 Pointeur vers le troisième hexagone
     * @param hex4 Pointeur vers le quatrième hexagone
     */
    Tuile(Hexagone *hex1, Hexagone *hex2, Hexagone *hex3, Hexagone *hex4);

    // Getters

    /**
     * @brief Retourne les offsets de la tuile
     * @return const std::vector<OffsetQR>& : référence constante vers le vecteur des offsets
     */
    const std::vector<OffsetQR> &getOffsets() const { return offsets; }

    /**
     * @brief Retourne le nombre d'hexagones dans la tuile
     * @return size_t : nombre d'hexagones
     */
    size_t getNbHexa() const { return hex.size(); }

    // Manipulations de la tuile

    /**
     * @brief Pivote la tuile de 60 degrés
     */
    void pivoterTuile()
    {
        for (auto &o : offsets)
            rotation60deg(o);
    }

    /**
     * @brief Pivote la tuile de k * 60 degrés
     * @param k Nombre de rotations de 60 degrés à effectuer
     */
    void pivoterTuile(int k)
    {
        k %= 6;
        for (int i = 0; i < k; ++i)
            for (auto &o : offsets)
                rotation60deg(o);
    }

    /**
     * @brief Surcharge de l'opérateur de flux pour afficher la tuile
     * @param os Flux de sortie
     * @param t Tuile à afficher
     * @return std::ostream& : référence vers le flux de sortie modifié
     */
    friend std::ostream &operator<<(std::ostream &os, const Tuile &t)
    {
        if (t.hex.empty())
            return os << "(tuile vide)\n";
        std::map<int, std::vector<size_t>> lignes;
        for (size_t i = 0; i < t.offsets.size(); ++i)
            lignes[t.offsets[i].r].push_back(i);
        for (auto &[r, id] : lignes)
        {
            std::sort(id.begin(), id.end(), [&](size_t a, size_t b)
                      { return t.offsets[a].q < t.offsets[b].q; });
            for (size_t k = 0; k < id.size(); ++k)
            {
                t.hex[id[k]]->afficher(true);
                if (k + 1 < id.size())
                    os << ' ';
            }
            os << '\n';
        }
        return os;
    }

    /**
     * @brief Affiche le Tuile avec sa position
     */
    void afficherDetails() const
    {
        std::cout << ' ';
        for (const auto *h : hex)
        {
            h->afficher(false);
            std::cout << ' ';
        }
    }
};

#endif
