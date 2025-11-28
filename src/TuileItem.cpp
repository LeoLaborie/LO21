#include "TuileItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QtMath>

static int taille=50;
TuileItem::TuileItem(QGraphicsItem* parent)
    : QGraphicsItemGroup(parent)
{
    setFlag(ItemIsMovable, true);
    setFlag(ItemIsSelectable, true);
    setFlag(ItemSendsGeometryChanges, true);
    setTransformOriginPoint(boundingRect().center());
}

TuileItem::TuileItem(Tuile& ref, QGraphicsItem* parent)
    : QGraphicsItemGroup(parent)
{
    setFlag(ItemIsMovable, true);
    setFlag(ItemIsSelectable, true);
    setFlag(ItemSendsGeometryChanges, true);
    int i=0;
    for (Hexagone* h : ref.getHexagones()) {
        auto* hexItem = new HexItem(h, taille);
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
    prepareGeometryChange();
    setTransformOriginPoint(boundingRect().center());
}

void TuileItem::rotate60()
{
    setRotation(rotation() + 60.0);
    replacerCorrectement();
}




void TuileItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
        emit rightClicked();

    QGraphicsItemGroup::mousePressEvent(event);
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
    const QPointF cScene = hexRef->mapToScene(hexRef->boundingRect().center());
    const QPointF axialF = pixelVersAxial(cScene.x(), cScene.y(), taille);
    const int q = qRound(axialF.x());
    const int r = qRound(axialF.y());
    const QPointF cibleScene = axialVersPixel(q, r, taille);
    const QPointF deltaScene = cibleScene - cScene;
    setPos(pos() + deltaScene);
}
