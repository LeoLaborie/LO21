#include "TuileItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QtMath>

TuileItem::TuileItem(QGraphicsItem* parent,Mode m,int tailleTuile)
    : QGraphicsItemGroup(parent),tailleHex(tailleTuile),mode(m)
{
    //définit la taille utilisée pour dessiner cette tuile
    if (mode==Mode::ZoneJeu){
        setFlag(ItemIsMovable, true);
    }
    /* setFlag(ItemIsSelectable, true); peut etre utile mais pas encore utilisé*/
    setFlag(ItemIsSelectable, true);
    setTransformOriginPoint(boundingRect().center());
}

TuileItem::TuileItem(Tuile& ref, QGraphicsItem* parent,Mode m,int tailleTuile)
    : QGraphicsItemGroup(parent),tailleHex(tailleTuile),mode(m)
{
    //définit la taille utilisée pour dessiner cette tuile
    if (mode==Mode::ZoneJeu){
        setFlag(ItemIsMovable, true);
    }
    setFlag(ItemIsSelectable, true);
    /* setFlag(ItemSendsGeometryChanges, true); peut etre utile mais pas encore utilisé*/
    int i=0;
    for (Hexagone* h : ref.getHexagones()) {
        auto* hexItem = new HexItem(h, tailleHex);
        addToGroup(hexItem);
        if (i == 0) hexRef = hexItem;
        ++i;
    }
    setTransformOriginPoint(boundingRect().center());
    QObject::connect(this, &TuileItem::rightClicked,
                     this, &TuileItem::rotate60,
                     Qt::UniqueConnection);
}

void TuileItem::addHex(HexItem* hex)
{
    addToGroup(hex);
    if (!hexRef)
        hexRef = hex;
    prepareGeometryChange();
    setTransformOriginPoint(boundingRect().center());
}

void TuileItem::rotate60()
{
    if (!rotationAutorisee)
        return;
    setRotation(rotation() + 60.0);
    replacerCorrectement();
}


void TuileItem::setInteractivite(bool autoriserDeplacement, bool autoriserRotation)
{
    setFlag(ItemIsMovable, autoriserDeplacement);
    setFlag(ItemIsSelectable, autoriserDeplacement);
    rotationAutorisee = autoriserRotation;
}


void TuileItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::RightButton && rotationAutorisee)
        emit rightClicked();

    QGraphicsItemGroup::mousePressEvent(event);
    if (event->button() == Qt::LeftButton && mode==Mode::Pioche){
        emit estPiocher(indice);
    }
}

void TuileItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        replacerCorrectement();
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

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
    const QPointF cScene = hexRef->mapToScene(hexRef->boundingRect().center());
    const QPointF axialF = pixelVersAxial(cScene.x(), cScene.y(), tailleHex);
    const int q = qRound(axialF.x());
    const int r = qRound(axialF.y());
    const QPointF cibleScene = axialVersPixel(q, r, tailleHex);
    const QPointF deltaScene = cibleScene - cScene;
    setPos(pos() + deltaScene);
}


void piocherTuile(int indice){
    
}
