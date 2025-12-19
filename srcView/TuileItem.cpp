#include "TuileItem.h"

#include <QBrush>
#include <QGraphicsSceneMouseEvent>
#include <QLinearGradient>
#include <QPen>
#include <QColor>
#include <QtMath>
#include <algorithm>

namespace
{
    /**
     * @brief Génère une couleur de contour stable à partir d'un identifiant.
     */
    QColor couleurTuileDepuisId(int id)
    {
        const int hue = (id * 47) % 360;
        return QColor::fromHsv(hue, 140, 220);
    }
}

TuileItem::TuileItem(const Tuile& ref, QGraphicsItem* parent, Mode m, int tailleTuile, int indice)
    : QObject()
    , QGraphicsItemGroup(parent)
    , tailleHex(tailleTuile)
    , rotationAutorisee(true)
    , indice(indice)
    , tuileId(ref.getId())
    , mode(m)
{
    int qRef = 0;
    int rRef = 0;
    {
        Tuile::ConstIterator it = ref.getConstIterator();
        if (!it.isDone())
        {
            qRef = it.currentItem().getX();
            rRef = it.currentItem().getY();
        }
    }
    hexItems.clear();
    offsetsRelatifs.clear();
    hexItems.reserve(ref.getNbHexa());
    offsetsRelatifs.reserve(ref.getNbHexa());

    if (mode != Mode::Pioche)
    {
        // dès qu'on sort de la pioche, autorise le déplacement libre
        setFlag(ItemIsMovable, true);
    }
    setFlag(ItemIsSelectable, true);
    /* setFlag(ItemSendsGeometryChanges, true); peut etre utile mais pas encore utilisé*/
    int i = 0;
    for (Tuile::ConstIterator it = ref.getConstIterator(); !it.isDone(); it.next())
    {
        const Hexagone& hex = it.currentItem();
        offsetsRelatifs.push_back({hex.getX() - qRef, hex.getY() - rRef});
        auto* hexItem = new HexItem(&hex, tailleHex);
        // Place les hexagones relativement à l'hexagone de référence de la tuile.
        // - En mode placement: les coords sont déjà relatives (autour de 0,0), donc ça ne change rien.
        // - En mode plateau: les coords sont absolues, donc on "recentre" pour que la position de groupe corresponde au repère.
        hexItem->setPos(axialVersPixel(hex.getX() - qRef, hex.getY() - rRef, tailleHex));
        addToGroup(hexItem);
        hexItems.push_back(hexItem);
        if (i == 0)
            hexRef = hexItem;
        ++i;
    }
    setTransformOriginPoint(boundingRect().center());
    QObject::connect(this, &TuileItem::rightClicked, this, &TuileItem::rotate60, Qt::UniqueConnection);

    // Donne une bordure différente à chaque tuile pour les distinguer visuellement.
    static int nextVisualId = 0;
    const int visualId = (tuileId > 0) ? static_cast<int>(tuileId) : ((indice >= 0) ? static_cast<int>(indice) : nextVisualId++);
    const bool estTuileDepart = (ref.getNbHexa() == 4);
    const QPen pen(estTuileDepart ? QColor(Qt::black) : couleurTuileDepuisId(visualId), 3.0);
    for (QGraphicsItem* item : childItems())
    {
        if (auto* hex = dynamic_cast<HexItem*>(item))
            hex->setPen(pen);
    }
}

void TuileItem::rotate60()
{
    if (!rotationAutorisee)
        return;
    // rotation en pas de 60° puis réalignement sur la grille axiale
    setRotation(rotation() + 60.0);
    replacerCorrectement();
    emit rotationEffectuee(1);
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
    if (event->button() == Qt::LeftButton)
    {
        if (mode == Mode::Pioche)
        {
            emit estPiocher(tuileId);
        }
        else
        {
            // notifie la zone qu'un déplacement démarre pour masquer le widget de validation
            emit deplacementDemarre(this);
        }
    }
}

void TuileItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        replacerCorrectement();
        if (mode == Mode::Placement)
            // en mode placement on affiche le widget de validation via ce signal
            emit demandeValidationPlacement(this);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

/**
 * @brief Convertit des coordonnées pixel en coordonnées axiales approximatives.
 */
QPointF pixelVersAxial(double px, double py, double size)
{
    const double rt3 = std::sqrt(3.0);
    const double qf = px / (1.5 * size);
    const double rf = (py / (rt3 * size)) - (qf / 2.0);
    return {qf, rf};
}

void TuileItem::replacerCorrectement()
{
    if (!hexRef)
        return;
    const QPointF centreScene = hexRef->mapToScene(hexRef->boundingRect().center());
    const QPointF relatif = centreScene - origineGrilleScene;
    const QPointF axialF = pixelVersAxial(relatif.x(), relatif.y(), tailleHex);
    const int q = qRound(axialF.x());
    const int r = qRound(axialF.y());
    const QPointF cibleScene = origineGrilleScene + axialVersPixel(q, r, tailleHex);
    const QPointF deltaScene = cibleScene - centreScene;
    setPos(pos() + deltaScene);
}

/**
 * @brief Redimensionne chaque HexItem pour agrandir ou réduire la tuile.
 */
void TuileItem::setTaille(int nouvelleTaille)
{
    prepareGeometryChange();
    tailleHex = nouvelleTaille;
    for (QGraphicsItem* item : childItems())
    {
        HexItem* hex = dynamic_cast<HexItem*>(item);
        if (hex)
            hex->setTaille(nouvelleTaille);
    }
    // Repositionne aussi les hexagones : leurs positions dépendent directement de la taille (axialVersPixel).
    // Sans ça, un resize (ex: tuile du chantier vers zone) crée des "trous" entre hexagones.
    const int n = std::min(static_cast<int>(hexItems.size()), static_cast<int>(offsetsRelatifs.size()));
    for (int i = 0; i < n; ++i)
    {
        if (!hexItems[static_cast<size_t>(i)])
            continue;
        const auto& o = offsetsRelatifs[static_cast<size_t>(i)];
        hexItems[static_cast<size_t>(i)]->setPos(axialVersPixel(o.q, o.r, tailleHex));
    }
    setTransformOriginPoint(boundingRect().center());
    // Le décalage d'étage dépend de la taille des hexagones : on le recalcule.
    setNiveauGraphique(niveauHauteur);
    update();
}

void TuileItem::setNiveauGraphique(int niveau)
{
    niveauHauteur = std::max(0, niveau);
    setZValue(niveauHauteur * 10);
}

QPoint TuileItem::coordonneesAxiales(const QPointF& origineScene) const
{
    if (!hexRef)
        return QPoint();
    const QPointF centreScene = hexRef->mapToScene(hexRef->boundingRect().center());
    const QPointF relatif = centreScene - origineScene;
    const QPointF axialF = pixelVersAxial(relatif.x(), relatif.y(), tailleHex);
    return QPoint(qRound(axialF.x()), qRound(axialF.y()));
}
void TuileItem::ModifierCouleurEtage(int z)
{
    QLinearGradient grad(QPointF(-tailleHex, 0),QPointF(tailleHex, 0));
    grad.setColorAt(0.0, QColor(245, 245, 245).darker(100+40*z));
    grad.setColorAt(1.0, QColor(210, 210, 210).darker(100+40*z));

    QBrush brushFond(grad);

    for (QGraphicsItem* item : childItems())
    {
        if (auto* hex = dynamic_cast<HexItem*>(item))
        {
            hex->setBrush(brushFond);
        }
    }
}
