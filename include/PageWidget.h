#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

#include <QSpinBox>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QWidget>

class newPartiePage : public QWidget
{
    Q_OBJECT

public:
    explicit newPartiePage(QWidget* parent = nullptr);

private:
    QSpinBox* NbJoueurs;
    QVector<QLineEdit*> PseudoJoueurs;
    QVector<QCheckBox*> variantesOptions;

private slots:
    void lancerLaPartie();   
};

#endif
