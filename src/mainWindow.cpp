#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include "TuileItem.h"
#include "Tuile.h"
#include "Hexagone.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);

    auto* scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    auto* h1 = new Hexagone( 0,  0,  0, TypeHex::PHabitation);
    auto* h2 = new Hexagone(0, -1,  0, TypeHex::Carriere);
    auto* h3 = new Hexagone( -1, 1 , 0, TypeHex::Carriere);
    auto* h4 = new Hexagone( 1, 0, 0, TypeHex::Carriere);

    Tuile tuile(h1,h2,h3,h4);
    auto* t = new TuileItem(tuile);
    scene->addItem(t);
    t->setPos(0, 0);
    t->setFlag(QGraphicsItem::ItemIsMovable, false);
    t->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);



    h1 = new Hexagone( 0,  0,  0, TypeHex::PHabitation);
    h2 = new Hexagone(+1, -1,  0, TypeHex::PCaserne);
    h3 = new Hexagone( 0, -1, 0, TypeHex::Carriere);

    tuile=Tuile(h1, h2, h3);
    auto* t2 = new TuileItem(tuile);
    scene->addItem(t2);
    t2->setPos(10, 10);
    h1 = new Hexagone( 0,  0,  0, TypeHex::PHabitation);
    h2 = new Hexagone(+1, -1,  0, TypeHex::Marche);
    h3 = new Hexagone( 0, -1, 0, TypeHex::Carriere);

    tuile=Tuile(h1, h2, h3);
    auto* t3 = new TuileItem(tuile);
    scene->addItem(t3);
    t3->setPos(10, 10);
    h1 = new Hexagone( 0,  0,  0, TypeHex::PMarche);
    h2 = new Hexagone(+1, -1,  0, TypeHex::Carriere);
    h3 = new Hexagone( 0, -1, 0, TypeHex::Carriere);

    tuile=Tuile(h1, h2, h3);
    auto* t4 = new TuileItem(tuile);
    scene->addItem(t4);
    t4->setPos(10, 10);
    scene->setSceneRect(scene->itemsBoundingRect().adjusted(-60, -60, 60, 60));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QApplication::quit();
}




