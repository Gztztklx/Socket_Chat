#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QMessageBox>
#include <QByteArray>
#include <QDebug>
#include "tcpserver.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    loadConfig();//获取ip地址和端口号
    TcpServer::getInstance().listen(QHostAddress(m_strIp),m_usPort);

}

Widget::~Widget()
{
    delete ui;
}
void Widget::loadConfig()
{
    QFile file(":/new/prefix1/server.txt");
    if(file.open(QIODevice::ReadOnly))//打开文件
    {
        QByteArray baData = file.readAll();//读取数据
        QString strData = baData.toStdString().c_str();
        file.close();
        strData.replace("\r\n"," ");
        QStringList strList = strData.split(" ");
        m_strIp = strList.at(0);
        m_usPort = strList.at(1).toUShort();
  //      qDebug()<<"ip"<<m_strIp<<"port"<<m_usPort;
    }else{
        QMessageBox::information(this,"打开文件","文件打开失败");
    }
}

void Widget::address()
{
    
}
