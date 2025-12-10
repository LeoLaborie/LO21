#ifndef ZONEJEUWIDGET_H
#define ZONEJEUWIDGET_H

#include <QGraphicsView>
#include <QRectF>

class QGraphicsRectItem;

/**
 * @brief Vue principale contenant la scène du plateau de jeu.
 */
class ZoneJeuWidget : public QGraphicsView
{
    Q_OBJECT

public:
    ZoneJeuWidget(int width, int height, QWidget* parent = nullptr);

    QGraphicsScene* getPlateauScene() const { return zoneJeuScene; }
    QRectF getZoneRect() const { return zoneJeuRect; }

private:
    QGraphicsScene* zoneJeuScene = nullptr;
    QGraphicsRectItem* zoneJeuRectItem = nullptr;
    QRectF zoneJeuRect;
};

#endif
