#ifndef CHANTIERWIDGET_H
#define CHANTIERWIDGET_H

#include "TuileItem.h"
#include <QGraphicsView>
#include <QRectF>
#include <vector>
#include <memory>
class QGraphicsRectItem;
/**
 * @brief Vue dédiée à l'affichage du chantier.
 */
class ChantierWidget : public QGraphicsView
{
    Q_OBJECT

public:
    ChantierWidget(int width, int height, QWidget* parent = nullptr);
    QGraphicsScene* getChantierScene() const { return chantierScene; }
    QRectF getZoneRect() const { return chantierZoneRect; }
    
private:
    QGraphicsScene* chantierScene = nullptr;
    QGraphicsRectItem* chantierZoneRectItem = nullptr;
    QRectF chantierZoneRect;
    std::vector <unique_ptr TuileItem>
private slots:
    void piocherTuile(int indice);
};

#endif // CHANTIERWIDGET_H
