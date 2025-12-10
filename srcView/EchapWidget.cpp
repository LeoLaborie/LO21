#include <QPushButton>
#include <QCursor>
#include "EchapWidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

EchapWidget::EchapWidget(QWidget * parent)
    : QWidget(parent)
{
    setStyleSheet("QWidget { color: #0f172a; }"
                  "QLineEdit, QSpinBox, QComboBox {"
                  "color: #0f172a;"
                  "background: #ffffff;"
                  "border: 1px solid #d0d5dd;"
                  "border-radius: 6px;"
                  "selection-background-color: #0a84ff;"
                  "selection-color: #ffffff;"
                  "}");

    setAttribute(Qt::WA_StyledBackground, true);
    auto *layoutCentral = new QVBoxLayout(this);
    layoutCentral->setContentsMargins(0, 0, 0, 0);
    layoutCentral->setAlignment(Qt::AlignCenter);

    auto *cadre = new QFrame();
    cadre->setObjectName("newGameCadre");
    cadre->setMaximumWidth(420);
    cadre->setStyleSheet(
        "#newGameCadre {background-color: #fafafa; border: 1px solid #dcdcdc; border-radius: 18px;}"
    );
    layoutCentral->addWidget(cadre, 0, Qt::AlignCenter);

    auto *layoutMenu = new QVBoxLayout(cadre);
    layoutMenu->setContentsMargins(32, 32, 32, 32);
    layoutMenu->setSpacing(16);

    auto *titre = new QLabel("Pause");
    titre->setAlignment(Qt::AlignCenter);
    titre->setStyleSheet("font-size: 32px; font-weight: 700;");
    layoutMenu->addWidget(titre);

    auto creerBouton = [] (const QString& texte) {
        auto *btn = new QPushButton(texte);
        btn->setFixedHeight(48);
        btn->setMinimumWidth(260);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(
            "QPushButton { font-size: 16px; font-weight: 600; color: white; background-color: #0078d4;"
            " border-radius: 10px; padding: 6px 14px; }"
            "QPushButton:hover { background-color: #0a84ff; }"
            "QPushButton:pressed { background-color: #0062a3; }"
        );
        return btn;
    };

    auto *boutonReprendre = creerBouton("Reprendre");
    auto *boutonParametres = creerBouton("Paramètres");
    auto *boutonMenu = creerBouton("Menu");
    auto *boutonSauvegarder = creerBouton("Sauvegarder");
    boutonSauvegarder->setContentsMargins(0, 0, 0, 8);
    auto *boutonQuitter = new QPushButton("Quitter");
    boutonQuitter->setObjectName("btnQuitterEchap");
    boutonQuitter->setFixedHeight(48);
    boutonQuitter->setMinimumWidth(260);
    boutonQuitter->setCursor(Qt::PointingHandCursor);
    boutonQuitter->setStyleSheet(
        "QPushButton#btnQuitterEchap { font-size: 16px; font-weight: 600; color: white; background-color: #dc2626;"
        " border-radius: 10px; padding: 6px 14px; }"
        "QPushButton#btnQuitterEchap:hover { background-color: #b91c1c; }"
        "QPushButton#btnQuitterEchap:pressed { background-color: #991b1b; }"
    );

    layoutMenu->addWidget(boutonReprendre);
    layoutMenu->addWidget(boutonParametres);
    layoutMenu->addWidget(boutonMenu);
    layoutMenu->addWidget(boutonSauvegarder);
    layoutMenu->addSpacing(20);
    layoutMenu->addWidget(boutonQuitter);

    connect(boutonReprendre, &QPushButton::clicked, this, &EchapWidget::demandeFermeture);
    connect(boutonParametres, &QPushButton::clicked, this, &EchapWidget::demandeParametres);
    connect(boutonMenu, &QPushButton::clicked, this, &EchapWidget::demandeRetourMenu);
    connect(boutonSauvegarder, &QPushButton::clicked, this, &EchapWidget::demandeSauvegarde);
    connect(boutonQuitter, &QPushButton::clicked, this, &EchapWidget::demandeQuitter);
}
