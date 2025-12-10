#include "ChantierWidget.h"
#include <QBrush>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPen>

static int NB_MAX_TUILES = 5; // en attendant le controleur

namespace {
void placerTuileCentre(TuileItem* tuile, const QPointF& centreScene)
{
    if (!tuile)
        return;
    const QPointF centreLocal = tuile->boundingRect().center();
    tuile->setPos(centreScene - centreLocal);
}
QPointF calculerPositionChantier(int index, const QRectF& zone)
{
    const int espaceMin = 30;
    const qreal espaceDisponible = zone.height() - espaceMin * (NB_MAX_TUILES - 1);
    qreal hauteurTuile = espaceDisponible / NB_MAX_TUILES;

    if (hauteurTuile < 0)
        hauteurTuile = 0;

    const qreal yCentre = zone.top() + hauteurTuile / 2.0 + index * (hauteurTuile + espaceMin);
    const qreal x = zone.left() + 20;
    return QPointF(x, yCentre);
}
}


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





void ChantierWidget::ajouterTuilleDansChantier(Tuile* t) {
    const int taille = qMin((width() / 2.5 - 20),(height()/(2.5*NB_MAX_TUILES)-20));
    const int indice = static_cast<int>(listeTuilesChantier.size());
    auto* tuile = new TuileItem(*t, nullptr, TuileItem::Mode::Pioche, taille, indice);
    connect(tuile, &TuileItem::estPiocher,this, &ChantierWidget::piocherTuile);

    QPointF pos = calculerPositionChantier(indice, chantierZoneRect);
    placerTuileCentre(tuile, pos);

    chantierScene->addItem(tuile);
    listeTuilesChantier.push_back(tuile);
    reordonnerTuiles();
}

TuileItem* ChantierWidget::retirerTuilleDeChantier(int indice)
{
    if (indice < 0 || indice >= static_cast<int>(listeTuilesChantier.size()))
        return nullptr;

    TuileItem* tuile = listeTuilesChantier[indice];
    chantierScene->removeItem(tuile);
    listeTuilesChantier.erase(listeTuilesChantier.begin() + indice);
    reordonnerTuiles();
    return tuile;
}
void ChantierWidget::piocherTuile(int indice){
    TuileItem* tuile = retirerTuilleDeChantier(indice);
    if (!tuile)
        return;

    tuile->setMode(TuileItem::Mode::ZoneJeu);
    tuile->setInteractivite(true, true);
    emit tuilePiochee(tuile);
}

void ChantierWidget::reordonnerTuiles()
{
    for (int i = 0; i < static_cast<int>(listeTuilesChantier.size()); ++i) {
        TuileItem* tuile = listeTuilesChantier[i];
        if (!tuile)
            continue;
        tuile->setIndiceDansPioche(static_cast<unsigned int>(i));
        QPointF pos = calculerPositionChantier(i, chantierZoneRect);
        placerTuileCentre(tuile, pos);
    }
}
