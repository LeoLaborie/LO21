#include "mainwindow.h"
#include <QCoreApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "PageWidget.h"
#include "PlateauWidget.h"
#include <QStackedWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    auto* stack = new QStackedWidget(this);
    setCentralWidget(stack);

    QWidget* menuPage = new QWidget(stack);
    menuPage->setObjectName("MenuPage");
    stack->addWidget(menuPage);

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


    auto * newGamePage = new newPartiePage(stack);
    newGamePage->setObjectName("NewGamePage");
    QWidget* loadPage    = new QWidget(stack);
    loadPage->setObjectName("LoadPage");
    QWidget* settingsPage= new QWidget(stack);
    settingsPage->setObjectName("SettingsPage");
    auto* plateau = new PlateauWidget(stack);

    stack->addWidget(newGamePage);
    stack->addWidget(loadPage);
    stack->addWidget(settingsPage);
    stack->addWidget(plateau);

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
    stack->setStyleSheet(stylesheet);

    connect(newGame,  &QPushButton::clicked, stack, [stack,newGamePage]{ stack->setCurrentWidget(newGamePage); });
    connect(loadGame, &QPushButton::clicked, stack, [stack,loadPage]{    stack->setCurrentWidget(loadPage); });
    connect(setting,  &QPushButton::clicked, stack, [stack,settingsPage]{stack->setCurrentWidget(settingsPage); });
    connect(quitter,  &QPushButton::clicked, this,  &QWidget::close);
    connect(newGamePage, &newPartiePage::afficherPlateau,this, [stack, plateau](int nb, const QStringList& pseudos, const QVector<bool>& variantes){
            //plateau->initialiser(nb, pseudos, variantes); à faire 
            stack->setCurrentWidget(plateau);          
    });
}
