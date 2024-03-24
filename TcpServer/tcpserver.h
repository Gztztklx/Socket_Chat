#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QList>
#include <QString>
#include "mytcpsocket.h"
class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    TcpServer();
    static TcpServer& getInstance();
    void incomingConnection(qintptr socketDescriptor);
    void resend(const char*pername,PDU* pdu);//转发函数
    void listShow();
    void disconnect(char* name);
    int* getA();
    int* getB();
    int* getC();
    int* getD();
public slots:
    void deleteSocket(MyTcpSocket* mysocket);
private:
    QList<MyTcpSocket*> m_tcpSocketList;
    int a;
    int b;
    int c;
    int d;
};

#endif // TCPSERVER_H
