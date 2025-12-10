#include "PlateauWidget.h"
#include "PiocheWidget.h"
#include "ScorePanel.h"
#include "ZoneJeuWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>


PlateauWidget::PlateauWidget(QWidget* parent)
    : QWidget(parent)
{
    //définit la taille de la fenetre de jeu
    setFixedSize(1920, 1080);

    //création de l'organisation de la page avec un layout central
    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    //on définit la taille qu'on va utiliser (peut etre modifié pour récuperer la taille de l'affichage sur l'ordi si on le temps )
    const int colonneDroiteLargeur = 250;
    const int scoreWidgetSize = 250;
    const int plateauWidth = width() - colonneDroiteLargeur;
    const int plateauHeight = height();
    const int piocheHeight = height() - scoreWidgetSize;


    //appelle le constructeur de la zone de jeu
    zoneJeuWidget = new ZoneJeuWidget(plateauWidth, plateauHeight, this);
    layout->addWidget(zoneJeuWidget, 1);

    //gère la partie droite (widget score au dessus et en dessous la scene pour la pioche)
    auto* panneauDroit = new QWidget(this);
    panneauDroit->setFixedWidth(colonneDroiteLargeur);
    auto* colonneDroite = new QVBoxLayout(panneauDroit);
    colonneDroite->setContentsMargins(0, 0, 0, 0);
    colonneDroite->setSpacing(0);
    layout->addWidget(panneauDroit, 0);

    //création de la widget score
    scorePanel = new ScorePanel(colonneDroiteLargeur, scoreWidgetSize, panneauDroit);
    colonneDroite->addWidget(scorePanel, 0, Qt::AlignTop);

    //création de la scène pioche
    piocheWidget = new PiocheWidget(colonneDroiteLargeur, piocheHeight, panneauDroit);
    colonneDroite->addWidget(piocheWidget, 1);
}
