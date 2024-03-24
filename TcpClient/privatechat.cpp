#include "privatechat.h"
#include "ui_privatechat.h"
#include "widget.h"

privateChat::privateChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::privateChat)
{
    ui->setupUi(this);
}

privateChat::~privateChat()
{
    delete ui;
}

privateChat &privateChat::getPrivateChat()
{
    static privateChat instance;
    return instance;
}

void privateChat::setPerName(QString data)
{
    perName = data;
    selfName = Widget::getWidget().getName();
}

void privateChat::updateMsg(const PDU *pdu)
{
    if(pdu == NULL){
        return;
    }
    QString strMsg = QString("【%1】 says: %2").arg(perName).arg((char*)(pdu->caMsg));
    ui->textEdit->append(strMsg);
}

void privateChat::on_pushButton_clicked()
{
    QString strMsg = ui->lineEdit->text();
    if(!strMsg.isEmpty()){
        PDU* pdu=mkPDU(strMsg.size()*4+1);
        pdu->uiMsgType = ENUM_MSG_TYPE_PRIVATR_CHAT_REQUEST;
        memcpy(pdu->caData,perName.toStdString().c_str(),32);//前面的是对方的名字
        memcpy(pdu->caData+32,selfName.toStdString().c_str(),32);//后面的是自己的名字
        strcpy((char*)pdu->caMsg,strMsg.toStdString().c_str());
        Widget::getWidget().getQtcpSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
        QString data = QString("%1 : says 【%2】 ").arg(strMsg).arg(selfName);
        ui->textEdit->append(data);
        ui->lineEdit->clear();
    }else{
        QMessageBox::information(this,"输入文本框","发送不能为空");
    }
}

