#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

#include <QtWidgets/QWidget>
#include <QtCore/QObject>

class newPartiePage : public QWidget
{
    Q_OBJECT

public:
    explicit newPartiePage(QWidget* parent = nullptr);
};

class chargePartieWidget: public QWidget
{
    Q_OBJECT

public:
    explicit chargePartieWidget(QWidget* parent = nullptr);
};

#endif
