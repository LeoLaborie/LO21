#include "ChantierWidget.h"
#include <QBrush>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPen>

ChantierWidget::ChantierWidget(int width, int height, QWidget* parent)
    : QGraphicsView(parent)
{
    //création de la scène
    chantierScene = new QGraphicsScene(this);
    setScene(chantierScene);

    //application des différentes préférences
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(width, height);

    //ajout dans la scène
    chantierScene->setSceneRect(0, 0, width, height);
    chantierZoneRectItem = chantierScene->addRect(0, 0, width, height, QPen(Qt::NoPen), QBrush(Qt::red));
    chantierZoneRect = chantierZoneRectItem->rect();
}
