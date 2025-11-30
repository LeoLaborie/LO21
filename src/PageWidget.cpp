#include "PageWidget.h"
#include <QLayout>
#include <QLabel>

newPartiePage::newPartiePage(QWidget* parent = nullptr){
    QWidget(parent);
    auto * label = new QLabel("test",this);
}
