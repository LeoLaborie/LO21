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
    /**
     * @brief Demande l'ouverture de la page de paramètres (menu pause).
     */
    void demandeParametres();
    /**
     * @brief Demande un retour au menu principal (menu pause).
     */
    void demandeRetourMenu();
    /**
     * @brief Demande une sauvegarde de la partie en cours (menu pause).
     */
    void demandeSauvegarde();
    /**
     * @brief Demande de quitter l'application (menu pause).
     */
    void demandeQuitter();
    void tourTermine();
    void tuileRotationnee(int joueur, int pas);
    void validationPlacementDemandee(ZoneJeuWidget* zone, int joueur, TuileItem* tuile, const QPoint& coordonneesTheoriques);
    void piocheAnnulee(int indiceTuile);

public slots:
    void afficherPlateauJoueur(const int& index);
    void chargerPlateauJoueur(const int& index, const std::vector<Tuile>& tuiles);
    void afficherTuileEnMain(const int& index, const Tuile& tuile);
    void finaliserTourApresPlacement(TuileItem* tuile, const QPointF& positionScene);
    void afficherMessage(const QString& message);
    void afficherErreur(const QString& message);
    void ModifierCouleurEtage(TuileItem* tuile, int z);

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
