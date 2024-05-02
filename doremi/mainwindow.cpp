#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "test.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::play() {
    player = new MediaPlayer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    play();
    Test *t = new Test();
    t->Test2();
}

