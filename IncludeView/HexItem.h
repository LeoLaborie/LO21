#ifndef HEXITEM_H
#define HEXITEM_H
#pragma once
#include <QGraphicsPolygonItem>
#include "Hexagone.h"

/**
 * @brief Représentation graphique d'un Hexagone métier dans la scène Qt.
 */
class HexItem : public QGraphicsPolygonItem {
public:
    /**
     * @brief Crée l'hexagone graphique en positionnant et texturant l'item selon son modèle.
     *
     * @param model Hexagone provenant du modèle métier à représenter.
     * @param size Rayon utilisé pour construire le polygone et choisir l'image.
     */
    HexItem(const Hexagone* model, double size = 36.0);

    /**
     * @brief Redimensionne l'hexagone tout en conservant son apparence.
     *
     * @param nouvelleTaille Rayon du nouvel hexagone.
     */
    void setTaille(double nouvelleTaille);
};

/**
 * @brief Convertit des coordonnées axiales (q,r) en coordonnées pixel isométriques.
 */
QPointF axialVersPixel(int q, int r, double size);

#endif // HEXITEM_H
