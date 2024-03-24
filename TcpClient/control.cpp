#include "control.h"
#include "ui_control.h"
#include "widget.h"
#include <QDebug>
#include "boardcast.h"
#include <QMessageBox>
Control::Control(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Control)
{
    ui->setupUi(this);
    
    t_3 = new thread_3(this);
    t_3->start();
    QObject::connect(t_3,SIGNAL(flush()),this,SLOT(on_pushButton_clicked()));
}

Control::~Control()
{
    delete ui;
    t_3->terminate();
}

Control &Control::getIntance()
{
    static Control instance;
    return instance;
}

void Control::setName()
{
    strLoginName=Widget::getWidget().getName();
    ui->label_2->setText(strLoginName);
}

void Control::on_pushButton_2_clicked()//创建房间
{
    
    PDU* pdu=mkPDU(0);
    pdu->uiMsgType=ENUM_MSG_TYPE_CREATE_ROOM_REQUEST;
    strcpy(pdu->caData,strLoginName.toStdString().c_str());
    Widget::getWidget().getQtcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
    qDebug()<<"创建房间的请求已经发送";
}

void Control::createRoomRespond(PDU *pdu)//创建房间的回复
{
    qDebug()<<"收到了创建房间的回复";
    if(strcmp(pdu->caData,CREATE_ROOM_FAILED) == 0){
        QMessageBox::information(this,"创建房间","创建房间失败");
    }else if(strcmp(pdu->caData,CREATE_ROOM_OK) == 0){
       // QMessageBox::information(this,"创建房间","创建房间成功");
        QString str = QString("%1").arg((char*)(pdu->caMsg));
        qDebug()<<"str:"<<str;
        addr.clear();
        qDebug()<<"addr已经清空";
        addr = QHostAddress(str);
        qDebug()<<addr;
        boardcast::getInterance().show();
        qDebug()<<"窗口出来";
        boardcast::getInterance().useOnCameraButton();//允许使用摄像机
        qDebug()<<"允许使用主播特权";
        boardcast::getInterance().joinDifferentGroup();//加入不同的组播组
        qDebug()<<"加入组播组";
    }
}

void Control::flushRoomRespond(PDU *pdu)//刷新房间的回复
{
    if(pdu == NULL){
        return;
    }
    ui->listWidget->clear();
    uint uiSize = pdu->uiMsgLen/20;
    char data[20] = {'\0'};
    for(uint i=0;i<uiSize;i++){
        memcpy(data,(char*)(pdu->caMsg)+i*20,20);
        ui->listWidget->addItem(data);
    }
}

QHostAddress Control::getAddr()
{
    return addr;
}

void Control::deleteRoomRequest()//离开房间，此时通知服务器，对房间进行处理
{
   // qDebug()<<"进入了deleteRoomRequest函数";
    PDU* pdu=mkPDU(0);
    pdu->uiMsgType=ENUM_MSG_TYPE_LEAVE_ROOM_REQUEST;
    strcpy(pdu->caData,strLoginName.toStdString().c_str());
    QString add = addr.toString();
   // qDebug()<<add;
    strcpy(pdu->caData+32,add.toStdString().c_str());
    Widget::getWidget().getQtcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
    addr.clear();
}

void Control::getIntoRoomRespond(PDU *pdu)
{
    if(strcmp(pdu->caData,GET_INTO_OK) == 0){
        char str[32] = {'\0'};
        memcpy(str,pdu->caMsg,32);
       // qDebug()<<"str"<<str;
        addr.clear();
        addr = QHostAddress(str);
       // qDebug()<<"addr"<<addr;
        boardcast::getInterance().show();
        boardcast::getInterance().closeOnCameraButton();//不允许使用摄像机
        boardcast::getInterance().joinDifferentGroup();//加入不同的组播组
    }else if(strcmp(pdu->caData,GET_INTO_FAILED) == 0){
        QMessageBox::information(this,"进入房间","重复登录");
    }
}



void Control::on_pushButton_clicked()//刷新房间按钮
{
    PDU* pdu=mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_ROOM_REQUEST;
    strcpy(pdu->caData,strLoginName.toStdString().c_str());
    Widget::getWidget().getQtcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

void Control::on_pushButton_3_clicked()//进入房间按钮
{
    if(ui->listWidget->currentItem() == NULL){
        QMessageBox::information(this,"进入房间","请选择房间");
        return;
    }
    PDU* pdu=mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_GET_INTO_ROOM_REQUEST;
    strcpy(pdu->caData,strLoginName.toStdString().c_str());//自己的名字
    
    
    QString str = ui->listWidget->currentItem()->text();

    strcpy(pdu->caData+32,str.toStdString().c_str());
    Widget::getWidget().getQtcpSocket().write((char*)pdu,pdu->uiPDULen);
    //qDebug()<<"进入房间请求已发送";
 //   qDebug()<"类型："<<pdu->uiMsgType;
    free(pdu);
    pdu = NULL;
}

void Control::on_pushButton_4_clicked()//注销
{
    PDU* pdu=mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_DESTORY_REQUEST;//注销
    strcpy(pdu->caData,strLoginName.toStdString().c_str());
    Widget::getWidget().getQtcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

void Control::on_pushButton_5_clicked()
{
    Widget::getWidget().getQtcpSocket().disconnectFromHost();
}
