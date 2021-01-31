#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QTextStream>

class server : public QTcpServer
{
    Q_OBJECT
public:
    server();
    ~server();
    QTcpSocket *socket;
    QByteArray Data;
    QString nameclient;
    QFile file = "chat.txt";
public slots:
    void startServer();
    virtual void incomingConnection(qintptr socketDescriptor);
    void sockReady();
    void sockDisc();
};

#endif // SERVER_H
