#include "WidgetUtilitaire.h"
#include "EchapWidget.h"
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QVBoxLayout>

WidgetUtilitaire::WidgetUtilitaire(QWidget* parent)
    : QWidget(parent)
    , echapWidget(new EchapWidget(this))
{
    setAttribute(Qt::WA_StyledBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint);
    setFocusPolicy(Qt::StrongFocus);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(echapWidget);

    connect(echapWidget, &EchapWidget::demandeFermeture,this, &WidgetUtilitaire::fermerWidget);
    adjustSize();
}

void WidgetUtilitaire::attacherAScene(QGraphicsScene* scene)
{
    //si on a pas définit la scene on return
    if (!scene || proxy) return;

    //on ajout la widget dans le proxy
    proxy = scene->addWidget(this);
    //on le met au premier plan
    proxy->setZValue(100);
    //on le rend invisible 
    proxy->setVisible(false);
}

void WidgetUtilitaire::afficherEchap(const QRectF& zoneReference)
{
    //si pas définit on ne fait rien
    if (!proxy) return;

    //on place bien la fenetre de pausse au milieu du rectangle passer en parametre grace à la fonction de mise à jour du placement
    mettreAJourPosition(zoneReference);
    //on le rend visible
    proxy->setVisible(true);
    //et on l'active
    actif = true;
    //on envoie le signal pour désactiver les tuiles et le reste
    emit visibiliteChangee(true);
    setFocus();
}

void WidgetUtilitaire::fermerWidget()
{
    //si pas définit ou pas actif on ne fait rien
    if (!proxy || !actif) return;

    //on le rend invisible
    proxy->setVisible(false);
    //on le désactive
    actif = false;
    //on envoie le signal pour réactiver les Tuiles et le reste
    emit visibiliteChangee(false);
}

void WidgetUtilitaire::mettreAJourPosition(const QRectF& zoneReference)
{
    if (!proxy) return;
    //calcul le placement x et y pour afficher le proxy au mileu
    const double x = zoneReference.center().x() - width() / 2.0;
    const double y = zoneReference.center().y() - height() / 2.0;
    proxy->setPos(x, y);
}
