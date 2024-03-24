#include "mytcpserver.h"
#include <QDebug>
MyTcpServer::MyTcpServer()
{
    
}

MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}
/*
void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"new client connect";
    MyTcpSocket* pTcpSocket = new MyTcpSocket;
    pTcpSocket->setSocketDescriptor(socketDescriptor);
    m_tcpSocketList.append(pTcpSocket);//将套接字指针加入链表
    connect(pTcpSocket,SIGNAL(offline(MyTcpSocket*)),this,SLOT(deleteSocket(MyTcpSocket*)));
}

void MyTcpServer::deleteSocket(MyTcpSocket *mysocket)
{
    
}
*/
