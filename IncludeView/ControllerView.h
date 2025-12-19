#ifndef CONTROLLERVIEW_H
#define CONTROLLERVIEW_H

#include <vector>
#include <string>
#include <QObject>
#include <QPoint>
#include <QString>
#include <QStringList>
#include "Partie.h"
#include "Tuile.h"
#include "Position.h"

class ZoneJeuWidget;
class TuileItem;

class ControllerView : public QObject
{
    Q_OBJECT

public:
    static ControllerView* giveInstance();
    static void freeInstance();

public slots:
    void creerNouvellePartie(int nb, const QStringList& pseudos, const QVector<bool>& variantes);
    bool chargerDepuisSauvegarde(const std::string& nomSauvegarde);
    void lancerTour();
    void finDeTour();
    void joueurPiocheTuile(TuileId idTuile);
    void annulerPiocheTuile(TuileId idTuile);
    void joueurPlaceTuiel(const Position& p);
    void rotationTuileGraphique(int joueur, int pas);
    void verifierPlacementGraphique(ZoneJeuWidget* zone, int joueur, TuileItem* tuileGraphique, const QPoint& coordonnees);
    /**
     * @brief Sauvegarde la partie courante depuis l'interface graphique.
     */
    void sauvegarderPartieGraphique();

signals:
    void setMainJoueurPlateau(int joueur);
    void setChantier(const std::vector<Tuile>& piles);
    void setNbPierres(int nbPierres);
    void setScore(int scoreTotal,
                  int scoreHabitation,
                  int scoreMarche,
                  int scoreCaserne,
                  int scoreTemple,
                  int scoreJardin);
    void joueurActifChange(const QString& nom);
    void valideTuilePiochee(TuileId idTuile);
    void validePasTuilePiochee(TuileId idTuile);
    void fauxJoueurPiocheTuile(TuileId idTuile);
    void chargerPlateauGraphique(int joueur, const std::vector<Tuile>& tuiles);
    void afficherTuileMain(int joueur, const Tuile& tuile);
    void afficherMessage(const QString& message);
    void afficherErreur(const QString& message);
    void etageDetermine(TuileItem* tuile, int z);
    void partieFinie();

private:
    static ControllerView* instance;
    Partie partie;
    ControllerView() = default;
    ~ControllerView() = default;

    void initPlateau();
    void synchroniserPlateauxGraphiques();
    void mettreAJourScoreCourant();
    void afficherFinPartie();

    bool piocheEnCours = false;
    int indicePiocheEnCours = -1;
    TuileId idPiocheEnCours = 0;
    Tuile tuilePiocheeInitiale;
    bool partieTerminee = false;
};

#endif // CONTROLLERVIEW_H
