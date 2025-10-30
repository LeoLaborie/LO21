#ifndef HEXITEM_H
#define HEXITEM_H
#pragma once
#include <QGraphicsPolygonItem>
#include "Hexagone.h"

class HexItem : public QGraphicsPolygonItem {
private:
public:
    HexItem(const Hexagone* model, double size = 36.0);
};


QPointF axialVersPixel(int q, int r, double size);



#endif // HEXITEM_H
