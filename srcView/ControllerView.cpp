#include <vector>
#include <QVector>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QString>
#include "../IncludeView/PlateauWidget.h"
#include "../include/Partie.h"
#include "../IncludeView/ControllerView.h"
#include "../IncludeView/mainwindow.h"

ControllerView* ControllerView::instance = nullptr;

void pause(int temps) //je n'ai aucune idée de comment va réagir cette fonction, j'ai demandé à Gemini
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(temps);
    loop.exec();
}


ControllerView* ControllerView::giveInstance(PlateauWidget& plateau){
    if (instance == nullptr){
        instance = new ControllerView(plateau);
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

    Partie partie = Partie(nb, p, variantesPoints, utiliserToutesLesTuiles);
    boucleDeJeu(partie);
}


void ControllerView::chargerDepuisSauvegarde(std::string nomSauvegarde){
    Partie partie = Partie::FromSave("saves/" + nomSauvegarde);
    boucleDeJeu(partie);
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

void ControllerView::boucleDeJeu(Partie& partie){

    Joueur& joueurcourant = partie.getJoueurMain();
    //plateau->afficher(joueurcourant)
    //plateau->setMaitreArchitecte(0)
    //plateau->changerPioche(partie.getChantier().getTuiles());
    //Mettre un score de 0 pour tous les joueurs dans le plateau

    while (partie.pilesRestantes() || partie.getChantier().getTaille() > 1)
    {
        QString message = QString("Nouveau Tour: %1 \n Il reste %2 piles de tuiles").
                          arg(partie.getNbrTours() + 1).
                          arg(partie.getNbrPiles());

        afficherMessageTemporaire(message);


        while (partie.getChantier().getTaille() > 1)
        {
            traiterTourHumain(partie);
            if (partie.fauxJoueurPresent())
                traiterTourIA(partie);
            partie.setProchainJoueur();
            joueurcourant = partie.getJoueurMain();
            //plateau->setJoueurActif(joueurcourant)
            system("clear");
            //plateau->afficher(joueurcourant)
        }

        message = QString("Fin de tour, il n'y a plus de tuiles dans la pioche, renouvellement de la pioche");

        afficherMessageTemporaire(message);

        partie.tourTermine();
        int maitreArchitecte = partie.getMaitreArchitecte();
        //plateau->setMaitreArchitecte(int)
        if (partie.pilesRestantes())
            partie.addTuileInChantierFromPiles();
            //plateau->changerPioche(partie.getChantier().getTuiles());

        system("sleep 4");
        system("clear");
        afficher_curseur();
    }

    //MainWindow::afficher(scores) un truc comme ça

}


void ControllerView::traiterTourHumain(Partie &partie){
    Joueur &joueur = partie.getJoueurMain();
    std::cout << "Tour de :\n" << joueur;
    std::cout << partie.getChantier();

    Tuile tuilePiochee;
    const bool tuileDejaEnMain = !joueur.getTuileEnMain().getHexagones().empty();
    if (!tuileDejaEnMain)
    {
        int idTuile = joueur.choixTuile(partie.getChantier());
        bool piocheOK = false;
        while (!piocheOK)
        {
            try
            {
                tuilePiochee = joueur.piocherTuile(idTuile, partie.getChantier(), partie.getFauxJoueur());
                std::cout << "\nTuile piochée :\n\n" << tuilePiochee;
                piocheOK = true;
            }
            catch (const std::exception &e)
            {
                afficherMessageErreurSaisie(e.what());
            }
        }
    }
    else
    {
        tuilePiochee = joueur.getTuileEnMain();
        std::cout << "\nTuile déjà en main.\n";
    }

    if (!phaseManipulationTuile(joueur, partie, tuilePiochee))
        return;

    phasePlacementTuile(joueur);
}


