#include <vector>
#include <QVector>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QString>
#include <QEventLoop>
#include "../IncludeView/PlateauWidget.h"
#include "../include/Partie.h"
#include "../IncludeView/ControllerView.h"
#include "../IncludeView/mainwindow.h"
#include "Tuile.h"

ControllerView* ControllerView::instance = nullptr;

void pause(int temps) //je n'ai aucune idée de comment va réagir cette fonction, j'ai demandé à Gemini
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(temps);
    loop.exec();
}


ControllerView* ControllerView::giveInstance(){
    if (instance == nullptr){
        Partie partie;
        instance = new ControllerView(partie);
    }
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


void ControllerView::chargerDepuisSauvegarde(std::string nomSauvegarde){
    partie = Partie::FromSave("saves/" + nomSauvegarde);
    initPlateau();
}

void ControllerView::afficherMessageTemporaire(const QString& message)
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

void ControllerView::initPlateau(){
    Joueur& joueurcourant = partie.getJoueurMain();
    emit setMainJoueurPlateau(partie.getMainJoueur());
    //plateau->setMaitreArchitecte(0)
    //plateau->changerPioche(partie.getChantier().getTuiles());
    //Mettre un score de 0 pour tous les joueurs dans le plateau
    Toursuivant();
}


void ControllerView::Toursuivant(){

    Joueur &joueur = partie.getJoueurMain();

    QString message = QString("C'est au tour de %1").arg(QString::fromStdString(joueur.getNom()));
    afficherMessageTemporaire(message);

    emit setMainJoueurPlateau(partie.getMainJoueur());

    if (partie.pilesRestantes() || partie.getChantier().getTaille() > 1){

        message = QString("Nouveau Tour: %1 \n Il reste %2 piles de tuiles").
                          arg(partie.getNbrTours() + 1).
                          arg(partie.getNbrPiles());

        afficherMessageTemporaire(message);
        if (partie.getChantier().getTaille() < 1){

            message = QString("Il n'y a plus de tuiles dans la pioche, renouvellement de la pioche");
            afficherMessageTemporaire(message);

            partie.tourTermine();
            int maitreArchitecte = partie.getMaitreArchitecte();
            //plateau->setMaitreArchitecte(int)
            partie.addTuileInChantierFromPiles();
            emit setChantier(partie.getChantier().getTuiles());

        }
        partie.setProchainJoueur();


    }else{

        //MainWindow::afficher(scores) un truc comme ça
    }
}



void ControllerView::joueurPiocheTuile(int& idTuile){

    Joueur &joueurcourant = partie.getJoueurMain();
    if (idTuile < joueurcourant.getNbrPierres()){
    Tuile tuilePiochee = joueurcourant.piocherTuile(idTuile ,partie.getChantier() ,partie.getFauxJoueur());

    }
    //plateau->updatePierres(joueur);
    if (partie.fauxJoueurPresent()){
        //plateau->updatePierres(partie.getFauxJoueur());
    }
}

void ControllerView::joueurPlaceTuiel(Position& p){
    Joueur& joueur = partie.getJoueurMain();
    Tuile tuile = joueur.getTuileEnMain();
    joueur.placerTuile(tuile, p);
}
