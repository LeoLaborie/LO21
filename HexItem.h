#ifndef HEXITEM_H
#define HEXITEM_H
#include <QGraphicsPolygonItem>

class Hexagone;

// Item graphique d’un hexagone (une case de tuile)
class HexItem : public QGraphicsPolygonItem
{
public:
    HexItem(const Hexagone *modele, double taille);

    const Hexagone *model() const { return m_model; }
    double size() const { return m_size; }

private:
    const Hexagone *m_model = nullptr;
    double m_size = 50.0;
    void setupAppearance();
    void placeIcon(const QString &resourcePath);
};
#endif // HEXITEM_H