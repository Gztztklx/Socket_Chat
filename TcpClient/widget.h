#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFile>
#include <QTcpSocket>
#include <QListWidget>
#include "protocol.h"  
#include <QByteArray>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void loadConfig();//加载资源文件，连接客户端
    static Widget& getWidget();
    QString getName();
    QTcpSocket& getQtcpSocket();
    void registRespond(PDU* pdu);
    void loginRespond(PDU* pdu);
    void disconnectRespond(PDU* pdu);
    void createRoomRespond(PDU* pdu);
    void flushRoomRespond(PDU* pdu);
    void getIntoRoomRespond(PDU* pdu);
    void flushMemberInRoomRespond(PDU* pdu);
    void privateCharQuest(PDU* pdu);
    
    
    
    
public slots:
    void showConnect();
    void recvMsg();


    
private slots:
    void on_pushButton_2_clicked();
    
    void on_pushButton_clicked();
    
private:
    Ui::Widget *ui;
    QString m_strIP;//客户端连接服务器时的ip地址
    quint16 m_usPort;//端口号
    QString m_strLoginName;//登录时自己的用户名
    QTcpSocket m_tcpSocket;//socket套接字，通过套接字连接服务器，和服务器进行数据交互
    //static Widget& instance;
};

#endif // WIDGET_H
