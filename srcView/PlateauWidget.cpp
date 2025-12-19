#include "PlateauWidget.h"

#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QKeySequence>
#include <QLabel>
#include <QPointF>
#include <QRect>
#include <QShortcut>
#include <QMessageBox>
#include <QTimer>
#include <QVBoxLayout>
#include <algorithm>
#include <QGuiApplication>
#include <QScreen>

#include "ChantierWidget.h"
#include "EchapWidget.h"
#include "ScorePanel.h"
#include "Tuile.h"
#include "HexItem.h"
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
    connect(echapWidget, &EchapWidget::demandeSauvegarde, this, [this]
            {
        echapWidget->fermerWidget();
        emit demandeSauvegarde(); });
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
    connect(chantierWidget, &ChantierWidget::tuileGraphiquePiochee, this, [this](TuileItem* tuile)
            {
        if (zoneJeuWidget)
        {
            if (tuile)
            {
                int taille = calculerTailleTuile(zoneJeuWidget);
                taille = std::max(20, static_cast<int>(taille * 0.8));
                tuile->setTaille(taille);
            }
            zoneJeuWidget->placerTuileDansZoneJeu(tuile);
        }
        if (!tuile)
            return;

        const int joueur = joueurActif;
        tuile->setProperty("joueurIndex", joueur);
        connect(tuile, &TuileItem::rotationEffectuee, this, &PlateauWidget::surRotationTuileEnMain, Qt::UniqueConnection); });
    for (auto* zone : zonesParJoueur)
    {
        connect(zone, &ZoneJeuWidget::placementTuileAnnule, chantierWidget, &ChantierWidget::remettreTuileDansChantier);
        connect(zone, &ZoneJeuWidget::placementTuileAnnule, this, [this](TuileItem* tuile)
                {
            if (!tuile) return;
            emit piocheAnnulee(static_cast<int>(tuile->getIndiceDansPioche()));
            });
        connect(zone, &ZoneJeuWidget::placementTuileFinalise, this, &PlateauWidget::finaliserTourApresPlacement);
        connect(zone, &ZoneJeuWidget::validationPlacementDemandee, this, &PlateauWidget::relayerValidationPlacementDemandee);
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
void PlateauWidget::finaliserTourApresPlacement(TuileItem* t, const QPointF& positionScene)
{
    (void)positionScene;
    if (!t)
        return;
    // vérification du placement pour le controleur à faire et définir un niveau ensuite
    t->setSelected(false);
    t->setInteractivite(false, false);
    if (chantierWidget)
        chantierWidget->setEnabled(true);
    // Le contrôleur gère le changement de joueur (main/plateau affiché, nb de pierres, etc.)
    // via ses signaux (`setMainJoueurPlateau`, `setNbPierres`, ...). On ne change pas de joueur ici.
    emit tourTermine();
}

void PlateauWidget::afficherPlateauJoueur(const int& index)
{
    ZoneJeuWidget* zone = recupererZone(index);
    if (!zone || !stackPlateaux)
        return;
    joueurActif = index;
    zoneJeuWidget = zone;
    stackPlateaux->setCurrentIndex(index);
    if (echapWidget)
        echapWidget->attacherAScene(zoneJeuWidget->scene());
}

void PlateauWidget::chargerPlateauJoueur(const int& index, const std::vector<Tuile>& tuiles)
{
    ZoneJeuWidget* zone = recupererZone(index);
    if (!zone)
        return;
    zone->viderZone();
    const QPointF origine = zone->getOrigineGrille();
    const int tailleZone = std::max(20, static_cast<int>(calculerTailleTuile(zone) * 0.8));
    for (const Tuile& tuile : tuiles)
    {
        auto* item = creerTuileGraphique(tuile, TuileItem::Mode::ZoneJeu, zone);
        if (!item)
            continue;
        int qRef = 0;
        int rRef = 0;
        {
            Tuile::ConstIterator it = tuile.getConstIterator();
            if (!it.isDone())
            {
                qRef = it.currentItem().getX();
                rRef = it.currentItem().getY();
            }
        }
        zone->ajouterTuileDepuisModele(item, origine + axialVersPixel(qRef, rRef, tailleZone));
        int z = 0;
        for (Tuile::ConstIterator it = tuile.getConstIterator(); !it.isDone(); it.next())
            z = std::max(z, it.currentItem().getZ());
        // Toutes les tuiles au même niveau graphique : l'ordre d'ajout dans la scène fait foi.
        item->setNiveauGraphique(0);
        item->ModifierCouleurEtage(z);
    }
}

void PlateauWidget::afficherTuileEnMain(const int& index, const Tuile& tuile)
{
    ZoneJeuWidget* zone = recupererZone(index);
    if (!zone)
        return;
    auto* item = creerTuileGraphique(tuile, TuileItem::Mode::Placement, zone);
    if (!item)
        return;
    item->setInteractivite(true, true);
    const int joueur = index;
    item->setProperty("joueurIndex", joueur);
    connect(item, &TuileItem::rotationEffectuee, this, &PlateauWidget::surRotationTuileEnMain, Qt::UniqueConnection);
    zone->placerTuileDansZoneJeu(item);
    if (chantierWidget)
        chantierWidget->setEnabled(false);
}

void PlateauWidget::surRotationTuileEnMain(int pas)
{
    auto* tuile = qobject_cast<TuileItem*>(sender());
    if (!tuile)
        return;
    bool ok = false;
    const int joueur = tuile->property("joueurIndex").toInt(&ok);
    if (!ok)
        return;
    emit tuileRotationnee(joueur, pas);
}

void PlateauWidget::afficherMessage(const QString& message)
{
    auto* popup = new QWidget();
    popup->setWindowTitle(tr("Info"));
    popup->setAttribute(Qt::WA_DeleteOnClose);
    popup->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);

    auto* layout = new QVBoxLayout(popup);
    layout->setContentsMargins(12, 10, 12, 12);
    layout->setSpacing(8);

    auto* label = new QLabel(message, popup);
    label->setWordWrap(true);
    layout->addWidget(label);

    popup->show();
    QTimer::singleShot(3000, popup, &QWidget::close);
}

void PlateauWidget::afficherErreur(const QString& message)
{
    QMessageBox box(this);
    box.setIcon(QMessageBox::Warning);
    box.setWindowTitle(tr("Oh zut..."));
    box.setText(message);
    box.setStandardButtons(QMessageBox::Ok);
    box.setStyleSheet(
        "QMessageBox QLabel { font-size: 18px; font-weight: 700; }"
        "QMessageBox QPushButton { font-size: 12px; min-width: 70px; padding: 4px 10px; }");
    box.exec();
}

void PlateauWidget::ModifierCouleurEtage(TuileItem* tuile, int z)
{
    if (!tuile)
        return;
    // Toutes les tuiles au même niveau graphique : l'ordre d'ajout dans la scène fait foi.
    tuile->setNiveauGraphique(0);
    tuile->ModifierCouleurEtage(z);
}

TuileItem* PlateauWidget::creerTuileGraphique(const Tuile& modele, TuileItem::Mode mode, ZoneJeuWidget* zone) const
{
    int taille = calculerTailleTuile(zone ? zone : zoneJeuWidget);
    // Dans la zone de jeu (placement + plateau), on réduit la taille des hexagones (~ -20%).
    if (mode != TuileItem::Mode::Pioche)
        taille = std::max(20, static_cast<int>(taille * 0.8));
    auto* item = new TuileItem(modele, nullptr, mode, taille);
    if (zone)
        item->definirOrigineGrille(zone->getOrigineGrille());
    return item;
}

ZoneJeuWidget* PlateauWidget::recupererZone(const int& index) const
{
    if (zonesParJoueur.empty())
        return nullptr;
    if (index < 0 || index >= static_cast<int>(zonesParJoueur.size()))
        return nullptr;
    return zonesParJoueur[static_cast<size_t>(index)];
}

int PlateauWidget::calculerTailleTuile(const ZoneJeuWidget* zone) const
{
    if (!zone)
    {
        if (chantierWidget)
            return chantierWidget->tailleTuileChantier();
        return 50;
    }
    const QRectF rect = zone->getZoneRect();
    const double base = std::min(rect.width(), rect.height());
    return std::max(30, static_cast<int>(base / 18.0));
}

void PlateauWidget::relayerValidationPlacementDemandee(TuileItem* tuile, const QPoint& coordonnees)
{
    auto* zone = qobject_cast<ZoneJeuWidget*>(sender());
    if (!zone || !tuile)
        return;
    emit validationPlacementDemandee(zone, joueurActif, tuile, coordonnees);
}
