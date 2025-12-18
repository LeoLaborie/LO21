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
    void chargerDepuisSauvegarde(const std::string& nomSauvegarde);
    void lancerTour();
    void finDeTour();
    void joueurPiocheTuile(int idTuile);
    void joueurPlaceTuiel(const Position& p);
    void rotationTuileGraphique(int joueur, int pas);
    void verifierPlacementGraphique(ZoneJeuWidget* zone, int joueur, TuileItem* tuileGraphique, const QPoint& coordonnees);
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
    void valideTuilePiochee(int idTuile);
    void validePasTuilePiochee(int idTuile);
    void chargerPlateauGraphique(int joueur, const std::vector<Tuile>& tuiles);
    void afficherTuileMain(int joueur, const Tuile& tuile);
    void afficherMessage(const QString& message);

private:
    static ControllerView* instance;
    Partie partie;
    ControllerView() = default;
    ~ControllerView() = default;

    void initPlateau();
    void synchroniserPlateauxGraphiques();
    void mettreAJourScoreCourant();
};

#endif // CONTROLLERVIEW_H
