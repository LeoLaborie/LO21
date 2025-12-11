#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QSpinBox>
#include <QStringList>
#include <QVBoxLayout>
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
        // déclarer le destructeur peut etre
    private slots:
        void lancerLaPartie();
    signals:
    void envoieArgument(int nb, const QStringList& pseudos,const QVector<bool>& variantes);
    void retourMenu();
};

class chargerPartiePage : public QWidget
{
    Q_OBJECT
    public:
        explicit chargerPartiePage(QWidget* parent = nullptr);
        QLabel* erreur;
    public slots:
        void rafraichirSauvegardes();

    private:
        QComboBox* NomSauvegarde;
    private slots:
        void chargerLaPartie();
    signals:
    void envoieArgument(std::string nom);
    void retourMenu();
};

#endif
