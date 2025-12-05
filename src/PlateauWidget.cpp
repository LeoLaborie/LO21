#include "PlateauWidget.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QFrame>
#include <QPen>
#include <QLabel>

void PlateauWidget::ajouterTuile(Tuile& t)
{
    if (!plateauScene)
        return;

    auto* tuileItem = new TuileItem(t);
    tuileItem->setPlateauOrigin(zonePlateauRect.topLeft());
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

    int scoreBandeHeight = 50;
    int piocheWidth = 250;

    plateauScene = new QGraphicsScene(this);
    auto* view = new QGraphicsView(plateauScene, this);
    view->setFrameStyle(QFrame::NoFrame);
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto* bandeScore = new QWidget(this);
    bandeScore->setFixedHeight(scoreBandeHeight);
    bandeScore->setStyleSheet("background-color: #f0f0f0;");
    auto* scoreLayout = new QHBoxLayout(bandeScore);
    scoreLayout->setContentsMargins(20, 10, 20, 10);
    scoreLayout->addWidget(new QLabel("Scores", bandeScore));

    layout->addWidget(bandeScore);
    layout->addWidget(view, 1);
    plateauScene->setSceneRect(0, 0, width(), height() - scoreBandeHeight);

    // Pioche à droite
    plateauScene->addRect(width() - piocheWidth, 0, piocheWidth, height() - scoreBandeHeight, QPen(Qt::NoPen), QBrush(Qt::red));

    // Zone du plateau
    zonePlateauRectItem = plateauScene->addRect(0, 0, width() - piocheWidth, height() - scoreBandeHeight, QPen(Qt::NoPen), QBrush(Qt::blue));
    zonePlateauRect = zonePlateauRectItem->rect();
}
