#include "tcpserver.h"
#include "mytcpsocket.h"
#include <string.h>
TcpServer::TcpServer()
{
    a = 224;
    b = 0;
    c = 2;
    d = 0;
    QString data = QString("%1.%2.%3.%4").arg(a).arg(b).arg(c).arg(d);
 //   qDebug()<<"data:"<<data;
    qDebug()<<"a:"<<&a;
    qDebug()<<"b:"<<&b;
    qDebug()<<"c:"<<&c;
    qDebug()<<"d:"<<&d;
}


TcpServer &TcpServer::getInstance()

{

    static TcpServer instance;
    return instance;
}

void TcpServer::incomingConnection(qintptr socketDescriptor)//该函数属于自动调用，在客户端链接服务器的时候
{
//    qDebug()<< "new client connect";
    MyTcpSocket* pTcpSocket = new MyTcpSocket;//产生一个指针，用指针指向new出来的一个对象
    pTcpSocket->setSocketDescriptor(socketDescriptor);
    m_tcpSocketList.append(pTcpSocket);//将套接字加入链表
    
   connect(pTcpSocket,SIGNAL(offline(MyTcpSocket*)),this,SLOT(deleteSocket(MyTcpSocket*)));
}

void TcpServer::resend(const char *pername, PDU *pdu)//转发函数
{
//    qDebug()<<"进入了转发函数";
    if(pername == NULL || pdu == NULL){
//        qDebug()<<"name = NULL";
        return ;
        }
    QString strName = pername;
    for(int i=0;i<m_tcpSocketList.size();i++){
        if(strName == m_tcpSocketList.at(i)->getName()){
            size_t ret = m_tcpSocketList.at(i)->write((char*)pdu,pdu->uiPDULen);
   //         qDebug()<<"转发对象："<<m_tcpSocketList.at(i)->getName();
  //          qDebug()<<"ret:"<<ret;
            break;
        }
    }
}

void TcpServer::listShow()
{
    for(int i=0;i<m_tcpSocketList.size();i++){
//       qDebug()<<m_tcpSocketList.at(i);
    }
}

void TcpServer::disconnect(char *name)
{
//    qDebug()<<"进入了disconnect函数";
    if(name == NULL){
        return;
    }
    QString strName = name;
    for(int i=0;i<m_tcpSocketList.size();i++){
        if(strName == m_tcpSocketList.at(i)->getName()){
            deleteSocket(m_tcpSocketList.at(i));
            break;
        }
    }
}

void TcpServer::deleteSocket(MyTcpSocket *mysocket)
{
    qDebug()<<"进入了删除套接字的函数" << m_tcpSocketList.size();
    QList<MyTcpSocket*>::iterator iter = m_tcpSocketList.begin();
//    for(;iter!=m_tcpSocketList.end();iter++){
//        if(mysocket == *iter){
//            //(*iter)->deleteLater();
            
//            //delete* iter;
            
//            //*iter=NULL;
//            m_tcpSocketList.erase(iter);
//            iter = NULL;
//            break;
//        }
//    }
    MyTcpSocket *tmp=NULL;
    for(;iter!=m_tcpSocketList.end();){
        if(mysocket == *iter){
            tmp = *iter;
            iter = m_tcpSocketList.erase(iter);
            
            break;
        }
        else
        {
            iter++;
        }
    }
    qDebug()<<"--》" << m_tcpSocketList.size();
    if (NULL!=tmp)
    {
        sleep(1);
        delete tmp;
        tmp=NULL;
    }
    
    qDebug()<<"开始打印链表";
//    for(int i=0;i<m_tcpSocketList.size();i++){
//        if(m_tcpSocketList.at(i) == mysocket){
//            delete m_tcpSocketList.at(i);
//        }
//    }
    qDebug()<<"链表打印结束";
    qDebug()<<"套接字已删除";
        for(int i=0;i<m_tcpSocketList.size();i++){
            qDebug()<<m_tcpSocketList.at(i)->getName()<<"____";
        }
   qDebug()<<"该函数马上退出";
}

int *TcpServer::getA()
{
    return &a;
}

int *TcpServer::getB()
{
    return &b;
}

int *TcpServer::getC()
{
    return &c;
}

int *TcpServer::getD()
{
    return &d;
}


