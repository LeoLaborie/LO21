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
    , mode(m)
{
    if (mode != Mode::Pioche)
    {
        // dès qu'on sort de la pioche, autorise le déplacement libre
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
    QObject::connect(this, &TuileItem::rightClicked, this, &TuileItem::rotate60, Qt::UniqueConnection);

    // Donne une bordure différente à chaque tuile pour les distinguer visuellement.
    static int nextVisualId = 0;
    const int visualId = (indice >= 0) ? indice : nextVisualId++;
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
            emit estPiocher(indice);
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
    // Compense le décalage visuel d'étage pour ne pas fausser l'alignement logique sur la grille.
    const QPointF centreLogique = centreScene - QPointF(0.0, decalageEtageY);
    const QPointF axialF = pixelVersAxial(centreLogique.x(), centreLogique.y(), tailleHex);
    const int q = qRound(axialF.x());
    const int r = qRound(axialF.y());
    const QPointF cibleScene = axialVersPixel(q, r, tailleHex) + QPointF(0.0, decalageEtageY);
    const QPointF deltaScene = cibleScene - centreScene;
    setPos(pos() + deltaScene);
}

/**
 * @brief Redimensionne chaque HexItem pour agrandir ou réduire la tuile.
 */
void TuileItem::setTaille(int nouvelleTaille)
{
    tailleHex = nouvelleTaille;
    for (QGraphicsItem* item : childItems())
    {
        HexItem* hex = dynamic_cast<HexItem*>(item);
        if (hex)
            hex->setTaille(nouvelleTaille);
    }
    prepareGeometryChange();
    setTransformOriginPoint(boundingRect().center());
    // Le décalage d'étage dépend de la taille des hexagones : on le recalcule.
    setNiveauGraphique(niveauHauteur);
}

void TuileItem::setNiveauGraphique(int niveau)
{
    niveauHauteur = std::max(0, niveau);
    setZValue(niveauHauteur * 10);

    // Décalage visuel (vers le haut) pour distinguer les étages.
    const double nouveauDecalage = -static_cast<double>(niveauHauteur) * (tailleHex * 0.35);
    const double delta = nouveauDecalage - decalageEtageY;
    decalageEtageY = nouveauDecalage;
    if (delta != 0.0)
        setPos(pos() + QPointF(0.0, delta));

    replacerCorrectement();
}

QPoint TuileItem::coordonneesAxiales(const QPointF& origineScene) const
{
    if (!hexRef)
        return QPoint();
    const QPointF centreScene = hexRef->mapToScene(hexRef->boundingRect().center());
    // Compense le décalage visuel d'étage pour retourner des coordonnées logiques cohérentes.
    const QPointF centreLogique = centreScene - QPointF(0.0, decalageEtageY);
    const QPointF relatif = centreLogique - origineScene;
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

