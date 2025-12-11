#include "ScorePanel.h"

#include <QLabel>
#include <QVBoxLayout>

ScorePanel::ScorePanel(int width, int height, QWidget* parent)
    : QWidget(parent)
{
    // définir la taille et paramètre
    setFixedSize(width, height);
    setStyleSheet("background-color: #f0f0f0;");

    // création du layout central
    auto* scoreLayout = new QVBoxLayout(this);
    scoreLayout->setContentsMargins(20, 20, 20, 20);
    scoreLayout->setSpacing(8);

    // ajout des éléments utiles pour l'affichage du score (pas terminer ici c'est un exemple)
    labelScore = new QLabel("Scores", this);
    labelJoueurActif = new QLabel("Joueur actif", this);

    // ajout de paramètre sur les éléments du layouts
    for (auto* label : {labelScore, labelJoueurActif})
    {
        label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        scoreLayout->addWidget(label);
    }
}
