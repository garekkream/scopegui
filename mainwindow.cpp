#include <QUiLoader>
#include <QFile>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(this->size());

    addUiWidget(":/forms/info.ui", "Info");
    addUiWidget(":/forms/connection.ui", "Connection");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addUiWidget(const char *uiFile, QString name)
{
    QUiLoader loader;
    QFile file(uiFile);
    QWidget *w;

    file.open(QFile::ReadOnly);
    w = loader.load(&file, NULL);
    ui->tabWidget->addTab(w, name);

    file.close();
}
