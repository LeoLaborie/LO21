#include <QWidget>

class EchapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EchapWidget(QWidget* parent = nullptr);

signals:
    void demandeFermeture();
    void demandeParametres();
    void demandeRetourMenu();
    void demandeSauvegarde();
    void demandeQuitter();
};
