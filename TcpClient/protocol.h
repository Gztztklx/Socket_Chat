#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef unsigned int uint;

#define REGIST_OK "regist ok"//注册成功
#define REGIST_FAILED "regist failed ： name existed"//注册失败:用户名重复
#define LOGIN_OK "login ok"//登录成功
#define LOGIN_FAILED "login failed ： name error or pwd error or relogin"//登录失败:用户名或密码或重复登录

#define LOGIN_FAILED_ERROR_PWD "login failed : pwd error"//登陆失败，密码错误
#define LOGIN_FAILED_ERROR_NAME "login failed : name error"//登陆失败，用户名不存在
#define OTHER_LOGIN "other login"// 已经在其他地方登录了

#define SEARCH_USR_NO "no such people"//没有该用户
#define SEARCH_USR_ONLINE "online"//该用户在线
#define SEARCH_USR_OFFLINE "offline"//该用户离开

#define UNKNOW_ERROR "unknow err" //未知错误
#define EXISTED_FRIEND "friend exist" //朋友已经存在
#define ADD_FRIEND_OFFLINE "usr offline" //要添加的用户已离开
#define ADD_FRIEND_NO_EXIST "usr not exist" //要添加的用户不存在

#define DELETE_FRIEND_OK "delete friend ok"  //删除好友成功

#define CREATE_ROOM_OK "create room ok" //创建房间成功
#define CREATE_ROOM_FAILED "create room failed" //创建房间失败

#define LEAVE_ROOM_OK "leave room ok" //成功离开了房间
#define LEAVE_ROOM_FAILED "leave room failed" //房间删除未成功

#define GET_INTO_OK "get into room ok"//成功进入房间
#define GET_INTO_FAILED "get into room failed"//进入房间失败

#define SPEAK_OK "speak ok gold_coin++" //发言成功，金币自增
#define SPEAK_FAILED "speak failed gold_coin+0" //发言失败，金币无变化

#define GET_GOLD_OK "get gold ok" //获取金币数量成功
#define GET_GOLD_FAILED "get gold failed" //获取金币数量失败

#define RECHARGE_OK "recharge ok" //充值成功
#define RECHARGE_FAILED "recharge failed" //充值失败


#define GIVE_FLOWER_OK "give flower ok" //赠送鲜花成功
#define GIVE_FLOWER_FAILED "give flower failed" //赠送鲜花失败
#define GIVE_ROCKET_OK "give rocket ok" //赠送火箭成功
#define GIVE_ROCKET_FAILED "give rocket failed" //赠送火箭失败
#define GIVE_AIRCRAFT_OK "give aircraft ok" //赠送航母成功
#define GIVE_AIRCRAFT_FAILED "give aircraft failed" //赠送航母失败

struct AudioPackage
{
    char data[1024];
    int dataLen;
};

enum ENUM_MSG_TYPE{
    ENUM_MSG_TYPE_MIN = 0,//最小
    
    ENUM_MSG_TYPE_REGIST_REQUEST,//注册请求 1
    ENUM_MSG_TYPE_REGIST_RESPOND,//注册回复 2
    
    ENUM_MSG_TYPE_LOGIN_REQUEST,//登录请求 3
    ENUM_MSG_TYPE_LOGIN_RESPOND,//登录回复 4

    ENUM_MSG_TYPE_PRIVATR_CHAT_REQUEST,//私聊请求 5
    ENUM_MSG_TYPE_PRIVATR_CHAT_RESPOND,//私聊回复 6
    
    ENUM_MSG_TYPE_DISCONNECT_RESPOND,//断开连接通知 7
    
    ENUM_MSG_TYPE_CREATE_ROOM_REQUEST,//创建房间请求 8
    ENUM_MSG_TYPE_CREATE_ROOM_RESPOND,//创建房间回复 9
    
    ENUM_MSG_TYPE_FLUSH_ROOM_REQUEST,//刷新房间请求 10
    ENUM_MSG_TYPE_FLUSH_ROOM_RESPOND,//刷新房间回复 11
    
    ENUM_MSG_TYPE_LEAVE_ROOM_REQUEST,//离开房间请求 12
    ENUM_MSG_TYPE_LEAVE_ROOM_RESPOND,//离开房间回复 13
    
    ENUM_MSG_TYPE_GET_INTO_ROOM_REQUEST,//进入房间请求 14
    ENUM_MSG_TYPE_GET_INTO_ROOM_RESPOND,//进入房间回复 15
    
    ENUM_MSG_TYPE_FLUSH_MEMBER_IN_ROOM_REQUEST,//房间内部刷新成员列表请求 16
    ENUM_MSG_TYPE_FLUSH_MEMBER_IN_ROOM_RESPOND,//房间内部刷新成员列表回复 17
    
    ENUM_MSG_TYPE_SPEAK_REQUEST,//发言请求 18
    ENUM_MSG_TYPE_SPEAK_RESPOND,//发言回复 19
        
    ENUM_MSG_TYPE_DESTORY_REQUEST,//注销请求 20
    ENUM_MSG_TYPE_DESTORY_RESPOND,//注销回复 21
    
    ENUM_MSG_TYPE_GET_GOLD_REQUEST,//获取金币数量请求 22
    ENUM_MSG_TYPE_GET_GOLD_RESPOND,//获得金币数量回复 23
    
    ENUM_MSG_TYPE_GIVE_FLOWER_REQUEST,//赠送鲜花请求 24
    ENUM_MSG_TYPE_GIVE_FLOWER_RESPOND,//赠送鲜花回复 25
    
    ENUM_MSG_TYPE_GIVE_ROCKET_REQUEST,//赠送火箭请求 26
    ENUM_MSG_TYPE_GIVE_ROCKET_RESPOND,//赠送火箭回复 27
    
    ENUM_MSG_TYPE_GIVE_AIRCRAFT_REQUEST,//赠送航母请求 28
    ENUM_MSG_TYPE_GIVE_AIRCRAFT_RESPOND,//赠送航母回复 29
    
    ENUM_MSG_TYPE_RECHARGE_REQUEST,//充值请求 30
    ENUM_MSG_TYPE_RECHARGE_RESPOND,//充值回复 31
    
    ENUM_MSG_TYPE_MAX = 0x00ffffff,//最大32
};
struct PDU
{
    uint uiPDULen;//总的协议数据单元大小
    uint uiMsgType;//消息类型
    char caData[64];
    uint uiMsgLen;//实际消息长度
    int caMsg[];//实际消息
};

PDU *mkPDU(uint uiMsgLen);

#endif // PROTOCOL_H
