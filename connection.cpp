#include "connection.h"
#include "ui_connection.h"

#include <QDebug>

Connection::Connection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Connection)
{
    ui->setupUi(this);

    this->client_data.client_id = 0;
}

Connection::~Connection()
{
    delete ui;
}

void Connection::on_pushConnection_clicked()
{
    setServerConnected(true);
}

void Connection::on_pushDiscover_clicked()
{

}

void Connection::on_pushDisconnect_clicked()
{
    setServerConnected(false);
}

void Connection::setServerConnected(bool status)
{
    this->ui->pushConnection->setEnabled(!status);
    this->ui->pushDisconnect->setEnabled(status);

    if(status) {
        this->ui->labelIP_val->setText(hostAddress.toString());
        this->ui->labelSWVer_val->setText(QString(VER));
        this->ui->labelStatus_val->setText("<p style=\"color:green\"> <b> CONNECTED </b> </p>");
    } else {
        this->ui->labelIP_val->setText("Unknown");
        this->ui->labelSWVer_val->setText("Unknown");
        this->ui->labelStatus_val->setText("<p style=\"color:red\"> <b> DISCONNECTED </b> </p>");
    }
}

