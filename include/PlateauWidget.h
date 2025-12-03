#ifndef PLATEAUWIDGET_H
#define PLATEAUWIDGET_H
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QRectF>
#include "Partie.h"
#include "TuileItem.h"

class PlateauWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlateauWidget(QWidget* parent = nullptr);
    void ajouterTuile(Tuile & t);

private:
    QGraphicsScene* plateauScene = nullptr;
    QGraphicsRectItem* zonePlateauRectItem = nullptr;
    QRectF zonePlateauRect;
};

#endif
