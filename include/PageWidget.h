#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

#include <QLineEdit>
#include "PageWidget.h"
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QVector>
#include <QVBoxLayout>
#include <string>
#include <iostream>

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
