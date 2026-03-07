#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QSqlDatabase>
#include<QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<QSqlDatabase::drivers();
}

MainWindow::~MainWindow()
{
    delete ui;
}
