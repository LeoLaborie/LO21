#include "HexItem.h"
#include <QtMath>
#include <QPolygonF>
#include <QPen>
#include <QBrush>
#include "Hexagone.h"
#include <QFile>


QPointF axialVersPixel(int q, int r, double size) {
    const double rt3 = std::sqrt(3.0);
    const double px = size * (1.5 * q);
    const double py = size * ((rt3 / 2.0) * q + rt3 * r);
    return {px, py};
}


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

    QString imgPath;
    switch (modele->getType())
    {
    case TypeHex::PHabitation: imgPath = "C:/Users/thibault/source/repos/LeoLaborie/LO21/img/PlaceHabitation.png"; break;
    case TypeHex::PMarche:     imgPath = "C:/Users/thibault/source/repos/LeoLaborie/LO21/img/PlaceMarche.png"; break;
    case TypeHex::PTemple:     imgPath = "C:/Users/thibault/source/repos/LeoLaborie/LO21/img/PlaceTemple.png"; break;
    case TypeHex::PCaserne:    imgPath = "C:/Users/thibault/source/repos/LeoLaborie/LO21/img/placeCarserne.png"; break;
    case TypeHex::PJardin:     imgPath = "C:/Users/thibault/source/repos/LeoLaborie/LO21/img/placeJardin.png"; break;
    case TypeHex::Habitation:  imgPath = "C:/Users/thibault/source/repos/LeoLaborie/LO21/img/Habitation.png"; break;
    case TypeHex::Marche:      imgPath = "C:/Users/thibault/source/repos/LeoLaborie/LO21/img/Marche.png"; break;
    case TypeHex::Temple:      imgPath = "C:/Users/thibault/source/repos/LeoLaborie/LO21/img/Temple.png"; break;
    case TypeHex::Caserne:     imgPath = "C:/Users/thibault/source/repos/LeoLaborie/LO21/img/Carserne.png"; break;
    case TypeHex::Jardin:      imgPath = "C:/Users/thibault/source/repos/LeoLaborie/LO21/img/Jardin.png"; break;
    case TypeHex::Carriere:    imgPath = "C:/Users/thibault/source/repos/LeoLaborie/LO21/img/Carriere.png"; break;
    default:                   imgPath = "C:/Users/thibault/source/repos/LeoLaborie/LO21/img/placeHabitation.png"; break;
    }

    QPixmap pix(imgPath);
    QPixmap scaled = pix.scaled(taille * 1.5, taille * 1.5,
                                Qt::KeepAspectRatio,
                                Qt::SmoothTransformation);

    auto *icon = new QGraphicsPixmapItem(scaled, this);
    icon->setOffset(-scaled.width() / 2.0, -scaled.height() / 2.0);
    icon->setZValue(1);
}
