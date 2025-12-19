#include <vector>
#include <QString>
#include <QStringList>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QDialog>
#include <QPushButton>
#include <QApplication>
#include <algorithm>
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
    emit setMainJoueurPlateau(partie.getMainJoueur());
    emit joueurActifChange(QString::fromStdString(partie.getJoueurMain().getNom()));
    mettreAJourScoreCourant();
    lancerTour();
}


void ControllerView::lancerTour(){

    Joueur &joueur = partie.getJoueurMain();

    QString message = QString("C'est au tour de %1").arg(QString::fromStdString(joueur.getNom()));
    afficherMessage(message);
    //modifier le label dans le panel score



    if (partie.pilesRestantes() || partie.getChantier().getTaille() > 1){


        if (partie.getChantier().getTaille() < 1){
            message = QString("Il n'y a plus de tuiles dans la pioche, renouvellement de la pioche");
            afficherMessage(message);

            partie.tourTermine();
            int maitreArchitecte = partie.getMaitreArchitecte();
            //plateau->setMaitreArchitecte(int)
            partie.addTuileInChantierFromPiles();
            emit setChantier(partie.getChantier().getTuiles());

        }

        if (joueur.isIA()){
            IllustreArchitecte &ia = dynamic_cast<IllustreArchitecte &>(joueur);
            int idTuile = ia.choixTuile(partie.getChantier());
            Tuile& tuile = ia.piocherTuile(idTuile, partie.getChantier(), nullptr);
            ia.placerTuile(tuile);
            emit fauxJoueurPiocheTuile(idTuile);
            afficherInfoIA(idTuile);
        }else{
            message = QString("Nouveau Tour: %1 \n Il reste %2 piles de tuiles").arg(partie.getNbrTours() + 1).arg(partie.getNbrPiles());
            afficherMessage(message);
            emit setNbPierres(joueur.getNbrPierres());
            emit setMainJoueurPlateau(partie.getMainJoueur());
            emit joueurActifChange(QString::fromStdString(joueur.getNom()));
            mettreAJourScoreCourant();
            partie.setProchainJoueur();
        }



    }else{

        afficheScore();
    }
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
    const std::vector<int> tabscore = joueur.getPlateau().calculerPointsTab();
    const int total =joueur.getNbrPoints();
    emit setScore(total, tabscore[0], tabscore[1], tabscore[2], tabscore[3], tabscore[4]);
}

void ControllerView::joueurPiocheTuile(int idTuile){

    Joueur &joueurcourant = partie.getJoueurMain();

    if (idTuile < joueurcourant.getNbrPierres()){
        joueurcourant.piocherTuile(idTuile ,partie.getChantier() ,partie.getFauxJoueur());
        emit valideTuilePiochee(idTuile);
        emit setNbPierres(joueurcourant.getNbrPierres());
    }else{
        const QString message = QString("Vous n'avez pas assez de pierres pour piocher cette tuile");
        afficherMessage(message);
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
        afficherMessage(message);
    }
}

void ControllerView::afficherMessage(const QString& message)
{
    QWidget *popup = new QWidget();
    popup->setWindowTitle("Info");

    popup->setAttribute(Qt::WA_DeleteOnClose);

    popup->setWindowFlags(Qt::WindowStaysOnTopHint);

    QVBoxLayout *layout = new QVBoxLayout(popup);
    QLabel *label = new QLabel(message, popup);
    layout->addWidget(label);

    popup->show();

    QTimer::singleShot(3000, popup, &QWidget::close);
}

void ControllerView::afficherInfoIA(int& idTuile){
    QDialog dialog;
    dialog.setWindowTitle("Tour Illustre Architecte");
    dialog.resize(400, 500);
    dialog.setModal(true);

    dialog.setStyleSheet(
        "QDialog { background-color: #2c3e50; color: white; }"
        "QLabel { font-size: 18px; margin: 5px; }"
        "QPushButton { "
        "    background-color: #e67e22; color: white; "
        "    padding: 12px; border-radius: 6px; font-weight: bold; font-size: 16px;"
        "}"
        "QPushButton:hover { background-color: #d35400; }"
        );

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *title = new QLabel("Infos sur le tour de l'Illustre Architecte");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 26px; font-weight: bold; color: #f1c40f; margin-bottom: 20px;");
    layout->addWidget(title);

    QString tuile = QString("Il a pioché la tuile n°%1").arg(idTuile);
    QLabel *labeltuile = new QLabel(tuile);
    labeltuile->setAlignment(Qt::AlignCenter);

    QString nbPierres = QString("Il a %1 pierres").arg(partie.getFauxJoueur()->getNbrPierres());
    QLabel *labelnbPierres = new QLabel(nbPierres);
    labelnbPierres->setAlignment(Qt::AlignCenter);

    QString score = QString("Il a %1 points").arg(partie.getFauxJoueur()->getNbrPoints());
    QLabel *labelscore = new QLabel(score);
    labelscore->setAlignment(Qt::AlignCenter);

    layout->addWidget(labeltuile);
    layout->addWidget(labelnbPierres);
    layout->addWidget(labelscore);

    layout->addStretch();

    QPushButton *btnMenu = new QPushButton("OK");
    layout->addWidget(btnMenu);

    connect(btnMenu, &QPushButton::clicked, &dialog, &QDialog::reject);
    int result = dialog.exec();

    if (result == QDialog::Rejected) {
        partie.setProchainJoueur();
        lancerTour();
    }

}


void ControllerView::afficheScore(){
    QList<QPair<int, QString>> listeScores;

    const auto& joueurs = partie.getJoueurs();
    for(int i = 0; i < partie.getNbrJoueurs(); i++){
        Joueur* j = joueurs[i];
        // On stocke le score en premier pour faciliter le tri
        listeScores.append(qMakePair(j->getNbrPoints(), QString::fromStdString(j->getNom())));
    }

    std::sort(listeScores.begin(), listeScores.end(), std::greater<QPair<int, QString>>());

    QDialog dialog;
    dialog.setWindowTitle("Fin de partie - Akropolis");
    dialog.resize(400, 500); // Un peu plus grand
    dialog.setModal(true);

    dialog.setStyleSheet(
        "QDialog { background-color: #2c3e50; color: white; }"
        "QLabel { font-size: 18px; margin: 5px; }"
        "QPushButton { "
        "    background-color: #e67e22; color: white; "
        "    padding: 12px; border-radius: 6px; font-weight: bold; font-size: 16px;"
        "}"
        "QPushButton:hover { background-color: #d35400; }"
        );

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *title = new QLabel("🏛️ RÉSULTATS 🏛️");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 26px; font-weight: bold; color: #f1c40f; margin-bottom: 20px;");
    layout->addWidget(title);

    for(const auto &pair : listeScores) {
        // pair.first = score, pair.second = nom
        QString texte = QString("%1 : %2 pts").arg(pair.second).arg(pair.first);

        QLabel *lbl = new QLabel(texte);
        lbl->setAlignment(Qt::AlignCenter);

        // Le gagnant est doré
        if (pair == listeScores.first()) {
            lbl->setStyleSheet("color: #f1c40f; font-weight: bold; font-size: 22px; border: 2px solid #f1c40f; border-radius: 5px; padding: 5px;");
            lbl->setText("🏆 " + texte + " 🏆");
        }

        layout->addWidget(lbl);
    }

    layout->addStretch();

    QPushButton *btnMenu = new QPushButton("Retour au Menu Principal");
    layout->addWidget(btnMenu);

    connect(btnMenu, &QPushButton::clicked, &dialog, &QDialog::accept);
    int result = dialog.exec();

    if (result == QDialog::Accepted) {
        emit partieFinie();
    }
    else {
        // Clic sur la croix
        QApplication::quit();
    }
}
