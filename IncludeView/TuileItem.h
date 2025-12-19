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
         * @brief Définit l'origine (en scène) utilisée pour l'alignement sur la grille.
         *
         * Quand une tuile est sur le plateau, l'origine correspond typiquement au centre logique de la zone de jeu.
         */
        void definirOrigineGrille(const QPointF& origine)
        {
            origineGrilleScene = origine;
        }
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
         *
         * En plus du Z-order (`setZValue`), la tuile est légèrement décalée en Y pour rendre
         * les étages plus lisibles tout en conservant des coordonnées axiales correctes.
         */
        void setNiveauGraphique(int niveau);
        int getNiveauGraphique() const
        {
            return niveauHauteur;
        }
        /**
         * @brief Retourne les coordonnées axiales (q,r) de la tuile dans la scène.
         * @param origineScene Origine utilisée comme repère (ex: centre de la scène).
         */
        QPoint coordonneesAxiales(const QPointF& origineScene = QPointF()) const;
    public slots:
        void ModifierCouleurEtage(int z);

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
    QPointF origineGrilleScene = QPointF(0.0, 0.0);
};

#endif  // TUILEITEM_H
