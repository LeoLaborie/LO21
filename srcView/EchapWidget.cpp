#include <QPushButton>
#include "EchapWidget.h"
#include <QVBoxLayout>
#include <QLabel>


EchapWidget::EchapWidget(QWidget * parent) : QWidget(parent){
    setStyleSheet("QWidget { color: #0f172a; }"
                  "QLineEdit, QSpinBox, QComboBox {"
                  "color: #0f172a;"
                  "background: #ffffff;"
                  "border: 1px solid #d0d5dd;"
                  "border-radius: 6px;"
                  "selection-background-color: #0a84ff;"
                  "selection-color: #ffffff;"
                  "}");

    setAttribute(Qt::WA_StyledBackground, true);
    auto *LayoutCentral = new QVBoxLayout(this);
    LayoutCentral->setContentsMargins(0, 0, 0, 0);
    LayoutCentral->setAlignment(Qt::AlignCenter);

    auto *Cadre = new QFrame();
    Cadre->setObjectName("newGameCadre");
    Cadre->setMaximumWidth(400);
    Cadre->setStyleSheet(
        "#newGameCadre {background-color: #fafafa; border: 1px solid #dcdcdc; border-radius: 18px;}"
    );
    auto *layoutMenu= new QVBoxLayout(Cadre);
    auto *boutonRetour = new QPushButton("Retour");
    boutonRetour->setStyleSheet("QPushButton { font-size: 15px; font-weight: 600; color: white; background-color: #0078d4; border-radius: 8px; padding: 6px 14px; }"
                               "QPushButton:hover { background-color: #0a84ff; }"
                               "QPushButton:pressed { background-color: #0062a3; }");
    layoutMenu->addWidget(boutonRetour);

}