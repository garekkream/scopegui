#ifndef CONNECTION_H
#define CONNECTION_H

#include <QWidget>
#include <QtNetwork>

namespace Ui {
class Connection;
}

class Connection : public QWidget
{
    Q_OBJECT

public:
    explicit Connection(QWidget *parent = 0);
    ~Connection();

private slots:
    void on_pushDiscover_clicked();
    void on_pushDisconnect_clicked();
    void on_pushConnection_clicked();

private:
    Ui::Connection *ui;

    QTcpSocket socket;
    QHostAddress hostAddress;

    struct __client_data {
        char client_id;
        char name[20];
        char sw_version[20];
    } client_data;

    void setServerConnected(bool status);
};

#endif // CONNECTION_H
