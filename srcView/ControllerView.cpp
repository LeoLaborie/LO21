#include <algorithm>
#include <exception>
#include <QDebug>
#include <vector>
#include <QString>
#include <QStringList>
#include "../include/Partie.h"
#include "../include/Sauvegarde.h"
#include "../IncludeView/ControllerView.h"
#include "../IncludeView/ZoneJeuWidget.h"

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
    try
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
    catch (const std::exception& e)
    {
        emit afficherErreur(QString::fromStdString(e.what()));
    }
}


bool ControllerView::chargerDepuisSauvegarde(const std::string& nomSauvegarde){
    try
    {
        std::string nom = nomSauvegarde;
        constexpr const char* extension = ".ratatata";
        constexpr size_t extensionLen = 9; // strlen(".ratatata")
        if (nom.size() < extensionLen || nom.substr(nom.size() - extensionLen) != extension)
            nom += ".ratatata";
        partie = Partie::FromSave("saves/" + nom);
        initPlateau();
        return true;
    }
    catch (const std::exception& e)
    {
        emit afficherErreur(QString::fromStdString(e.what()));
        return false;
    }
}

void ControllerView::sauvegarderPartieGraphique()
{
    const bool ok = sauvegarderPartie(partie);
    if (ok)
        emit afficherMessage(QStringLiteral("Sauvegarde effectuée"));
    else
        emit afficherErreur(QStringLiteral("Erreur : sauvegarde échouée"));
}

void ControllerView::initPlateau(){
    // Synchronise toute l'UI avec l'état courant du modèle (plateaux, chantier, pierres), puis démarre le tour.
    synchroniserPlateauxGraphiques();
    emit setChantier(partie.getChantier().getTuiles());
    emit setNbPierres(partie.getJoueurMain().getNbrPierres());
    lancerTour();
}


void ControllerView::lancerTour(){

    Joueur &joueur = partie.getJoueurMain();
    // Nouveau tour : on "oublie" toute pioche en cours (on ne doit plus pouvoir annuler une ancienne sélection).
    piocheEnCours = false;
    indicePiocheEnCours = -1;
    tuilePiocheeInitiale = Tuile();
    emit setMainJoueurPlateau(partie.getMainJoueur());
    emit joueurActifChange(QString::fromStdString(joueur.getNom()));
    const QString message = QString("C'est au tour de %1").arg(QString::fromStdString(joueur.getNom()));
    emit afficherMessage(message);
    emit setNbPierres(joueur.getNbrPierres());
    mettreAJourScoreCourant();
}

void ControllerView::finDeTour()
{
    // Fin de tour : si le chantier est presque vide, on le ré-alimente avec une pile (ou fin de partie).
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
    // Recharge tous les plateaux depuis le modèle (utile après chargement ou actions IA).
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
    // Recalcule/rafraîchit le score du joueur courant.
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
        // L'IA a joué directement : on resynchronise toute la vue.
        synchroniserPlateauxGraphiques();
        emit setChantier(partie.getChantier().getTuiles());
        emit setNbPierres(joueurcourant.getNbrPierres());
        mettreAJourScoreCourant();
        return;
    }

    try
    {
        joueurcourant.piocherTuile(idTuile, partie.getChantier(), partie.getFauxJoueur());
    }
    catch (const std::exception& e)
    {
        emit afficherErreur(QString::fromStdString(e.what()));
        emit validePasTuilePiochee(idTuile);
        return;
    }
    // Pioche validée : on mémorise l'état pour permettre une annulation (remettre la tuile et rembourser les pierres).
    piocheEnCours = true;
    indicePiocheEnCours = idTuile;
    tuilePiocheeInitiale = joueurcourant.getTuileEnMain();
    emit valideTuilePiochee(idTuile);
    emit setNbPierres(joueurcourant.getNbrPierres());


    //pas utile je pense mais je laisse la au cas où
    // if (partie.fauxJoueurPresent()){
    //     plateau->updatePierres(partie.getFauxJoueur());
    // }
}

void ControllerView::annulerPiocheTuile(int idTuile)
{
    // Annulation via l'UI (bouton "annuler placement") : on remet la tuile dans le chantier et on rembourse les pierres.
    if (!piocheEnCours)
        return;

    Joueur& joueurCourant = partie.getJoueurMain();
    if (joueurCourant.getTuileEnMain().getNbHexa() == 0)
        return;

    const int indice = (idTuile >= 0) ? idTuile : indicePiocheEnCours;
    if (indice != indicePiocheEnCours)
        return;

    partie.getChantier().insererTuile(indice, tuilePiocheeInitiale);
    joueurCourant.setTuileEnMain(Tuile());
    joueurCourant.setNbrPierres(joueurCourant.getNbrPierres() + indice);
    if (Joueur* faux = partie.getFauxJoueur())
        faux->setNbrPierres(faux->getNbrPierres() - indice);

    piocheEnCours = false;
    indicePiocheEnCours = -1;
    tuilePiocheeInitiale = Tuile();

    emit setChantier(partie.getChantier().getTuiles());
    emit setNbPierres(joueurCourant.getNbrPierres());
}

void ControllerView::joueurPlaceTuiel(const Position& p){
    Joueur& joueur = partie.getJoueurMain();
    Tuile tuile = joueur.getTuileEnMain();
    std::string raison;
    if (!joueur.getPlateau().verifierPlacementTuile(p, tuile, &raison))
    {
        if (raison.empty())
            raison = "Vous ne pouvez pas placer cette tuile ici";
        emit afficherErreur(QString::fromStdString(raison));
        return;
    }

    try
    {
        joueur.placerTuile(tuile, const_cast<Position&>(p));
        mettreAJourScoreCourant();
    }
    catch (const std::exception& e)
    {
        emit afficherErreur(QString::fromStdString(e.what()));
    }
}

void ControllerView::rotationTuileGraphique(int joueur, int pas)
{
    if (pas == 0)
        return;
    (void)joueur;
    Joueur& joueurCourant = partie.getJoueurMain();
    Tuile tuile = joueurCourant.getTuileEnMain();
    if (tuile.getNbHexa() == 0)
        return;
    tuile.pivoterTuile(pas);
    joueurCourant.setTuileEnMain(tuile);
}

void ControllerView::verifierPlacementGraphique(ZoneJeuWidget* zone, int joueur, TuileItem* tuileGraphique, const QPoint& coordonnees)
{
    // Vérifie côté modèle si le placement demandé par l'UI est autorisé, puis valide (ou affiche une erreur).
    if (!zone || !tuileGraphique || partie.getNbrJoueurs() == 0 || joueur != partie.getMainJoueur())
        return;

    Joueur& joueurCourant = partie.getJoueurMain();
    Plateau& plateau = joueurCourant.getPlateau();
    const int x = coordonnees.x();
    const int y = coordonnees.y();
    Tuile tuileEnMain = joueurCourant.getTuileEnMain();
    if (tuileEnMain.getNbHexa() == 0)
    {
        emit afficherErreur(QStringLiteral("Placement invalide : aucune tuile en main"));
        return;
    }

    const auto positionsLegales = plateau.getPositionsLegales(tuileEnMain);
    const auto it = std::find_if(positionsLegales.begin(), positionsLegales.end(), [&](const Position& p) { return p.x == x && p.y == y; });
    if (it == positionsLegales.end())
    {
        // Si (x,y) n'est pas légal, on redemande au à plateau une raison explicite avec un z candidat
        int zMax = -1;
        plateau.pourChaqueHexagone([&](const Hexagone* h)
                                   {
            if (h && h->getX() == x && h->getY() == y)
                zMax = std::max(zMax, h->getZ());
        });
        const int zPossible = std::max(0, zMax + 1);
        std::string raison;
        plateau.verifierPlacementTuile(Position{x, y, zPossible}, tuileEnMain, &raison);
        emit afficherErreur(QString::fromStdString(raison.empty() ? "Placement invalide" : raison));
        return;
    }

    Position positionChoisie = *it;
    try
    {
        joueurCourant.placerTuile(tuileEnMain, positionChoisie);
    }
    catch (const std::exception& e)
    {
        emit afficherErreur(QString::fromStdString(e.what()));
        return;
    }
    emit etageDetermine(tuileGraphique, positionChoisie.z);
    emit setNbPierres(joueurCourant.getNbrPierres());
    // Le placement est validé : il n'y a plus de pioche "annulable".
    piocheEnCours = false;
    indicePiocheEnCours = -1;
    tuilePiocheeInitiale = Tuile();
    mettreAJourScoreCourant();
    zone->confirmerPlacementApprouve(tuileGraphique);
}
