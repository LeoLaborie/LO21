#ifndef WIDGETUTILITAIRE_H
#define WIDGETUTILITAIRE_H

#include <QWidget>
#include <QRectF>

class QGraphicsScene;
class QGraphicsProxyWidget;
class EchapWidget;

/**
 * @brief Gestionnaire générique de widget utilitaire inséré dans une QGraphicsScene via proxy.
 */
class WidgetUtilitaire : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetUtilitaire(QWidget* parent = nullptr);

    void attacherAScene(QGraphicsScene* scene);
    void afficherEchap(const QRectF& zoneReference);
    void fermerWidget();
    bool estActif() const { return actif; }

signals:
    void visibiliteChangee(bool actif);

private:
    void mettreAJourPosition(const QRectF& zoneReference);

    QGraphicsProxyWidget* proxy = nullptr;
    EchapWidget* echapWidget = nullptr;
    bool actif = false;
};

#endif // WIDGETUTILITAIRE_H
