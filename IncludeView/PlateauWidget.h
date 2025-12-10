#ifndef PLATEAUWIDGET_H
#define PLATEAUWIDGET_H
#include <QWidget>

class PiocheWidget;
class ScorePanel;
class ZoneJeuWidget;

/**
 * @brief Widget principal affichant le plateau de jeu, la pioche et les informations de score.
 * Il encapsule deux scènes graphiques distinctes (plateau/pioche) afin de gérer
 * indépendamment le zoom et l'affichage.
 */
class PlateauWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construit le widget et installe les vues/scènes du plateau et de la pioche.
     */
    explicit PlateauWidget(QWidget* parent = nullptr);
    /**
     * @brief Ajoute graphiquement une tuile dans la scène principale.
     * @param t modèle métier servant de source pour les HexItem.
     * @param centrer centre automatiquement la tuile dans la zone si true.
     * @return pointeur vers le TuileItem créé pour manipulations ultérieures.
     */
private:
    ZoneJeuWidget* zoneJeuWidget = nullptr;
    PiocheWidget* piocheWidget = nullptr;
    ScorePanel* scorePanel = nullptr;
};

#endif
