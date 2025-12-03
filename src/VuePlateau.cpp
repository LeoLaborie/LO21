#include "VuePlateau.h"
#include <QGraphicsScene>


VuePlateau::VuePlateau(QWidget* parent)
    : QWidget(parent)
{
    auto plateau = new QGraphicsScene(this);
    
}
