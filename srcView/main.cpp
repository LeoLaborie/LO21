#include <QApplication>
#include "mainwindow.h"
#include <string>
#include "ControleConsole.h"
int main(int argc, char *argv[])
{
    std::string argument="";
    if (argc>1){argument = argv[1];}
    if (argument=="-C" || argument=="-c"){
        controleConsole();
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
