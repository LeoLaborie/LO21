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

namespace {

Tuile creerTuileDepartAkropolis()
{
    auto* place = new Hexagone(0, 0, 0, TypeHex::PHabitation);
    auto* carriereOuest = new Hexagone(-1, 1, 0, TypeHex::Carriere);
    auto* carriereSud = new Hexagone(0, -1, 0, TypeHex::Carriere);
    auto* carriereEst = new Hexagone(1, 0, 0, TypeHex::Carriere);
    return Tuile(place, carriereOuest, carriereSud, carriereEst);
}

Tuile creerTuileMarcheTemple()
{
    auto* placeMarche = new Hexagone(0, 0, 0, TypeHex::PMarche);
    auto* temple = new Hexagone(1, -1, 0, TypeHex::Temple);
    auto* marche = new Hexagone(0, -1, 0, TypeHex::Marche);
    return Tuile(placeMarche, temple, marche);
}

Tuile creerTuileCaserneCarriere()
{
    auto* placeCaserne = new Hexagone(0, 0, 0, TypeHex::PCaserne);
    auto* caserne = new Hexagone(1, -1, 0, TypeHex::Caserne);
    auto* carriere = new Hexagone(0, -1, 0, TypeHex::Carriere);
    return Tuile(placeCaserne, caserne, carriere);
}

Tuile creerTuileTempleJardin()
{
    auto* placeTemple = new Hexagone(0, 0, 0, TypeHex::PTemple);
    auto* jardin = new Hexagone(1, -1, 0, TypeHex::Jardin);
    auto* habitation = new Hexagone(0, -1, 0, TypeHex::Habitation);
    return Tuile(placeTemple, jardin, habitation);
}

Tuile creerTuileHabitationMarche()
{
    auto* placeHabitation = new Hexagone(0, 0, 0, TypeHex::PHabitation);
    auto* marche = new Hexagone(1, -1, 0, TypeHex::Marche);
    auto* caserne = new Hexagone(0, -1, 0, TypeHex::Caserne);
    return Tuile(placeHabitation, marche, caserne);
}

struct TuilePlacement
{
    Tuile (*fabrique)();
    int q;
    int r;
};

}

TuileItem* PlateauWidget::ajouterTuile(Tuile& t, bool centrer)
{
    if (!plateauScene)
        return nullptr;

    auto* tuileItem = new TuileItem(t);
    tuileItem->setPlateauOrigin(zonePlateauRect.center());
    plateauScene->addItem(tuileItem);

    if (centrer) {
        const QPointF center = zonePlateauRect.center();
        const QRectF itemBounds = tuileItem->boundingRect();
        tuileItem->setPos(center - itemBounds.center());
    }

    return tuileItem;
}

PlateauWidget::PlateauWidget(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(1920, 1080);

    constexpr int colonneDroiteLargeur = 250;
    constexpr int scoreWidgetSize = 250;

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

    initialiserPlateauDemo();
}

void PlateauWidget::initialiserPlateauDemo()
{
    if (!zonePlateauRectItem)
        return;

    {
        Tuile tuileDepart = creerTuileDepartAkropolis();
        if (auto* item = ajouterTuile(tuileDepart, false)) {
            item->setInteractivite(false, false);
            item->positionnerSurAxial(0, 0);
        }
    }

    const std::array<TuilePlacement, 4> placements = {{
        {creerTuileMarcheTemple, 2, -1},
        {creerTuileCaserneCarriere, -2, 1},
        {creerTuileTempleJardin, 1, 2},
        {creerTuileHabitationMarche, -1, -2}
    }};

    for (const auto& placement : placements) {
        Tuile tuile = placement.fabrique();
        if (auto* item = ajouterTuile(tuile, false)) {
            item->positionnerSurAxial(placement.q, placement.r);
        }
    }
}
