#include "PlateauWidget.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QPainter>
#include <QFrame>
#include <QPen>

void PlateauWidget::ajouterTuile(Tuile& t)
{
    if (!plateauScene)
        return;

    auto* tuileItem = new TuileItem(t);
    plateauScene->addItem(tuileItem);

    if (zonePlateauRectItem) {
        const QPointF center = zonePlateauRect.center();
        const QRectF itemBounds = tuileItem->boundingRect();
        tuileItem->setPos(center - itemBounds.center());
    }
}

PlateauWidget::PlateauWidget(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(1920, 1080);
    plateauScene = new QGraphicsScene(this);
    auto* view = new QGraphicsView(plateauScene, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setFrameStyle(QFrame::NoFrame);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(view);

    // Zone du plateau
    zonePlateauRectItem = plateauScene->addRect(0, 0,width() - 250, height(),QPen(Qt::NoPen),QBrush(Qt::yellow));
    zonePlateauRect = zonePlateauRectItem->rect();

    // Score
    plateauScene->addRect(width() - 250, 0,250, 250,QPen(Qt::NoPen),QBrush(Qt::blue));

    // Pioche
    plateauScene->addRect(width() - 250, 250,250, height() - 250,QPen(Qt::NoPen),QBrush(Qt::green));
    plateauScene->setSceneRect(0, 0, width(), height());

}


