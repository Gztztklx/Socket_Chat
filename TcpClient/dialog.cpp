#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include "control.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    selfName = Widget::getWidget().getName();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_4_clicked()//鲜花
{
    //qDebug()<<"客户想要向主播赠送鲜花";
    this->hide();
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_GIVE_FLOWER_REQUEST;
    addr = Control::getIntance().getAddr();
    QString add = addr.toString();
    strcpy(pdu->caData,selfName.toStdString().c_str());
    strcpy(pdu->caData+32,add.toStdString().c_str());
    Widget::getWidget().getQtcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=NULL;
   // qDebug()<<"主播想要赠送鲜花的请求已经被发送";
}

void Dialog::on_pushButton_5_clicked()//火箭
{
    //qDebug()<<"客户想要向主播赠送火箭";
    this->hide();
    addr = Control::getIntance().getAddr();
    QString add = addr.toString();
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_GIVE_ROCKET_REQUEST;
    strcpy(pdu->caData,selfName.toStdString().c_str());
    strcpy(pdu->caData+32,add.toStdString().c_str());
    Widget::getWidget().getQtcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=NULL;
   // qDebug()<<"主播想要赠送火箭的请求已经被发送";
}

void Dialog::on_pushButton_6_clicked()//航母
{
  //  qDebug()<<"客户想要向主播赠送航母";
    this->hide();
    addr = Control::getIntance().getAddr();
    QString add = addr.toString();
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_GIVE_AIRCRAFT_REQUEST;
    qDebug()<<selfName;
    strcpy(pdu->caData,selfName.toStdString().c_str());
    strcpy(pdu->caData+32,add.toStdString().c_str());
    Widget::getWidget().getQtcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=NULL;
   // qDebug()<<"主播想要赠送航母的请求已经被发送";
}
