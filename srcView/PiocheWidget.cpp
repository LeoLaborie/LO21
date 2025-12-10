#include "PiocheWidget.h"
#include <QBrush>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPen>

PiocheWidget::PiocheWidget(int width, int height, QWidget* parent)
    : QGraphicsView(parent)
{
    //création de la scène
    piocheScene = new QGraphicsScene(this);
    setScene(piocheScene);

    //application des différentes préférences
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(width, height);

    //ajout dans la scène
    piocheScene->setSceneRect(0, 0, width, height);
    piocheZoneRectItem = piocheScene->addRect(0, 0, width, height, QPen(Qt::NoPen), QBrush(Qt::red));
    piocheZoneRect = piocheZoneRectItem->rect();
}
