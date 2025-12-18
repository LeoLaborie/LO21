#include <vector>
#include <QString>
#include <QStringList>
#include "../include/Partie.h"
#include "../IncludeView/ControllerView.h"

ControllerView* ControllerView::instance = nullptr;

ControllerView* ControllerView::giveInstance()
{
    if (!instance)
        instance = new ControllerView();
    return instance;
}

void ControllerView::freeInstance(){
    delete instance;
    instance = nullptr;
}



void ControllerView::creerNouvellePartie(int nb, const QStringList& pseudos, const QVector<bool>& variantes)
{
    bool utiliserToutesLesTuiles = variantes[0];

    bool variantesPoints[5] = {false};

    for (int i = 1; i < 5; ++i) {
        if (i + 1 < variantes.size()) {
            variantesPoints[i] = variantes[i + 1];
        }
    }

    std::vector<std::string> p;
    p.reserve(pseudos.size());

    for (const QString& pseudo : pseudos) {
        p.push_back(pseudo.toStdString());
    }

    partie = Partie(nb, p, variantesPoints, utiliserToutesLesTuiles);
    initPlateau();
}


void ControllerView::chargerDepuisSauvegarde(const std::string& nomSauvegarde){
    partie = Partie::FromSave("saves/" + nomSauvegarde);
    initPlateau();
}

void ControllerView::initPlateau(){
    synchroniserPlateauxGraphiques();
    emit setChantier(partie.getChantier().getTuiles());
    emit setNbPierres(partie.getJoueurMain().getNbrPierres());
    lancerTour();
}


void ControllerView::lancerTour(){

    Joueur &joueur = partie.getJoueurMain();
    emit setMainJoueurPlateau(partie.getMainJoueur());
    emit joueurActifChange(QString::fromStdString(joueur.getNom()));
    const QString message = QString("C'est au tour de %1").arg(QString::fromStdString(joueur.getNom()));
    emit afficherMessage(message);
    emit setNbPierres(joueur.getNbrPierres());
    mettreAJourScoreCourant();
}

void ControllerView::finDeTour()
{
    if (partie.getChantier().getTaille() <= 1)
    {
        emit afficherMessage(QStringLiteral("Il reste une seule tuile dans le chantier."));
        partie.tourTermine();
        if (partie.pilesRestantes())
        {
            partie.addTuileInChantierFromPiles();
            emit setChantier(partie.getChantier().getTuiles());
        }
        else
        {
            emit afficherMessage(QStringLiteral("Plus de piles disponibles : fin de partie."));
            return;
        }
    }

    partie.setProchainJoueur();
    emit setMainJoueurPlateau(partie.getMainJoueur());
    lancerTour();
}

void ControllerView::synchroniserPlateauxGraphiques()
{
    const auto& joueurs = partie.getJoueurs();
    for (size_t i = 0; i < joueurs.size(); ++i)
    {
        Joueur* joueur = joueurs[i];
        if (!joueur)
            continue;
        emit chargerPlateauGraphique(static_cast<int>(i), joueur->getPlateau().getTuiles());
    }

    const int joueurCourant = partie.getMainJoueur();
    if (joueurCourant >= 0 && joueurCourant < static_cast<int>(joueurs.size()))
    {
        const Joueur* joueur = joueurs[static_cast<size_t>(joueurCourant)];
        if (joueur && joueur->getTuileEnMain().getNbHexa() > 0)
            emit afficherTuileMain(joueurCourant, joueur->getTuileEnMain());
    }
}

void ControllerView::mettreAJourScoreCourant()
{
    if (partie.getNbrJoueurs() == 0)
        return;

    Joueur& joueur = partie.getJoueurMain();
    joueur.setNbrPoints();
    const int total = joueur.getNbrPoints();
    emit setScore(total, 0, 0, 0, 0, 0);
}

void ControllerView::joueurPiocheTuile(int idTuile){

    Joueur &joueurcourant = partie.getJoueurMain();

    //pas à faire ici !!!!!!!!!!!!!!!!!!!
    if (joueurcourant.isIA()){
        IllustreArchitecte &ia = dynamic_cast<IllustreArchitecte &>(joueurcourant);
        Tuile& tuile = ia.piocherTuile(idTuile, partie.getChantier(), nullptr);
        ia.placerTuile(tuile);
    }

    if (idTuile < joueurcourant.getNbrPierres()){
        joueurcourant.piocherTuile(idTuile ,partie.getChantier() ,partie.getFauxJoueur());
        //plateau->updatePierres(joueur);
        emit valideTuilePiochee(idTuile);
        emit setNbPierres(joueurcourant.getNbrPierres());
    }else{
        const QString message = QString("Vous n'avez pas assez de pierres pour piocher cette tuile");
        emit afficherMessage(message);
        emit validePasTuilePiochee(idTuile);
    }


    //pas utile je pense mais je laisse la au cas où
    // if (partie.fauxJoueurPresent()){
    //     plateau->updatePierres(partie.getFauxJoueur());
    // }
}

void ControllerView::joueurPlaceTuiel(const Position& p){
    Joueur& joueur = partie.getJoueurMain();
    Tuile tuile = joueur.getTuileEnMain();
    if (joueur.getPlateau().verifierPlacementTuile(p,tuile)){
        joueur.placerTuile(tuile, const_cast<Position&>(p));
        mettreAJourScoreCourant();
    }else{
        const QString message = QString("Vous ne pouvez pas placer cette tuile ici");
        emit afficherMessage(message);
    }
}
