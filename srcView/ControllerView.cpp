#include <algorithm>
#include <exception>
#include <QDebug>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QApplication>
#include <vector>
#include <QString>
#include <QStringList>
#include "../include/Partie.h"
#include "../include/Sauvegarde.h"
#include "../IncludeView/ControllerView.h"
#include "../IncludeView/ZoneJeuWidget.h"

ControllerView *ControllerView::instance = nullptr;

ControllerView *ControllerView::giveInstance()
{
    if (!instance)
        instance = new ControllerView();
    return instance;
}

void ControllerView::freeInstance()
{
    delete instance;
    instance = nullptr;
}

void ControllerView::creerNouvellePartie(int nb, const QStringList &pseudos, const QVector<bool> &variantes)
{
    try
    {
        bool utiliserToutesLesTuiles = variantes[0];

        bool variantesPoints[5] = {false};

        for (int i = 1; i < 5; ++i)
        {
            if (i + 1 < variantes.size())
            {
                variantesPoints[i] = variantes[i + 1];
            }
        }

        std::vector<std::string> p;
        p.reserve(pseudos.size());

        for (const QString &pseudo : pseudos)
        {
            p.push_back(pseudo.toStdString());
        }

        partie = Partie(nb, p, variantesPoints, utiliserToutesLesTuiles);
        initPlateau();
    }
    catch (const std::exception &e)
    {
        emit afficherErreur(QString::fromStdString(e.what()));
    }
}

bool ControllerView::chargerDepuisSauvegarde(const std::string &nomSauvegarde)
{
    try
    {
        std::string nom = nomSauvegarde;
        constexpr const char *extension = ".ratatata";
        constexpr size_t extensionLen = 9; // strlen(".ratatata")
        if (nom.size() < extensionLen || nom.substr(nom.size() - extensionLen) != extension)
            nom += ".ratatata";
        partie = Partie::FromSave("saves/" + nom);
        initPlateau();
        return true;
    }
    catch (const std::exception &e)
    {
        emit afficherErreur(QString::fromStdString(e.what()));
        return false;
    }
}

void ControllerView::sauvegarderPartieGraphique()
{
    const bool ok = sauvegarderPartie(partie);
    if (ok)
        emit(QStringLiteral("Sauvegarde effectuée"));
    else
        emit afficherErreur(QStringLiteral("Erreur : sauvegarde échouée"));
}

void ControllerView::initPlateau()
{
    // Synchronise toute l'UI avec l'état courant du modèle (plateaux, chantier, pierres), puis démarre le tour.
    synchroniserPlateauxGraphiques();
    emit setChantier(partie.getChantier().getTuiles());
    emit setNbPierres(partie.getJoueurMain().getNbrPierres());
    lancerTour();
}

void ControllerView::lancerTour()
{

    Joueur &joueur = partie.getJoueurMain();
    // Nouveau tour : on "oublie" toute pioche en cours (on ne doit plus pouvoir annuler une ancienne sélection).
    piocheEnCours = false;
    indicePiocheEnCours = -1;
    idPiocheEnCours = 0;
    tuilePiocheeInitiale = Tuile();
    emit setMainJoueurPlateau(partie.getMainJoueur());
    emit joueurActifChange(QString::fromStdString(joueur.getNom()));
    const QString message = QString("C'est au tour de %1").arg(QString::fromStdString(joueur.getNom()));
    emit afficherMessage(message);
    emit setNbPierres(joueur.getNbrPierres());
    mettreAJourScoreCourant();

    if (partieTerminee)
        return;

    if (joueur.isIA())
    {
        const int joueurIndex = partie.getMainJoueur();
        if (partieTerminee)
            return;
        if (partie.getMainJoueur() != joueurIndex)
            return;
        Joueur &joueurCourant = partie.getJoueurMain();
        if (!joueurCourant.isIA())
            return;

        IllustreArchitecte &ia = dynamic_cast<IllustreArchitecte &>(joueurCourant);
        const int indiceChoisi = ia.choixTuile(partie.getChantier());
        const TuileId idTuile = partie.getChantier().getTuiles()[indiceChoisi].getId();

        // Joueur* receveurPierres = nullptr;
        // if (partie.fauxJoueurPresent())
        // {
        //     // Dans le mode "1 joueur + Illustre Architecte", quand l'IA pioche, les pierres vont au joueur humain.
        //     const auto& joueurs = partie.getJoueurs();
        //     const int dernierIndex = partie.getNbrJoueurs() - 1;
        //     if (joueurIndex == dernierIndex && !joueurs.empty())
        //         receveurPierres = joueurs.front();
        //     else
        //         receveurPierres = partie.getFauxJoueur();
        // }

        try
        {
            Tuile &tuile = ia.piocherTuile(idTuile, partie.getChantier(), nullptr);
            ia.placerTuile(tuile);
        }
        catch (const std::exception &e)
        {
            emit afficherErreur(QString::fromStdString(e.what()));
            return;
        }

        emit fauxJoueurPiocheTuile(idTuile);
        emit setNbPierres(joueurCourant.getNbrPierres());
        emit chargerPlateauGraphique(joueurIndex, joueurCourant.getPlateau().getTuiles());
        mettreAJourScoreCourant();

        afficherInfoIA(indiceChoisi);
    }
}

void ControllerView::finDeTour()
{
    if (partieTerminee)
        return;
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
            afficherFinPartie();
            return;
        }
    }

    partie.setProchainJoueur();
    emit setMainJoueurPlateau(partie.getMainJoueur());
    lancerTour();
}

void ControllerView::afficherFinPartie()
{
    if (partieTerminee)
        return;
    partieTerminee = true;

    QList<QPair<int, QString>> scores;
    const auto &joueurs = partie.getJoueurs();
    for (int i = 0; i < partie.getNbrJoueurs(); ++i)
    {
        Joueur *j = joueurs[static_cast<size_t>(i)];
        if (!j)
            continue;
        j->setNbrPoints();
        scores.append(qMakePair(j->getNbrPoints(), QString::fromStdString(j->getNom())));
    }

    std::sort(scores.begin(), scores.end(), [](const QPair<int, QString> &a, const QPair<int, QString> &b)
              { return a.first > b.first; });

    QDialog dialog;
    dialog.setWindowTitle(QStringLiteral("Fin de partie"));
    dialog.setModal(true);
    dialog.resize(420, 520);

    auto *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(18, 18, 18, 18);
    layout->setSpacing(10);

    auto *title = new QLabel(QStringLiteral("Résultats"), &dialog);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 24px; font-weight: 700;");
    layout->addWidget(title);

    for (int i = 0; i < scores.size(); ++i)
    {
        const auto &pair = scores[i];
        auto *lbl = new QLabel(QString("%1 : %2 pts").arg(pair.second).arg(pair.first), &dialog);
        lbl->setAlignment(Qt::AlignCenter);
        if (i == 0)
            lbl->setStyleSheet("font-size: 20px; font-weight: 700;");
        layout->addWidget(lbl);
    }

    layout->addStretch(1);

    auto *btnMenu = new QPushButton(QStringLiteral("Retour au menu"), &dialog);
    btnMenu->setFixedHeight(44);
    layout->addWidget(btnMenu);

    QObject::connect(btnMenu, &QPushButton::clicked, &dialog, &QDialog::accept);
    const int result = dialog.exec();
    if (result == QDialog::Accepted)
        emit partieFinie();
    else
        QApplication::quit();
}

void ControllerView::synchroniserPlateauxGraphiques()
{
    // Recharge tous les plateaux depuis le modèle (utile après chargement ou actions IA).
    const auto &joueurs = partie.getJoueurs();
    for (size_t i = 0; i < joueurs.size(); ++i)
    {
        Joueur *joueur = joueurs[i];
        if (!joueur)
            continue;
        emit chargerPlateauGraphique(static_cast<int>(i), joueur->getPlateau().getTuiles());
    }

    const int joueurCourant = partie.getMainJoueur();
    if (joueurCourant >= 0 && joueurCourant < static_cast<int>(joueurs.size()))
    {
        const Joueur *joueur = joueurs[static_cast<size_t>(joueurCourant)];
        if (joueur && joueur->getTuileEnMain().getNbHexa() > 0)
            emit afficherTuileMain(joueurCourant, joueur->getTuileEnMain());
    }
}

void ControllerView::mettreAJourScoreCourant()
{
    // Recalcule/rafraîchit le score du joueur courant.
    if (partie.getNbrJoueurs() == 0)
        return;

    Joueur &joueur = partie.getJoueurMain();
    if (joueur.isIA())
    {
        IllustreArchitecte &ia = dynamic_cast<IllustreArchitecte &>(joueur);
        ia.setNbrPoints();
        const std::vector<int> tabscore = joueur.getPlateau().calculerPointsiaTab(ia.getdifficulte());
        const int total = joueur.getNbrPoints();
        if (tabscore.size() >= 6)
            emit setScore(total, tabscore[0], tabscore[1], tabscore[2], tabscore[3], tabscore[4]);
        else
            emit setScore(total, 0, 0, 0, 0, 0);
    }
    else
    {
        joueur.setNbrPoints();
        const std::vector<int> tabscore = joueur.getPlateau().calculerPointsTab();
        const int total = joueur.getNbrPoints();
        if (tabscore.size() >= 5)
            emit setScore(total, tabscore[0], tabscore[1], tabscore[2], tabscore[3], tabscore[4]);
        else
            emit setScore(total, 0, 0, 0, 0, 0);
    }
}

void ControllerView::joueurPiocheTuile(TuileId idTuile)
{
    Joueur &joueurcourant = partie.getJoueurMain();

    // Pendant le tour de l'IA, l'utilisateur ne doit pas pouvoir piocher.
    if (joueurcourant.isIA())
    {
        emit afficherMessage(QStringLiteral("C'est le tour de l'Illustre Architecte."));
        emit validePasTuilePiochee(idTuile);
        return;
    }

    const int indice = partie.getChantier().indexOf(idTuile);
    if (indice < 0)
    {
        emit afficherErreur(QStringLiteral("Tuile introuvable."));
        emit validePasTuilePiochee(idTuile);
        return;
    }

    try
    {
        joueurcourant.piocherTuile(idTuile, partie.getChantier(), partie.getFauxJoueur());
    }
    catch (const std::exception &e)
    {
        emit afficherErreur(QString::fromStdString(e.what()));
        emit validePasTuilePiochee(idTuile);
        return;
    }
    // Pioche validée : on mémorise l'état pour permettre une annulation (remettre la tuile et rembourser les pierres).
    piocheEnCours = true;
    indicePiocheEnCours = indice;
    idPiocheEnCours = idTuile;
    tuilePiocheeInitiale = joueurcourant.getTuileEnMain();
    emit valideTuilePiochee(idTuile);
    emit setNbPierres(joueurcourant.getNbrPierres());
}

void ControllerView::annulerPiocheTuile(TuileId idTuile)
{
    // Annulation via l'UI (bouton "annuler placement") : on remet la tuile dans le chantier et on rembourse les pierres.
    if (!piocheEnCours)
        return;

    Joueur &joueurCourant = partie.getJoueurMain();
    if (joueurCourant.getTuileEnMain().getNbHexa() == 0)
        return;

    if (idTuile != idPiocheEnCours)
        return;
    const int indice = indicePiocheEnCours;
    if (indice < 0)
        return;

    partie.getChantier().insererTuile(indice, tuilePiocheeInitiale);
    joueurCourant.setTuileEnMain(Tuile());
    joueurCourant.setNbrPierres(joueurCourant.getNbrPierres() + indice);
    if (Joueur *faux = partie.getFauxJoueur())
        faux->setNbrPierres(faux->getNbrPierres() - indice);

    piocheEnCours = false;
    indicePiocheEnCours = -1;
    idPiocheEnCours = 0;
    tuilePiocheeInitiale = Tuile();

    emit setChantier(partie.getChantier().getTuiles());
    emit setNbPierres(joueurCourant.getNbrPierres());
}

void ControllerView::joueurPlaceTuiel(const Position &p)
{
    Joueur &joueur = partie.getJoueurMain();
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
        joueur.placerTuile(tuile, const_cast<Position &>(p));
        mettreAJourScoreCourant();
    }
    catch (const std::exception &e)
    {
        emit afficherErreur(QString::fromStdString(e.what()));
    }
}

void ControllerView::rotationTuileGraphique(int, int pas)
{
    if (pas == 0)
        return;
    Joueur &joueurCourant = partie.getJoueurMain();
    Tuile tuile = joueurCourant.getTuileEnMain();
    if (tuile.getNbHexa() == 0)
        return;
    tuile.pivoterTuile(pas);
    joueurCourant.setTuileEnMain(tuile);
}

void ControllerView::verifierPlacementGraphique(ZoneJeuWidget *zone, int joueur, TuileItem *tuileGraphique, const QPoint &coordonnees)
{
    // Vérifie côté modèle si le placement demandé par l'UI est autorisé, puis valide (ou affiche une erreur).
    if (!zone || !tuileGraphique || partie.getNbrJoueurs() == 0 || joueur != partie.getMainJoueur())
        return;

    Joueur &joueurCourant = partie.getJoueurMain();
    Plateau &plateau = joueurCourant.getPlateau();
    const int x = coordonnees.x();
    const int y = coordonnees.y();
    Tuile tuileEnMain = joueurCourant.getTuileEnMain();
    if (tuileEnMain.getNbHexa() == 0)
    {
        emit afficherErreur(QStringLiteral("Placement invalide : aucune tuile en main"));
        return;
    }

    const auto positionsLegales = plateau.getPositionsLegales(tuileEnMain);
    bool trouve = false;
    Position positionChoisie{};
    for (const Position &p : positionsLegales)
    {
        if (p.x != x || p.y != y)
            continue;
        if (!trouve || p.z > positionChoisie.z)
        {
            positionChoisie = p;
            trouve = true;
        }
    }
    if (!trouve)
    {
        // Si (x,y) n'est pas légal, on redemande au à plateau une raison explicite avec un z candidat
        int zMax = -1;
        plateau.pourChaqueHexagone([&](const Hexagone *h)
                                   {
            if (h && h->getX() == x && h->getY() == y)
                zMax = std::max(zMax, h->getZ()); });
        const int zPossible = std::max(0, zMax + 1);
        std::string raison;
        plateau.verifierPlacementTuile(Position{x, y, zPossible}, tuileEnMain, &raison);
        emit afficherErreur(QString::fromStdString(raison.empty() ? "Placement invalide" : raison));
        return;
    }
    try
    {
        joueurCourant.placerTuile(tuileEnMain, positionChoisie);
    }
    catch (const std::exception &e)
    {
        emit afficherErreur(QString::fromStdString(e.what()));
        return;
    }
    emit etageDetermine(tuileGraphique, positionChoisie.z);
    emit setNbPierres(joueurCourant.getNbrPierres());
    // Le placement est validé : il n'y a plus de pioche "annulable".
    piocheEnCours = false;
    indicePiocheEnCours = -1;
    idPiocheEnCours = 0;
    tuilePiocheeInitiale = Tuile();
    mettreAJourScoreCourant();
    zone->confirmerPlacementApprouve(tuileGraphique);
}

void ControllerView::afficherInfoIA(const int &idTuile)
{
    QDialog dialog;
    dialog.setWindowTitle("Tour Illustre Architecte");
    dialog.resize(320, 260);
    dialog.setModal(true);

    dialog.setStyleSheet(
        "QDialog { background-color: #2c3e50; color: white; }"
        "QLabel { font-size: 18px; margin: 5px; }"
        "QPushButton { "
        "    background-color: #e67e22; color: white; "
        "    padding: 12px; border-radius: 6px; font-weight: bold; font-size: 16px;"
        "}"
        "QPushButton:hover { background-color: #d35400; }");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *title = new QLabel("Infos sur le tour de l'Illustre Architecte");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 26px; font-weight: bold; color: #f1c40f; margin-bottom: 20px;");
    layout->addWidget(title);

    QString tuile = QString("Il a pioché la tuile n°%1").arg(idTuile).toUpper();
    QLabel *labeltuile = new QLabel(tuile);
    labeltuile->setAlignment(Qt::AlignCenter);
    labeltuile->setStyleSheet("font-size: 20px; font-weight: bold; text-transform: uppercase; margin: 8px 0;");

    QString nbPierres = QString("Il a %1 pierres").arg(partie.getFauxJoueur()->getNbrPierres()).toUpper();
    QLabel *labelnbPierres = new QLabel(nbPierres);
    labelnbPierres->setAlignment(Qt::AlignCenter);
    labelnbPierres->setStyleSheet("font-size: 20px; font-weight: bold; text-transform: uppercase; margin: 8px 0;");

    QString score = QString("et %1 points").arg(partie.getFauxJoueur()->getNbrPoints()).toUpper();
    QLabel *labelscore = new QLabel(score);
    labelscore->setAlignment(Qt::AlignCenter);
    labelscore->setStyleSheet("font-size: 20px; font-weight: bold; text-transform: uppercase; margin: 8px 0;");

    layout->addWidget(labeltuile);
    layout->addWidget(labelnbPierres);
    layout->addWidget(labelscore);

    layout->addStretch();
    QPushButton *btnMenu = new QPushButton("OK");
    layout->addWidget(btnMenu);
    connect(btnMenu, &QPushButton::clicked, &dialog, &QDialog::reject);
    int result = dialog.exec();

    if (result == QDialog::Rejected)
    {
        finDeTour();
    }
}
