#include "TuileItem.h"
#include "HexItem.h"
#include "Hexagone.h"
#include "Tuile.h"
#include <QGraphicsSceneMouseEvent>
#include <QtMath>

// Taille par défaut des hex (utilisée pour la grille)
static int g_taille = 50;

// --- Formules cohérentes avec ton rendu actuel ---
QPointF axialVersPixel(int q, int r, double size)
{
    const double rt3 = std::sqrt(3.0);
    const double px = size * (1.5 * q);
    const double py = size * ((rt3 / 2.0) * q + rt3 * r);
    return {px, py};
}

QPointF pixelVersAxial(double px, double py, double size)
{
    const double rt3 = std::sqrt(3.0);
    const double qf = px / (1.5 * size);
    const double rf = (py / (rt3 * size)) - (qf / 2.0);
    return {qf, rf};
}

// ----------------- Implémentation TuileItem -----------------

TuileItem::TuileItem(QGraphicsItem *parent)
    : QObject(), QGraphicsItemGroup(parent)
{
    setFlag(ItemIsMovable, true);
    setFlag(ItemIsSelectable, true);
    setFlag(ItemSendsGeometryChanges, true);
    setTransformOriginPoint(boundingRect().center());
}

TuileItem::TuileItem(Tuile &ref, QGraphicsItem *parent)
    : QObject(), QGraphicsItemGroup(parent)
{
    setFlag(ItemIsMovable, true);
    setFlag(ItemIsSelectable, true);
    setFlag(ItemSendsGeometryChanges, true);

    int i = 0;
    for (Hexagone *h : ref.getHexagones())
    {
        auto *hexItem = new HexItem(h, g_taille);
        addToGroup(hexItem);
        if (i == 0)
            hexRef = hexItem;
        ++i;
    }
    setTransformOriginPoint(boundingRect().center());
}

void TuileItem::addHex(HexItem *hex)
{
    addToGroup(hex);
    prepareGeometryChange();
    setTransformOriginPoint(boundingRect().center());
    if (!hexRef)
        hexRef = hex;
}

void TuileItem::rotate60()
{
    setRotation(rotation() + 60.0);
}

void TuileItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        emit rightClicked();

    QGraphicsItemGroup::mousePressEvent(event);
}

void TuileItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        replacerCorrectement(); // snap plateau
    }
    QGraphicsItemGroup::mouseReleaseEvent(event);
}

// Snap robuste (même après rotation) : on aligne le centre de hexRef en SCÈNE
void TuileItem::replacerCorrectement()
{
    if (!hexRef)
        return;

    // 1) centre de l’hex d’ancrage en coordonnées scène
    const QPointF cScene = hexRef->mapToScene(hexRef->boundingRect().center());

    // 2) scène -> axial (fractionnaire) puis arrondi
    const QPointF axialF = pixelVersAxial(cScene.x(), cScene.y(), g_taille);
    const int q = qRound(axialF.x());
    const int r = qRound(axialF.y());

    // 3) centre cible (scène) de la cellule (q, r)
    const QPointF cibleScene = axialVersPixel(q, r, g_taille);

    // 4) translation nécessaire (en scène)
    const QPointF deltaScene = cibleScene - cScene;

    setPos(pos() + deltaScene);
}
