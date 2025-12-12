#include "PlateauWidget.h"

#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QKeySequence>
#include <QPointF>
#include <QRect>
#include <QShortcut>
#include <QVBoxLayout>
#include <algorithm>
#include <QGuiApplication>
#include <QScreen>

#include "ChantierWidget.h"
#include "EchapWidget.h"
#include "ScorePanel.h"
#include "Tuile.h"
#include "ZoneJeuWidget.h"

PlateauWidget::PlateauWidget(QWidget* parent, int nbJoueurs)
    : QWidget(parent)
{
    // définit la taille de la fenetre de jeu en fonction de la taille de l'écran
    resize(QGuiApplication::primaryScreen()->availableGeometry().size());              
    // création de l'organisation de la page avec un layout central
    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // on définit la taille qu'on va utiliser (peut etre modifié pour récuperer la taille de l'affichage sur l'ordi si on le temps )
    const int colonneDroiteLargeur = static_cast<int>(width() / 8);
    const int scoreWidgetSize = static_cast<int>(height() / 4.5);
    const int plateauWidth = width() - colonneDroiteLargeur;
    const int plateauHeight = height();
    const int chantierHeight = height() - scoreWidgetSize;

    // pile contenant une ZoneJeuWidget par joueur pour passer rapidement d'un plateau à l'autre
    stackPlateaux = new QStackedWidget(this);
    const int nbScenes = std::max(1, nbJoueurs);
    zonesParJoueur.reserve(nbScenes);
    for (int i = 0; i < nbScenes; ++i)
    {
        auto* zone = new ZoneJeuWidget(plateauWidth, plateauHeight, this);
        zonesParJoueur.push_back(zone);
        stackPlateaux->addWidget(zone);
    }
    // zone par défaut = premier joueur
    zoneJeuWidget = zonesParJoueur.front();
    stackPlateaux->setCurrentWidget(zoneJeuWidget);
    layout->addWidget(stackPlateaux, 1);

    echapWidget = new EchapWidget();
    echapWidget->attacherAScene(zoneJeuWidget->scene());
    connect(echapWidget, &EchapWidget::visibiliteChangee, this, &PlateauWidget::gererBlocageInteractions);
    connect(echapWidget, &EchapWidget::demandeParametres, this, [this]
            {
        echapWidget->fermerWidget();
        emit demandeParametres(); });
    connect(echapWidget, &EchapWidget::demandeRetourMenu, this, [this]
            {
        echapWidget->fermerWidget();
        emit demandeRetourMenu(); });
    connect(echapWidget, &EchapWidget::demandeQuitter, this, [this]
            {
        echapWidget->fermerWidget();
        emit demandeQuitter(); });

    raccourciEchap = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    connect(raccourciEchap, &QShortcut::activated, this, &PlateauWidget::basculerMenuEchap);
    // gère la partie droite (widget score au dessus et en dessous la scène pour le chantier)
    auto* panneauDroit = new QWidget(this);
    panneauDroit->setFixedWidth(colonneDroiteLargeur);
    auto* colonneDroite = new QVBoxLayout(panneauDroit);
    colonneDroite->setContentsMargins(0, 0, 0, 0);
    colonneDroite->setSpacing(0);
    layout->addWidget(panneauDroit, 0);

    // création de la widget score
    scorePanel = new ScorePanel(colonneDroiteLargeur, scoreWidgetSize, panneauDroit);
    colonneDroite->addWidget(scorePanel, 0, Qt::AlignTop);

    // création de la scène chantier
    chantierWidget = new ChantierWidget(colonneDroiteLargeur, chantierHeight, panneauDroit);
    colonneDroite->addWidget(chantierWidget, 1);

    // gestion des flux entre le chantier et la zone de jeu (pioche / validation / annulation)
    //  Le contrôleur doit alimenter le chantier via ajouterTuilleDansChantier()
    //  et appeler afficherPlateauJoueur(index) lorsqu'il change de joueur actif.
    connect(chantierWidget, &ChantierWidget::tuilePiochee, this, [this](TuileItem* tuile)
            {
        if (zoneJeuWidget)
            zoneJeuWidget->placerTuileDansZoneJeu(tuile); });
    for (auto* zone : zonesParJoueur)
    {
        connect(zone, &ZoneJeuWidget::validationPlacementAnnulee, chantierWidget, &ChantierWidget::remettreTuileDansChantier);
        connect(zone, &ZoneJeuWidget::validationPlacementConfirmee, this, &PlateauWidget::validerPlacementTuile);
    }
}

void PlateauWidget::basculerMenuEchap()
{
    // si pas définit on ne fais rien
    if (!echapWidget || !zoneJeuWidget)
        return;

    // si la widget est déjà actives on la ferme
    if (echapWidget->estActif())
        echapWidget->fermerWidget();
    else
    {
        // calcul la position de la widget
        const QRect vue = zoneJeuWidget->viewport()->rect();
        const QPointF topLeft = zoneJeuWidget->mapToScene(vue.topLeft());
        const QPointF bottomRight = zoneJeuWidget->mapToScene(vue.bottomRight());
        const QRectF visibleRect(topLeft, bottomRight);
        echapWidget->afficherEchap(visibleRect);
    }
}

void PlateauWidget::gererBlocageInteractions(bool widgetActif)
{
    // on désactive tt ou réactive tous en fonction du bool passé en paramètre
    if (zoneJeuWidget)
        zoneJeuWidget->setBlocageInteractions(widgetActif);
    if (chantierWidget)
        chantierWidget->setEnabled(!widgetActif);
    if (scorePanel)
        scorePanel->setEnabled(!widgetActif);
}
void PlateauWidget::validerPlacementTuile(TuileItem* t)
{
    if (!t)
        return;
    // vérification du placement pour le controleur à faire et définir un niveau ensuite
    t->setSelected(false);
    t->setInteractivite(false, false);
    if (chantierWidget)
        chantierWidget->setEnabled(true);
    emit placementTermine();
    if (!zonesParJoueur.empty())
    {
        joueurActif = (joueurActif + 1) % static_cast<int>(zonesParJoueur.size());
        afficherPlateauJoueur(joueurActif);
    }
    std::cout << t->getNiveauGraphique() << std::endl;
}

void PlateauWidget::afficherPlateauJoueur(int index)
{
    if (!stackPlateaux || zonesParJoueur.empty())
        return;
    if (index < 0 || index >= static_cast<int>(zonesParJoueur.size()))
        return;
    joueurActif = index;
    zoneJeuWidget = zonesParJoueur[index];
    stackPlateaux->setCurrentIndex(index);
    if (echapWidget && zoneJeuWidget)
        echapWidget->attacherAScene(zoneJeuWidget->scene());
}
