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
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
     
    connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady())); 
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));

    qDebug() << socketDescriptor<< "Client connected";  
    QString chat_data = "";
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Cannot open file for reading"); 
    }
    QTextStream out(stdout);
    QTextStream in(&file); 
    while (!in.atEnd())
    { 
        chat_data = in.readLine(); 
        out << chat_data << endl;
    }
    file.close();

    socket->write(chat_data.toUtf8());
} 
void server::sockReady()
{
    QByteArray buffer; 
    buffer.append(socket->readAll());
    if (file.open(QIODevice::Append))
    { 
        QTextStream out(&file);
        out << QString::fromStdString(buffer.toStdString());
        out << endl;
    }
    else {
        qWarning("Could not open file");
    }
    file.close();
}
void server::sockDisc()
{
    qDebug() << "Disconnect";
    socket->deleteLater();
}
