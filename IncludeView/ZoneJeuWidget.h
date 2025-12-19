#ifndef ZONEJEUWIDGET_H
#define ZONEJEUWIDGET_H

#include <QGraphicsView>
#include <QPoint>
#include <QPointF>
#include <QRectF>
#include <QMouseEvent>

#include "TuileItem.h"

class QGraphicsRectItem;
class QGraphicsProxyWidget;
class ValiderPlacementWidget;

/**
 * @brief Vue principale contenant la scène du plateau de jeu.
 */
class ZoneJeuWidget : public QGraphicsView
{
    Q_OBJECT

    public:
        ZoneJeuWidget(int width, int height, QWidget* parent = nullptr);
        void ajouterTuileDansZoneJeu(TuileItem * t, int x, int y);
        QGraphicsScene* getPlateauScene() const
        {
            return zoneJeuScene;
        }
        QRectF getZoneRect() const
        {
            return zoneJeuRect;
        }
        QPointF getOrigineGrille() const;
        void viderZone();
        void ajouterTuileDepuisModele(TuileItem* tuile);
        void ajouterTuileDepuisModele(TuileItem* tuile, const QPointF& positionScene);
        void setBlocageInteractions(bool bloque);
        bool interactionsBloquees() const
        {
            return blocageInteractions;
        }

    public slots:
        void placerTuileDansZoneJeu(TuileItem * tuile);
        void confirmerPlacementApprouve(TuileItem* tuile);

    signals:
    void placementTuileFinalise(TuileItem* tuile, const QPointF& positionScene);
    void placementTuileAnnule(TuileItem* tuile);
    void validationPlacementDemandee(TuileItem* tuile, const QPoint& coordonneesTheoriques);

private:
    QGraphicsScene* zoneJeuScene = nullptr;
    QGraphicsRectItem* zoneJeuRectItem = nullptr;
    QRectF zoneJeuRect;
    std::vector<TuileItem*> tuilesZoneJeu;
    bool blocageInteractions = false;
    ValiderPlacementWidget* validerPlacementWidget = nullptr;
    QGraphicsProxyWidget* validerPlacementProxy = nullptr;
    TuileItem* tuileEnValidation = nullptr;

    void masquerWidgetValidation();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private slots:
    void afficherPanneauValidation(TuileItem* tuile);
    void surConfirmationDemandee();
    void surAnnulationDemandee();
    void surDebutDeplacementTuile(TuileItem* tuile);

private:
    bool panActif = false;
    QPoint panDernierePos{};
    qreal zoomCourant = 1.0;
};

#endif
