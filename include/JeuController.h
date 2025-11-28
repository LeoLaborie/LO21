#pragma once

#include <array>
#include <vector>
#include <string>
#include "Partie.h"

/**
 * @brief Contrôleur responsable d'orchestrer une partie complète.
 *
 * Cette classe regroupe l'ensemble de la logique qui était précédemment
 * dispersée dans le main ou dans des fonctions libres. Elle peut être
 * réutilisée aussi bien par l'interface console que par une future
 * interface Qt.
 */
class JeuController
{
public:
    JeuController() = default;

    /**
     * @brief Lance la boucle d'interaction en console.
     */
    void executerConsole();

private:
    Partie creerNouvellePartie();
    bool chargerDepuisSauvegarde(Partie &partie);
    bool boucleDeJeu(Partie &partie);
    void traiterTourHumain(Partie &partie);
    void traiterTourIA(Partie &partie);
    bool phaseManipulationTuile(Joueur &joueur, Partie &partie, Tuile &tuilePiochee);
    void phasePlacementTuile(Joueur &joueur);
    Position demanderPosition() const;
    bool confirmerQuitter(Partie &partie);
};
