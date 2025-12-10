#include "ChantierWidget.h"
#include <QBrush>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPen>

ChantierWidget::ChantierWidget(int width, int height, QWidget* parent)
    : QGraphicsView(parent)
{
    //création de la scène
    chantierScene = new QGraphicsScene(this);
    setScene(chantierScene);

    //application des différentes préférences
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(width, height);

    //ajout dans la scène
    chantierScene->setSceneRect(0, 0, width, height);
    chantierZoneRectItem = chantierScene->addRect(0, 0, width, height, QPen(Qt::NoPen), QBrush(Qt::red));
    chantierZoneRect = chantierZoneRectItem->rect();
}
void ChantierWidget::ajouterTuilleDansChantier(Tuile* t) {
    const int taille = qMin((width() / 2 - 20),(height()/10-30));
    const int indice = static_cast<int>(listeTuilesChantier.size());
    auto tuile = std::make_unique<TuileItem>(*t, this, TuileItem::Mode::Pioche, taille, indice);
    chantierScene->addItem(tuile.get());              
    listeTuilesChantier.push_back(std::move(tuile));  
}


std::unique_ptr<TuileItem> ChantierWidget::retirerTuilleDeChantier(int indice)
{
    if (indice < 0 || indice >= static_cast<int>(listeTuilesChantier.size()))
        return nullptr;

    chantierScene->removeItem(listeTuilesChantier[indice].get());
    auto tuile = std::move(listeTuilesChantier[indice]);
    listeTuilesChantier.erase(listeTuilesChantier.begin() + indice);
    return tuile;
}
void ChantierWidget::piocherTuile(int indice){
    
}