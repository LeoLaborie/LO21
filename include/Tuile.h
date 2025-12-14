#ifndef TUILE_H
#define TUILE_H

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

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
    std::vector<std::shared_ptr<Hexagone>> hex;
    std::vector<OffsetQR> offsets;

public:
    /**
     * @class Iterator
     * @brief Itérateur pour parcourir les hexagones d'une tuile
     */
    class Iterator
    {
        std::vector<std::shared_ptr<Hexagone>> &listeHex; // NON const
        Hexagone *hexCourant;              // NON const
        size_t nb;
        size_t idx = 0;

    public:
        /**
         * @brief Constructeur de l'itérateur
         * @param h Vecteur de pointeurs vers les hexagones
         * @param n Nombre d'hexagones
         */
        Iterator(std::vector<std::shared_ptr<Hexagone>> &h, size_t n)
            : listeHex(h), nb(n)
        {
            if (nb > 0)
                hexCourant = listeHex[idx].get();
        }

        /**
         * @brief Vérifie si l'itérateur a atteint la fin
         * @return bool : true si terminé, false sinon
         */
        bool isDone() const { return nb == 0; }

        /**
         * @brief Avance l'itérateur au prochain hexagone
         */
        void next()
        {
            nb--;
            if (nb > 0)
                hexCourant = listeHex[++idx].get();
        }

        /**
         * @brief Retourne l'indice courant de l'itérateur
         * @return size_t : indice courant
         */
        size_t currentIndex() const
        {
            return idx;
        }

        /**
         * @brief Retourne l'hexagone courant
         * @return Hexagone& : référence vers l'hexagone courant
         */
        Hexagone &currentItem()
        {
            return *hexCourant;
        }
    };

    /**
     * @class ConstIterator
     * @brief Itérateur constant pour parcourir les hexagones d'une tuile
     */
    class ConstIterator
    {
        const std::vector<std::shared_ptr<Hexagone>> &listeHex;
        const Hexagone *hexCourant;
        size_t nb;
        size_t idx = 0;

    public:
        /**
         * @brief Constructeur de l'itérateur constant
         * @param h Vecteur constant de pointeurs vers les hexagones
         * @param n Nombre d'hexagones
         */
        ConstIterator(const std::vector<std::shared_ptr<Hexagone>> &h, size_t n)
            : listeHex(h), nb(n)
        {
            if (nb > 0)
                hexCourant = listeHex[idx].get();
        }

        /**
         * @brief Vérifie si l'itérateur a atteint la fin
         * @return bool : true si terminé, false sinon
         */
        bool isDone() const { return nb == 0; }

        /**
         * @brief Avance l'itérateur au prochain hexagone
         */
        void next()
        {
            nb--;
            if (nb > 0)
                hexCourant = listeHex[++idx].get();
        }

        /**
         * @brief Retourne l'indice courant de l'itérateur
         * @return size_t : indice courant
         */
        size_t currentIndex() const
        {
            return idx;
        }

        /**
         * @brief Retourne l'hexagone courant
         * @return const Hexagone& : référence constante vers l'hexagone courant
         */
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

    /**
     * @brief Retourne un itérateur constant pour parcourir les hexagones de la tuile
     * @return ConstIterator : itérateur constant pour les hexagones
     */
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
    const std::vector<OffsetQR> &getOffsets() const
    {
        return offsets;
    }

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
        for (const auto &h : hex)
        {
            h->afficher(false);
            std::cout << ' ';
        }
    }
};

#endif
