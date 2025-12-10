#ifndef TUILEITEM_H
#define TUILEITEM_H

#include <QGraphicsItemGroup>
#include <QPointF>
#include <QObject>
#include "HexItem.h"
#include "Tuile.h"

/**
 * @brief Représentation graphique d'une tuile (ensemble d'hexagones) dans la scène.
 * Gère les interactions (rotation/déplacement) et assure l'alignement sur la grille axiale.
 */
class TuileItem : public QObject ,public QGraphicsItemGroup {
    Q_OBJECT
    HexItem* hexRef = nullptr;
    QPointF plateauOrigin{0,0};
    bool rotationAutorisee = true;
public:
    /**
     * @brief Construit une tuile graphique vide pour une éventuelle composition manuelle.
     */
    explicit TuileItem(QGraphicsItem* parent = nullptr);
    /**
     * @brief Construit une tuile graphique à partir d'une tuile métier et crée les HexItem associés.
     */
    TuileItem(Tuile& modele, QGraphicsItem* parent = nullptr);
    void setCoordonnee(int x,int y);
    /**
     * @brief Ajoute dynamiquement un hexagone à la tuile graphique.
     */
    void addHex(HexItem* hex);
    /**
     * @brief Pivote la tuile de 60° autour de son centre si l'interaction est autorisée.
     */
    void rotate60();
    /**
     * @brief Repositionne la tuile pour que la référence tombe exactement sur la grille axiale.
     */
    void replacerCorrectement();
    void debugMarkers();
    /**
     * @brief Définit l'origine (repère) utilisée pour convertir axial <-> pixels.
     */
    void setPlateauOrigin(const QPointF& origin) { plateauOrigin = origin; }
    /**
     * @brief Active/désactive le déplacement et la rotation de la tuile.
     */
    void setInteractivite(bool autoriserDeplacement, bool autoriserRotation);
    /**
     * @brief Déplace la tuile pour que la référence soit posée sur les coordonnées axiales (q,r).
     */
    void positionnerSurAxial(int q, int r);
signals:
    void rightClicked();

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif // TUILEITEM_H
