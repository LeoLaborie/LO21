#ifndef PLATEAUWIDGET_H
#define PLATEAUWIDGET_H
#include <QStackedWidget>
#include <QWidget>
#include <vector>
#include "TuileItem.h"

class ChantierWidget;
class ScorePanel;
class ZoneJeuWidget;
class EchapWidget;
class QShortcut;
class Tuile;

class PlateauWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlateauWidget(QWidget* parent = nullptr, int nbJoueurs = 1);

    ChantierWidget* getChantierWidget() const { return chantierWidget; }
    ScorePanel* getScorePanel() const { return scorePanel; }

signals:
    void demandeParametres();
    void demandeRetourMenu();
    void demandeQuitter();
    void placementValide();

public slots:
    void afficherPlateauJoueur(const int& index);
    void chargerPlateauJoueur(const int& index, const std::vector<Tuile>& tuiles);
    void afficherTuileEnMain(const int& index, const Tuile& tuile);
    void validerPlacementTuile(TuileItem* tuile, const QPointF& positionScene);

private:
    QStackedWidget* stackPlateaux = nullptr;
    std::vector<ZoneJeuWidget*> zonesParJoueur;
    ZoneJeuWidget* zoneJeuWidget = nullptr;
    int joueurActif = 0;
    ChantierWidget* chantierWidget = nullptr;
    ScorePanel* scorePanel = nullptr;
    EchapWidget* echapWidget = nullptr;
    QShortcut* raccourciEchap = nullptr;

    void basculerMenuEchap();
    void gererBlocageInteractions(bool widgetActif);
    TuileItem* creerTuileGraphique(const Tuile& modele, TuileItem::Mode mode, ZoneJeuWidget* zone = nullptr) const;
    ZoneJeuWidget* recupererZone(const int& index) const;
    int calculerTailleTuile(const ZoneJeuWidget* zone) const;
};

#endif
