#ifndef PLATEAUWIDGET_H
#define PLATEAUWIDGET_H
#include <QStackedWidget>
#include <QWidget>
#include <vector>
#include "TuileItem.h"
#include "ZoneJeuWidget.h"
#include <QString>

class ChantierWidget;
class ScorePanel;
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
    void demandeSauvegarde();
    void demandeQuitter();
    void tourTermine();
    void tuileRotationnee(int joueur, int pas);
    void validationPlacementDemandee(ZoneJeuWidget* zone, int joueur, TuileItem* tuile, const QPoint& coordonneesTheoriques);

public slots:
    void afficherPlateauJoueur(const int& index);
    void chargerPlateauJoueur(const int& index, const std::vector<Tuile>& tuiles);
    void afficherTuileEnMain(const int& index, const Tuile& tuile);
    void finaliserTourApresPlacement(TuileItem* tuile, const QPointF& positionScene);
    void afficherMessage(const QString& message);

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
    void relayerValidationPlacementDemandee(TuileItem* tuile, const QPoint& coordonnees);
    void surRotationTuileEnMain(int pas);
};

#endif
