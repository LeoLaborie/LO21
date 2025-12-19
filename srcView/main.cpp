#include <QApplication>
#include <QPalette>
#include <QStyleFactory>
#include <fstream>
#include <string>

#include "ControleConsole.h"
#include "mainwindow.h"

// petite fonction utilitaire pour afficher un fichier texte dans la console (README notamment)
void ouvrirFichier(const std::string &readme)
{
    std::ifstream f(readme);
    if (!f)
    {
        std::cout << "Aucun readme disponible" << std::endl;
        return;
    }
    std::cout << f.rdbuf() << std::endl;
}
int main(int argc, char *argv[])
{
    // par défaut on lance l'interface graphique si aucun argument n'est fourni
    std::string argument;
    if (argc == 1)
    {
        argument = "-g";
    }
    else
    {
        argument = argv[1];
    }
    if (argument == "-C" || argument == "-c")
    {
        // mode console, délègue totalement au contrôleur texte
        controleConsole();
        return 0;
    }
    if (argument == "-h" || argument == "-H" || argument == "--help")
    {
        ouvrirFichier("README.md");
        return 0;
    }
    if (argument == "-g" || argument == "-G")
    {
        QApplication app(argc, argv);
        app.setStyle(QStyleFactory::create("Fusion"));
        QPalette pal;
        pal.setColor(QPalette::Window, Qt::white);
        pal.setColor(QPalette::WindowText, Qt::black);
        pal.setColor(QPalette::Base, Qt::white);
        pal.setColor(QPalette::Text, Qt::black);
        pal.setColor(QPalette::Button, Qt::white);
        pal.setColor(QPalette::ButtonText, Qt::black);
        app.setPalette(pal);

        MainWindow w;
        w.showFullScreen();
        return app.exec();
    }
    else
    {
        std::cout << "Argument invalide : \n -C -c : Lancer le mode console. \n -H -h : Readme. \n -G -g : Lancer l'interface graphique. \n Acun argument : Lancer l'interface graphique. \n Pour plus d'information se référer au Readme."
                  << std::endl;
    }
}
