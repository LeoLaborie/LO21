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


    //pour la mainwindow ou le plateau (je sais pas trop)
    void setPlateau(int& nbJoueurs,
                           Chantier& chantier,
                           std::vector<Joueur *>& joueurs,
                           bool& fauxJoueurP);//attention au comportement du faux joueur pour la pioche et le placement dans le plateau
                                              // et aussi pour le score

    //je sais pas si c'est vraiment utile
    void setMaitreArchitecte(int& joueur);

    //signals pour le plateau
    void setMainJoueurPlateau(const int& joueur);
    void tuilePlacee(const int& hauteur); //dans PlateauWidget.h tu demande un TuileItem dans le slot mais je sais pas comment te l'envoyer
    void tuilePasPlacee();

    //signals pour le score
    void setScore(int& scoreH, int& scoreM, int& scoreC, int& scoreT, int& scoreJ);
    void setNbPierres(const int& nbPierres);

    //signals pour la pioche
    void setChantier(const std::vector<Tuile>& piles);
    void valideTuilePiochee(int& idTuile);
    void validePasTuilePiochee(int& idTuile);

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
