#include "server.h"
server::server(){}
 
server::~server(){}
 
void server::startServer() 
{
    if (!key_file.open(QIODevice::ReadOnly)) {
        qWarning("Cannot open file for reading");
    }
    QTextStream in(&key_file);
    while (!in.atEnd())                                    //set key
    {
        key = key_file.readAll();
    }
    key_file.close();

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

    //////////////////////////////////////////

    clients[socketDescriptor] = new QTcpSocket(this);
    clients[socketDescriptor]->setSocketDescriptor(socketDescriptor);

    //////////////////////////////////////////

    clients[socketDescriptor]->waitForReadyRead(2500);

    QByteArray key_client = 0;
    key_client.clear();
    key_client.append(clients[socketDescriptor]->readAll());
    qDebug() << key_client;
    if (Key_update == key_client)
    {
        qDebug() << socketDescriptor << " update connected and started";
        //
        if (!file_exe.open(QIODevice::ReadOnly))
        {
            qDebug() << "Error open file_exe";
        }
        else
        {
             file_update = file_exe.readAll();
             file_exe.close();
             file_size.clear();
             file_size = file_update.size();
             clients[socketDescriptor]->write(file_size.toUtf8());
             clients[socketDescriptor]->waitForReadyRead();
             clients[socketDescriptor]->write(file_update);
              qDebug() << socketDescriptor << "File sent successfully";
              qDebug() << "Disconnect update client " << socketDescriptor;
            //clients[socketDescriptor]->deleteLater();
            //clients.erase(clients.find(socketDescriptor));
             file_update.clear();
        }
    }
    else if (key_client != key)
    {
        qDebug() << "Disconnect / Not a valid key" << socketDescriptor;
        clients[socketDescriptor]->deleteLater();
        clients.erase(clients.find(socketDescriptor));
    }
    else
    {
        //////////////////////////////////////////

        key_client.clear();
        qDebug() << socketDescriptor << "Client connected";
        QString chat_data = "";
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning("Cannot open file for reading");
        }
        QTextStream in(&file);                                               //getting the chat history from the database and sending it
        while (!in.atEnd())
        {
            chat_data = file.readAll();
        }
        file.close();

        clients[socketDescriptor]->write(Version + chat_data.toUtf8());

        //////////////////////////////////////////

        connect(
            clients[socketDescriptor],
            &QTcpSocket::readyRead,
            [this, socketDescriptor]() {
                this->sockReady(socketDescriptor);
            }
        );
                                                                      //initialization signal socket
        connect(
            clients[socketDescriptor],
            &QTcpSocket::disconnected,
            [this, socketDescriptor]() {
                this->sockDisc(socketDescriptor);
            }
        );

        //////////////////////////////////////////
    }
} 
void server::sockReady(qintptr socketDescriptor)
{
    //////////////////////////////////////////

    QByteArray buffer; 
    buffer.append(clients[socketDescriptor]->readAll());
   
    for (auto c : clients)
    {
        if (c.first != socketDescriptor)                             //Sending messages to other participants
        {
            c.second->write(buffer);
        }
    }

    //////////////////////////////////////////

    if (file.open(QIODevice::Append))
    { 
        QTextStream out(&file);
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
        QString fileData = QString::fromLocal8Bit(buffer);
        out << (QString::fromStdString(fileData.toStdString())) + "\n";                          //save message in database
    }
    else {
        qWarning("Could not open file");
    }
    file.close();

    //////////////////////////////////////////
}
void server::sockDisc(qintptr socketDescriptor)
{
    qDebug() << "Disconnect " << socketDescriptor;
    clients[socketDescriptor]->deleteLater();
    clients.erase(clients.find(socketDescriptor));
}