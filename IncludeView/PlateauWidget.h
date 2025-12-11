#ifndef PLATEAUWIDGET_H
#define PLATEAUWIDGET_H
#include <QWidget>
#include <memory>
#include <vector>

class ChantierWidget;
class ScorePanel;
class ZoneJeuWidget;
class EchapWidget;
class QShortcut;
class Tuile;
class Hexagone;

/**
 * @brief Widget principal affichant le plateau de jeu, le chantier et les informations de score.
 * Il encapsule deux scènes graphiques distinctes (plateau/chantier) afin de gérer
 * indépendamment le zoom et l'affichage.
 */
class PlateauWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construit le widget et installe les vues/scènes du plateau et du chantier.
     */
    explicit PlateauWidget(QWidget* parent = nullptr);
    /**
     * @brief Ajoute graphiquement une tuile dans la scène principale.
     * @param t modèle métier servant de source pour les HexItem.
     * @param centrer centre automatiquement la tuile dans la zone si true.
     * @return pointeur vers le TuileItem créé pour manipulations ultérieures.
     */
signals:
    void demandeParametres();
    void demandeRetourMenu();
    void demandeQuitter();

private:
    ZoneJeuWidget* zoneJeuWidget = nullptr;
    ChantierWidget* chantierWidget = nullptr;
    ScorePanel* scorePanel = nullptr;
    EchapWidget* echapWidget = nullptr;
    QShortcut* raccourciEchap = nullptr;
    std::vector<std::unique_ptr<Tuile>> tuilesDeTest;
    std::vector<std::unique_ptr<Hexagone>> hexagonesDeTest;

    void genererTuilesTests();
    void basculerMenuEchap();
    void gererBlocageInteractions(bool widgetActif);
};

#endif
