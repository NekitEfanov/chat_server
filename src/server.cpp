#include "server.h"
server::server(){}
 
server::~server(){}
 
void server::startServer() 
{
    if(this->listen(QHostAddress::Any, 60111))
    { 
        qDebug()<< "listening"; 
    }
    else
    { 
        qDebug()<<"Not listening"; 
    }
} 
  
void server::incomingConnection(qintptr socketDescriptor)
{
    clients[socketDescriptor] = new QTcpSocket(this);
    clients[socketDescriptor]->setSocketDescriptor(socketDescriptor);
    //////////////////////////////

     //////////////////////////////

    connect(
        clients[socketDescriptor],
        &QTcpSocket::readyRead,
        [this, socketDescriptor]() {
            this->sockReady(socketDescriptor);
        }
    );

    connect(
        clients[socketDescriptor],
        &QTcpSocket::disconnected,
        [this, socketDescriptor]() {
            this->sockDisc(socketDescriptor);
        }
    );

    qDebug() << socketDescriptor<< "Client connected";  
    QString chat_data = "";
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Cannot open file for reading"); 
    }
    QTextStream out(stdout);
    QTextStream in(&file); 
    while (!in.atEnd())
    { 
        chat_data = file.readAll();
        out << chat_data << endl;
    }
    file.close();

    clients[socketDescriptor]->write(chat_data.toUtf8());
} 
void server::sockReady(qintptr socketDescriptor)
{
    QByteArray buffer; 
    buffer.append(clients[socketDescriptor]->readAll());
    //////////////////////////////
   
    for (auto c : clients)
    {
        if (c.first != socketDescriptor)
        {
            c.second->write(buffer);
        }
    }

     //////////////////////////////
    if (file.open(QIODevice::Append))
    { 
        QTextStream out(&file);
        out << QString::fromStdString(buffer.toStdString());
    }
    else {
        qWarning("Could not open file");
    }
    file.close();

}
void server::sockDisc(qintptr socketDescriptor)
{
    qDebug() << "Disconnect " << socketDescriptor;
    clients[socketDescriptor]->deleteLater();
    clients.erase(clients.find(socketDescriptor));
}