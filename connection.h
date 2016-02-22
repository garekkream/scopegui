#ifndef CONNECTION_H
#define CONNECTION_H

#include <QWidget>

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
    void on_pushButton_clicked();
    void on_pushDiscover_clicked();

private:
    Ui::Connection *ui;
};

#endif // CONNECTION_H
