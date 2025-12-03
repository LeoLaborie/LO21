#include "PlateauWidget.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QPainter>
#include <QFrame>

PlateauWidget::PlateauWidget(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(1920, 1080);

    auto* scene = new QGraphicsScene(this);
    auto* view = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setFrameStyle(QFrame::NoFrame);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(view);
    
    auto* zonePlateau = scene->addRect(0, 0, width() - 250, height());
    zonePlateau->setBrush(Qt::yellow);

    auto* score = scene->addRect(width() - 250, 0, 250, 250);
    score->setBrush(Qt::blue);

    auto* pioche = scene->addRect(width() - 250, 250, 250, height() - 250);
    pioche->setBrush(Qt::green);

    scene->setSceneRect(0, 0, width(), height());
}
