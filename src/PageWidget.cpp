#include "PageWidget.h"
#include <iostream>
#include <QCheckBox>
#include <QFrame>
#include <QLineEdit>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include <QFrame>
#include <QGridLayout>
#include <QCompleter>
#include <QComboBox>
#include <QSignalBlocker>
#include <string>
#include "Sauvegarde.h"
void newPartiePage::lancerLaPartie()
{
    int nb = NbJoueurs->value();
    QStringList pseudos;
    for (int i = 0; i < nb; ++i)
        pseudos << PseudoJoueurs[i]->text();
    QVector<bool> variantes;
    variantes.reserve(variantesOptions.size());
    for (auto* checkbox : variantesOptions)
        variantes.push_back(checkbox->isChecked());
    emit envoieArgument(nb, pseudos, variantes);
}



newPartiePage::newPartiePage(QWidget* parent) : QWidget(parent)
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
    //bouton retour 

    auto * boutonRetour = new QPushButton("Retour");
    boutonRetour->setStyleSheet("font-size:20px;");
    layoutJoueur->addWidget(boutonRetour);
    connect(boutonRetour, &QPushButton::clicked, this, [this]{
        emit retourMenu();
    });


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





//charger Une partie page Qt


//gestion du slot

void chargerPartiePage::chargerLaPartie(){
   emit envoieArgument(this->NomSauvegarde->currentText().toStdString());
   std::cout<<this->NomSauvegarde->currentText().toStdString()<<std::endl;
}

chargerPartiePage::chargerPartiePage(QWidget* parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);

    auto* LayoutCentral = new QVBoxLayout(this);
    LayoutCentral->setContentsMargins(0, 0, 0, 0);
    LayoutCentral->setAlignment(Qt::AlignCenter);

    // cadre pour le widget
    auto* Cadre = new QFrame();
    Cadre->setObjectName("loadGameCadre");
    Cadre->setFixedSize(300, 350);
    Cadre->setStyleSheet(
        "#loadGameCadre {background-color: #fafafa; border: 1px solid #dcdcdc; border-radius: 18px;}"
    );
    LayoutCentral->addWidget(Cadre, 0, Qt::AlignCenter);

    auto * layoutLoad = new QVBoxLayout(Cadre);

    //bouton retour 
    layoutLoad->addSpacing(20);
    auto * boutonRetour = new QPushButton("Retour");
    boutonRetour->setStyleSheet("font-size:20px;");
    layoutLoad->addWidget(boutonRetour);
    connect(boutonRetour, &QPushButton::clicked, this, [this]{emit retourMenu();});

    layoutLoad->setAlignment(Qt::AlignTop); 
    layoutLoad->addSpacing(20);


    // titre
    auto * titre = new QLabel("<u>Charger une partie</u>");
    titre->setStyleSheet("font-size: 25px; font-weight: 600;");
    titre->setAlignment(Qt::AlignCenter); 
    layoutLoad->addWidget(titre, 0, Qt::AlignCenter);
    layoutLoad->addSpacing(45);

    //ajout un QLabel pour gérer les erreurs
    erreur = new QLabel();
    erreur->setStyleSheet("font-size: 15px; font-weight: 600;color : red");
    erreur->setAlignment(Qt::AlignCenter);  
    layoutLoad->addWidget(erreur, 0, Qt::AlignCenter);

    // liste des saugegarde
    NomSauvegarde = new QComboBox();
    NomSauvegarde->setFixedHeight(34);
    NomSauvegarde->setFixedWidth(260);
    NomSauvegarde->setEditable(true);
    NomSauvegarde->setStyleSheet("QLineEdit { font-size: 14px; padding: 4px 8px; }");
    layoutLoad->addWidget(NomSauvegarde, 0, Qt::AlignHCenter);

    // Complétion
    QCompleter* completer = new QCompleter(NomSauvegarde->model(), NomSauvegarde);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    NomSauvegarde->setCompleter(completer);

    rafraichirSauvegardes();
    
    layoutLoad->addSpacing(40);  


    // bouton confirmer
    auto StartButton = new QPushButton("Confirmer");
    StartButton->setFixedHeight(44);
    StartButton->setStyleSheet(
        "QPushButton { font-size: 15px; font-weight: 600; color: white; background-color: #0078d4; border-radius: 8px; padding: 6px 14px; }"
        "QPushButton:hover { background-color: #0a84ff; }"
        "QPushButton:pressed { background-color: #0062a3; }"
    );
    connect(StartButton, &QPushButton::clicked,this, &chargerPartiePage::chargerLaPartie);
    layoutLoad->addWidget(StartButton, 0, Qt::AlignHCenter);
    layoutLoad->addSpacing(15);  
}

QString recupererNomSansExtension(const std::string& nomAvecExtension) {
    QString nom;
    for (char c : nomAvecExtension) {
        if (c == '.')
            break;
        nom.append(QChar::fromLatin1(c));
    }
    return nom;
}


void chargerPartiePage::rafraichirSauvegardes()
{
    NomSauvegarde->clear();
    if (getSauvegardes().size()==0){
        this->erreur->setText("Pas de sauvegardes disponible");
    }
    for (auto& sauvegarde : getSauvegardes())
        NomSauvegarde->addItem(recupererNomSansExtension(sauvegarde));
}
