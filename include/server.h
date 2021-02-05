#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QTextStream>
#include <map>
#include <QTextCodec>
class server : public QTcpServer
{
    Q_OBJECT
public:
    server();
    ~server();
    std::map<qintptr, QTcpSocket*> clients;
    QTcpSocket *socket;
    QByteArray Data;
    QString nameclient;
    QFile file = "chat.txt";
public slots:
    void startServer();
    virtual void incomingConnection(qintptr socketDescriptor);
    void sockReady(qintptr socketDescriptor);
    void sockDisc(qintptr socketDescriptor);
};

#endif // SERVER_H
