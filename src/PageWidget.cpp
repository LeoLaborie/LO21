#include "PageWidget.h"
#include <QCheckBox>
#include <QFrame>
#include <QAbstractSpinBox>
#include <QGridLayout>

void newPartiePage::lancerLaPartie()
{
    int nb = NbJoueurs->value();
    std::cout << "Nombre de joueurs : " << nb << "\n";
    for (int i = 0; i < nb; i++) {
        std::cout << "Joueur " << i+1 << " : "
                  << PseudoJoueurs[i]->text().toStdString() << "\n";
    }
    std::cout << "Variantes activées :\n";
    for (const auto* checkbox : variantesOptions) {
        if (checkbox->isChecked()) {
            std::cout << " - " << checkbox->text().toStdString() << "\n";
        }
    }
}

newPartiePage::newPartiePage(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);

    auto* LayoutCentral = new QVBoxLayout(this);
    LayoutCentral->setContentsMargins(0, 0, 0, 0);
    LayoutCentral->setAlignment(Qt::AlignCenter);

    auto* Cadre = new QFrame();
    Cadre->setObjectName("newGameCadre");
    Cadre->setMaximumWidth(420);
    Cadre->setStyleSheet(
        "#newGameCadre {background-color: #fafafa; border: 1px solid #dcdcdc; border-radius: 18px;}"
    );

    auto* layoutJoueur = new QVBoxLayout(Cadre);
    layoutJoueur->setContentsMargins(32, 32, 32, 32);
    layoutJoueur->setSpacing(18);

    auto* titre = new QLabel("Nouvelle partie");
    titre->setAlignment(Qt::AlignCenter);
    titre->setStyleSheet("font-size: 22px; font-weight: 600;");
    layoutJoueur->addWidget(titre);

    auto* labelNbJoueurs = new QLabel("Nombre de joueurs");
    labelNbJoueurs->setStyleSheet("font-size: 14px; font-weight: 600;");
    layoutJoueur->addWidget(labelNbJoueurs);

    NbJoueurs = new QSpinBox();
    NbJoueurs->setRange(1, 4);
    NbJoueurs->setFixedHeight(36);
    NbJoueurs->setFixedWidth(100);
    NbJoueurs->setAlignment(Qt::AlignCenter);
    NbJoueurs->setButtonSymbols(QAbstractSpinBox::PlusMinus);
    NbJoueurs->setStyleSheet("QSpinBox { font-size: 16px; }");
    layoutJoueur->addWidget(NbJoueurs, 0, Qt::AlignCenter);

    auto* pseudoLabel = new QLabel("Pseudos des joueurs");
    pseudoLabel->setStyleSheet("font-size: 14px; font-weight: 600;");
    layoutJoueur->addWidget(pseudoLabel);

    auto* GrillePseudo = new QVBoxLayout();
    GrillePseudo->setSpacing(8);
    GrillePseudo->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    layoutJoueur->addLayout(GrillePseudo);

    auto updateNbPseudo = [this,GrillePseudo](int nbJoueur)
    {
        while (PseudoJoueurs.size() > nbJoueur) {
            auto* suppression = PseudoJoueurs.takeLast();
            GrillePseudo->removeWidget(suppression);
            suppression->deleteLater();
        }

        while (PseudoJoueurs.size() < nbJoueur) {
            auto* ChampPseudo = new QLineEdit();
            ChampPseudo->setText("Joueur " + QString::number(PseudoJoueurs.size() + 1));
            ChampPseudo->setFixedHeight(34);
            ChampPseudo->setFixedWidth(260);
            ChampPseudo->setStyleSheet("QLineEdit { font-size: 14px; padding: 4px 8px; }");
            GrillePseudo->addWidget(ChampPseudo, 0, Qt::AlignCenter);
            PseudoJoueurs.append(ChampPseudo);
        }
    };

    QObject::connect(NbJoueurs,QOverload<int>::of(&QSpinBox::valueChanged),updateNbPseudo);
    updateNbPseudo(NbJoueurs->value());

    auto * group = new QGroupBox("Variantes de la partie");
    group->setStyleSheet("QGroupBox { font-size: 14px; font-weight: 600; border: 1px solid #dcdcdc; border-radius: 12px; margin-top: 6px;}"
                         "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 8px; }"
                         "QCheckBox { font-size: 13px; }"
                         "QCheckBox::indicator { width: 20px; height: 20px; }");
    auto * LayoutVarianteScore = new QVBoxLayout(group);
    LayoutVarianteScore->setContentsMargins(20, 28, 20, 12);
    LayoutVarianteScore->setSpacing(10);

    QStringList variantes = {"Jouer avec toutes les tuiles", "Variante score habitation", "Variante score marché",
         "Variante score caserne", "Variante score temple", "Variante score jardin" };
    for (const QString& v : variantes) {
        auto * checkbox = new QCheckBox(v);
        LayoutVarianteScore->addWidget(checkbox);
        variantesOptions.append(checkbox);
    }
    LayoutVarianteScore->addStretch();
    layoutJoueur->addWidget(group);

    auto StartButton = new QPushButton("Lancer la partie");
    StartButton->setFixedHeight(44);
    StartButton->setStyleSheet("QPushButton { font-size: 15px; font-weight: 600; color: white; background-color: #0078d4; border-radius: 8px; padding: 6px 14px; }"
                               "QPushButton:hover { background-color: #0a84ff; }"
                               "QPushButton:pressed { background-color: #0062a3; }");
    layoutJoueur->addWidget(StartButton);
    LayoutCentral->addWidget(Cadre, 0, Qt::AlignCenter);
    connect(StartButton, &QPushButton::clicked,this, &newPartiePage::lancerLaPartie);
}
