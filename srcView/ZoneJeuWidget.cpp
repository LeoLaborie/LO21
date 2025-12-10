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
void ZoneJeuWidget::ajouterTuileDansZoneJeu(TuileItem* t, int x, int y)
{
    if (!t) return;

    // Définir la position de la tuile
    t->setPos(x, y);

    // Ajouter à la scène
    zoneJeuScene->addItem(t);

    // Stocker la tuile dans le tableau
    tuilesZoneJeu.push_back(t);
}

void ZoneJeuWidget::placerTuileDansZoneJeu(TuileItem* tuile)
{
    if (!tuile)
        return;

    const QPointF centre = zoneJeuRect.center();
    ajouterTuileDansZoneJeu(tuile, static_cast<int>(centre.x()), static_cast<int>(centre.y()));
}
