#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTcpSocket>
#include <QDebug>
#include "qstring.h"
#include <QStringList>
#include "protocol.h"//协议
#include "opedb.h"

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    ~MyTcpSocket();
    
    QString getName();
    void registRequest(PDU* pdu);//注册请求
    void loginRequest(PDU* pdu);//登录请求
    void createRoom(PDU* pdu);//创建房间请求
    QString getAddress();//给创建的房间分配一个地址
    void flushRoom(PDU* pdu);//刷新房间，返回一个字符串链表
    void leaveRoom(PDU* pdu);//离开房间
    void dropRoom(PDU* pdu);//销毁房间
    bool getIntoRoom(PDU* pdu);//进入房间
    void flushMemberInRoom(PDU* pdu);//刷新房间内部成员列表
    void noticeEverybodyFlushList(PDU* pdu);//通知房间内的每一个人房间列表发生改变
    void speak(PDU* pdu);//在房间里发言
    void getGold(PDU* pdu);//获取金币数量
    void recharge(PDU* pdu);//充值
    void giveFlower(PDU* pdu);//赠送鲜花
    void giveRocket(PDU* pdu);//赠送火箭
    void giveAircraft(PDU* pdu);//赠送航母
    void privateChatRequest(PDU* pdu);//私聊请求
    void destory(PDU* pdu);
 
    
signals:
    void offline(MyTcpSocket* mysocket);
public slots:
    void recvMsg();//接收发送过来的信息
    void clientOffline();//用来发送客户端下线的信号
    
    

private:
    QString m_strName;

};

#endif // MYTCPSOCKET_H
