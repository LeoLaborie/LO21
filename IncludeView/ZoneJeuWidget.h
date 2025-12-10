#ifndef ZONEJEUWIDGET_H
#define ZONEJEUWIDGET_H

#include <QGraphicsView>
#include <QRectF>
#include "TuileItem.h"

class QGraphicsRectItem;

/**
 * @brief Vue principale contenant la scène du plateau de jeu.
 */
class ZoneJeuWidget : public QGraphicsView
{
    Q_OBJECT

public:
    ZoneJeuWidget(int width, int height, QWidget* parent = nullptr);
    void ajouterTuileDansZoneJeu(TuileItem * t,int x,int y);
    QGraphicsScene* getPlateauScene() const { return zoneJeuScene; }
    QRectF getZoneRect() const { return zoneJeuRect; }
    void setBlocageInteractions(bool bloque);
    bool interactionsBloquees() const { return blocageInteractions; }

public slots:
    void placerTuileDansZoneJeu(TuileItem* tuile);

private:
    QGraphicsScene* zoneJeuScene = nullptr;
    QGraphicsRectItem* zoneJeuRectItem = nullptr;
    QRectF zoneJeuRect;
    std::vector<TuileItem*> tuilesZoneJeu;
    bool blocageInteractions = false;
};

#endif
