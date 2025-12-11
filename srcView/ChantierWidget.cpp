#include "ChantierWidget.h"
#include <QBrush>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPen>

static int NB_MAX_TUILES = 4; // en attendant le controleur

namespace {
void placerTuileCentre(TuileItem* tuile, const QPointF& centreScene)
{
    if (!tuile)
        return;
     // calcule la différence entre le centre du rectangle formé par les 3 hexagones
    // et le centre souhaité dans la scène, puis décale la tuile pour la centrer
    const QPointF centreLocal = tuile->boundingRect().center();
    tuile->setPos(centreScene - centreLocal);
}}

ChantierWidget::ChantierWidget(int width, int height, QWidget* parent)
    : QGraphicsView(parent)
{
    setObjectName("chantierView");
    setStyleSheet(
        "#chantierView {"
        "  border: 2px solid black;"           
        "  background-color: grey;"  
        "}"
    );
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
    chantierZoneRectItem = chantierScene->addRect(0, 0, width, height, QPen(Qt::NoPen));
}





void ChantierWidget::ajouterTuilleDansChantier(Tuile* t) {
    const int taille = qMin((width()  - 2.0 * 10) / (1.5 + std::sqrt(3.0)),(height() - 2.0 * 20) / ((2.0 + std::sqrt(3.0)) * NB_MAX_TUILES))-10/NB_MAX_TUILES;
    const int indice = static_cast<int>(listeTuilesChantier.size());
    auto* tuile = new TuileItem(*t, nullptr, TuileItem::Mode::Pioche, taille, indice);
    connect(tuile, &TuileItem::estPiocher, this, &ChantierWidget::piocherTuile);
    chantierScene->addItem(tuile);
    listeTuilesChantier.push_back(tuile);
    reordonnerTuiles();  // va tout placer correctement
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
    const int n = static_cast<int>(listeTuilesChantier.size());
    if (n == 0) return;
    const QRectF rect = chantierZoneRectItem->rect();
    const double xCentre = rect.center().x();
    // On prend la hauteur réelle d'une tuile
    const double hauteurHex = listeTuilesChantier[0]->boundingRect().height();
    const double paddingTop = 10.0;   // marge en haut
    const double gap        = 5.0;   // espace entre deux tuiles

    for (int i = 0; i < n; ++i) {
        TuileItem* tuile = listeTuilesChantier[i];
        if (!tuile) continue;
        tuile->setIndiceDansPioche(static_cast<unsigned int>(i));
        const double yCentre =rect.top() + paddingTop + hauteurHex / 2.0 + i * (hauteurHex + gap);

        placerTuileCentre(tuile, QPointF(xCentre, yCentre));
    }
}


