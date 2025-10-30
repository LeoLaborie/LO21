#ifndef TUILEITEM_H
#define TUILEITEM_H

#include <QGraphicsItemGroup>
#include <QObject>
#include "HexItem.h"
#include "Tuile.h"

class TuileItem : public QObject ,public QGraphicsItemGroup {
    Q_OBJECT
    HexItem* hexRef;
public:
    explicit TuileItem(QGraphicsItem* parent = nullptr);
    TuileItem(Tuile& modele, QGraphicsItem* parent = nullptr);
    void setCoordonnee(int x,int y);
    void addHex(HexItem* hex);
    void rotate60();
    void replacerCorrectement();
    void debugMarkers();
signals:
    void rightClicked();

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif // TUILEITEM_H
