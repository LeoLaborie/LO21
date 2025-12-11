#include "mainwindow.h"
#include <QCoreApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "PageWidget.h"
#include "PlateauWidget.h"
#include <QStackedWidget>
#include <algorithm>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    //panneau central contenant toutes les pages (menu, plateau…)
    stackWidget = new QStackedWidget(this);
    resize(1920, 1080);
    setCentralWidget(stackWidget);

    menuPage = new QWidget(stackWidget);
    menuPage->setObjectName("MenuPage");
    stackWidget->addWidget(menuPage);

    auto* root = new QVBoxLayout(menuPage);
    root->setContentsMargins(24,140,24,24);
    auto* menu = new QWidget(menuPage);
    menu->setObjectName("MenuPanel");
    menu->setFixedSize(420,390);
    root->addWidget(menu, 0, Qt::AlignCenter);

    auto* col = new QVBoxLayout(menu);
    col->setContentsMargins(24,24,24,24);
    col->setSpacing(25);

    auto* newGame  = new QPushButton("Nouvelle partie", menu);
    auto* loadGame = new QPushButton("Charger une partie", menu);
    auto* setting  = new QPushButton("Paramètres", menu);
    auto* quitter  = new QPushButton("Quitter le jeu", menu);
    for (auto* b : {newGame, loadGame, setting, quitter}) {
        b->setMinimumSize(260,44);
        b->setCursor(Qt::PointingHandCursor);
        col->addWidget(b);
    }
    col->addStretch();


    //les différentes pages sont instanciées une seule fois et stockées dans le stack
    auto * newGamePage = new newPartiePage(stackWidget);
    newGamePage->setObjectName("NewGamePage");
    auto* loadPage    = new chargerPartiePage(stackWidget);
    loadPage->setObjectName("LoadPage");
    settingsPage= new QWidget(stackWidget);
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
    )").arg(background);
    stackWidget->setStyleSheet(stylesheet);

    //navigation principale entre les différentes pages
    connect(newGame,  &QPushButton::clicked, this, [this,newGamePage]{ stackWidget->setCurrentWidget(newGamePage); });
    connect(loadGame, &QPushButton::clicked, this, [this,loadPage]{    stackWidget->setCurrentWidget(loadPage); });
    connect(setting,  &QPushButton::clicked, this, [this]{ stackWidget->setCurrentWidget(settingsPage); });
    connect(quitter,  &QPushButton::clicked, this,  &QWidget::close);
    connect(newGamePage, &newPartiePage::envoieArgument,this, [this](int nb, const QStringList& pseudos, const QVector<bool>& variantes){
            // -> Le contrôleur doit lancer ici la création de la Partie (nb, pseudos, variantes)
            creerLePlateau(nb);




            if (plateauWidget)
                stackWidget->setCurrentWidget(plateauWidget);
    });
    connect(loadPage, &chargerPartiePage::envoieArgument,this, [this](std::string nomSauvegarde){
        // -> Le contrôleur doit ici charger la Partie depuis la sauvegarde indiquée
        creerLePlateau(1);




            if (plateauWidget)
                stackWidget->setCurrentWidget(plateauWidget);
    });

    connect(newGamePage, &newPartiePage::retourMenu, this, [this]{
        stackWidget->setCurrentWidget(menuPage);
    });
    connect(loadPage, &chargerPartiePage::retourMenu, this, [this]{
        stackWidget->setCurrentWidget(menuPage);
    });

    //rafraîchit automatiquement les sauvegardes lorsqu'on entre dans la page de chargement
    connect(stackWidget, &QStackedWidget::currentChanged, this, [this, loadPage](int index){
        if (stackWidget->widget(index) == loadPage)
            loadPage->rafraichirSauvegardes();
    });
}

void MainWindow::creerLePlateau(int nbJoueurs)
{
    if (!stackWidget)
        return;

    //si il y déjà un plateau on le détruit
    if (plateauWidget) {
        stackWidget->removeWidget(plateauWidget);
        plateauWidget->deleteLater();
        plateauWidget = nullptr;
    }
    //on créer le nb plateau pour chaque joueuers et on connect les signals aux slots
    plateauWidget = new PlateauWidget(stackWidget, std::max(1, nbJoueurs));
    stackWidget->addWidget(plateauWidget);
    stackWidget->setMinimumSize(plateauWidget->size());
    resize(plateauWidget->size());

    // -> Connecter ici PlateauWidget aux signaux du contrôleur (pioche, placements, tours, scores).


    connect(plateauWidget, &PlateauWidget::demandeParametres, this, [this]{
        if (stackWidget && settingsPage)
            stackWidget->setCurrentWidget(settingsPage);
    });
    connect(plateauWidget, &PlateauWidget::demandeRetourMenu, this, [this]{
        if (stackWidget && menuPage)
            stackWidget->setCurrentWidget(menuPage);
    });
    connect(plateauWidget, &PlateauWidget::demandeQuitter, this, [this]{
        close();
    });
}
