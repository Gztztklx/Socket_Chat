#include "widget.h"
#include "ui_widget.h"
#include <QByteArray>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>
#include "boardcast.h"
#include "privatechat.h"
#include "control.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    resize(207,20);
    loadConfig();//调用这个函数，获取地址和端口号
    
    m_tcpSocket.connectToHost(QHostAddress(m_strIP),m_usPort);//用ip地址和端口号去链接服务器
    connect(&m_tcpSocket,SIGNAL(connected()),this,SLOT(showConnect()));//如果客户端收到连接服务器成功的信号，就跳出一个小窗口
    connect(&m_tcpSocket,SIGNAL(readyRead()),this,SLOT(recvMsg()));//如果客户端读取到了服务器发送来的数据，就转到数据接收函数处理
}

Widget::~Widget()
{
    delete ui;
}

void Widget::loadConfig()
{
    QFile file(":/new/prefix1/client.txt");
    //qDebug()<<"调用config函数";
    if(file.open(QIODevice::ReadOnly)){//只读方式打开文件
        //qDebug()<<"成功打开文件";
        QByteArray baData = file.readAll();//读取数据
        QString strData = baData.toStdString().c_str();//转换成字符串
        file.close();//关闭文件
        //qDebug()<<strData;//打印出来之后发现ip地址和端口号之间有一个"\r\n"
        strData.replace("\r\n"," ");//用空格替换\r\n
       // qDebug()<<strData;
        QStringList strList = strData.split(" ");//用空格对该字符串进行分割
        m_strIP = strList.at(0);//ip地址
        m_usPort = strList.at(1).toUShort();
        //qDebug()<<"ip"<<m_strIP<<"port"<<m_usPort;
    }
}

Widget &Widget::getWidget()
{
    static Widget instance;
    return instance;
}

QString Widget::getName()
{
    //qDebug()<<"调用了getName函数";
    //qDebug()<<"m_strLoginName:"<<m_strLoginName;
    return m_strLoginName;
    
}

QTcpSocket &Widget::getQtcpSocket()
{
    return m_tcpSocket;
}

void Widget::showConnect()
{
    QMessageBox::information(this,"连接服务器","服务器连接成功");
}

void Widget::recvMsg()
{
   // qDebug()<<"进入了接收消息类型的函数";
    
    //qDebug()<<m_tcpSocket.bytesAvailable();//获取发送来的数据的大小，有可能会出错
   
    uint uiPDULen = 0;
    m_tcpSocket.read((char*)&uiPDULen,sizeof(uint));//先收取四个字节，获取此次发送来的数据总的大小
    uint uiMsgLen = uiPDULen-sizeof(PDU);//实际消息的长度=总的大小-结构体的大小
    PDU *pdu = mkPDU(uiMsgLen);
    m_tcpSocket.read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));
    // qDebug()<<"uiMsgType:"<<pdu->uiMsgType;
    switch (pdu->uiMsgType){
    case ENUM_MSG_TYPE_REGIST_RESPOND: //1 注册
    {
        registRespond(pdu);
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_RESPOND: //2 登录
    {
        loginRespond(pdu);
        break;
    }
    case ENUM_MSG_TYPE_DISCONNECT_RESPOND: //3
    {
        disconnectRespond(pdu);
        break;
    }
    case ENUM_MSG_TYPE_CREATE_ROOM_RESPOND: //4 创造房间
    {
        createRoomRespond(pdu);
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_ROOM_RESPOND: //5 刷新房间
    {
        flushRoomRespond(pdu);
        break;
    }
    case ENUM_MSG_TYPE_GET_INTO_ROOM_RESPOND: //6 进入房间
    {
        getIntoRoomRespond(pdu);
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_MEMBER_IN_ROOM_RESPOND: //7 刷新房间内部的成员列表
    {
     
        flushMemberInRoomRespond(pdu);
    }
    case ENUM_MSG_TYPE_SPEAK_RESPOND://8 发言
    {
        boardcast::getInterance().speakRespond(pdu); 
        break;
    }
    case ENUM_MSG_TYPE_GET_GOLD_RESPOND: //9 获得金币
    {
        boardcast::getInterance().setgoldCoin(pdu); 
        break;
    }
    case ENUM_MSG_TYPE_RECHARGE_RESPOND: //10 充值
    {
        boardcast::getInterance().rechargeRespond(pdu); 
        break;
    }
    case ENUM_MSG_TYPE_GIVE_FLOWER_RESPOND: //11
    {
        
        boardcast::getInterance().giveFlowerRespond(pdu);
        break;
    }
    case ENUM_MSG_TYPE_GIVE_ROCKET_RESPOND://12
    {
      
        boardcast::getInterance().giveRocketRespond(pdu); 
        break;
    }
    case ENUM_MSG_TYPE_GIVE_AIRCRAFT_RESPOND://13
    {
      
        qDebug()<<"判断出类型为赠送航母返回";
        boardcast::getInterance().giveAircraftRespond(pdu);
        break;
    }
    case ENUM_MSG_TYPE_PRIVATR_CHAT_REQUEST: //私聊请求 14
    {
        privateCharQuest(pdu);
        
        break;
    }
    case ENUM_MSG_TYPE_DESTORY_RESPOND:
    {
        exit(0);
        break;
    }
        
        
    default:
        break;
    }
    free(pdu);
    pdu=NULL;
}

void Widget::registRespond(PDU* pdu)
{
   // qDebug()<<"已经收到消息";
    if (strcmp(pdu->caData,REGIST_OK)==0){
        QMessageBox::information(this,"注册",REGIST_OK);
    }else if(strcmp(pdu->caData,REGIST_FAILED)==0){
        QMessageBox::warning(this,"注册",REGIST_FAILED);
    }
}

void Widget::loginRespond(PDU *pdu)
{
    if (strcmp(pdu->caData,LOGIN_OK)==0){
        QMessageBox::information(this,"登录","登录成功");
        Control::getIntance().show();
        Control::getIntance().setName();
        this->close();
    }else if(strcmp(pdu->caData,LOGIN_FAILED_ERROR_NAME)==0){
        QMessageBox::warning(this,"登录","用户名错误");
        m_strLoginName.clear();
    }else if(strcmp(pdu->caData,LOGIN_FAILED_ERROR_PWD)==0){
        QMessageBox::warning(this,"登录","密码错误");
        m_strLoginName.clear();
    }else{
        QMessageBox::warning(this,"登录",LOGIN_FAILED);
        m_strLoginName.clear();
    }
}

void Widget::disconnectRespond(PDU* pdu)
{
    if(strcmp(pdu->caData,OTHER_LOGIN)==0){
        qDebug()<<"收到了重复登陆的通知";
        QMessageBox::information(this,"disconnected","您的账号已经在其他客户端登录成功,该页面将于点击确定后关闭");
        exit(0);//关闭退出主程序
    }
}

void Widget::createRoomRespond(PDU *pdu)
{
    Control::getIntance().createRoomRespond(pdu);
    
}

void Widget::flushRoomRespond(PDU *pdu)
{
    Control::getIntance().flushRoomRespond(pdu);
}

void Widget::getIntoRoomRespond(PDU *pdu)
{
    Control::getIntance().getIntoRoomRespond(pdu);
}

void Widget::flushMemberInRoomRespond(PDU *pdu)
{
    boardcast::getInterance().flushMemberInRoom(pdu);
}

void Widget::privateCharQuest(PDU *pdu)
{
    if(privateChat::getPrivateChat().isHidden()){
        privateChat::getPrivateChat().show();
    }
    char myName[32] = {'\0'};
    char perName[32] = {'\0'};
    strncpy(myName,pdu->caData,32);
    strncpy(perName,pdu->caData+32,32);
    QString senderName = perName;
    privateChat::getPrivateChat().setPerName(senderName);
    privateChat::getPrivateChat().updateMsg(pdu);
}

void Widget::on_pushButton_2_clicked()//注册
{
    QString strName = ui->lineEdit->text();
    QString strPwd = ui->lineEdit_2->text();
    if(strName.isEmpty()){
         QMessageBox::information(this,"注册","注册失败：用户名为空");
    }else if(strPwd.isEmpty()){
        QMessageBox::information(this,"注册","注册失败：密码为空");
    }else{
      //  qDebug()<<strName;
       // qDebug()<<strPwd;
        PDU* pdu=mkPDU(0);
        pdu->uiMsgType=ENUM_MSG_TYPE_REGIST_REQUEST;
        strncpy(pdu->caData,strName.toStdString().c_str(),32);
        strncpy(pdu->caData+32,strPwd.toStdString().c_str(),32);
        m_tcpSocket.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
    }
}

void Widget::on_pushButton_clicked()//登录
{
    QString strName = ui->lineEdit->text();
    QString strPwd = ui->lineEdit_2->text();
    if(strName.isEmpty()){
         QMessageBox::information(this,"登录","登录失败：用户名为空");
    }else if(strPwd.isEmpty()){
        QMessageBox::information(this,"登录","登录失败：密码为空");
    }else{
       // qDebug()<<strName;
        //qDebug()<<strPwd;
        PDU* pdu=mkPDU(0);
        pdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_REQUEST;
        strncpy(pdu->caData,strName.toStdString().c_str(),32);
        strncpy(pdu->caData+32,strPwd.toStdString().c_str(),32);
        m_tcpSocket.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
        m_strLoginName = strName;
    }
}
