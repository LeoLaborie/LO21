#ifndef CONTROLLERVIEW_H
#define CONTROLLERVIEW_H

#include <array>
#include <vector>
#include <string>
#include <QMainWindow>
#include "Partie.h"
#include "PlateauWidget.h"
#include  "Tuile.h"
#include "Position.h"


class ControllerView
{
public:
    static ControllerView* giveInstance(PlateauWidget* plateau=nullptr);
    static void freeInstance();
    void executerGraphique();

    bool chargerDepuisSauvegarde();
    // bool phaseManipulationTuile(Joueur &joueur, Tuile &tuilePiochee);
    // void phasePlacementTuile(Joueur &joueur);
    // Position demanderPosition() const;
    bool confirmerQuitter();
    void afficherMessageTemporaire(const QString& message);
    void initPlateau();

signals:
    void creerPlateau(int& nbJoueurs);

public slots:
    //void recupArgument(int nb, const QStringList& pseudos, const QVector<bool>& variantes);//rajouter si on veut utiliser toutes les tuiles
    void creerNouvellePartie(int nb, const QStringList& pseudos, const QVector<bool>& variantes);
    void chargerDepuisSauvegarde(std::string nomSauvegarde);
    void Toursuivant();
    void joueurPiocheTuile(int& idTuile);
    void joueurPlaceTuiel(Position& p);


private:
    static ControllerView* instance;
    PlateauWidget& plateau;
    Partie& partie;
    explicit ControllerView(PlateauWidget& pl, Partie& pa):plateau(pl),partie(pa){}
    ~ControllerView()=default;
};


#endif // CONTROLLERVIEW_H
