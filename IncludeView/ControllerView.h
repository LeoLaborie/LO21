#ifndef CONTROLLERVIEW_H
#define CONTROLLERVIEW_H

#include <array>
#include <vector>
#include <string>
#include <QMainWindow>
#include "Partie.h"
#include "PlateauWidget.h"


class ControllerView
{
public:
    static ControllerView* giveInstance(PlateauWidget& plateau);
    static void freeInstance();
    void executerGraphique();

    bool chargerDepuisSauvegarde(Partie &partie);
    void boucleDeJeu(Partie &partie);
    void traiterTourHumain(Partie &partie);
    void traiterTourIA(Partie &partie);
    bool phaseManipulationTuile(Joueur &joueur, Partie &partie, Tuile &tuilePiochee);
    void phasePlacementTuile(Joueur &joueur);
    Position demanderPosition() const;
    bool confirmerQuitter(Partie &partie);
    void afficherMessageTemporaire(const QString& message);

public slots:
    //void recupArgument(int nb, const QStringList& pseudos, const QVector<bool>& variantes);//rajouter si on veut utiliser toutes les tuiles
    void creerNouvellePartie(int nb, const QStringList& pseudos, const QVector<bool>& variantes);
    void chargerDepuisSauvegarde(std::string nomSauvegarde);

private:
    static ControllerView* instance;
    PlateauWidget& plateau;
    explicit ControllerView(PlateauWidget& p):plateau(p){}
    ~ControllerView()=default;
};


#endif // CONTROLLERVIEW_H
