#ifndef WIDGETUTILITAIRE_H
#define WIDGETUTILITAIRE_H

#include <QWidget>

class QPushButton;

/**
 * @brief Petit widget utilitaire affichant des actions de validation d'un placement.
 */
class ValiderPlacementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ValiderPlacementWidget(QWidget* parent = nullptr);

    QPushButton* getBoutonConfirmer() const { return boutonConfirmer; }
    QPushButton* getBoutonAnnuler() const { return boutonAnnuler; }

signals:
    void confirmationDemandee();
    void annulationDemandee();

private:
    QPushButton* boutonConfirmer = nullptr;
    QPushButton* boutonAnnuler = nullptr;
};

#endif // WIDGETUTILITAIRE_H
