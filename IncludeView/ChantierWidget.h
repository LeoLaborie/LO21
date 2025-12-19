#ifndef CHANTIERWIDGET_H
#define CHANTIERWIDGET_H

#include <QGraphicsView>
#include <QRectF>
#include <vector>

#include "Partie.h"
#include "TuileItem.h"
class QGraphicsRectItem;
/**
 * @brief Vue dédiée à l'affichage du chantier.
 */
class ChantierWidget : public QGraphicsView
{
    Q_OBJECT

public:
    ChantierWidget(int width, int height, int nbJoueurs, QWidget* parent = nullptr);
    QGraphicsScene* getChantierScene() const
    {
        return chantierScene;
    }
    int tailleTuileChantier() const;

    void ajouterTuilleDansChantier(Tuile * t);
    TuileItem* retirerTuilleDeChantier(int indice);

signals:
    void tuileSelectionnee(TuileId id);
    void tuileGraphiquePiochee(TuileItem* tuile);

public slots:
    void definirChantier(const std::vector<Tuile>& tuiles);
    void mettreAJourPierres(int nbPierres);
    void remettreTuileDansChantier(TuileItem* tuile);
    void annulerPiocheEnCours(TuileId id);
    void fauxJoueurRetireTuile(TuileId id);

private:
    QGraphicsScene* chantierScene = nullptr;
    QGraphicsRectItem* chantierZoneRectItem = nullptr;
    std::vector<TuileItem*> listeTuilesChantier;
    TuileItem* tuileEnTransit = nullptr;
    int nbPierresDisponibles = 0;
    int nbMaxTuilesAffichees = 4;
    void reordonnerTuiles();
    void viderChantier();
    void mettreAJourDisponibilite();
    int calculerTailleTuile() const;
    int trouverIndiceDepuisId(TuileId id) const;

private slots:
    void piocherTuile(TuileId id);
};

#endif  // CHANTIERWIDGET_H
