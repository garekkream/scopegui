#include <QUiLoader>
#include <QFile>

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connectionWidget = new Connection;
    infoWidget = new Info;

    this->setFixedSize(this->size());

    ui->tabWidget->addTab(infoWidget, "Info");
    ui->tabWidget->addTab(connectionWidget, "Connection");
}

MainWindow::~MainWindow()
{
    delete ui;
}
