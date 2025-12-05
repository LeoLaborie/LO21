#ifndef PLATEAUWIDGET_H
#define PLATEAUWIDGET_H
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QRectF>
#include "Partie.h"
#include "TuileItem.h"
#include <QLabel>

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
    TuileItem* ajouterTuile(Tuile & t, bool centrer = true);
    /**
     * @brief Ajoute et positionne immédiatement une tuile sur les coordonnées axiales (q,r).
     */
    TuileItem* ajouterTuile(Tuile & t, int q, int r);

private:
    QGraphicsScene* plateauScene = nullptr;
    QGraphicsScene* piocheScene = nullptr;
    QGraphicsRectItem* zonePlateauRectItem = nullptr;
    QGraphicsRectItem* zonePiocheRectItem = nullptr;
    QRectF zonePlateauRect;
    QRectF zonePiocheRect;
    QGraphicsView* plateauView = nullptr;
    QGraphicsView* piocheView = nullptr;
    QWidget* panneauScores = nullptr;
    QLabel * score = nullptr;
    QLabel *erreur = nullptr;
    QLabel * joueurSelect = nullptr;
    /**
     * @brief Construit un setup de démonstration avec la tuile de départ et quelques exemples.
     */
    void initialiserPlateauDemo();
};

#endif
