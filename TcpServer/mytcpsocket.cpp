#include "mytcpsocket.h"
#include "tcpserver.h"
#include <stdio.h>
#include "protocol.h"
#include "QString"
#include <QDebug>
MyTcpSocket::MyTcpSocket()
{

    connect(this,SIGNAL(readyRead()),this,SLOT(recvMsg()));//如果读取到了客户端发送过来的数据，就转到数据接收函数
    connect(this,SIGNAL(disconnected()),this,SLOT(clientOffline()));//只要客户端结束，就会收到disconnect信号
    
}

MyTcpSocket::~MyTcpSocket()
{
    disconnect(this, NULL, NULL, NULL);
//    close();
    qDebug()<<"虚构函数";
}

QString MyTcpSocket::getName()
{
    return m_strName;
}

void MyTcpSocket::recvMsg()
{
   // qDebug()<<this->bytesAvailable();
    uint uiPDULen = 0;
    this->read((char*)&uiPDULen,sizeof(uint));//先读取四个字节，获取此次发送来的数据总的大小
    uint uiMsgLen = uiPDULen-sizeof(PDU);
    PDU* pdu=mkPDU(uiMsgLen);
    this->read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));
    switch(pdu->uiMsgType){
    case ENUM_MSG_TYPE_REGIST_REQUEST:{//如果是注册请求 1
        registRequest(pdu);
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_REQUEST://登录请求 2
    {
       loginRequest(pdu);
       break;
    }
    case ENUM_MSG_TYPE_CREATE_ROOM_REQUEST://创建房间请求 3
    {
        createRoom(pdu);//创建房间地址，在address表单中插入房间的地址，就代表创建成功了
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_ROOM_REQUEST://刷新房间请求 4
    {
       // qDebug()<<"收到了刷新房间的请求";
        flushRoom(pdu);
       // qDebug()<<"已退出刷新房间函数";
        break;
    }
    case ENUM_MSG_TYPE_LEAVE_ROOM_REQUEST://离开房间请求 5
    {
        leaveRoom(pdu);
        dropRoom(pdu);//离开了房间以后，看看房间有没有人，没有人就销毁掉
        break;
    }
    case ENUM_MSG_TYPE_GET_INTO_ROOM_REQUEST://进入房间请求 6
    {
        bool ret = getIntoRoom(pdu);
        if(ret == true){//此人成功进入房间，房间列表发生改变，需要通知房间内的每一个人
            noticeEverybodyFlushList(pdu);
        }
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_MEMBER_IN_ROOM_REQUEST://刷新成员列表请求 7
    {
        flushMemberInRoom(pdu);
        break;
    }
    case ENUM_MSG_TYPE_SPEAK_REQUEST: //8
    {
        speak(pdu);
        break;
    }
    case ENUM_MSG_TYPE_GET_GOLD_REQUEST: //9
    {
       // qDebug()<<"判断出类型为获取金币余额";
        getGold(pdu);
        break;
    }
    case ENUM_MSG_TYPE_GIVE_FLOWER_REQUEST: //10
    {
       // qDebug()<<"类型为赠送鲜花请求";
        giveFlower(pdu);
        break;
    }
    case ENUM_MSG_TYPE_GIVE_ROCKET_REQUEST:  //11
    {
       // qDebug()<<"类型为赠送火箭请求";
        giveRocket(pdu);
        break;
    }
    case ENUM_MSG_TYPE_GIVE_AIRCRAFT_REQUEST:  //12
    {
      //  qDebug()<<"类型为赠送航母请求";
        giveAircraft(pdu);
        break;
    }
    case ENUM_MSG_TYPE_RECHARGE_REQUEST:  //13
    {
      //  qDebug()<<"类型为充值请求";
        recharge(pdu);
        break;
    }
    case ENUM_MSG_TYPE_PRIVATR_CHAT_REQUEST: //14 服务器收到了私聊的请求
    {
        privateChatRequest(pdu);
        break;
    }
    case ENUM_MSG_TYPE_DESTORY_REQUEST:
    {
        destory(pdu);
        break;
    }
        
    
    default:
        break;
    }
    free(pdu);
    pdu=NULL;
}

void MyTcpSocket::clientOffline()
{
    qDebug()<<"收到了断开的信号";
    OpeDB::getInstance().handleOffline(m_strName.toStdString().c_str());
    m_strName.clear();
    emit offline(this);//将信号发送出去
}

void MyTcpSocket::registRequest(PDU* pdu)
{
    char caName[32]={'\0'};
    char caPwd[32]={'\0'};
    strncpy(caName,pdu->caData,32);
    strncpy(caPwd,pdu->caData+32,32);
   // qDebug()<<"name:"<<caName<<"pswd:"<<caPwd;
    bool ret = OpeDB::getInstance().handleRegist(caName,caPwd);
   // qDebug()<<ret;
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType=ENUM_MSG_TYPE_REGIST_RESPOND;//类型为注册回复
    if(ret){
        strcpy (respdu->caData,REGIST_OK);
    }else{
        strcpy (respdu->caData,REGIST_FAILED);
    }
    write((char*)respdu,respdu->uiPDULen);//把结果返回给客户端
   // qDebug()<<"uiMsgType:"<<respdu->uiMsgType;
    //qDebug()<<"结果已经发送";
    free(respdu);
    respdu=NULL;
}

void MyTcpSocket::loginRequest(PDU *pdu)
{
    char caName[32]={'\0'};
    char caPwd[32]={'\0'};
    strncpy(caName,pdu->caData,32);
    strncpy(caPwd,pdu->caData+32,32);
 //   qDebug()<<"name:"<<caName<<"pswd:"<<caPwd;
    bool ret = OpeDB::getInstance().handleNameExists(caName);//看看用户名有没有错
   
    if(ret == false){//用户名错了
        PDU* respdu = mkPDU(0);
        respdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_RESPOND;//类型为登录回复
        strcpy(respdu->caData,LOGIN_FAILED_ERROR_NAME);

        write((char*)respdu,respdu->uiPDULen);
        free(respdu);
        respdu=NULL;
        return;
    }else{//如果账号正确
        bool res = OpeDB::getInstance().handlePswdRight(caName,caPwd);//看看密码有没有错
        if(res == false){
            PDU* respdu = mkPDU(0);
            respdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_RESPOND;//类型为登录回复
            strcpy(respdu->caData,LOGIN_FAILED_ERROR_PWD);
            write((char*)respdu,respdu->uiPDULen);
            free(respdu);
            respdu=NULL;
            return;
        }else{//密码正确
            bool rew = OpeDB::getInstance().handleLoginRepeat(caName,caPwd);
            if(rew == true){
                bool qwe = OpeDB::getInstance().handleChangeOnline(caName,caPwd);
                if(qwe == true){
                    PDU* respdu = mkPDU(0);
                    respdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_RESPOND;//类型为登录回复
                    strcpy(respdu->caData,LOGIN_OK);//登录成功
                    write((char*)respdu,respdu->uiPDULen);
                    free(respdu);
                    respdu=NULL;
                    m_strName = caName;
                }else{
                    PDU* respdu = mkPDU(0);
                    respdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_RESPOND;//类型为登录回复
                    strcpy(respdu->caData,LOGIN_FAILED);//登陆失败
                    write((char*)respdu,respdu->uiPDULen);
                    free(respdu);
                    respdu=NULL;
                }
            }else{//代表重复登陆了
                PDU* respdu = mkPDU(0);
                respdu->uiMsgType=ENUM_MSG_TYPE_DISCONNECT_RESPOND;//类型为断开连接回复
                strcpy(respdu->caData,OTHER_LOGIN);//已经在其他地方登陆了
               // qDebug()<<"已经开始安排下线";
                //qDebug()<<"m_strName:"<<m_strName;
                TcpServer::getInstance().resend(caName,respdu);
               // qDebug()<<respdu->caData;
               // qDebug()<<respdu->uiMsgType;
                free(respdu);
                respdu=NULL;
                bool qwe = OpeDB::getInstance().handleChangeOnline(caName,caPwd);
                if(qwe == true){
                    PDU* respdu = mkPDU(0);
                    respdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_RESPOND;//类型为登录回复
                    strcpy(respdu->caData,LOGIN_OK);//登录成功
                    write((char*)respdu,respdu->uiPDULen);
                    free(respdu);
                    respdu=NULL;
                    m_strName = caName;
                }else{
                    PDU* respdu = mkPDU(0);
                    respdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_RESPOND;//类型为登录回复
                    strcpy(respdu->caData,LOGIN_FAILED);//登陆失败
                    write((char*)respdu,respdu->uiPDULen);
                    free(respdu);
                    respdu=NULL;
                }
            }
        }
    }
}

void MyTcpSocket::createRoom(PDU *pdu)
{
 //   qDebug()<<"收到了创建房间的请求";
    char caName[32]={'\0'};
    strncpy(caName,pdu->caData,32);
    QString data = getAddress();
  //  qDebug()<<"data:"<<data;
    bool ret = OpeDB::getInstance().handleCreateRoom(data,caName);
    PDU* respdu = mkPDU(data.size()+1);
    respdu->uiMsgType = ENUM_MSG_TYPE_CREATE_ROOM_RESPOND;
    if(ret == true){

        bool res = OpeDB::getInstance().handleCreateThisAddressTable(data);
        if(res == true){
            QString str = QString("创建地址表单：\'%1\'成功").arg(data);
            qDebug()<<str;
            bool reb = OpeDB::getInstance().handleAnchorJoinRoom(caName,data);
            if(reb == true){
                strcpy(respdu->caData,CREATE_ROOM_OK);
                strcpy((char*)respdu->caMsg,data.toStdString().c_str());
            }else{
                QString str = QString("主播加入房间失败");
               qDebug()<<str;
                strcpy(respdu->caData,CREATE_ROOM_FAILED);//此时，已经创建房间，但加入房间失败，就需要将房间删除掉
            }
        }else{
            QString str = QString("创建地址表单：\'%1\'失败").arg(data);
            qDebug()<<str;
            strcpy(respdu->caData,CREATE_ROOM_FAILED);
        }
    }else{
        strcpy(respdu->caData,CREATE_ROOM_FAILED);
    }
    write((char*)respdu,respdu->uiPDULen);
    free(respdu);
    respdu = NULL;
    
}

QString MyTcpSocket::getAddress()
{
    int* a = TcpServer::getInstance().getA();
   // qDebug()<<"a:"<<*a;
   // qDebug()<<"&a"<<a;
    int* b = TcpServer::getInstance().getB();
   // qDebug()<<"b:"<<*b;
   // qDebug()<<"&b"<<b;
    int* c = TcpServer::getInstance().getC();
  //  qDebug()<<"c:"<<*c;
  //  qDebug()<<"&c"<<c;
    int* d = TcpServer::getInstance().getD();
   // qDebug()<<"d:"<<*d;
   // qDebug()<<"&d"<<d;
    QString data = QString("%1.%2.%3.%4").arg(*a).arg(*b).arg(*c).arg(*d);
  //  qDebug()<<"第一次："<<data;
    
    if((++(*d))>255){
        *d=0;
     //   qDebug()<<"d>255";
        (*c)++;
        if(((*c))>255){
            *c=0;
       //     qDebug()<<"c>255";
            (*b)++;
            if(((*b))>255){
                *b=0;
         //       qDebug()<<"b>255";
                (*a)++;
                if(*a>255){
              //      qDebug()<<"a>255";
                    return NULL;
                }
            }
        }
    }
  //  qDebug()<<"第二次："<<data;
    return data;
}

void MyTcpSocket::flushRoom(PDU* pdu)
{
    QStringList result = OpeDB::getInstance().handleFlushRoom();
    qDebug()<<result;
    uint uiMsgLen = result.size()*20;
    PDU* respdu = mkPDU(uiMsgLen);
    respdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_ROOM_RESPOND;
    for(int i=0;i<result.size();i++){
      //  qDebug()<<"开始复制列表的第i个元素";
        memcpy((char*)(respdu->caMsg)+i*20,result.at(i).toStdString().c_str(),result.at(i).size());
    }
 //   qDebug("元素复制完毕");
    write((char*)respdu,respdu->uiPDULen);
 //   qDebug()<<"发送完毕";
    free(respdu);
    respdu = NULL;
  //  qDebug()<<"刷新房间函数已执行完毕";
}

void MyTcpSocket::leaveRoom(PDU *pdu)
{
    char name[32] = {'\0'};
    char add[32] = {'\0'};
    strncpy(name,pdu->caData,32);
    strncpy(add,pdu->caData+32,32);
    int ref = OpeDB::getInstance().handleDeleteMemberOfRoom(name,add);//从聊天室中删除这个人
    if(ref == true){
      //  qDebug()<<"此人已经从房间离去";
    }else{
      //  qDebug()<<"为将此人的记录转移走";
    }
    bool ret = OpeDB::getInstance().handleWhoLeaveRoom(name,add);
    if(ret == true){//代表主播离开了房间
        bool isAnchro = OpeDB::getInstance().handleDeleteRoom(name,add);//删除address表单中属于该房间的记录，
        PDU* respdu = mkPDU(0);
        respdu->uiMsgType = ENUM_MSG_TYPE_LEAVE_ROOM_RESPOND;
        if(isAnchro == false){
          //  qDebug()<<"该房间成功删除";
            strcpy(respdu->caData,LEAVE_ROOM_OK);
        }else if(isAnchro == true){
          //  qDebug()<<"该房间未成功删除";
            strcpy(respdu->caData,LEAVE_ROOM_FAILED);
        }
    }else if(ret == false){//代表聊天室的成员离开了房间
        qDebug()<<"聊天室成员离开了房间";
        PDU* respdu = mkPDU(0);
        respdu->uiMsgType = ENUM_MSG_TYPE_LEAVE_ROOM_RESPOND;
        if(ref == true){
            strcpy(respdu->caData,LEAVE_ROOM_OK);
         //   qDebug()<<"聊天室的成员成功离开了房间";
        }else{
            strcpy(respdu->caData,LEAVE_ROOM_FAILED);
         //   qDebug()<<"该成员离开房间的时候，在房间的记录没有删除干净";
        }
        write((char*)respdu,respdu->uiPDULen);
        free(respdu);
        respdu = NULL;  
    }

    return;
}

void MyTcpSocket::dropRoom(PDU *pdu)
{
    char add[32] = {'\0'};
    strncpy(add,pdu->caData+32,32);
    int num = OpeDB::getInstance().handleDropTable(add);
  //  qDebug()<<"销毁房间函数的返回值 num："<<num;
    if(num == -2){
   //     qDebug()<<"该房间虽然没有人，但房间销毁没有成功";
    }else if(num == -1){
   //     qDebug()<<"该直播间还有人没有离去,该房间不能销毁";
    }else if(num == 0){
    //    qDebug()<<"该房间已经成功销毁";  
    }
}

bool MyTcpSocket::getIntoRoom(PDU *pdu)
{
    char name[32] = {'\0'};
    char add[32] = {'\0'};
    strncpy(name,pdu->caData,32);
    strncpy(add,pdu->caData+32,32);

    bool flag = OpeDB::getInstance().handleGetIntoRoom(name,add);
    if(flag == false){
        PDU* respdu = mkPDU(0);
        respdu->uiMsgType = ENUM_MSG_TYPE_GET_INTO_ROOM_RESPOND;
        strcpy(respdu->caData,GET_INTO_FAILED);
        write((char*)respdu,respdu->uiPDULen);
        free(respdu);
        pdu=NULL;
        return false;
    }else{
        PDU* respdu = mkPDU(32);
        respdu->uiMsgType = ENUM_MSG_TYPE_GET_INTO_ROOM_RESPOND;
        strcpy(respdu->caData,GET_INTO_OK);
        memcpy((char*)respdu->caMsg,add,32);
        write((char*)respdu,respdu->uiPDULen);
        free(respdu);
        pdu=NULL;
        return true;
    }
}

void MyTcpSocket::flushMemberInRoom(PDU *pdu)
{
 //   qDebug()<<"收到了刷新房间内部成员列表的请求";
    char name[32] = {'\0'};
    char add[32] = {'\0'};
    strncpy(name,pdu->caData,32);
    strncpy(add,pdu->caData+32,32);
    QStringList result = OpeDB::getInstance().handleFlushMemberInRoom(add);
  //  qDebug()<<result;
    uint uiMsgLen = result.size()*32;
    PDU* respdu = mkPDU(uiMsgLen);
    respdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_MEMBER_IN_ROOM_RESPOND;
    for(int i=0;i<result.size();i++){
     //   qDebug()<<"开始复制列表的第i个元素";
        memcpy((char*)respdu->caMsg+32*i,result.at(i).toStdString().c_str(),result.at(i).size());
    }
    write((char*)respdu,respdu->uiPDULen);
    for(int j=0;j<result.size();j++){
        memcpy(name,(char*)respdu->caMsg+j*32,32);
        qDebug()<<name;
    }
    free(respdu);
    respdu = NULL;
}

void MyTcpSocket::noticeEverybodyFlushList(PDU *pdu)
{
    char add[32] = {'\0'};
    char perName[32] = {'\0'};
    strncpy(add,pdu->caData+32,32);
    QStringList result = OpeDB::getInstance().handleFlushMemberInRoom(add);
    uint uiMsgLen = result.size()*32;
    PDU* respdu = mkPDU(uiMsgLen);
    respdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_MEMBER_IN_ROOM_RESPOND;
    for(int i=0;i<result.size();i++){
      //  qDebug()<<"开始复制列表的第i个元素";
        memcpy((char*)respdu->caMsg+32*i,result.at(i).toStdString().c_str(),result.at(i).size());
    }
    for(int j=0;j<result.size();j++){
        QString data = QString("房间刷新列表转发的第\'%1\'次；").arg(j);
      //  qDebug()<<data;
        memcpy(perName,(char*)respdu->caMsg+j*32,32);
        TcpServer::getInstance().resend(perName,respdu);
    }
    free(respdu);
    pdu=NULL;
}

void MyTcpSocket::speak(PDU *pdu)
{
    char name[32] = {'\0'};
    strncpy(name,pdu->caData,32);
    int num = OpeDB::getInstance().handleSpeak(name);
    PDU* respdu = mkPDU(0);
   // qDebug()<<"num:"<<num;
    respdu->uiMsgType = ENUM_MSG_TYPE_SPEAK_RESPOND;
    if(num > 0){
        strcpy(respdu->caData,SPEAK_OK);
    }else{
        strcpy(respdu->caData,SPEAK_FAILED);
        num = 0-num;
    }
    
    QString str = QString("%1").arg(num);
    strcpy(respdu->caData+32,str.toStdString().c_str());
    write((char*)respdu,pdu->uiPDULen);
    free(respdu);
    respdu = NULL;
}

void MyTcpSocket::getGold(PDU *pdu)
{
   // qDebug()<<"进入mytcpSocket文件中获取金币余额的函数";
    char name[32] = {'\0'};
    strncpy(name,pdu->caData,32);
    QString it = OpeDB::getInstance().handleGetGold(name);
   // qDebug()<<"在MySocket函数中得到的金币数量"<<it;
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_GET_GOLD_RESPOND;
    if(it!=nullptr){
       // qDebug()<<"it！=NULL";
        strncpy(respdu->caData,GET_GOLD_OK,32);
       // qDebug()<<"respdu->caData"<<respdu->caData;
        strncpy(respdu->caData+32,it.toStdString().c_str(),32);
       // qDebug()<<"respdu->caData+32"<<respdu->caData;
    }else{
        
        strncpy(respdu->caData,GET_GOLD_FAILED,32);
    }
    write((char*)respdu,respdu->uiPDULen);
    free(respdu);
    respdu = NULL;
}

void MyTcpSocket::recharge(PDU *pdu)//充值
{
    char name[32] = {'\0'};
    char mon[32] = {'\0'};
    strncpy(name,pdu->caData,32);
    strncpy(mon,pdu->caData+32,32);
    QString money = OpeDB::getInstance().handleRecharge(name,mon);
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_RECHARGE_RESPOND;
    if(money!=nullptr){
        strcpy(respdu->caData,RECHARGE_OK);
        strncpy(respdu->caData+32,money.toStdString().c_str(),32);
    }else{
        strcpy(respdu->caData,RECHARGE_FAILED);
    }
    write((char*)respdu,respdu->uiPDULen);
    free(respdu);
    respdu=NULL;
}

void MyTcpSocket::giveFlower(PDU *pdu)//赠送鲜花
{
    char name[32] = {'\0'};
    strncpy(name,pdu->caData,32);
    char add[32] = {'\0'};
    strncpy(add,pdu->caData+32,32);
    QString it = OpeDB::getInstance().handleGiveFlow(name,add);
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_GIVE_FLOWER_RESPOND;
    if(it != nullptr){
        strcpy(respdu->caData,GIVE_FLOWER_OK);
        strncpy(respdu->caData+32,it.toStdString().c_str(),it.size());
    }else{
        strcpy(respdu->caData,GIVE_FLOWER_FAILED);
    }
    write((char*)respdu,respdu->uiPDULen);
   // qDebug()<<"成功发送返回回复";
    free(respdu);
    respdu = NULL;
}

void MyTcpSocket::giveRocket(PDU *pdu)//赠送火箭
{
    char name[32] = {'\0'};
    strncpy(name,pdu->caData,32);
    char add[32] = {'\0'};
    strncpy(add,pdu->caData+32,32);
    QString it = OpeDB::getInstance().handleGiveRocket(name,add);
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_GIVE_ROCKET_RESPOND;
    if(it != nullptr){
        strcpy(respdu->caData,GIVE_ROCKET_OK);
        strncpy(respdu->caData+32,it.toStdString().c_str(),it.size());
    }else{
        strcpy(respdu->caData,GIVE_ROCKET_FAILED);
    }
    write((char*)respdu,respdu->uiPDULen);
  //  qDebug()<<"成功发送返回回复";
    free(respdu);
    respdu = NULL;
}

void MyTcpSocket::giveAircraft(PDU *pdu)//赠送航母
{
    char name[32] = {'\0'};
    strncpy(name,pdu->caData,32);
    char add[32] = {'\0'};
    strncpy(add,pdu->caData+32,32);
    QString it = OpeDB::getInstance().handleGiveAircraft(name,add);
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_GIVE_AIRCRAFT_RESPOND;
    if(it != nullptr){
        strcpy(respdu->caData,GIVE_AIRCRAFT_OK);
        strncpy(respdu->caData+32,it.toStdString().c_str(),it.size());
    }else{
        strcpy(respdu->caData,GIVE_AIRCRAFT_FAILED);
    }
    write((char*)respdu,respdu->uiPDULen);
 //   qDebug()<<"成功发送返回回复";
    free(respdu);
    respdu = NULL;
}

void MyTcpSocket::privateChatRequest(PDU *pdu)
{
    char perName[32] = {'\0'};
    strncpy(perName,pdu->caData,32);
    TcpServer::getInstance().resend(perName,pdu);
}

void MyTcpSocket::destory(PDU *pdu)
{
    qDebug()<<"收到了注销的请求";
    char name[32] = {'\0'};
    strcpy(name,pdu->caData);
    qDebug()<<"注销者："<<name;
    OpeDB::getInstance().handleDestory(name);
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_DESTORY_RESPOND;
    write((char*)respdu,respdu->uiPDULen);
    free(respdu);
    respdu=NULL;
}


