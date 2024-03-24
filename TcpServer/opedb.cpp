#include "opedb.h"
#include <QMessageBox>
#include <QDebug>
OpeDB::OpeDB(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

OpeDB &OpeDB::getInstance()
{
    static OpeDB instance;
    return instance;
}

void OpeDB::init()
{
    m_db.setHostName("localhost");//本地数据库
    m_db.setDatabaseName("C:\\Users\\86187\\Desktop\\qt\\project\\Socket_chat\\TcpServer\\cloud.db");
    if(m_db.open()){
        //qDebug()<<"success open"<<"打开数据库成功";
        QSqlQuery query;
        query.exec("select* from us");
        while(query.next()){
            QString data = QString("%1,%2,%3").arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(2).toString());
          //  qDebug()<<data;
        }
        /*
            us表单：id name pswd online gold_coin
        */
        QString data = QString("update us set online=0 where online=1");
        query.exec(data);
        data = QString("delete from address;");
        query.exec(data);
        query.exec("select name from sqlite_master where type='table' order by name;");
        int i=0;
        while(query.next()){//服务器重新开启一次，发现有直播间没有被销毁，就开启销毁
            QString data = QString("%1").arg(query.value(i).toString());
          //  qDebug()<<"没有被销毁的直播间"<<data;
            if((strcmp(data.toStdString().c_str(),"address") == 0) || (strcmp(data.toStdString().c_str(),"us") == 0)){
             //   qDebug()<<"address 和 us 不可以被销毁";
                break;
            }
            if(strcmp(data.toStdString().c_str(),"sqlite_sequence") == 0){
                break;
            }
            QString str = QString("drop table \'%1\';").arg(data);
            query.exec(str);
            i++;
        }
    }else{
        QMessageBox::information(NULL,"打开数据库","打开数据库失败");
    }
}

OpeDB::~OpeDB()
{
    m_db.close();
}
bool OpeDB::handleRegist(const char *name,const char *pwd)
{
    if(name == NULL || pwd == NULL){
     //   qDebug()<<"name | pwd is NULL";
        return false;
    }else{
        QString data = QString("insert into us(name,pswd) values(\'%1\',\'%2\');").arg(name).arg(pwd);//插入数据库的语句
       // qDebug()<<data;
        QSqlQuery query;
        return query.exec(data);//返回执行的结果
    }
}

bool OpeDB::handleNameExists(const char *name)//账号正确
{
    if(name == NULL){
       // qDebug()<<"name is NULL";
        return false;
    }
    QString data = QString("select name from us where name=\'%1\';").arg(name);
  //  qDebug()<<data;
    QSqlQuery query;
    query.exec(data);
    if(query.next()){
        return true;
    }
    return false;
}

bool OpeDB::handlePswdRight(const char *name, const char *pswd)//密码正确
{
    if(name == NULL || pswd == NULL){
     //   qDebug()<<"name | pwd is NULL";
        return false;
    }
    QString data = QString("select * from us where name=\'%1\' and pswd=\'%2\';").arg(name).arg(pswd);
 //   qDebug()<<data;
    QSqlQuery query;
    query.exec(data);
    if(query.next()){
        return true;
    }
    return false;
}

bool OpeDB::handleChangeOnline(const char *name, const char *pswd)//更改在线状态
{
    
        QString data = QString("update us set online=1 where name=\'%1\' and pswd=\'%2\' and online=0;").arg(name).arg(pswd);//就将在线状态改为1 
    //    qDebug()<<data;
        QSqlQuery query;
        query.exec(data);
        data = QString("select* from us where name=\'%1\' and pswd=\'%2\' and online=1;").arg(name).arg(pswd);
      //  qDebug()<<data;
        query.clear();
        query.exec(data);
        if(query.next()){
            return true;
        }
        return false;
}

void OpeDB::handleOffline(const char *name)
{
    qDebug()<<"进入offline函数";
    if(name == NULL){
        qDebug()<<"name=NULL";
        return;
    }
    QString data = QString("update us set online=0 where name=\'%1\';").arg(name);
    qDebug()<<data;
    QSqlQuery query;
    query.exec(data);
    
}

bool OpeDB::handleCreateRoom(QString data, const char *name)
{
    if(data == nullptr || name == nullptr){
     //   qDebug("data or name =NULL");
        return false;
    }
    QString str = QString("insert into address(name,addr) values(\'%1\',\'%2\');").arg(name).arg(data);
    QSqlQuery query;
    query.exec(str);
    str = QString("select* from address where name=\'%1\' and addr=\'%2\';").arg(name).arg(data);
    query.exec(str);
    if(query.next()){
     //   qDebug()<<"插入地址成功";
        return true;
    }
    return false;
}

bool OpeDB::handleWhoLeaveRoom(const char *name, const char *add)
{
    if(name == NULL || add == NULL){
        return false;
    }
    QString data = QString("select* from address where name=\'%1\' and addr=\'%2\';").arg(name).arg(add);
    QSqlQuery query;
    query.exec(data);
    if(query.next()){
       // qDebug()<<"在判断是什么人离开房间的函数中，查找到有这样一条记录,姓名和地址一样，这是一位主播";
        return true;
    }
    return false;
}

int OpeDB::handleDropTable(const char *add)
{
    if(add == NULL){
        return -1;
    }
 //   qDebug()<<"进入了销毁房间的函数";
    QString data = QString("select* from \'%1\';").arg(add);
  //  qDebug()<<data;
    QSqlQuery query;
    query.exec(data);
    if(query.next()){//说明该直播间还有人没有离去
   //     qDebug()<<"该直播间还有人没有离去";
        data = QString("%1").arg(query.value(0).toString());
   //     qDebug()<<"此人为data："<<data;
        return -1;
    }else{
        QString str = QString("drop table \'%1\';").arg(add);
    //    qDebug()<<str;
        QSqlQuery query1;
        query1.exec(str);
        query1.clear();
        QString str1 = QString("select* from \'%1\';").arg(add);
        bool ret = query1.exec(str1);
        if(ret == true){
      //      qDebug()<<"这个地址表单没有删除成功";
            return -2;    
        }else if(ret == false){//这条语句没有成功执行，说明这个表单已经成功删除了
       //     qDebug()<<"该房间成功销毁";
            return 0;
        }
    }
    return -2;
}

bool OpeDB::handleDeleteRoom(const char *name, const char *add)
{
    if(name == NULL || add == NULL){
        return false;
    }
    QString data = QString("delete from address where name=\'%1\' and addr=\'%2\';").arg(name).arg(add);
    QSqlQuery query;
    query.exec(data);
    QString data2 = QString("select* from address where name=\'%1\' and addr=\'%2\';").arg(name).arg(add);
    query.clear();
    query.exec(data2);
    if(query.next()){
        return true;
    }
    return false;
}

bool OpeDB::handleCreateThisAddressTable(QString data)//给地址创建表单，存储进入的成员
{
    if(data == nullptr){
        return false;
    }
    QString str = QString("create table \'%1\'(name char[32] unique);").arg(data);
    qDebug()<<str;
    QSqlQuery query;
    query.exec(str);
    QString str1 = QString("select* from \'%1\';").arg(data);
    return query.exec(str1);
}

bool OpeDB::handleAnchorJoinRoom(const char *name, QString data)
{
    if(name == nullptr || data == nullptr){
        return false;
    }
    QString del = QString("delete from \'%1\';").arg(data);
    QSqlQuery delet;
    if(delet.exec(del)){
     //   qDebug()<<"房间是空的";
    }
    QString str = QString("insert into \'%1\'(name) values(\'%2\');").arg(data).arg(name);
 //   qDebug()<<"将主播加入房间"<<str;
    QSqlQuery query;
    query.exec(str);
    query.clear();
    QString str1 = QString("select* from \'%1\' where name=\'%2\';").arg(data).arg(name);
//    qDebug()<<"查找主播到底有没有加入房间"<<str1;
    query.exec(str1);
    if(query.next()){
 //       qDebug()<<"查找到了这样一条记录，主播加入了房间";
        return true;
    }
    return false;
}

bool OpeDB::handleDeleteMemberOfRoom(const char *name, const char* add)
{
    if(name == NULL || add == NULL){
        return false;
    }
    QString str = QString("delete from \'%1\' where name=\'%2\';").arg(add).arg(name);
    QSqlQuery query;
    query.exec(str);
    QString stv = QString("select* from \'%1\' where name=\%2\';").arg(add).arg(name);
    query.exec(stv);
    if(query.next()){
        return false;
    }
    return true;
}

QStringList OpeDB::handleFlushRoom()
{
    QString data = QString("select* from address;");
    QSqlQuery query;
    query.exec(data);
    QStringList result;
    result.clear();
    while(query.next()){
        result.append(query.value(1).toString());
    }
    return result;
}

bool OpeDB::handleGetIntoRoom(const char *name, const char *add)
{
    if(name == NULL || add == NULL){
        return false;
    }
    QString data = QString("select* from \'%1\';").arg(add);
    QSqlQuery query;
    if(query.exec(data)){
     //   qDebug()<<data;
      //  qDebug()<<"该表单存在";
    }else{
      //  qDebug()<<"该表单不存在";
        return false;
    }
    data = QString("select* from \'%1\' where name=\'%2\';").arg(add).arg(name);//先在该地址表单中查找该用户，看是不是已经加入进去了
  //  qDebug()<<"data";
    query.clear();
    query.exec(data);
    if(query.next()){
 //       qDebug()<<"已经加入该直播间了，重复加入 ";
        return false;
    }else{
    //    qDebug()<<"没有加入该直播间，允许加入";
    }
    data = QString("insert into \'%1\'(name) values(\'%2\');").arg(add).arg(name);
//    qDebug()<<data;
    query.exec(data);
    data = QString("select* from \'%1\' where name=\'%2\';").arg(add).arg(name);
//    qDebug()<<"data";
    query.clear();
    query.exec(data);
    if(query.next()){
        QString data1 = QString("在该地址表单中找到了属于\'%1\'的记录，加入成功").arg(add).arg(name);
     //   qDebug()<<data1;
        return true;
    }
    QString data2 = QString("在该地址表单中没有找到找到了属于\'%1\'的记录，加入失败").arg(add).arg(name);
//    qDebug()<<data2;
    return false;
}

QStringList OpeDB::handleFlushMemberInRoom(const char *add)
{
    QString data = QString("select* from \'%1\';").arg(add);
    QSqlQuery query;
    query.exec(data);
    QStringList result;
    while (query.next()) {
        result.append(query.value(0).toString());
    }
//    qDebug()<<"房间内部成员列表已经出现";
    return result;
}

int OpeDB::handleSpeak(const char *name)//发言
{
    if(name == NULL){
        return 0;
    }
    QString data = QString("select * from us where name=\'%1\';").arg(name);
 //   qDebug()<<data;
    QSqlQuery query;
    if(query.exec(data)){
  //     qDebug()<<"发言在数据库中查询以前的余额的语句顺利执行";
    }else{
  //      qDebug()<<"没有顺利的执行";
    }
    int it;
    int past;
    if(query.next()){//查询金币的数量
  //      qDebug()<<"查询到金币的数量了吗";
        it = query.value(4).toInt();
  //      qDebug()<<it;
        past = it;
   //     qDebug()<<"已经得到了未曾发言之前金币的数量";
    }
    ++it;
    QString data1 = QString("update us set gold_coin=\'%1\' where name=\'%2\';").arg(it).arg(name);
    if(query.exec(data1)){//给数据库里加一个进去
   //     qDebug()<<"给数据库的金币记录加1的操作执行成功";
    }else{
   //     qDebug()<<"给数据库的金币记录加1的操作执行失败";
    }
    query.exec(data);//再查询一次
    if(query.next()){//查询金币的数量
        it = query.value(4).toInt();
    //    qDebug()<<it;
    //    qDebug()<<"已经得到了现在金币的数量";
    }
    if(it - past == 1){
        return it;
    }
    return 0-past;
}

QString OpeDB::handleGetGold(const char *name)
{
//    qDebug()<<"进入了数据库中获取金币余额的函数";
    if(name == NULL){
        return NULL;
    }
    QString data = QString("select * from us where name=\'%1\';").arg(name);
//    qDebug()<<data;
    QSqlQuery query;
    if(query.exec(data)){
//        qDebug()<<"顺利执行";   
    }else{
 //       qDebug()<<"没有顺利的执行";
    }
    if(query.next()){
    //    qDebug()<<"获取到了内容";
        QString it = QString("%1").arg(query.value(4).toString());
     //   qDebug()<<it;
        return it;
    }
    return NULL;
}

QString OpeDB::handleRecharge(const char *name, const char *mony)
{
    if(name == NULL || mony == NULL){
    //    qDebug()<<"name | money is NULL";
        return NULL;
    }
    QString money = QString("%1").arg(mony);
    QString data = QString("select* from us where name=\'%1\';").arg(name);
  //  qDebug()<<"data:"<<data;
    QSqlQuery query;
    if(query.exec(data)){
 //       qDebug()<<"查询语句成功";
    }else{
  //      qDebug()<<"查询语句失败";
        return NULL;
    }
    QString it;
    if(query.next()){
  //      qDebug()<<"获取到了内容";
        it = QString("%1").arg(query.value(4).toString());
   //     qDebug()<<"未充值以前金币余额："<<it;
    }
    int money1 = it.toInt();
    int money2 = money.toInt();
    int money3 = money1 + money2;
    QString data1 = QString("update us set gold_coin=\'%1\' where name=\'%2\';").arg(money3).arg(name);
    if(query.exec(data1)){
 //       qDebug()<<"更新语句很顺利的执行了";
    }else{
 //       qDebug()<<"更新语句执行失败了";
        return NULL;
    }
    data = QString("select* from us where name=\'%1\';").arg(name);
 //   qDebug()<<"data:"<<data;
    if(query.exec(data)){
 //       qDebug()<<"第二次查询语句成功";
    }else{
//        qDebug()<<"第二次查询语句失败";
        return NULL;
    }
    if(query.next()){
 //       qDebug()<<"获取到了内容";
        it = QString("%1").arg(query.value(4).toString());
  //      qDebug()<<"充值以后现在金币余额："<<it;
    }
    return it;
    
}

QString OpeDB::handleGiveFlow(const char *name,const char* add)//赠送鲜花
{
    if(name == NULL){
        qDebug()<<"name=NULL";
        return NULL;
    }
    QString data = QString("select* from us where name=\'%1\';").arg(name);
//    qDebug()<<"data:"<<data;
    QSqlQuery query;
    if(query.exec(data)){
  //      qDebug()<<"查询语句成功";
    }else{
  //      qDebug()<<"查询语句失败";
        return NULL;
    }
    QString it;
    if(query.next()){
 //       qDebug()<<"获取到了内容";
        it = QString("%1").arg(query.value(4).toString());
 //       qDebug()<<"账户余额："<<it;
    }else{
        return NULL;
    }
    int its = it.toInt();
    if(its >= 10){
        its = its - 10;
    }else{
        return NULL;
    }
    QString data1 = QString("update us set gold_coin=\'%1\' where name=\'%2\';").arg(its).arg(name);
    if(query.exec(data1)){
    //    qDebug()<<"更新语句很顺利的执行了";
    }else{
    //    qDebug()<<"更新语句执行失败了";
        return NULL;
    }
    data = QString("select* from us where name=\'%1\';").arg(name);
 //   qDebug()<<"data:"<<data;
    if(query.exec(data)){
    //    qDebug()<<"第二次查询语句成功";
    }else{
 //       qDebug()<<"第二次查询语句失败";
        return NULL;
    }
    if(query.next()){
  //      qDebug()<<"获取到了内容";
        it = QString("%1").arg(query.value(4).toString());
  //      qDebug()<<"赠送鲜花以后现在金币余额："<<it;
    }else{
        return NULL;
    }
    QString dataAdd = QString("select* from address where addr=\'%1\';").arg(add);
    query.clear();
    query.exec(dataAdd);
    QString perName;
    if(query.next()){
        qDebug()<<"找到了这个主播";
        perName = QString("%1").arg(query.value(0).toString());
    }else{
        qDebug()<<"没有找到了这个主播";
        return NULL;
    }
    QString datas = QString("select* from us where name=\'%1\';").arg(perName);
    query.clear();
    query.exec(datas);
    QString gold;
    if(query.next()){
        qDebug()<<"找到了这位主播的数据";
        gold = QString("%1").arg(query.value(4).toString());
        qDebug()<<"金币数量："<<gold;
    }else{
        return NULL;
    }
    int io = gold.toInt();
    io = io+5;
    datas = QString("update us set gold_coin=\'%1\' where name=\'%2\';").arg(io).arg(perName);
    query.exec(datas);
    return it;
    
    
    return it;
}

QString OpeDB::handleGiveRocket(const char *name,const char* add)//赠送火箭
{
    if(name == NULL){
//        qDebug()<<"name=NULL";
        return NULL;
    }
    QString data = QString("select* from us where name=\'%1\';").arg(name);
 //   qDebug()<<"data:"<<data;
    QSqlQuery query;
    if(query.exec(data)){
 //       qDebug()<<"查询语句成功";
    }else{
 //       qDebug()<<"查询语句失败";
        return NULL;
    }
    QString it;
    if(query.next()){
   //     qDebug()<<"获取到了内容";
        it = QString("%1").arg(query.value(4).toString());
  //      qDebug()<<"账户余额："<<it;
    }else{
        return NULL;
    }
    int its = it.toInt();
    if(its >= 1000){
        its = its - 1000;
    }else{
        return NULL;
    }
    QString data1 = QString("update us set gold_coin=\'%1\' where name=\'%2\';").arg(its).arg(name);
    if(query.exec(data1)){
 //       qDebug()<<"更新语句很顺利的执行了";
    }else{
  //      qDebug()<<"更新语句执行失败了";
        return NULL;
    }
    data = QString("select* from us where name=\'%1\';").arg(name);
//    qDebug()<<"data:"<<data;
    if(query.exec(data)){
//        qDebug()<<"第二次查询语句成功";
    }else{
//        qDebug()<<"第二次查询语句失败";
        return NULL;
    }
    if(query.next()){
 //       qDebug()<<"获取到了内容";
        it = QString("%1").arg(query.value(4).toString());
  //      qDebug()<<"赠送火箭以后现在金币余额："<<it;
        qDebug()<<"扣除金币成功";
    }else{
        return NULL;
    }
    QString dataAdd = QString("select* from address where addr=\'%1\';").arg(add);
    query.clear();
    query.exec(dataAdd);
    QString perName;
    if(query.next()){
        qDebug()<<"找到了这个主播";
        perName = QString("%1").arg(query.value(0).toString());
    }else{
        qDebug()<<"没有找到了这个主播";
        return NULL;
    }
    QString datas = QString("select* from us where name=\'%1\';").arg(perName);
    query.clear();
    query.exec(datas);
    QString gold;
    if(query.next()){
        qDebug()<<"找到了这位主播的数据";
        gold = QString("%1").arg(query.value(4).toString());
        qDebug()<<"金币数量："<<gold;
    }else{
        return NULL;
    }
    int io = gold.toInt();
    io = io+500;
    datas = QString("update us set gold_coin=\'%1\' where name=\'%2\';").arg(io).arg(perName);
    query.exec(datas);
    return it;
}

QString OpeDB::handleGiveAircraft(const char *name,const char* add)//赠送航母
{
    if(name == NULL){
 //       qDebug()<<"name=NULL";
        return NULL;
    }
    QString data = QString("select* from us where name=\'%1\';").arg(name);
//    qDebug()<<"data:"<<data;
    QSqlQuery query;
    if(query.exec(data)){
    //    qDebug()<<"查询语句成功";
    }else{
   //     qDebug()<<"查询语句失败";
        return NULL;
    }
    QString it;
    if(query.next()){
  //      qDebug()<<"获取到了内容";
        it = QString("%1").arg(query.value(4).toString());
  //      qDebug()<<"账户余额："<<it;
    }else{
        return NULL;
    }
    int its = it.toInt();
    if(its >= 10000){
        its = its - 10000;
    }else{
        return NULL;
    }
    QString data1 = QString("update us set gold_coin=\'%1\' where name=\'%2\';").arg(its).arg(name);
    if(query.exec(data1)){
 //       qDebug()<<"更新语句很顺利的执行了";
    }else{
 //       qDebug()<<"更新语句执行失败了";
        return NULL;
    }
    data = QString("select* from us where name=\'%1\';").arg(name);
//    qDebug()<<"data:"<<data;
    if(query.exec(data)){
//        qDebug()<<"第二次查询语句成功";
    }else{
//        qDebug()<<"第二次查询语句失败";
        return NULL;
    }
    if(query.next()){
  //      qDebug()<<"获取到了内容";
        it = QString("%1").arg(query.value(4).toString());
  //      qDebug()<<"赠送航母以后现在金币余额："<<it;
        
    }else{
        return NULL;
    }
    QString dataAdd = QString("select* from address where addr=\'%1\';").arg(add);
    query.clear();
    query.exec(dataAdd);
    QString perName;
    if(query.next()){
        qDebug()<<"找到了这个主播";
        perName = QString("%1").arg(query.value(0).toString());
    }else{
        qDebug()<<"没有找到了这个主播";
        return NULL;
    }
    QString datas = QString("select* from us where name=\'%1\';").arg(perName);
    query.clear();
    query.exec(datas);
    QString gold;
    if(query.next()){
        qDebug()<<"找到了这位主播的数据";
        gold = QString("%1").arg(query.value(4).toString());
        qDebug()<<"金币数量："<<gold;
    }else{
        return NULL;
    }
    int io = gold.toInt();
    io = io+5000;
    datas = QString("update us set gold_coin=\'%1\' where name=\'%2\';").arg(io).arg(perName);
    query.exec(datas);
    return it;
    
    
    return it;
}

void OpeDB::handleDestory(const char *name)
{
    QString data = QString("delete from us where name=\'%1\';").arg(name);
    qDebug()<<data;
    QSqlQuery query;
    query.exec(data);
}

bool OpeDB::handleLoginRepeat(const char *name, const char *pswd)//看看有没有重复登陆
{
    if(name == NULL || pswd == NULL){
 //       qDebug()<<"name | pwd is NULL";
        return false;
    }
    QString data = QString("select * from us where name=\'%1\' and pswd=\'%2\' and online=0;").arg(name).arg(pswd);
//    qDebug()<<data;
    QSqlQuery query;
    query.exec(data);//执行语句，看在线状态是不是等于0
    if(query.next()){       
        return true;
    }
    return false;
}
