#include "PageWidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include "../include/Sauvegarde.h"

newPartiePage::newPartiePage(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    auto* label  = new QLabel("test", this);
    layout->addWidget(label);
}

chargePartieWidget::chargePartieWidget(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    auto* layout_liste = new QVBoxLayout();

    const auto sauvegardes = getSauvegardes();
    if (sauvegardes.empty())
    {
        QLabel* noSaves = new QLabel("Aucune sauvegarde disponible");
        layout->addWidget(noSaves);
        layout->setAlignment(noSaves,Qt::AlignCenter);

        return;
    }

}
