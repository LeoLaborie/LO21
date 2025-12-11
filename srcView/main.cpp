#include <QApplication>
#include "mainwindow.h"
#include <string>
#include <fstream>
#include "ControleConsole.h"


void ouvrirFichier(const std::string& readme){
    std::ifstream f(readme);
    if (!f)
    {
     std::cout<<"Aucun readme disponible"<<std::endl;
     return;
    }   
    std::cout<<f.rdbuf();
}
int main(int argc, char *argv[])
{
    std::string argument="";
    if (argc>1){argument = argv[1];}
    if (argument=="-C" || argument=="-c"){
        controleConsole();
        return 0;
    }
    if (argument=="-h" || argument=="-H" || argument=="--help"){
            ouvrirFichier("README.md");
            return 0;
    }
    else{
        QApplication app(argc, argv);
        MainWindow w;
        w.resize(1920, 1080);
        w.show();
        return app.exec();
    }
}
