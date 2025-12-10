#ifndef PIOCHEWIDGET_H
#define PIOCHEWIDGET_H

#include <QGraphicsView>
#include <QRectF>

class QGraphicsRectItem;

/**
 * @brief Vue dédiée à l'affichage de la pioche.
 */
class PiocheWidget : public QGraphicsView
{
    Q_OBJECT

public:
    PiocheWidget(int width, int height, QWidget* parent = nullptr);

    QGraphicsScene* getPiocheScene() const { return piocheScene; }
    QRectF getZoneRect() const { return piocheZoneRect; }

private:
    QGraphicsScene* piocheScene = nullptr;
    QGraphicsRectItem* piocheZoneRectItem = nullptr;
    QRectF piocheZoneRect;
};

#endif
