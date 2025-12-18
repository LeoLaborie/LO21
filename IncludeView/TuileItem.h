#ifndef TUILEITEM_H
#define TUILEITEM_H

#include <QGraphicsItemGroup>
#include <QObject>
#include <QPoint>
#include <QPointF>

#include "HexItem.h"
#include "Tuile.h"

/**
 * @brief Représentation graphique d'une tuile (ensemble d'hexagones) dans la scène.
 * Gère les interactions (rotation/déplacement) et assure l'alignement sur la grille axiale.
 */
class TuileItem : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
    public:
        /**
         * @brief Définit le mode de fonctionnement de la TuileItem si elle est dans le pioche ou dans le zone de jeu.
         */
        enum class Mode
        {
            Pioche,
            Placement,
            ZoneJeu
        };
        /**
         * @brief Construit une tuile graphique à partir d'une tuile métier et crée les HexItem associés.
         */
        TuileItem(const Tuile & modele, QGraphicsItem* parent = nullptr, Mode m = Mode::Pioche, int tailleTuile = 50, int indice = -1);
        /**
         * @brief Pivote la tuile de 60° autour de son centre si l'interaction est autorisée.
         */
        void rotate60();
        /**
         * @brief Repositionne la tuile pour que la référence tombe exactement sur la grille axiale.
         */
        void replacerCorrectement();
        /**
         * @brief Active/désactive le déplacement et la rotation de la tuile.
         */
        void setInteractivite(bool autoriserDeplacement, bool autoriserRotation);
        /**
         * @brief Met à jour l'indice de la tuile dans la pioche.
         */
        void setIndiceDansPioche(unsigned int nouvelIndice);
        unsigned int getIndiceDansPioche() const
        {
            return indice;
        }
        /**
         * @brief Changer le mode de la TuileItem.
         */
        void setMode(Mode m)
        {
            mode = m;
        }
        Mode modeCourant() const
        {
            return mode;
        }

        /**
         * @brief Ajuste la taille des hexagones et recalcule le centre de rotation.
         *
         * @param nouvelleTaille diamètre utilisé pour redimensionner chaque HexItem.
         */
        void setTaille(int nouvelleTaille);
        /**
         * @brief Définit le niveau visuel de la tuile (hauteur affichée).
         */
        void setNiveauGraphique(int niveau);
        int getNiveauGraphique() const
        {
            return niveauHauteur;
        }
        QPoint coordonneesAxiales(const QPointF& origineScene = QPointF()) const;

    signals:
    void rightClicked();
    void estPiocher(int indice);
    void demandeValidationPlacement(TuileItem* tuile);
    void deplacementDemarre(TuileItem* tuile);
    void rotationEffectuee(int pas);

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    HexItem* hexRef = nullptr;
    int tailleHex = 50;
    bool rotationAutorisee = true;
    unsigned int indice;  // utiliser que quand la tuile item est dans la pioche sinon inutile
    Mode mode;
    int niveauHauteur = 0;
};

#endif  // TUILEITEM_H
