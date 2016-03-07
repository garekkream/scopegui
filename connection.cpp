#include <QMessageBox>
#include <QDebug>

#include "connection.h"
#include "ui_connection.h"


Connection::Connection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Connection)
{
    ui->setupUi(this);

    this->client_data.client_id = 0;
    memcpy(this->client_data.name, CLIENT_GUI_NAME, strlen((const char *)CLIENT_GUI_NAME));
    memcpy(this->client_data.sw_version, CLIENT_GUI_SW_VERSION, strlen((const char *)CLIENT_GUI_SW_VERSION));

    foreach (const QHostAddress& address, QNetworkInterface::allAddresses()) {
        if(address.protocol() == QAbstractSocket::IPv4Protocol) {
            this->clientIP = address.toString();
            this->ui->labelClientIP_val->setText(address.toString());
        }
    }

    this->ui->labelClientSWVer_val->setText(QString(CLIENT_GUI_SW_VERSION));

    setServerConnected(false);
}

Connection::~Connection()
{
    google::protobuf::ShutdownProtobufLibrary();

    delete ui;
}

void Connection::send_data(scopeMsgClientReq_scopeMsgIdReq id, int flags, char *data, int size)
{
    char payload[MAX_BUFFER_SIZE] = {0x00};
    char buffer[MAX_BUFFER_SIZE] = {0x00};
    scopeMsgClientReq request;

    request.set_msg_id(id);
    request.set_device_id((int32_t)this->client_data.client_id);

    if(flags != 0x00)
        request.set_payload_flags(flags);

    if((data != NULL) && (size > 0)) {
        memcpy(payload, data, size + 1);
        request.set_payload_data(payload, size + 1);
    }

    request.SerializePartialToArray((void *)buffer, request.ByteSize());

    socket.abort();
    socket.connectToHost(this->hostAddress, this->serverPort);

    socket.write(buffer, request.ByteSize());
}

void Connection::on_pushConnection_clicked()
{
    int32_t flags = 0x00;
    char buffer[MAX_BUFFER_SIZE] = {0x00};
    int size = 0;
    int index = 0;
    scopeMsgServerRes response;

    this->serverIP = this->ui->lineIP->text();
    this->serverPort = this->ui->linePort->text().toInt();

    hostAddress.setAddress(this->serverIP);
    if(hostAddress.protocol() != QAbstractSocket::IPv4Protocol) {
        QMessageBox::warning(this, "Error!", "Invalid IP address!");
        return;
    }

    size = strlen((const char *)this->client_data.name);
    flags |= scopeMsgClientReq_scopeRegisterFlagsReq_CLIENT_NAME;
    buffer[index++] = size;
    memcpy(&(buffer[index]), this->client_data.name, size);
    index += size;

    flags |= scopeMsgClientReq_scopeRegisterFlagsReq_CLIENT_SW_VER;
    buffer[index++] = strlen((const char *)this->client_data.sw_version);
    memcpy(&(buffer[index]), this->client_data.sw_version, strlen((const char *)this->client_data.sw_version));
    size += strlen((const char *)this->client_data.sw_version);

    size += 1;

    send_data(scopeMsgClientReq_scopeMsgIdReq_SCOPE_MSGID_REGISTER_REQ, flags, buffer, size);

    memset(buffer, 0x00, MAX_BUFFER_SIZE);

    if(waitForResponse(scopeMsgServerRes_scopeMsgIdRes_SCOPE_MSGID_REGISTER_RES, &response)) {
        setServerConnected(true);
        setConnectionInfo(&response);
    } else {
        QMessageBox::warning(this, "Error!", "No response!");
    }

//    while(this->socket.waitForReadyRead(CLIENT_TIMEOUT_MS)) {
//        int size = 0;
//        scopeMsgServerRes response;

//        size = socket.read(buffer, MAX_BUFFER_SIZE - 1);
//        response.ParseFromArray(buffer, size);

//        if(response.id() == scopeMsgServerRes_scopeMsgIdRes_SCOPE_MSGID_REGISTER_RES) {
//            setConnectionInfo(&response);
//            setServerConnected(true);
//        } else {
//            QMessageBox::warning(this, "Error!", "Wrong message id!");
//        }
//    }
}

void Connection::on_pushDiscover_clicked()
{
}

void Connection::on_pushDisconnect_clicked()
{
    scopeMsgServerRes response;

    send_data(scopeMsgClientReq_scopeMsgIdReq_SCOPE_MSGID_UNREGISTER_REQ);

    if(waitForResponse(scopeMsgServerRes_scopeMsgIdRes_SCOPE_MSGID_UNREGISTER_RES, &response)) {
        setServerConnected(false);

        this->socket.disconnectFromHost();
    } else {
        QMessageBox::warning(this, "Error!", "No response!");
    }
}

bool Connection::waitForResponse(int id, scopeMsgServerRes *response)
{
    while(this->socket.waitForReadyRead(CLIENT_TIMEOUT_MS)) {
        int size = 0;
        char buffer[MAX_BUFFER_SIZE] = {0x00};

        size = socket.read(buffer, MAX_BUFFER_SIZE - 1);
        response->ParseFromArray(buffer, size);

        if(response->id() == id) {
            return true;
        } else {
            QMessageBox::warning(this, "Error!", "Wrong msg ID!");
            return false;
        }
    }

    return false;
}

void Connection::setServerConnected(bool status)
{
    this->ui->pushConnection->setEnabled(!status);
    this->ui->pushDisconnect->setEnabled(status);

    if(status) {
        this->ui->labelServerIP_val->setText(this->socket.peerAddress().toString());
        this->ui->labelStatus_val->setText("<p style=\"color:green\"><b>CONNECTED</b></p>");
    } else {
        this->ui->labelServerSWVer_val->setText("Unknown");
        this->ui->labelServerIP_val->setText("Unknown");
        this->ui->labelClientDevID_val->setText("Unknown");
        this->ui->labelStatus_val->setText("<p style=\"color:red\"><b>DISCONNECTED</b></p>");
    }
}

void Connection::setConnectionInfo(scopeMsgServerRes *response)
{
    char buffer[MAX_BUFFER_SIZE] = {0x00};

    memcpy(buffer, response->payload_data().data(), response->payload_data().length());

    if(response->payload_flags() & scopeMsgServerRes_scopeRegisterFlagsRes_SERVER_SW_VER) {
        char sw_ver[MAX_BUFFER_SIZE] = {0x00};

        memcpy(sw_ver, &(buffer[1]), buffer[0]);

        this->ui->labelServerSWVer_val->setText(sw_ver);
    }

    this->ui->labelServerIP_val->setText(this->socket.peerAddress().toString());
    this->ui->labelClientDevID_val->setText(QString::number(response->device_id()));

    this->client_data.client_id = response->device_id();
}

