#include "WidgetUtilitaire.h"

#include <QHBoxLayout>
#include <QPushButton>

ValiderPlacementWidget::ValiderPlacementWidget(QWidget* parent)
    : QWidget(parent)
{
    // widget léger translucide qui flotte au-dessus des tuiles
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);
    setWindowFlags(Qt::FramelessWindowHint);
    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(10);

    // création des boutons de décision autour du placement
    boutonConfirmer = new QPushButton("Confirmer", this);
    boutonAnnuler = new QPushButton("Annuler", this);
    boutonConfirmer->setObjectName("btnConfirmerPlacement");
    boutonAnnuler->setObjectName("btnAnnulerPlacement");

    layout->addWidget(boutonConfirmer);
    layout->addWidget(boutonAnnuler);

    // style commun reprenant les couleurs Akropolis et les états hover/pressed
    const char* styleBoutons =
        "QPushButton#btnConfirmerPlacement {"
        "  font-size: 14px;"
        "  font-weight: 600;"
        "  color: white;"
        "  background-color: #059669;"
        "  border: none;"
        "  border-radius: 10px;"
        "  padding: 6px 14px;"
        "}"
        "QPushButton#btnConfirmerPlacement:hover { background-color: #10b981; }"
        "QPushButton#btnConfirmerPlacement:pressed { background-color: #047857; }"
        "QPushButton#btnAnnulerPlacement {"
        "  font-size: 14px;"
        "  font-weight: 600;"
        "  color: #0f172a;"
        "  background-color: #e2e8f0;"
        "  border: none;"
        "  border-radius: 10px;"
        "  padding: 6px 14px;"
        "}"
        "QPushButton#btnAnnulerPlacement:hover { background-color: #cbd5f5; }"
        "QPushButton#btnAnnulerPlacement:pressed { background-color: #94a3b8; }";
    boutonConfirmer->setStyleSheet(styleBoutons);
    boutonAnnuler->setStyleSheet(styleBoutons);
    boutonConfirmer->setCursor(Qt::PointingHandCursor);
    boutonAnnuler->setCursor(Qt::PointingHandCursor);

    // propagation directe des clics aux signaux publics
    connect(boutonConfirmer, &QPushButton::clicked, this, &ValiderPlacementWidget::confirmationDemandee);
    connect(boutonAnnuler, &QPushButton::clicked, this, &ValiderPlacementWidget::annulationDemandee);
    setStyleSheet("background-color: transparent;");
}
