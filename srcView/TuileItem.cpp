#include "TuileItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QtMath>
#include <algorithm>


TuileItem::TuileItem(Tuile& ref, QGraphicsItem* parent,Mode m,int tailleTuile,int indice)
    : QObject()
    , QGraphicsItemGroup(parent)
    , tailleHex(tailleTuile)
    , rotationAutorisee(true)
    , indice(indice)
    , mode(m)
{
    if (mode != Mode::Pioche){
        //dès qu'on sort de la pioche, autorise le déplacement libre
        setFlag(ItemIsMovable, true);
    }
    setFlag(ItemIsSelectable, true);
    /* setFlag(ItemSendsGeometryChanges, true); peut etre utile mais pas encore utilisé*/
    int i=0;
    for(Tuile::ConstIterator it = ref.getConstIterator(); !it.isDone(); it.next()){
        auto* hexItem = new HexItem(&it.currentItem(), tailleHex);
        addToGroup(hexItem);
        if (i == 0) hexRef = hexItem;
        ++i;
    }
    setTransformOriginPoint(boundingRect().center());
    QObject::connect(this, &TuileItem::rightClicked,this, &TuileItem::rotate60,Qt::UniqueConnection);
}

void TuileItem::rotate60()
{
    if (!rotationAutorisee)
        return;
    //rotation en pas de 60° puis réalignement sur la grille axiale
    setRotation(rotation() + 60.0);
    replacerCorrectement();
}

void TuileItem::setInteractivite(bool autoriserDeplacement, bool autoriserRotation)
{
    setFlag(ItemIsMovable, autoriserDeplacement);
    setFlag(ItemIsSelectable, autoriserDeplacement);
    rotationAutorisee = autoriserRotation;
}

void TuileItem::setIndiceDansPioche(unsigned int nouvelIndice)
{
    indice = nouvelIndice;
}

void TuileItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::RightButton && rotationAutorisee && mode != Mode::Pioche)
        emit rightClicked();

    QGraphicsItemGroup::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        if (mode == Mode::Pioche) {
            emit estPiocher(indice);
        } else {
            //notifie la zone qu'un déplacement démarre pour masquer le widget de validation
            emit deplacementDemarre(this);
        }
    }
}

void TuileItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        replacerCorrectement();
        if (mode == Mode::Placement)
            //en mode placement on affiche le widget de validation via ce signal
            emit demandeValidationPlacement(this);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

/**
 * @brief Convertit des coordonnées pixel en coordonnées axiales approximatives.
 */
QPointF pixelVersAxial(double px, double py, double size) {
    const double rt3 = std::sqrt(3.0);
    const double qf = px / (1.5 * size);
    const double rf = (py / (rt3 * size)) - (qf / 2.0);
    return { qf, rf };
}

void TuileItem::replacerCorrectement()
{
    if (!hexRef)
        return;
    const QPointF centreScene = hexRef->mapToScene(hexRef->boundingRect().center());
    const QPointF offsetScene(0.0, -niveauHauteur * decalageHauteurPixels);
    const QPointF centreBase = centreScene - offsetScene;
    const QPointF axialF = pixelVersAxial(centreBase.x(), centreBase.y(), tailleHex);
    const int q = qRound(axialF.x());
    const int r = qRound(axialF.y());
    const QPointF cibleScene = axialVersPixel(q, r, tailleHex);
    const QPointF cibleAvecOffset = cibleScene + offsetScene;
    const QPointF deltaScene = cibleAvecOffset - centreScene;
    setPos(pos() + deltaScene);
}

/**
 * @brief Redimensionne chaque HexItem pour agrandir ou réduire la tuile.
 */
void TuileItem::setTaille(int nouvelleTaille)
{
    tailleHex = nouvelleTaille;
    for (QGraphicsItem* item : childItems()) {
        HexItem* hex = dynamic_cast<HexItem*>(item);
        if (hex)
            hex->setTaille(nouvelleTaille);
    }
    prepareGeometryChange();
    setTransformOriginPoint(boundingRect().center());
    replacerCorrectement();
}

void TuileItem::setNiveauGraphique(int niveau)
{
    niveauHauteur = std::max(0, niveau);
    setZValue(niveauHauteur * 10);
    replacerCorrectement();
}
