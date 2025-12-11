#include "HexItem.h"
#include <QtMath>
#include <QPolygonF>
#include <QPen>
#include <QBrush>
#include <QGraphicsPixmapItem>
#include "Hexagone.h"
#include <QFile>
#include <QCoreApplication>

/**
 * @brief Convertit des coordonnées axiales en coordonnées pixel dans la scène.
 */
QPointF axialVersPixel(int q, int r, double size) {
    const double rt3 = std::sqrt(3.0);
    const double px = size * (1.5 * q);
    const double py = size * ((rt3 / 2.0) * q + rt3 * r);
    return {px, py};
}

/**
 * @brief Construit un polygone représentant un hexagone régulier de la taille donnée.
 */
static QPolygonF creerHexagone(double taille) {
    QPolygonF p;
    const double angle0 = 0;
    for (int i = 0; i < 6; ++i) {
        const double a = qDegreesToRadians(60.0 * i + angle0);
        p << QPointF(taille * std::cos(a), taille * std::sin(a));
    }
    return p;
}

HexItem::HexItem(const Hexagone* modele, double taille)
    : QGraphicsPolygonItem(nullptr)
{
    setPolygon(creerHexagone(taille));

    const int q = modele->getX();
    const int r = modele->getY();
    setPos(axialVersPixel(q, r, taille));

    setPen(QPen(Qt::black, 2.0));

    QLinearGradient grad(QPointF(-taille, 0), QPointF(taille, 0));
    grad.setColorAt(0.0, Qt::white);
    grad.setColorAt(1.0, QColor(220, 220, 220));
    setBrush(QBrush(grad));

    const QString basePath = QCoreApplication::applicationDirPath() + "/img/";
    QString imgFile;
    switch (modele->getType())
    {
    case TypeHex::PHabitation: imgFile = "placeHabitation.png"; break;
    case TypeHex::PMarche:     imgFile = "PlaceMarche.png"; break;
    case TypeHex::PTemple:     imgFile = "PlaceTemple.png"; break;
    case TypeHex::PCaserne:    imgFile = "PlaceCarserne.png"; break;
    case TypeHex::PJardin:     imgFile = "PlaceJardin.png"; break;
    case TypeHex::Habitation:  imgFile = "Habitation.png"; break;
    case TypeHex::Marche:      imgFile = "Marche.png"; break;
    case TypeHex::Temple:      imgFile = "Temple.png"; break;
    case TypeHex::Caserne:     imgFile = "Carserne.png"; break;
    case TypeHex::Jardin:      imgFile = "Jardin.png"; break;
    case TypeHex::Carriere:    imgFile = "Carriere.png"; break;
    default:                   imgFile = "placeHabitation.png"; break;
    }

    QPixmap pix(basePath + imgFile);
    QPixmap scaled = pix.scaled(taille * 1.5, taille * 1.5,
                                Qt::KeepAspectRatio,
                                Qt::SmoothTransformation);

    auto *icon = new QGraphicsPixmapItem(scaled, this);
    icon->setOffset(-scaled.width() / 2.0, -scaled.height() / 2.0);
    icon->setZValue(1);
}

void HexItem::setTaille(double nouvelleTaille)
{
    prepareGeometryChange();
    setPolygon(creerHexagone(nouvelleTaille));
}
