#include "ChantierWidget.h"
#include <QBrush>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPen>

static int NB_MAX_TUILES = 5; // en attendant le controleur


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


static QPointF calculerPositionChantier(int index, int totalHauteur)
{
    const int espaceMin = 30;
    const int espaceDisponible = totalHauteur - espaceMin * (NB_MAX_TUILES - 1);
    int hauteurTuile = espaceDisponible / NB_MAX_TUILES;

    if (hauteurTuile < 0)
        hauteurTuile = 0;

    int y = index * (hauteurTuile + espaceMin);
    return QPointF(20, y);
}


void ChantierWidget::ajouterTuilleDansChantier(Tuile* t) {
    const int taille = qMin((width() / 2 - 20),(height()/10-30));
    const int indice = static_cast<int>(listeTuilesChantier.size());
    auto* tuile = new TuileItem(*t, nullptr, TuileItem::Mode::Pioche, taille, indice);
    connect(tuile, &TuileItem::estPiocher,this, &ChantierWidget::piocherTuile);

    QPointF pos = calculerPositionChantier(indice, height());
    tuile->setPos(pos);

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
        QPointF pos = calculerPositionChantier(i, height());
        tuile->setPos(pos);
    }
}
