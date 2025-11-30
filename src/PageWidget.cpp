#include "PageWidget.h"

#include <QLabel>
#include <QVBoxLayout>

newPartiePage::newPartiePage(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    auto* label  = new QLabel("test", this);
    layout->addWidget(label);
}
