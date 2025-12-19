#include "ChantierWidget.h"
#include <QBrush>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPen>
#include <algorithm>

namespace
{
    void placerTuileCentre(TuileItem* tuile, const QPointF& centreScene)
    {
        if (!tuile)
            return;
        // calcule la différence entre le centre du rectangle formé par les 3 hexagones
        // et le centre souhaité dans la scène, puis décale la tuile pour la centrer
        const QPointF centreLocal = tuile->boundingRect().center();
        tuile->setPos(centreScene - centreLocal);
    }
}  // namespace

ChantierWidget::ChantierWidget(int width, int height, int nbJoueurs, QWidget* parent)
    : QGraphicsView(parent)
{
    nbMaxTuilesAffichees = std::max(1, nbJoueurs + 2);
    setObjectName("chantierView");
    setStyleSheet(
        "#chantierView {"
        "  border: 2px solid black;"
        "  background-color: grey;"
        "}");
    // création de la scène
    chantierScene = new QGraphicsScene(this);
    setScene(chantierScene);
    // application des différentes préférences
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(width, height);

    // ajout dans la scène
    chantierScene->setSceneRect(0, 0, width, height);
    chantierZoneRectItem = chantierScene->addRect(0, 0, width, height, QPen(Qt::NoPen));
}

void ChantierWidget::ajouterTuilleDansChantier(Tuile* t)
{
    if (!t)
        return;
    const int taille = calculerTailleTuile();
    const int indice = static_cast<int>(listeTuilesChantier.size());
    auto* tuile = new TuileItem(*t, nullptr, TuileItem::Mode::Pioche, taille, indice);
    connect(tuile, &TuileItem::estPiocher, this, &ChantierWidget::piocherTuile);
    chantierScene->addItem(tuile);
    listeTuilesChantier.push_back(tuile);
    reordonnerTuiles();
    mettreAJourDisponibilite();
}

void ChantierWidget::definirChantier(const std::vector<Tuile>& tuiles)
{
    viderChantier();
    for (const Tuile& tuile : tuiles)
    {
        const int taille = calculerTailleTuile();
        const int indice = static_cast<int>(listeTuilesChantier.size());
        auto* tuileItem = new TuileItem(tuile, nullptr, TuileItem::Mode::Pioche, taille, indice);
        connect(tuileItem, &TuileItem::estPiocher, this, &ChantierWidget::piocherTuile);
        chantierScene->addItem(tuileItem);
        listeTuilesChantier.push_back(tuileItem);
    }
    reordonnerTuiles();
    mettreAJourDisponibilite();
}

void ChantierWidget::mettreAJourPierres(int nbPierres)
{
    nbPierresDisponibles = nbPierres;
    mettreAJourDisponibilite();
}

TuileItem* ChantierWidget::retirerTuilleDeChantier(int indice)
{
    if (indice < 0 || indice >= static_cast<int>(listeTuilesChantier.size()))
        return nullptr;

    // supprime la tuile de la scène visuelle et du tableau pour la donner à la zone de jeu
    TuileItem* tuile = listeTuilesChantier[indice];
    chantierScene->removeItem(tuile);
    listeTuilesChantier.erase(listeTuilesChantier.begin() + indice);
    reordonnerTuiles();
    return tuile;
}
void ChantierWidget::piocherTuile(TuileId id)
{
    const int indice = trouverIndiceDepuisId(id);
    if (indice < 0)
        return;
    TuileItem * tuile = retirerTuilleDeChantier(indice);
    if (!tuile)
        return;
    tuileEnTransit = tuile;
    // passe en mode placement pour autoriser déplacement/rotation dans la zone de jeu, à mettre dans
    tuile->setMode(TuileItem::Mode::Placement);
    tuile->setInteractivite(true, true);
    emit tuileGraphiquePiochee(tuile);
    emit tuileSelectionnee(id);
    setEnabled(false);
}

void ChantierWidget::remettreTuileDansChantier(TuileItem* tuile)
{
    if (!tuile)
        return;

    // on récupere les informations qu'on a besion (comme l'utilisateur ne peux pas ajouter manuelement des tuiles dans le chantier pas besion de vérfier le nb max de Tuile est déjà atteint)
    const int index = tuile->getIndiceDansPioche();
    // on remet le mode de la tuile sur Pioche et désactive tout ce qu'on a pas besion
    tuile->setMode(TuileItem::Mode::Pioche);
    tuile->setInteractivite(false, false);
    tuile->setSelected(false);
    tuile->setRotation(0.0);
    tuile->setNiveauGraphique(0);
    tuile->setTaille(calculerTailleTuile());

    // plus qu'a la remettre et rappeler la fonction qui ordonne les TUiles dans le chaniter
    chantierScene->addItem(tuile);
    listeTuilesChantier.insert(listeTuilesChantier.begin() + index, tuile);
    reordonnerTuiles();
    mettreAJourDisponibilite();
    setEnabled(true);
    if (tuile == tuileEnTransit)
        tuileEnTransit = nullptr;
}

void ChantierWidget::annulerPiocheEnCours(TuileId id)
{
    if (!tuileEnTransit || tuileEnTransit->getTuileId() != id)
        return;
    remettreTuileDansChantier(tuileEnTransit);
    tuileEnTransit = nullptr;
}

void ChantierWidget::fauxJoueurRetireTuile(TuileId id)
{
    // Le faux joueur joue sans interaction : on retire juste visuellement la tuile choisie.
    // On ignore toute tuile en transit (elle ne devrait pas exister pendant le tour IA).
    const int indice = trouverIndiceDepuisId(id);
    if (indice < 0)
        return;
    TuileItem* tuile = retirerTuilleDeChantier(indice);
    if (!tuile)
        return;
    delete tuile;
    setEnabled(true);
    mettreAJourDisponibilite();
}

void ChantierWidget::reordonnerTuiles()
{
    // réaligne verticalement toutes les tuiles en conservant leur indice
    const int n = static_cast<int>(listeTuilesChantier.size());
    if (n == 0) return;
    const QRectF rect = chantierZoneRectItem->rect();
    const double xCentre = rect.center().x();
    // On prend la hauteur réelle d'une tuile
    const double hauteurHex = listeTuilesChantier[0]->boundingRect().height();
    const double paddingTop = 10.0;  // marge en haut
    const double gap = 5.0;          // espace entre deux tuiles

    for (int i = 0; i < n; ++i)
    {
        TuileItem* tuile = listeTuilesChantier[i];
        if (!tuile) continue;
        tuile->setIndiceDansPioche(static_cast<unsigned int>(i));
        const double yCentre = rect.top() + paddingTop + hauteurHex / 2.0 + i * (hauteurHex + gap);

        placerTuileCentre(tuile, QPointF(xCentre, yCentre));
    }
}

void ChantierWidget::viderChantier()
{
    for (auto* tuile : listeTuilesChantier)
    {
        if (!tuile)
            continue;
        chantierScene->removeItem(tuile);
        delete tuile;
    }
    listeTuilesChantier.clear();
    tuileEnTransit = nullptr;
}

void ChantierWidget::mettreAJourDisponibilite()
{
    for (size_t i = 0; i < listeTuilesChantier.size(); ++i)
    {
        TuileItem* tuile = listeTuilesChantier[i];
        if (!tuile)
            continue;
        const bool abordable = static_cast<int>(i) <= nbPierresDisponibles;
        tuile->setEnabled(abordable);
        tuile->setOpacity(abordable ? 1.0 : 0.35);
    }
}

int ChantierWidget::trouverIndiceDepuisId(TuileId id) const
{
    for (int i = 0; i < static_cast<int>(listeTuilesChantier.size()); ++i)
    {
        TuileItem* tuile = listeTuilesChantier[static_cast<size_t>(i)];
        if (tuile && tuile->getTuileId() == id)
            return i;
    }
    return -1;
}

int ChantierWidget::calculerTailleTuile() const
{
    const double largeur = width() - 20.0;
    const double hauteur = height() - 40.0;
    const double base = std::min(largeur / (1.5 + std::sqrt(3.0)),
                                 hauteur / ((2.0 + std::sqrt(3.0)) * std::max(1, nbMaxTuilesAffichees)));
    return std::max(30, static_cast<int>(base));
}

int ChantierWidget::tailleTuileChantier() const
{
    return calculerTailleTuile();
}
