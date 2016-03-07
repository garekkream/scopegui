#ifndef CONNECTION_H
#define CONNECTION_H

#include <QWidget>
#include <QtNetwork>
#include <QString>

#include "scopeproto/scope_msg_request.pb.h"
#include "scopeproto/scope_msg_response.pb.h"

#define MAX_BUFFER_SIZE         256
#define CLIENT_GUI_NAME         "scopeGUI"
#define CLIENT_GUI_SW_VERSION   VER

#define CLIENT_TIMEOUT_MS       1000

namespace Ui {
class Connection;
}

class Connection : public QWidget
{
    Q_OBJECT

public:
    explicit Connection(QWidget *parent = 0);
    ~Connection();

    void send_data(scopeMsgClientReq_scopeMsgIdReq id, int flags = 0x00, char *data = NULL, int size = 0);

private slots:
    void on_pushDiscover_clicked();
    void on_pushDisconnect_clicked();
    void on_pushConnection_clicked();

private:
    Ui::Connection *ui;

    QTcpSocket socket;
    QHostAddress hostAddress;

    QString clientIP;
    QString serverIP;
    qint64 serverPort;

    struct __client_data {
        char client_id;
        char name[20];
        char sw_version[20];
    } client_data;

    void setServerConnected(bool status);
    void setConnectionInfo(scopeMsgServerRes *response);
    bool waitForResponse(int id, scopeMsgServerRes *response);
};

#endif // CONNECTION_H
