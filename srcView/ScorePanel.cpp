#include "ScorePanel.h"

#include <QCoreApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QStringList>
#include <QWidget>
#include <QSizePolicy>
#include <algorithm>

ScorePanel::ScorePanel(int width, int height, QWidget *parent)
    : QWidget(parent)
{
    // défintion des styles
    setAttribute(Qt::WA_StyledBackground, true);
    setFixedWidth(width);
    setMinimumHeight(height);
    setObjectName("scorePanelWidget");
    setStyleSheet(
        "#scorePanelWidget {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "  stop:0 #fefefe, stop:0.5 #f4f4f4, stop:1 #e4e4e4);"
        "  border: 2px solid black;"
        "  border-bottom: none;"
        "  padding: 4px;"
        "}"
        "#scoreBloc {"
        "  border: 2px solid #1c1c1c;"
        "  border-top-color: #ffffff;"
        "  border-left-color: #f8f8f8;"
        "  border-bottom-color: #8a8a8a;"
        "  border-right-color: #8a8a8a;"
        "  border-radius: 15px;"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "      stop:0 #ffffff, stop:0.4 #f3f3f3, stop:1 #d5d5d5);"
        "}");
    const QString basePath = QCoreApplication::applicationDirPath() + "/img/";

    auto *scoreLayout = new QVBoxLayout(this);
    scoreLayout->setContentsMargins(13, 8, 8, 8);
    scoreLayout->setSpacing(4);

    // Ligne joueur courant
    auto *joueurLayout = new QHBoxLayout();
    joueurLayout->setSpacing(4);
    auto *joueurLabel = new QLabel(tr("Joueur courant :"), this);

    joueurLabel->setStyleSheet("font-weight: 600; color: #111; text-decoration: underline;");
    joueurLayout->addWidget(joueurLabel);
    labelJoueurActif = new QLabel(tr("Joueur 1"), this);
    joueurLayout->addWidget(labelJoueurActif, 1);
    scoreLayout->addLayout(joueurLayout);

    // Ligne pierres
    auto *pierreLayout = new QHBoxLayout();
    pierreLayout->setSpacing(4);
    auto *pierreLibelle = new QLabel("Nombre de pierre :", this);
    pierreLibelle->setStyleSheet("color: #444;");
    pierreLayout->addWidget(pierreLibelle, 1);

    // gestion label du nombre de score
    labelNombrePierre = new QLabel("0", this);
    labelNombrePierre->setStyleSheet("font-size: 16px; font-weight: bold;");
    pierreLayout->addWidget(labelNombrePierre);

    // ajout de l'image
    auto *pierreImage = new QLabel(this);
    pierreImage->setFixedSize(28, 28);
    QPixmap pierrePix(basePath + "Pierre.png");
    if (!pierrePix.isNull())
    {
        pierreImage->setPixmap(pierrePix.scaled(pierreImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    pierreLayout->addWidget(pierreImage);
    scoreLayout->addLayout(pierreLayout);

    // creaton du bloc scores
    auto *blocScoresWidget = new QWidget(this);
    blocScoresWidget->setObjectName("scoreBloc");
    auto *blocScoresLayout = new QVBoxLayout(blocScoresWidget);
    blocScoresLayout->setContentsMargins(6, 6, 6, 6);
    blocScoresLayout->setSpacing(4);
    scoreLayout->addWidget(blocScoresWidget, 1);

    const QStringList labels = {"Score total :", "Score habitation :", "Score marché :",
                                "Score caserne :", "Score temple :", "Score jardin :"};
    const QStringList images = {"", "Habitation.png", "Marche.png", "Carserne.png", "Temple.png", "Jardin.png"};

    // ajout de chaque label du score et des images
    for (int i = 0; i < labels.size(); ++i)
    {
        auto *ligne = new QHBoxLayout();
        ligne->setSpacing(4);
        // définition de chaque label
        auto *libelle = new QLabel(labels[i], blocScoresWidget);
        libelle->setStyleSheet("color: #444;");
        ligne->addWidget(libelle, 1);

        auto *valeurLabel = new QLabel("0", blocScoresWidget);
        valeurLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
        ligne->addWidget(valeurLabel);

        // ajout d'image pour chaque score
        if (!images[i].isEmpty())
        {
            auto *imageLabel = new QLabel(blocScoresWidget);
            imageLabel->setFixedSize(22, 22);
            QPixmap pix(basePath + images[i]);
            if (!pix.isNull())
            {
                imageLabel->setPixmap(pix.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
            ligne->addWidget(imageLabel);
        }
        // ajout du label score dans la ligne
        blocScoresLayout->addLayout(ligne);
        // ajout dans le vector du label
        scoreLabels.append(valeurLabel);
    }

    auto *pileLayout = new QHBoxLayout();
    pileLayout->setSpacing(4);
    auto *pileLibelle = new QLabel("Piles restantes :", this);
    pileLibelle->setStyleSheet("color: #444;");
    pileLayout->addWidget(pileLibelle, 1);

    labelNombrePiles = new QLabel("0", this);
    labelNombrePiles->setStyleSheet("font-size: 16px; font-weight: bold;");
    pileLayout->addWidget(labelNombrePiles);

    auto *pileImage = new QLabel(this);
    pileImage->setFixedSize(28, 28);
    QPixmap pilePix(basePath + "pile.png");
    if (!pilePix.isNull())
        pileImage->setPixmap(pilePix.scaled(pileImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    pileLayout->addWidget(pileImage);

    scoreLayout->addLayout(pileLayout);
}

void ScorePanel::setScore(int scoreTotal,
                          int scoreHabitation,
                          int scoreMarche,
                          int scoreCaserne,
                          int scoreTemple,
                          int scoreJardin)
{
    constexpr int nbValeurs = 6;
    const int valeurs[nbValeurs] = {scoreTotal, scoreHabitation, scoreMarche, scoreCaserne, scoreTemple, scoreJardin};
    const int count = std::min(static_cast<int>(scoreLabels.size()), nbValeurs);
    for (int i = 0; i < count; ++i)
    {
        if (scoreLabels[i])
            scoreLabels[i]->setText(QString::number(valeurs[i]));
    }
    for (int i = count; i < scoreLabels.size(); ++i)
    {
        if (scoreLabels[i])
            scoreLabels[i]->setText("0");
    }
}

void ScorePanel::setNbPierres(int nbPierres)
{
    if (labelNombrePierre)
        labelNombrePierre->setText(QString::number(nbPierres));
}

void ScorePanel::setNbPiles(int nbPiles)
{
    if (labelNombrePiles)
        labelNombrePiles->setText(QString::number(nbPiles));
}

void ScorePanel::setNomJoueurActif(const QString &nom)
{
    if (labelJoueurActif)
    {
        labelJoueurActif->setText(nom.toUpper());
        labelJoueurActif->setStyleSheet("font-weight: 800;");
    }
}
