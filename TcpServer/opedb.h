#ifndef OPEDB_H
#define OPEDB_H

#include <QObject>
#include <QSqlDatabase>//连接数据库
#include <QSqlQuery>//数据库操作
#include <QStringList>
#include "opedb.h"

class OpeDB : public QObject
{
    Q_OBJECT
public:
    explicit OpeDB(QObject *parent = nullptr);
    static OpeDB& getInstance();
    void init();
    ~OpeDB();
    
    bool handleRegist(const char *name,const char *pwd);//数据库里处理注册的函数，如果插入数据成功，就代表注册成功
    bool handleNameExists(const char* name);//该用户存在不存在，看能不能找到用户名
    bool handlePswdRight(const char* name, const char* pswd);//看看账号密码是不是同时正确
    bool handleLoginRepeat(const char* name,const char* pswd);//看看有没有重复登录
    bool handleChangeOnline(const char* name,const char* pswd);//更改在线状态online
    void handleOffline(const char* name);//将online=1改为online=0，在数据库中，对他进行下线处理
    bool handleCreateRoom(QString data,const char* name);//在数据库里插入一条创建房间的数据
    bool handleWhoLeaveRoom(const char* name,const char* add);//查看是谁离开了房间
    int handleDropTable(const char* add);
    bool handleDeleteRoom(const char* name,const char* add);//主播离开了房间，删除address中该房间的记录
    bool handleCreateThisAddressTable(QString data);//有主播成功申请了一块地址，现在给这块地址创建一张表单，用来存储进入该房间的成员
    bool handleAnchorJoinRoom(const char* name,QString data);//将主播加入房间
    bool handleDeleteMemberOfRoom(const char* name,const char* add);//将用户从聊天室的房间中删除
    QStringList handleFlushRoom();
    bool handleGetIntoRoom(const char* name,const char* add);//将用户加入到聊天室的房间里
    QStringList handleFlushMemberInRoom(const char* add);//刷新房间内部成员列表
    int handleSpeak(const char* name);//发言
    QString handleGetGold(const char* name);//获取金币数量
    QString handleRecharge(const char* name,const char*money);//充值
    QString handleGiveFlow(const char* name,const char* add);//赠送鲜花
    QString handleGiveRocket(const char* name,const char* add);//赠送火箭
    QString handleGiveAircraft(const char* name,const char* add);//赠送航母
    void handleDestory(const char* name);//注销
    
    
    
signals:
    
public slots:
private:
    QSqlDatabase m_db;
};

#endif // OPEDB_H
