#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QStackedWidget;
class PlateauWidget;
class QWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public slots:
    void retourMenu();
    public:
        explicit MainWindow(QWidget* parent = nullptr);

    private:
        void creerLePlateau(int nbJoueurs);

        QStackedWidget* stackWidget = nullptr;
        PlateauWidget* plateauWidget = nullptr;
        QWidget* menuPage = nullptr;
        QWidget* settingsPage = nullptr;
};

#endif
