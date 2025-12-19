#include "ZoneJeuWidget.h"

#include <QBrush>
#include <QGraphicsProxyWidget>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPen>
#include <QPointF>
#include <QSize>
#include <QWheelEvent>
#include <QApplication>
#include <algorithm>

#include "WidgetUtilitaire.h"

ZoneJeuWidget::ZoneJeuWidget(int width, int height, QWidget *parent)
    : QGraphicsView(parent)
{
    // création de la scène
    zoneJeuScene = new QGraphicsScene(this);
    setScene(zoneJeuScene);

    // application de paramètre et préférence
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(QGraphicsView::NoDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setRenderHint(QPainter::Antialiasing, true);
    setFixedSize(width, height);

    // Scène "très grande" pour éviter toute dépendance aux scrollbars.
    qreal sceneSize = 20000.0;
    zoneJeuScene->setSceneRect(-sceneSize / 2.0, -sceneSize / 2.0, sceneSize, sceneSize);
    zoneJeuRectItem = zoneJeuScene->addRect(zoneJeuScene->sceneRect(), QPen(Qt::NoPen), QBrush(Qt::blue));
    zoneJeuRect = zoneJeuScene->sceneRect();
    centerOn(zoneJeuRect.center());

    // widget flottant utilisé pour confirmer/annuler un placement
    validerPlacementWidget = new ValiderPlacementWidget();
    validerPlacementProxy = zoneJeuScene->addWidget(validerPlacementWidget);
    validerPlacementProxy->setZValue(1000);
    validerPlacementProxy->setVisible(false);
    connect(validerPlacementWidget, &ValiderPlacementWidget::confirmationDemandee, this, &ZoneJeuWidget::surConfirmationDemandee);
    connect(validerPlacementWidget, &ValiderPlacementWidget::annulationDemandee, this, &ZoneJeuWidget::surAnnulationDemandee);
}

void ZoneJeuWidget::mousePressEvent(QMouseEvent *event)
{
    if (event && event->button() == Qt::MiddleButton)
    {
        panActif = true;
        panBouton = Qt::MiddleButton;
        panDernierePos = event->pos();
        if (!curseurPanActif)
        {
            QApplication::setOverrideCursor(Qt::ClosedHandCursor);
            curseurPanActif = true;
        }
        event->accept();
        return;
    }
    QGraphicsView::mousePressEvent(event);
}

void ZoneJeuWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (panActif && event)
    {
        const QPoint deltaPx = event->pos() - panDernierePos;
        panDernierePos = event->pos();

        const QPointF centreScene = mapToScene(viewport()->rect().center());
        const QPointF deltaScene = mapToScene(QPoint(0, 0)) - mapToScene(deltaPx);
        centerOn(centreScene + deltaScene);

        event->accept();
        return;
    }
    QGraphicsView::mouseMoveEvent(event);
}

void ZoneJeuWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (panActif && event && event->button() == panBouton)
    {
        panActif = false;
        panBouton = Qt::NoButton;
        if (curseurPanActif)
        {
            QApplication::restoreOverrideCursor();
            curseurPanActif = false;
        }
        event->accept();
        return;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void ZoneJeuWidget::wheelEvent(QWheelEvent *event)
{
    if (!event)
        return;
    // Molette = zoom
    constexpr qreal zoomStep = 1.15;
    constexpr qreal zoomMin = 0.25;
    constexpr qreal zoomMax = 3.5;

    const qreal oldZoom = zoomCourant;
    if (event->angleDelta().y() > 0)
        zoomCourant *= zoomStep;
    else if (event->angleDelta().y() < 0)
        zoomCourant /= zoomStep;

    zoomCourant = std::clamp(zoomCourant, zoomMin, zoomMax);
    const qreal factor = (oldZoom == 0.0) ? 1.0 : (zoomCourant / oldZoom);
    if (factor != 1.0)
        scale(factor, factor);

    event->accept();
}

QPointF ZoneJeuWidget::getOrigineGrille() const
{
    if (!zoneJeuScene)
        return QPointF();
    return zoneJeuScene->sceneRect().center();
}

void ZoneJeuWidget::viderZone()
{
    masquerWidgetValidation();
    for (auto *tuile : tuilesZoneJeu)
    {
        if (!tuile)
            continue;
        if (QGraphicsScene *sceneActuelle = tuile->scene())
            sceneActuelle->removeItem(tuile);
        delete tuile;
    }
    tuilesZoneJeu.clear();
}

void ZoneJeuWidget::ajouterTuileDepuisModele(TuileItem *t)
{
    ajouterTuileDepuisModele(t, getOrigineGrille());
}

void ZoneJeuWidget::ajouterTuileDepuisModele(TuileItem *t, const QPointF &positionScene)
{
    if (!t || !zoneJeuScene)
        return;
    t->setMode(TuileItem::Mode::ZoneJeu);
    t->setInteractivite(false, false);
    t->definirOrigineGrille(getOrigineGrille());
    t->setPos(positionScene);
    zoneJeuScene->addItem(t);
    // aligne sur la grille pour éviter les petits décalages dus aux conversions pixels<->axial
    t->replacerCorrectement();
    tuilesZoneJeu.push_back(t);
}
void ZoneJeuWidget::ajouterTuileDansZoneJeu(TuileItem *t, int x, int y)
{
    if (!t)
        return;

    if (QGraphicsScene *sceneActuelle = t->scene())
    {
        if (sceneActuelle != zoneJeuScene)
            sceneActuelle->removeItem(t);
    }

    // positionne la tuile et désactive son interaction en pause
    t->definirOrigineGrille(getOrigineGrille());
    t->setPos(x, y);
    t->setEnabled(!blocageInteractions);

    // ajout dans la scène et alignement automatique sur la grille
    if (t->scene() != zoneJeuScene)
        zoneJeuScene->addItem(t);
    t->replacerCorrectement();
    // brancher les signaux utilisés pour afficher ou cacher le widget de validation
    connect(t, &TuileItem::demandeValidationPlacement, this, &ZoneJeuWidget::afficherPanneauValidation, Qt::UniqueConnection);
    connect(t, &TuileItem::deplacementDemarre, this, &ZoneJeuWidget::surDebutDeplacementTuile, Qt::UniqueConnection);
    if (t->modeCourant() == TuileItem::Mode::Placement)
        afficherPanneauValidation(t);

    // Stocker la tuile dans le tableau
    tuilesZoneJeu.push_back(t);
}

void ZoneJeuWidget::placerTuileDansZoneJeu(TuileItem *tuile)
{
    if (!tuile)
        return;

    // place la nouvelle tuile au centre de la zone
    const QPointF centre = zoneJeuRect.center();
    ajouterTuileDansZoneJeu(tuile, static_cast<int>(centre.x()), static_cast<int>(centre.y()));
}

void ZoneJeuWidget::setBlocageInteractions(bool bloque)
{
    if (blocageInteractions == bloque)
        return;
    // on bloque les interaction avec les tuiles et débloque quand on veut afficher le menu de pause
    blocageInteractions = bloque;
    for (auto *tuile : tuilesZoneJeu)
    {
        if (tuile)
            tuile->setEnabled(!blocageInteractions);
    }
    if (blocageInteractions)
        masquerWidgetValidation();
}

void ZoneJeuWidget::afficherPanneauValidation(TuileItem *tuile)
{
    if (!tuile || !validerPlacementProxy || !validerPlacementWidget)
        return;

    // mémorise la tuile en attente et calcule une position juste au-dessus de celle-ci
    tuileEnValidation = tuile;
    validerPlacementWidget->adjustSize();
    const QSize widgetSize = validerPlacementWidget->size();
    const QRectF sceneRect = tuile->sceneBoundingRect();
    QPointF pos(sceneRect.center().x() - widgetSize.width() / 2.0, sceneRect.top() - widgetSize.height() - 8.0);
    if (pos.y() < zoneJeuScene->sceneRect().top())
        pos.setY(sceneRect.bottom() + 8.0);
    validerPlacementProxy->setPos(pos);
    validerPlacementProxy->setVisible(true);
}

void ZoneJeuWidget::masquerWidgetValidation()
{
    if (!validerPlacementProxy)
        return;
    // cache le widget et oublie la tuile suivie
    validerPlacementProxy->setVisible(false);
    tuileEnValidation = nullptr;
}

void ZoneJeuWidget::surConfirmationDemandee()
{
    if (!tuileEnValidation)
        return;
    const QPointF origineScene = getOrigineGrille();
    const QPoint coordonnees = tuileEnValidation->coordonneesAxiales(origineScene);
    emit validationPlacementDemandee(tuileEnValidation, coordonnees);
}

void ZoneJeuWidget::surAnnulationDemandee()
{
    TuileItem *tuile = tuileEnValidation;
    if (!tuile)
    {
        masquerWidgetValidation();
        return;
    }

    if (QGraphicsScene *sceneActuelle = tuile->scene())
        sceneActuelle->removeItem(tuile);
    auto it = std::find(tuilesZoneJeu.begin(), tuilesZoneJeu.end(), tuile);
    if (it != tuilesZoneJeu.end())
        tuilesZoneJeu.erase(it);

    masquerWidgetValidation();
    emit placementTuileAnnule(tuile);
}

void ZoneJeuWidget::surDebutDeplacementTuile(TuileItem *tuile)
{
    if (!tuile)
        return;
    // si la tuile suivie recommence un déplacement on masque le widget
    if (tuile == tuileEnValidation)
        masquerWidgetValidation();
}

void ZoneJeuWidget::confirmerPlacementApprouve(TuileItem *tuile)
{
    if (!tuile)
        return;
    tuile->setMode(TuileItem::Mode::ZoneJeu);
    emit placementTuileFinalise(tuile, tuile->pos());
    if (tuile == tuileEnValidation)
        masquerWidgetValidation();
}
