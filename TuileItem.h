#ifdef TUILEITEM_H
#define TUILEITEM_H
#include <QGraphicsItemGroup>

class HexItem;
class QGraphicsSceneMouseEvent;

class TuileItem : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    explicit TuileItem(QGraphicsItem *parent = nullptr);
    explicit TuileItem(class Tuile &ref, QGraphicsItem *parent = nullptr);

    void addHex(HexItem *hex);
    void rotate60();             // rotation par paliers
    void replacerCorrectement(); // snap sur la grille (OK même après rotation)

signals:
    void rightClicked();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    HexItem *hexRef = nullptr; // hexagone “ancre” pour le snap
};

// Helpers utiles si besoin ailleurs (déclarés ici, définis dans .cpp)
QPointF pixelVersAxial(double px, double py, double size);
QPointF axialVersPixel(int q, int r, double size);

#endif // TUILEITEM_H