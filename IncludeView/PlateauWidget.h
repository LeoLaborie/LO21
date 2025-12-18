#ifndef PLATEAUWIDGET_H
#define PLATEAUWIDGET_H
#include <QStackedWidget>
#include <QWidget>
#include <vector>

class ChantierWidget;
class ScorePanel;
class ZoneJeuWidget;
class EchapWidget;
class QShortcut;
class Tuile;
class Hexagone;
class TuileItem;

/**
 * @brief Widget principal affichant le plateau de jeu, le chantier et les informations de score.
 * Le contrôleur doit indiquer le nombre de joueurs lors de la création et
 * alimenter le chantier en TuileItem pour chaque tour.
 */
class PlateauWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construit le widget et installe les vues/scènes du plateau et du chantier.
     */
    explicit PlateauWidget(QWidget* parent = nullptr, int nbJoueurs = 1);
    /**
     * @brief Le contrôleur doit appeler afficherPlateauJoueur(index)
     * avant d'envoyer une TuileItem dans la zone choisie.
     */

    ChantierWidget* getChantierWidget()const {return chantierWidget;}
    ScorePanel* getScorePanel()const {return scorePanel;}

signals:
    void demandeParametres();
    void demandeRetourMenu();
    void demandeQuitter();
    void placementTermine();  // pour que le controleur passe au tour suivant

public slots:
    /**
     * @brief Affiche la scène correspondant au joueur demandé.
     * @param index Indice du joueur actif (0-based).
     */
    void afficherPlateauJoueur(const int& index);

    void validerPlacementTuile(TuileItem* t);
    void replacerTuile();

// private slots:
//     void validerPlacementTuile(TuileItem* t);
//     void replacerTuile();

private:
    QStackedWidget* stackPlateaux = nullptr;
    std::vector<ZoneJeuWidget*> zonesParJoueur;
    ZoneJeuWidget* zoneJeuWidget = nullptr;
    int joueurActif = 0;
    ChantierWidget* chantierWidget = nullptr;
    ScorePanel* scorePanel = nullptr;
    EchapWidget* echapWidget = nullptr;
    QShortcut* raccourciEchap = nullptr;
    // Le contrôleur doit alimenter le chantier et gérer la pile métier,
    // le widget ne crée plus de tuiles de test.
    void basculerMenuEchap();
    void gererBlocageInteractions(bool widgetActif);
};

#endif
