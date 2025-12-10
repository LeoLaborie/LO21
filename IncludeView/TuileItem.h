#ifndef TUILEITEM_H
#define TUILEITEM_H

#include <QGraphicsItemGroup>
#include <QPointF>
#include <QObject>
#include "HexItem.h"
#include "Tuile.h"

/**
 * @brief Représentation graphique d'une tuile (ensemble d'hexagones) dans la scène.
 * Gère les interactions (rotation/déplacement) et assure l'alignement sur la grille axiale.
 */
class TuileItem : public QObject ,public QGraphicsItemGroup {
    Q_OBJECT
public:
    /**
     * @brief Définit le mode de fonctionnement de la TuileItem si elle est dans le pioche ou dans le zone de jeu.
     */
    enum class Mode { Pioche, ZoneJeu };
    /**
     * @brief Construit une tuile graphique à partir d'une tuile métier et crée les HexItem associés.
     */
    TuileItem(Tuile& modele, QGraphicsItem* parent = nullptr,Mode m=Mode::Pioche,int tailleTuile=50,int indice=NULL);
    void setCoordonnee(int x,int y);
    /**
     * @brief Ajoute dynamiquement un hexagone à la tuile graphique.
     */
    void addHex(HexItem* hex);
    /**
     * @brief Pivote la tuile de 60° autour de son centre si l'interaction est autorisée.
     */
    void rotate60();
    /**
     * @brief Repositionne la tuile pour que la référence tombe exactement sur la grille axiale.
     */
    void replacerCorrectement();
    /**
     * @brief Active/désactive le déplacement et la rotation de la tuile.
     */
    void setInteractivite(bool autoriserDeplacement, bool autoriserRotation);
    /**
     * @brief Retourne l'indice de l'objet quand il était dans le pioche.
     */
    unsigned int getIndiceDansPioche()const {return indice;} 
    /**
     * @brief Retourne le mode de la TuileItem.
     */
    Mode getMode() const {return mode;};
    /**
     * @brief Changer le mode de la TuileItem.
     */
    void setMode(Mode m) {mode=m;};

    void setTaille(int nouvelleTaille);

signals:
    void rightClicked();
    void estPiocher(int indice);

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    HexItem* hexRef = nullptr;
    int tailleHex = 50;
    bool rotationAutorisee = true;
    unsigned int indice; //utiliser que quand la tuile item est dans la pioche sinon inutile
    Mode mode;
};

#endif // TUILEITEM_H
