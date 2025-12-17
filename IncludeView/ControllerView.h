#ifndef CONTROLLERVIEW_H
#define CONTROLLERVIEW_H

#include <array>
#include <vector>
#include <string>
#include <QMainWindow>
#include "Partie.h"
#include "PlateauWidget.h"
#include "Tuile.h"
#include "Position.h"


class ControllerView
{
public:
    static ControllerView* giveInstance();
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
    void envoiePourPlateau(int& nbJoueurs,
                           Chantier& chantier,
                           std::vector<Joueur *>& joueurs,
                           bool& fauxJoueurP);
    void setMainJoueurPlateau(const int& joueur);
    void setMaitreArchitecte(int& joueur);
    void setChantier(const std::vector<Tuile>& piles);
    void valideTuilePiochee(int& idTuile);

public slots:
    //void recupArgument(int nb, const QStringList& pseudos, const QVector<bool>& variantes);//rajouter si on veut utiliser toutes les tuiles
    void creerNouvellePartie(int nb, const QStringList& pseudos, const QVector<bool>& variantes);
    void chargerDepuisSauvegarde(std::string nomSauvegarde);
    void Toursuivant();
    void joueurPiocheTuile(int& idTuile);
    void joueurPlaceTuiel(Position& p);


private:
    static ControllerView* instance;
    Partie& partie;
    explicit ControllerView(Partie& pa):partie(pa){}
    ~ControllerView()=default;
};


#endif // CONTROLLERVIEW_H
