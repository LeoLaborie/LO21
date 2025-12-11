#include <QApplication>
#include <fstream>
#include <string>

#include "ControleConsole.h"
#include "mainwindow.h"

// petite fonction utilitaire pour afficher un fichier texte dans la console (README notamment)
void ouvrirFichier(const std::string& readme)
{
    std::ifstream f(readme);
    if (!f)
    {
        std::cout << "Aucun readme disponible" << std::endl;
        return;
    }
    std::cout << f.rdbuf();
}
int main(int argc, char* argv[])
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
        // mode graphique standard
        QApplication app(argc, argv);
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
