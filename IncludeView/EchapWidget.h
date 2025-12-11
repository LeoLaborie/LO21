#ifndef ECHAPWIDGET_H
#define ECHAPWIDGET_H

#include <QRectF>
#include <QWidget>

class QGraphicsScene;
class QGraphicsProxyWidget;

/**
 * @brief Widget d'échappement affiché dans un QGraphicsProxyWidget.
 * Regroupe la construction de l'UI et la gestion d'affichage/masquage.
 */
class EchapWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit EchapWidget(QWidget* parent = nullptr);

        void attacherAScene(QGraphicsScene * scene);
        void afficherEchap(const QRectF& zoneReference);
        void fermerWidget();
        bool estActif() const
        {
            return actif;
        }

    signals:
    void visibiliteChangee(bool actif);
    void demandeParametres();
    void demandeRetourMenu();
    void demandeSauvegarde();
    void demandeQuitter();

private:
    void mettreAJourPosition(const QRectF& zoneReference);
    void construireInterface();

    QGraphicsProxyWidget* proxy = nullptr;
    bool actif = false;
};

#endif  // ECHAPWIDGET_H
