#include "ZoneJeuWidget.h"
#include <QBrush>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPen>

ZoneJeuWidget::ZoneJeuWidget(int width, int height, QWidget* parent)
    : QGraphicsView(parent)
{
    //création de la scène
    zoneJeuScene = new QGraphicsScene(this);
    setScene(zoneJeuScene);

    //application de paramètre et préférence
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    //on applique la taille et création de rectangle pour visualiser
    zoneJeuScene->setSceneRect(0, 0, width, height);
    zoneJeuRectItem = zoneJeuScene->addRect(0, 0, width, height, QPen(Qt::NoPen), QBrush(Qt::blue));
    zoneJeuRect = zoneJeuRectItem->rect();
}
