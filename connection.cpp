#include "connection.h"
#include "ui_connection.h"

Connection::Connection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Connection)
{
    ui->setupUi(this);
}

Connection::~Connection()
{
    delete ui;
}

void Connection::on_pushButton_clicked()
{

}

void Connection::on_pushDiscover_clicked()
{

}
