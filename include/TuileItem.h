#ifndef TUILEITEM_H
#define TUILEITEM_H

#include <QGraphicsItemGroup>
#include <QPointF>
#include <QObject>
#include "HexItem.h"
#include "Tuile.h"

class TuileItem : public QObject ,public QGraphicsItemGroup {
    Q_OBJECT
    HexItem* hexRef;
    QPointF plateauOrigin{0,0};
public:
    explicit TuileItem(QGraphicsItem* parent = nullptr);
    TuileItem(Tuile& modele, QGraphicsItem* parent = nullptr);
    void setCoordonnee(int x,int y);
    void addHex(HexItem* hex);
    void rotate60();
    void replacerCorrectement();
    void debugMarkers();
    void setPlateauOrigin(const QPointF& origin) { plateauOrigin = origin; }
signals:
    void rightClicked();

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif // TUILEITEM_H
