#include "HexItem.h"
#include "Hexagone.h"
#include <QtMath>
#include <QPolygonF>
#include <QPen>
#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QLinearGradient>
#include <QPixmap>
#include <QColor>

static QPolygonF creerHexagone(double taille)
{
    QPolygonF p;
    const double angle0 = 0.0;
    for (int i = 0; i < 6; ++i)
    {
        const double a = qDegreesToRadians(60.0 * i + angle0);
        p << QPointF(taille * std::cos(a), taille * std::sin(a));
    }
    return p;
}

static QPointF axialVersPixel(int q, int r, double size)
{
    const double rt3 = std::sqrt(3.0);
    const double px = size * (1.5 * q);
    const double py = size * ((rt3 / 2.0) * q + rt3 * r);
    return {px, py};
}

HexItem::HexItem(const Hexagone *modele, double taille)
    : QGraphicsPolygonItem(nullptr), m_model(modele), m_size(taille)
{
    setPolygon(creerHexagone(m_size));
    const int q = m_model->getX();
    const int r = m_model->getY();
    setPos(axialVersPixel(q, r, m_size));
    setupAppearance();
    QString imgPath;
    switch (m_model->getType())
    {
    case TypeHex::PHabitation:
        imgPath = ":/images/PlaceHabitation.png";
        break;
    case TypeHex::PMarche:
        imgPath = ":/images/PlaceMarche.png";
        break;
    case TypeHex::PTemple:
        imgPath = ":/images/PlaceTemple.png";
        break;
    case TypeHex::PCaserne:
        imgPath = ":/images/PlaceCaserne.png";
        break;
    case TypeHex::PJardin:
        imgPath = ":/images/PlaceJardin.png";
        break;
    case TypeHex::Habitation:
        imgPath = ":/images/Habitation.png";
        break;
    case TypeHex::Marche:
        imgPath = ":/images/Marche.png";
        break;
    case TypeHex::Temple:
        imgPath = ":/images/Temple.png";
        break;
    case TypeHex::Caserne:
        imgPath = ":/images/Caserne.png";
        break;
    case TypeHex::Jardin:
        imgPath = ":/images/Jardin.png";
        break;
    case TypeHex::Carriere:
        imgPath = ":/images/Carriere.png";
        break;
    default:
        imgPath = ":/images/Habitation.png";
        break;
    }
    placeIcon(imgPath);
}

void HexItem::setupAppearance()
{
    setPen(QPen(Qt::black, 2.0));
    QLinearGradient grad(QPointF(-m_size, 0), QPointF(m_size, 0));
    grad.setColorAt(0.0, Qt::white);
    grad.setColorAt(1.0, QColor(220, 220, 220));
    setBrush(QBrush(grad));
}

void HexItem::placeIcon(const QString &resourcePath)
{
    QPixmap pix(resourcePath);
    if (pix.isNull())
        return;
    QPixmap scaled = pix.scaled(m_size * 1.5, m_size * 1.5,
                                Qt::KeepAspectRatio,
                                Qt::SmoothTransformation);
    auto *icon = new QGraphicsPixmapItem(scaled, this);
    icon->setOffset(-scaled.width() / 2.0, -scaled.height() / 2.0);
    icon->setZValue(1);
}
