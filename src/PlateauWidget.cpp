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
#include <array>


PlateauWidget::PlateauWidget(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(1920, 1080);

    int colonneDroiteLargeur = 250;
    int scoreWidgetSize = 250;
    /* Gestion plateau */
    plateauScene = new QGraphicsScene(this);
    piocheScene = new QGraphicsScene(this);
    plateauView = new QGraphicsView(plateauScene, this);
    plateauView->setFrameStyle(QFrame::NoFrame);
    plateauView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    plateauView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    piocheView = new QGraphicsView(piocheScene, this);
    piocheView->setFrameStyle(QFrame::NoFrame);
    piocheView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    piocheView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(plateauView, 1);

    auto* panneauDroit = new QWidget(this);
    panneauDroit->setFixedWidth(colonneDroiteLargeur);
    auto* colonneDroite = new QVBoxLayout(panneauDroit);
    colonneDroite->setContentsMargins(0, 0, 0, 0);
    colonneDroite->setSpacing(0);
    layout->addWidget(panneauDroit, 0);

    /* Gestion scores */
    panneauScores = new QWidget(panneauDroit);
    panneauScores->setFixedSize(colonneDroiteLargeur, scoreWidgetSize);
    panneauScores->setStyleSheet("background-color: #f0f0f0;");
    auto* scoreLayout = new QVBoxLayout(panneauScores);
    scoreLayout->setContentsMargins(20, 20, 20, 20);
    scoreLayout->setSpacing(8);
    score = new QLabel("Scores", panneauScores);
    joueurSelect = new QLabel("Joueur actif", panneauScores);
    erreur = new QLabel("", panneauScores);
    for (auto* label : {score, joueurSelect, erreur}) {
        label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        scoreLayout->addWidget(label);
    }
    colonneDroite->addWidget(panneauScores, 0, Qt::AlignTop);

    /* Gestion pioche */
    piocheView->setFixedWidth(colonneDroiteLargeur);
    piocheView->setFixedHeight(height() - scoreWidgetSize);
    colonneDroite->addWidget(piocheView, 1);

    const int plateauWidth = width() - colonneDroiteLargeur;
    const int plateauHeight = height();
    plateauScene->setSceneRect(0, 0, plateauWidth, plateauHeight);
    zonePlateauRectItem = plateauScene->addRect(0, 0, plateauWidth, plateauHeight,
                                               QPen(Qt::NoPen), QBrush(Qt::blue));
    zonePlateauRect = zonePlateauRectItem->rect();

    const int piocheHeight = height() - scoreWidgetSize;
    piocheScene->setSceneRect(0, 0, colonneDroiteLargeur, piocheHeight);
    zonePiocheRectItem = piocheScene->addRect(0, 0, colonneDroiteLargeur, piocheHeight,
                                              QPen(Qt::NoPen), QBrush(Qt::red));
    zonePiocheRect = zonePiocheRectItem->rect(); 
}
