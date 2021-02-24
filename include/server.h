#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QTextStream>
#include <map>
#include <QTextCodec>
#include <iostream>
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
    QByteArray Version = "0002";
    QByteArray key = "";
    QByteArray Key_update = "F5kGIi65k3tyOGp6y5PghUPDATE";
    QFile key_file = "Parameters/key.txt";
    QFile file_exe = "Update/Qt5Client.exe";
    QByteArray file_update;
    QString file_size = "";
private:
    QTextStream* in;
    QTextStream* out;
public slots:
    void startServer();
    virtual void incomingConnection(qintptr socketDescriptor);
    void sockReady(qintptr socketDescriptor);
    void sockDisc(qintptr socketDescriptor);
};

#endif // SERVER_H
