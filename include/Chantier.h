#ifndef CHANTIER_H
#define CHANTIER_H

#include <vector>
#include <iostream>
#include "Tuile.h"
#include "couleurs_console.h"

/**
 * @class Chantier
 * @brief Représente le chantier contenant les tuiles disponibles pour les joueurs.
 */
class Chantier
{
private:
    std::vector<Tuile> tuilesChantier;

public:
    /**
     * @brief Constructeur par défaut de Chantier
     */
    Chantier() {};

    /**
     * @brief Destructeur de Chantier
     */
    ~Chantier() = default;

    // Getters

    /**
     * @brief Retourne le nombre de tuiles dans le chantier
     * @return int : nombre de tuiles dans le chantier
     */
    int getTaille() const { return tuilesChantier.size(); }

    /**
     * @brief Retourne les tuiles du chantier
     * @return const std::vector<Tuile>& : référence constante vers le vecteur de tuiles
     */
    const std::vector<Tuile> &getTuiles() const { return tuilesChantier; }

    // Manipulations

    /**
     * @brief Ajoute une tuile au chantier
     * @param t Tuile à ajouter
     */
    void ajouterTuile(const Tuile &t) { tuilesChantier.push_back(t); }

    /**
     * @brief Retire une tuile du chantier
     * @param id Identifiant de la tuile à retirer
     */
    void retirerTuile(int id);

    /**
     * @brief Surcharge de l'opérateur d'affichage pour le Chantier
     * @param os Flux de sortie
     * @param c Chantier à afficher
     * @return Flux de sortie modifié
     */
    friend std::ostream& operator<<(std::ostream& os,const Chantier& c){
        os << "\nChantier contient " << c.tuilesChantier.size() << " tuiles :\n";
        for (size_t i = 0; i < c.tuilesChantier.size(); i++) {
            texte_gras_on();
            os << "\n\t ---- " << i << " ----\n\n";
            texte_reset();
            os<<c.tuilesChantier[i];
        }
        os << std::endl;
        return os;
    }
};

#endif
