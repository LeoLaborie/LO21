#include "ChantierWidget.h"
#include <QBrush>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPen>
#include <algorithm>

static int NB_MAX_TUILES = 4;  // en attendant le controleur

namespace
{
    void placerTuileCentre(TuileItem* tuile, const QPointF& centreScene)
    {
        if (!tuile)
            return;
        // calcule la différence entre le centre du rectangle formé par les 3 hexagones
        // et le centre souhaité dans la scène, puis décale la tuile pour la centrer
        const QPointF centreLocal = tuile->boundingRect().center();
        tuile->setPos(centreScene - centreLocal);
    }
}  // namespace

ChantierWidget::ChantierWidget(int width, int height, QWidget* parent)
    : QGraphicsView(parent)
{
    setObjectName("chantierView");
    setStyleSheet(
        "#chantierView {"
        "  border: 2px solid black;"
        "  background-color: grey;"
        "}");
    // création de la scène
    chantierScene = new QGraphicsScene(this);
    setScene(chantierScene);
    // application des différentes préférences
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(width, height);

    // ajout dans la scène
    chantierScene->setSceneRect(0, 0, width, height);
    chantierZoneRectItem = chantierScene->addRect(0, 0, width, height, QPen(Qt::NoPen));
}

void ChantierWidget::ajouterTuilleDansChantier(Tuile* t)
{
    // calcule dynamiquement la taille des tuiles pour qu'elles tiennent dans le chantier
    const int taille = qMin((width() - 2.0 * 10) / (1.5 + std::sqrt(3.0)), (height() - 2.0 * 20) / ((2.0 + std::sqrt(3.0)) * NB_MAX_TUILES)) - 10 / NB_MAX_TUILES;
    const int indice = static_cast<int>(listeTuilesChantier.size());
    auto* tuile = new TuileItem(*t, nullptr, TuileItem::Mode::Pioche, taille, indice);
    connect(tuile, &TuileItem::estPiocher, this, &ChantierWidget::piocherTuile);
    // ajout physique dans la scène puis ré-ordonnancement vertical
    chantierScene->addItem(tuile);
    listeTuilesChantier.push_back(tuile);
    reordonnerTuiles();  // va tout placer correctement
}

TuileItem* ChantierWidget::retirerTuilleDeChantier(int indice)
{
    if (indice < 0 || indice >= static_cast<int>(listeTuilesChantier.size()))
        return nullptr;

    // supprime la tuile de la scène visuelle et du tableau pour la donner à la zone de jeu
    TuileItem* tuile = listeTuilesChantier[indice];
    chantierScene->removeItem(tuile);
    listeTuilesChantier.erase(listeTuilesChantier.begin() + indice);
    reordonnerTuiles();
    return tuile;
}
void ChantierWidget::piocherTuile(int indice)
{
    TuileItem * tuile = retirerTuilleDeChantier(indice);
    if (!tuile)
        return;
    // passe en mode placement pour autoriser déplacement/rotation dans la zone de jeu, à mettre dans
    tuile->setMode(TuileItem::Mode::Placement);
    tuile->setInteractivite(true, true);
    emit tuilePiochee(tuile);
    // tant que la tuile n'est pas validée/annulée on bloque la pioche
    setEnabled(false);
}

void ChantierWidget::remettreTuileDansChantier(TuileItem* tuile)
{
    if (!tuile)
        return;

    // on récupere les informations qu'on a besion (comme l'utilisateur ne peux pas ajouter manuelement des tuiles dans le chantier pas besion de vérfier le nb max de Tuile est déjà atteint)
    const int index = tuile->getIndiceDansPioche();
    // on remet le mode de la tuile sur Pioche et désactive tout ce qu'on a pas besion
    tuile->setMode(TuileItem::Mode::Pioche);
    tuile->setInteractivite(false, false);
    tuile->setSelected(false);

    // plus qu'a la remettre et rappeler la fonction qui ordonne les TUiles dans le chaniter
    chantierScene->addItem(tuile);
    listeTuilesChantier.insert(listeTuilesChantier.begin() + index, tuile);
    reordonnerTuiles();
    setEnabled(true);
}

void ChantierWidget::reordonnerTuiles()
{
    // réaligne verticalement toutes les tuiles en conservant leur indice
    const int n = static_cast<int>(listeTuilesChantier.size());
    if (n == 0) return;
    const QRectF rect = chantierZoneRectItem->rect();
    const double xCentre = rect.center().x();
    // On prend la hauteur réelle d'une tuile
    const double hauteurHex = listeTuilesChantier[0]->boundingRect().height();
    const double paddingTop = 10.0;  // marge en haut
    const double gap = 5.0;          // espace entre deux tuiles

    for (int i = 0; i < n; ++i)
    {
        TuileItem* tuile = listeTuilesChantier[i];
        if (!tuile) continue;
        tuile->setIndiceDansPioche(static_cast<unsigned int>(i));
        const double yCentre = rect.top() + paddingTop + hauteurHex / 2.0 + i * (hauteurHex + gap);

        placerTuileCentre(tuile, QPointF(xCentre, yCentre));
    }
}
