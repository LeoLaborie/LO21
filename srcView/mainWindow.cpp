#include "mainwindow.h"

#include <QCoreApplication>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QGuiApplication>
#include <QScreen>
#include <QVBoxLayout>
#include <QWidget>
#include <algorithm>
#include <fstream>

#include "PageWidget.h"
#include "PlateauWidget.h"
#include "ControllerView.h"
#include "ChantierWidget.h"
#include "ScorePanel.h"

namespace
{
    int lireNbJoueursSauvegarde(std::string nomSauvegarde)
    {
        constexpr const char* extension = ".ratatata";
        constexpr size_t extensionLen = 9; // strlen(".ratatata")
        if (nomSauvegarde.size() < extensionLen || nomSauvegarde.substr(nomSauvegarde.size() - extensionLen) != extension)
            nomSauvegarde += extension;

        std::ifstream f("saves/" + nomSauvegarde);
        if (!f)
            return 1;

        std::string tag;
        int nb = 1;
        if ((f >> tag >> nb) && tag == "PARTIE" && nb > 0)
            return nb;
        return 1;
    }
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // panneau central contenant toutes les pages (menu, plateau…)
    stackWidget = new QStackedWidget(this);
    resize(QGuiApplication::primaryScreen()->availableGeometry().size()); 
    setCentralWidget(stackWidget);

    menuPage = new QWidget(stackWidget);
    menuPage->setObjectName("MenuPage");
    stackWidget->addWidget(menuPage);

    auto* root = new QVBoxLayout(menuPage);
    root->setContentsMargins(24, 140, 24, 24);
    auto* menu = new QWidget(menuPage);
    menu->setObjectName("MenuPanel");
    menu->setFixedSize(420, 390);
    root->addWidget(menu, 0, Qt::AlignCenter);

    auto* col = new QVBoxLayout(menu);
    col->setContentsMargins(24, 24, 24, 24);
    col->setSpacing(25);

    auto* newGame = new QPushButton("Nouvelle partie", menu);
    auto* loadGame = new QPushButton("Charger une partie", menu);
    auto* setting = new QPushButton("Paramètres", menu);
    auto* quitter = new QPushButton("Quitter le jeu", menu);
    for (auto* b : {newGame, loadGame, setting, quitter})
    {
        b->setMinimumSize(260, 44);
        b->setCursor(Qt::PointingHandCursor);
        col->addWidget(b);
    }
    col->addStretch();

    // les différentes pages sont instanciées une seule fois et stockées dans le stack
    auto* newGamePage = new newPartiePage(stackWidget);
    newGamePage->setObjectName("NewGamePage");
    auto* loadPage = new chargerPartiePage(stackWidget);
    loadPage->setObjectName("LoadPage");
    settingsPage = new QWidget(stackWidget);
    settingsPage->setObjectName("SettingsPage");
    stackWidget->addWidget(newGamePage);
    stackWidget->addWidget(loadPage);
    stackWidget->addWidget(settingsPage);


    const QString background = QCoreApplication::applicationDirPath() + "/img/akropolis.png";
    const QString stylesheet = QString(R"(
        #MenuPage, #NewGamePage, #LoadPage, #SettingsPage {
            border-image: url("%1") 0 0 0 0 stretch stretch;
        }
        #MenuPanel { background: rgba(0,0,0,0.35); border-radius: 16px; }

        QPushButton {
            font-size: 35px; padding: 10px 18px; border-radius: 12px;
            background: #6CB7E0; color: #fff; border: 1px solid #204ea6;
        }
        QPushButton:hover  { background: #3b7bf0; }
        QPushButton:pressed{ background: #2a5ec2; }
    )")
                                   .arg(background);
    stackWidget->setStyleSheet(stylesheet);

    // navigation principale entre les différentes pages
    connect(newGame, &QPushButton::clicked, this, [this, newGamePage]
            { stackWidget->setCurrentWidget(newGamePage); });
    connect(loadGame, &QPushButton::clicked, this, [this, loadPage]
            { stackWidget->setCurrentWidget(loadPage); });
    connect(setting, &QPushButton::clicked, this, [this]
            {
        // TODO: Page paramètres à implémenter plus tard.
        // if (stackWidget && settingsPage)
        //     stackWidget->setCurrentWidget(settingsPage);
        QMessageBox::information(this,
                                 QStringLiteral("Info"),
                                 QStringLiteral("Paramètres : pas le temps de l'implémenter pour l'instant."));
            });
    connect(quitter, &QPushButton::clicked, this, &QWidget::close);
    connect(newGamePage, &newPartiePage::envoieArgument, this, [this](int nb, const QStringList& pseudos, const QVector<bool>& variantes, int difficulteIA)
            {
            creerLePlateau(nb);
            if (ControllerView* controleur = ControllerView::giveInstance())
                controleur->creerNouvellePartie(nb, pseudos, variantes, difficulteIA);
            if (plateauWidget)
                stackWidget->setCurrentWidget(plateauWidget); });


    connect(loadPage, &chargerPartiePage::envoieArgument, this, [this](std::string nomSauvegarde)
            {
        creerLePlateau(lireNbJoueursSauvegarde(nomSauvegarde));
        bool ok = false;
        if (ControllerView* controleur = ControllerView::giveInstance())
            ok = controleur->chargerDepuisSauvegarde(nomSauvegarde);
        if (ok)
        {
            if (plateauWidget)
                stackWidget->setCurrentWidget(plateauWidget);
        }
        else
        {
            if (stackWidget && menuPage)
                stackWidget->setCurrentWidget(menuPage);
        } });

    connect(newGamePage, &newPartiePage::retourMenu, this, [this]
            { stackWidget->setCurrentWidget(menuPage); });
    connect(loadPage, &chargerPartiePage::retourMenu, this, [this]
            { stackWidget->setCurrentWidget(menuPage); });

    // rafraîchit automatiquement les sauvegardes lorsqu'on entre dans la page de chargement
    connect(stackWidget, &QStackedWidget::currentChanged, this, [this, loadPage](int index)
            {
        if (stackWidget->widget(index) == loadPage)
            loadPage->rafraichirSauvegardes(); });

}

void MainWindow::creerLePlateau(int nbJoueurs)
{
    if (!stackWidget)
        return;

    // si il y déjà un plateau on le détruit
    if (plateauWidget)
    {
        stackWidget->removeWidget(plateauWidget);
        plateauWidget->deleteLater();
        plateauWidget = nullptr;
    }
    // on créer le nb plateau pour chaque joueuers et on connect les signals aux slots
    plateauWidget = new PlateauWidget(stackWidget, std::max(1, nbJoueurs));
    stackWidget->addWidget(plateauWidget);
    stackWidget->setMinimumSize(plateauWidget->size());
    resize(plateauWidget->size());

    if (ControllerView* controleur = ControllerView::giveInstance())
    {
        connect(plateauWidget, &PlateauWidget::tourTermine, controleur, &ControllerView::finDeTour);
        connect(plateauWidget, &PlateauWidget::tuileRotationnee, controleur, &ControllerView::rotationTuileGraphique);
        connect(plateauWidget, &PlateauWidget::validationPlacementDemandee, controleur, &ControllerView::verifierPlacementGraphique);
        connect(plateauWidget, &PlateauWidget::piocheAnnulee, controleur, &ControllerView::annulerPiocheTuile);
        connect(plateauWidget, &PlateauWidget::demandeSauvegarde, controleur, &ControllerView::sauvegarderPartieGraphique);
        connect(controleur, &ControllerView::setMainJoueurPlateau, plateauWidget, &PlateauWidget::afficherPlateauJoueur);
        connect(controleur, &ControllerView::chargerPlateauGraphique, plateauWidget, &PlateauWidget::chargerPlateauJoueur);
        connect(controleur, &ControllerView::afficherTuileMain, plateauWidget, &PlateauWidget::afficherTuileEnMain);
        connect(controleur, &ControllerView::afficherMessage, plateauWidget, &PlateauWidget::afficherMessage);
        connect(controleur, &ControllerView::afficherErreur, plateauWidget, &PlateauWidget::afficherErreur);
        connect(controleur, &ControllerView::etageDetermine, plateauWidget, &PlateauWidget::ModifierCouleurEtage);
        if (auto* chantier = plateauWidget->getChantierWidget())
        {
            connect(chantier, &ChantierWidget::tuileSelectionnee, controleur, &ControllerView::joueurPiocheTuile);
            connect(controleur, &ControllerView::validePasTuilePiochee, chantier, &ChantierWidget::annulerPiocheEnCours);
            connect(controleur, &ControllerView::fauxJoueurPiocheTuile, chantier, &ChantierWidget::fauxJoueurRetireTuile);
            connect(controleur, &ControllerView::setChantier, chantier, &ChantierWidget::definirChantier);
            connect(controleur, &ControllerView::setNbPierres, chantier, &ChantierWidget::mettreAJourPierres);
        }

        if (auto* score = plateauWidget->getScorePanel())
        {
            connect(controleur, &ControllerView::setNbPierres, score, &ScorePanel::setNbPierres);
            connect(controleur, &ControllerView::setScore, score, &ScorePanel::setScore);
            connect(controleur, &ControllerView::setNbPiles, score, &ScorePanel::setNbPiles);
            connect(controleur, &ControllerView::joueurActifChange, score, &ScorePanel::setNomJoueurActif);
        }

        connect(controleur, &ControllerView::partieFinie, this, &MainWindow::retourMenu);
    }

    // -> Connecter ici PlateauWidget aux signaux du contrôleur (pioche, placements, tours, scores).

    connect(plateauWidget, &PlateauWidget::demandeParametres, this, [this]
            {
        // TODO: Page paramètres à implémenter plus tard.
        // if (stackWidget && settingsPage)
        //     stackWidget->setCurrentWidget(settingsPage);
        if (plateauWidget)
            plateauWidget->afficherMessage(QStringLiteral("Paramètres : pas le temps de l'implémenter pour l'instant."));
            });
    connect(plateauWidget, &PlateauWidget::demandeRetourMenu, this, [this]
            {
        retourMenu(); });
    connect(plateauWidget, &PlateauWidget::demandeQuitter, this, [this]
            { close(); });
}

void MainWindow::retourMenu()
{
    if (!stackWidget || !menuPage)
        return;
    stackWidget->setCurrentWidget(menuPage);
    if (plateauWidget)
    {
        stackWidget->removeWidget(plateauWidget);
        plateauWidget->deleteLater();
        plateauWidget = nullptr;
    }
    ControllerView::freeInstance();
}
