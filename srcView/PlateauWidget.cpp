#include "PlateauWidget.h"
#include "ChantierWidget.h"
#include "ScorePanel.h"
#include "ZoneJeuWidget.h"
#include "Tuile.h"
#include "EchapWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <array>
#include <random>
#include <QShortcut>
#include <QKeySequence>
#include <QGraphicsScene>
#include <QRect>
#include <QPointF>

//pour tester
namespace {
constexpr std::array<OffsetQR, 3> kOffsetsTuile = { {
    {0, 0},
    {1, -1},
    {0, -1}
} };
constexpr std::array<TypeHex, 11> kTypesDisponibles = {
    TypeHex::Habitation,
    TypeHex::Marche,
    TypeHex::Temple,
    TypeHex::Caserne,
    TypeHex::Jardin,
    TypeHex::PHabitation,
    TypeHex::PMarche,
    TypeHex::PTemple,
    TypeHex::PCaserne,
    TypeHex::PJardin,
    TypeHex::Carriere
};

OffsetQR appliquerRotations(OffsetQR offset, int rotationCount)
{
    rotationCount = rotationCount % 6;
    for (int i = 0; i < rotationCount; ++i)
        rotation60deg(offset);
    return offset;
}

TypeHex tirerType(std::mt19937& gen)
{
    std::uniform_int_distribution<size_t> dist(0, kTypesDisponibles.size() - 1);
    return kTypesDisponibles[dist(gen)];
}
}

void PlateauWidget::genererTuilesTests()
{
    if (!chantierWidget)
        return;

    constexpr int nbTuiles = 4;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> rotations(0, 5);

    tuilesDeTest.clear();
    hexagonesDeTest.clear();
    tuilesDeTest.reserve(nbTuiles);
    hexagonesDeTest.reserve(nbTuiles * kOffsetsTuile.size());

    for (int i = 0; i < nbTuiles; ++i) {
        const int nbRotations = rotations(gen);
        std::array<Hexagone*, kOffsetsTuile.size()> hexRefs{};

        for (size_t j = 0; j < kOffsetsTuile.size(); ++j) {
            OffsetQR coord = appliquerRotations(kOffsetsTuile[j], nbRotations);
            const int q = coord.q;
            const int r = coord.r;
            const int z = -q - r;
            auto hex = std::make_unique<Hexagone>(q, r, z, tirerType(gen));
            hexRefs[j] = hex.get();
            hexagonesDeTest.push_back(std::move(hex));
        }

        auto tuile = std::make_unique<Tuile>(hexRefs[0], hexRefs[1], hexRefs[2]);
        chantierWidget->ajouterTuilleDansChantier(tuile.get());
        tuilesDeTest.push_back(std::move(tuile));
    }
}
//fin de pour tester

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
    const int chantierHeight = height() - scoreWidgetSize;


    //appelle le constructeur de la zone de jeu
    zoneJeuWidget = new ZoneJeuWidget(plateauWidth, plateauHeight, this);
    layout->addWidget(zoneJeuWidget, 1);

    echapWidget = new EchapWidget();
    echapWidget->attacherAScene(zoneJeuWidget->scene());
    connect(echapWidget, &EchapWidget::visibiliteChangee,this, &PlateauWidget::gererBlocageInteractions);
    connect(echapWidget, &EchapWidget::demandeParametres, this, [this]{
        echapWidget->fermerWidget();
        emit demandeParametres();
    });
    connect(echapWidget, &EchapWidget::demandeRetourMenu, this, [this]{
        echapWidget->fermerWidget();
        emit demandeRetourMenu();
    });
    connect(echapWidget, &EchapWidget::demandeQuitter, this, [this]{
        echapWidget->fermerWidget();
        emit demandeQuitter();
    });

    raccourciEchap = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    connect(raccourciEchap, &QShortcut::activated,this, &PlateauWidget::basculerMenuEchap);
    //gère la partie droite (widget score au dessus et en dessous la scène pour le chantier)
    auto* panneauDroit = new QWidget(this);
    panneauDroit->setFixedWidth(colonneDroiteLargeur);
    auto* colonneDroite = new QVBoxLayout(panneauDroit);
    colonneDroite->setContentsMargins(0, 0, 0, 0);
    colonneDroite->setSpacing(0);
    layout->addWidget(panneauDroit, 0);

    //création de la widget score
    scorePanel = new ScorePanel(colonneDroiteLargeur, scoreWidgetSize, panneauDroit);
    colonneDroite->addWidget(scorePanel, 0, Qt::AlignTop);

    //création de la scène chantier
    chantierWidget = new ChantierWidget(colonneDroiteLargeur, chantierHeight, panneauDroit);
    colonneDroite->addWidget(chantierWidget, 1);

    //gestion des flux entre le chantier et la zone de jeu (pioche / validation / annulation)
    connect(chantierWidget, &ChantierWidget::tuilePiochee,zoneJeuWidget, &ZoneJeuWidget::placerTuileDansZoneJeu);
    connect(zoneJeuWidget, &ZoneJeuWidget::validationPlacementAnnulee,chantierWidget, &ChantierWidget::remettreTuileDansChantier);
    connect(zoneJeuWidget, &ZoneJeuWidget::validationPlacementConfirmee,this, &PlateauWidget::validerPlacementTuile);


    // alimentation de test pour valider l'enchaînement pioche -> zone de jeu
    genererTuilesTests();
}

void PlateauWidget::basculerMenuEchap()
{
    //si pas définit on ne fais rien
    if (!echapWidget || !zoneJeuWidget)
        return;

    //si la widget est déjà actives on la ferme
    if (echapWidget->estActif())
        echapWidget->fermerWidget();
    else
    {
        //calcul la position de la widget
        const QRect vue = zoneJeuWidget->viewport()->rect();
        const QPointF topLeft = zoneJeuWidget->mapToScene(vue.topLeft());
        const QPointF bottomRight = zoneJeuWidget->mapToScene(vue.bottomRight());
        const QRectF visibleRect(topLeft, bottomRight);
        echapWidget->afficherEchap(visibleRect);
    }
}

void PlateauWidget::gererBlocageInteractions(bool widgetActif)
{
    if (zoneJeuWidget)
        zoneJeuWidget->setBlocageInteractions(widgetActif);
    if (chantierWidget)
        chantierWidget->setEnabled(!widgetActif);
    if (scorePanel)
        scorePanel->setEnabled(!widgetActif);
}
void PlateauWidget::validerPlacementTuile(TuileItem* t)
{
    //TODO: brancher sur la logique métier pour enregistrer définitivement la position
    Q_UNUSED(t);
}
