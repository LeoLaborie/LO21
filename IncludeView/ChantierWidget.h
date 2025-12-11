#ifndef CHANTIERWIDGET_H
#define CHANTIERWIDGET_H

#include "TuileItem.h"
#include <QGraphicsView>
#include <QRectF>
#include <vector>
#include "Partie.h"
class QGraphicsRectItem;
/**
 * @brief Vue dédiée à l'affichage du chantier.
 */
class ChantierWidget : public QGraphicsView
{
    Q_OBJECT

public:
    ChantierWidget(int width, int height, QWidget* parent = nullptr);
    QGraphicsScene* getChantierScene() const { return chantierScene; }
   
    void ajouterTuilleDansChantier(Tuile *t);
    TuileItem* retirerTuilleDeChantier(int indice);

signals:
    void tuilePiochee(TuileItem* tuile);

private:
    QGraphicsScene* chantierScene = nullptr;
    QGraphicsRectItem* chantierZoneRectItem = nullptr;
    std::vector<TuileItem*> listeTuilesChantier;
    void reordonnerTuiles();

private slots:
    void piocherTuile(int indice);
};

#endif // CHANTIERWIDGET_H
