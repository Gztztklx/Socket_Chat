#include "boardcast.h"
#include "ui_boardcast.h"
#include "control.h"
#include "widget.h"

#include <ctime>
#include <QDebug>
#include <QByteArray>
#include <QThread>
#include <QVector>
#include <QLabel>
#include <QPoint>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include "privatechat.h"

boardcast::boardcast(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::boardcast)
{
    qDebug()<<"boardcast窗口的单例模式从此开始";
    ui->setupUi(this);
    ui->lb_air->hide();
    ui->lb_flower->hide();
    ui->lb_rocket->hide();
    sender = new QUdpSocket(this);
    reciver = new QUdpSocket(this);
    qDebug()<<"29";
    selfName = Widget::getWidget().getName();
    //这是一条广播的代码
    //reciver->bind(12345,QAbstractSocket::ReuseAddressHint);
    reciver->bind(QHostAddress::AnyIPv4,56789,QAbstractSocket::ReuseAddressHint);
    addr = Control::getIntance().getAddr(); 
    //qDebug()<<"boardcast addr:"<<addr;
    reciver->joinMulticastGroup(addr);//组播代码，加入到广播组里去
    QCameraInfo ci = QCameraInfo::defaultCamera();
    camera = new QCamera(ci,this);
    abs = new AbstractVideoSurface(this);
    camera->setViewfinder(abs);
    qDebug()<<"41";
    QObject::connect(abs,SIGNAL(sndImage(QImage)),this,SLOT(sendImage(QImage)));
    QObject::connect(reciver,SIGNAL(readyRead()),this,SLOT(receive()));
    //qDebug()<<"关联已经设置成功";   
    ui->label_6->setText(selfName);
    m_pDialog = new Dialog;
    m_pDialog->hide();  
    t = new Thread(this);
    QObject::connect(t,SIGNAL(move_label()),this,SLOT(bullet_chat()));
    t->start();
    gift_thread_flower = new Thread(this);
    QObject::connect(gift_thread_flower,SIGNAL(move_flower()),this,SLOT(moveFlower()));
    qDebug()<<"53";
    gift_thread_rocket = new Thread(this);
    QObject::connect(gift_thread_rocket,SIGNAL(move_rocket()),this,SLOT(moveRocket()));
    gift_thread_aircra = new Thread(this);
    QObject::connect(gift_thread_aircra,SIGNAL(move_aircra()),this,SLOT(moveAircra()));
    pos_lb_air = ui->lb_air->pos();
    pos_lb_flower = ui->lb_flower->pos();
    pos_lb_rocket = ui->lb_rocket->pos();
    qDebug()<<"62";
    QAudioFormat format; //声音格式对象
    format.setSampleRate(8000); //采样率
    format.setChannelCount(1); //信道数
    format.setSampleSize(16); //样本大小
    format.setCodec("audio/pcm"); //编码方式
    format.setByteOrder(QAudioFormat::LittleEndian); //字节顺序
    format.setSampleType(QAudioFormat::SignedInt); //采样类型
    
    audioInput = new QAudioInput(format); //获取默认的输入设备 输入对象
    audioOutput = new QAudioOutput(format); //获取默认的输出设备 输出对象
    qDebug()<<"73";
    reci_video = new QUdpSocket(this);
    send_video = new QUdpSocket(this);
    reci_video->bind(QHostAddress::AnyIPv4,10008,QAbstractSocket::ReuseAddressHint);
    QObject::connect(reci_video,SIGNAL(readyRead()),this,SLOT(onReadyReadVideo()));
    qDebug()<<"75";
    reci_video->joinMulticastGroup(addr);
    qDebug()<<"77";
    audioOutputIODevice = audioOutput->start();//打开接受声音的接口
}

boardcast::~boardcast()
{
    delete ui;
    t->terminate();
    delete m_pDialog;
    
    
    
}

boardcast &boardcast::getInterance()
{
    static boardcast instance;
    return instance;
}

void boardcast::flushMemberInRoom(PDU *pdu)
{
    if(pdu == NULL){
        return;
    }
    boardcast::getInterance().show();//刷新列表的时候，让他们弹出来
    ui->listWidget->clear();
    uint size = pdu->uiMsgLen/32;
    char name[32] = {'\0'};
    for(uint i = 0;i < size;i ++){
        memcpy(name,(char*)pdu->caMsg+i*32,32);
        ui->listWidget->addItem(name);
        //qDebug()<<"name:"<<name;
    }
   // qDebug()<<"房间内部刷新成员列表函数马上结束";
}

void boardcast::speakRespond(PDU *pdu)
{
    char state[32] = {'\0'};
    char mon[32] = {'\0'};
    strncpy(state,pdu->caData,32);
    strncpy(mon,pdu->caData+32,32);
    QString money = QString("%1").arg(mon);
    int it = money.toInt();
    QString its = QString("%1").arg(it);
    //qDebug()<<"it"<<it;

    if(strcmp(state,SPEAK_OK) == 0){
        QString data = QString("系统通知：您已成功发言，金币已加一，现在余额为%1.").arg(it);
        ui->textEdit->append(data);
        ui->label_9->setText(its);
    }else{
        QString data = QString("系统通知：您已成功发言，金币未增加，现在余额为%1，请再接再厉.").arg(it);
        ui->textEdit->append(data);
        ui->label_9->setText(its);
    }
}

void boardcast::setgoldCoin(PDU *pdu)
{
    char data[32] = {'\0'};
    char money[32] = {'\0'};
    strncpy(data,pdu->caData,32);
    strncpy(money,pdu->caData+32,32);
    //qDebug()<<"data"<<data;
    if(strcmp(data,GET_GOLD_OK) == 0){
        QString data1 = QString("系统通知：余额已经刷新，现在余额为%1.").arg(money);
        ui->textEdit->append(data1);
        ui->label_9->setText(money);
    }else{
        QString data2 = QString("系统通知：余额刷新失败，现在余额为０．");
        ui->textEdit->append(data2);
        ui->label_9->setText("0");
    }
}

void boardcast::rechargeRespond(PDU *pdu)
{
    //qDebug()<<"收到了充值回复的类型";
    char data[32] = {'\0'};
    char money[32] = {'\0'};
    strncpy(data,pdu->caData,32);
    strncpy(money,pdu->caData+32,32);
    //qDebug()<<"data"<<data;
    if(strcmp(data,RECHARGE_OK) == 0){
        QString data1 = QString("系统通知：充值成功，现在余额为%1.").arg(money);
        ui->textEdit->append(data1);
        ui->label_9->setText(money);
    }else{
        QString data2 = QString("系统通知：充值失败，现在余额为０，请手动刷新．");
        ui->textEdit->append(data2);
        ui->label_9->setText("0");
    }
}

void boardcast::giveFlowerRespond(PDU *pdu)
{
    //qDebug()<<"收到了赠送鲜花回复的类型";
    char state[32] = {'\0'};
    char it[32] = {'\0'};
    strncpy(state,pdu->caData,32);
    if(strcmp(state,GIVE_FLOWER_OK) == 0){
        //qDebug()<<"赠送鲜花成功";
        strncpy(it,pdu->caData+32,32);
        ui->label_9->setText(it);
        QString data1 = QString("系统通知：打赏一朵鲜花成功，本次消费10金币，余额已经刷新，现在余额为%1.").arg(it);
        ui->textEdit->append(data1);
        char data[40]={'\0'};
        strncpy(data,"flower",6);
        strcpy(data+6,selfName.toStdString().c_str());
        QString str = QString("%1").arg(data);
        sender->writeDatagram(str.toLocal8Bit(),addr,56789);
        //qDebug()<<"赠送鲜花的语句已经发送";
    }else{
        //qDebug()<<"赠送鲜花失败";
        QString data1 = QString("系统通知：打赏一朵鲜花失败.");
        ui->textEdit->append(data1);
    }
}

void boardcast::giveRocketRespond(PDU *pdu)
{
    //qDebug()<<"收到了赠送火箭回复的类型";
    char state[32] = {'\0'};
    char it[32] = {'\0'};
    strncpy(state,pdu->caData,32);
    if(strcmp(state,GIVE_ROCKET_OK) == 0){
        //qDebug()<<"赠送火箭成功";
        strncpy(it,pdu->caData+32,32);
        ui->label_9->setText(it);
        QString data1 = QString("系统通知：打赏一支火箭成功，本次消费1000金币，余额已经刷新，现在余额为%1.").arg(it);
        ui->textEdit->append(data1);
        char data[40]={'\0'};
        strncpy(data,"rocket",6);
        strcpy(data+6,selfName.toStdString().c_str());
        QString str = QString("%1").arg(data);
        sender->writeDatagram(str.toLocal8Bit(),addr,56789);
        //qDebug()<<"赠送火箭的语句已经发送";
    }else{
        //qDebug()<<"赠送火箭失败";
        QString data1 = QString("系统通知：打赏一支火箭失败.");
        ui->textEdit->append(data1);
    }
}

void boardcast::giveAircraftRespond(PDU *pdu)
{
    //qDebug()<<"收到了赠送航母回复的类型";
    char state[32] = {'\0'};
    char it[32] = {'\0'};
    strncpy(state,pdu->caData,32);
    if(strcmp(state,GIVE_AIRCRAFT_OK) == 0){
        //qDebug()<<"赠送航母成功";
        strncpy(it,pdu->caData+32,32);
        ui->label_9->setText(it);
        QString data1 = QString("系统通知：打赏一艘航母成功，本次消费10000金币，余额已经刷新，现在余额为%1.").arg(it);
        ui->textEdit->append(data1);
        char data[40]={'\0'};
        strcpy(data,"aircra");
        strcpy(data+6,selfName.toStdString().c_str());
        QString str = QString("%1").arg(data);
        sender->writeDatagram(str.toLocal8Bit(),addr,56789);
        //qDebug()<<"赠送航母的语句已经发送";
    }else{
        //qDebug()<<"赠送航母失败";
        QString data1 = QString("系统通知：打赏一艘航母失败.");
        ui->textEdit->append(data1);
    }
}

void boardcast::closeOnCameraButton()//将这个按钮设置为不可用 进入房间时，不可以开启摄像头和开麦克风
{
    ui->pushButton->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
}

void boardcast::useOnCameraButton() //创建房间时，允许开启摄像头和麦克风
{
    ui->pushButton->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
}

void boardcast::joinDifferentGroup()
{
    addr = Control::getIntance().getAddr();
    reciver->joinMulticastGroup(addr);//组播代码，加入到广播组里去
    reci_video->joinMulticastGroup(addr);
}

void boardcast::onReadyReadVideo()//用户接收到声音，
{
    qDebug()<<"收听到声音";
    AudioPackage pack;
    memset(&pack,0,sizeof(AudioPackage));
    reci_video->readDatagram((char*)&pack,sizeof(AudioPackage));//用户从网络上读取声音数据
    sleep(0.5);
    audioOutputIODevice->write(pack.data,pack.dataLen);//用户从自己的设备上发出声音
}

void boardcast::video_rec()//自己得到了原始数据，接收到声音，声音处理
{
    AudioPackage pack;
    memset(&pack,0,sizeof(AudioPackage));
    pack.dataLen = audioInputIODevice->read(pack.data,1024);
    
    send_video->writeDatagram((char*)&pack,sizeof(AudioPackage),addr,10008);
    audioOutputIODevice->write(pack.data,pack.dataLen);//这一句是主播自己的设备收取到数据自己发出声音
}

void boardcast::moveFlower()
{
    QPoint p = ui->lb_flower->pos();
    int y = p.y()-1;
    p.setY(y);
    ui->lb_flower->move(p);
    y=p.y()+ui->lb_flower->height();//火箭的下端y坐标
    if(y<=0){
        ui->lb_flower->hide();
        ui->lb_flower->move(pos_lb_flower);//回到初始位置
        gift_thread_flower->terminate();
    }
}

void boardcast::moveAircra()
{
    //qDebug()<<"有收到移动航母的信号";
    if(ui->lb_air->isHidden()){
        ui->lb_air->show();
    }
    QPoint p = ui->lb_air->pos();
    //qDebug()<<"pp,y"<<pp.y();
    int y = p.y()-1;
    
    p.setY(y);
    ui->lb_air->move(p);
    y=p.y()+ui->lb_air->height();//火箭的下端y坐标
    //qDebug()<<"y:"<<y;
    if(y == 0){
        ui->lb_air->hide();
        ui->lb_air->move(pos_lb_air);//回到初始位置
        //qDebug()<<"航母回到了初始位置";
        gift_thread_aircra->terminate();
    }
}

void boardcast::moveRocket()
{
    QPoint p = ui->lb_rocket->pos();
    int y = p.y()-1;
    p.setY(y);
    ui->lb_rocket->move(p);
    y=p.y()+ui->lb_rocket->height();//火箭的下端y坐标
    if(y<=0){
        ui->lb_rocket->hide();
        ui->lb_rocket->move(pos_lb_rocket);//回到初始位置
        gift_thread_rocket->terminate();
    }
}

void boardcast::receive()
{
    //qDebug()<<"进入了接收消息的函数";
    QByteArray datagram;
    datagram.resize(reciver->pendingDatagramSize());
    if(datagram.size()<=300){
        //qDebug()<<"进入读取文本的函数";
        int size = datagram.size();
        //qDebug()<<"读取文本"<<size;
        if(size <= 0){
            return;
        }
        if(size<50){
            size = 50;
        }
        char buf[size+1] = {'\0'};
        reciver->readDatagram(buf,size);
        //qDebug()<<"buf:"<<buf;
        char gift[32] = {'\0'};
        char name[32] = {'\0'};
        strncpy(gift,buf,6);
        strncpy(name,buf+6,32);
        //qDebug()<<"gift:"<<gift;
        //qDebug()<<"name:"<<name;
        QString strd;
        if(strcmp(gift,"flower") == 0){
            //qDebug()<<name<<"赠送了礼物鲜花";
            gift_thread_flower->start();
            ui->lb_flower->show();
            strd = QString("%1送给主播一朵鲜花").arg(name);
            strcpy(buf,strd.toStdString().c_str());
        }else if(strcmp(gift,"rocket") == 0){
            //qDebug()<<name<<"赠送了礼物火箭";
            ui->lb_rocket->show();
            gift_thread_rocket->start();
            strd = QString("%1送给主播一支火箭").arg(name);
            strcpy(buf,strd.toStdString().c_str());
        }else if(strcmp(gift,"aircra") == 0){
            //qDebug()<<name<<"赠送了礼物航母";
            gift_thread_aircra->start();
            ui->lb_air->show();
            strd = QString("%1送给主播一艘航母").arg(name);
            strcpy(buf,strd.toStdString().c_str());
        }
        QString str = QString::fromLocal8Bit(buf);
        ui->textEdit_2->append(str); //消息列表中有了消息记录
        QLabel *lb = new QLabel(buf,this);
        int x = 271;
        int y = rand()%140;//0-139
        int lb_x;
        int lb_y;
        if(size<50){
            lb_x = size*10;
            lb_y = 16;
        }else if(size>= 50 && size<100){
            lb_x = size*5;
            lb_y = 32;
        }else if(size>=100 && size<150){
            lb_x = size*10;
            lb_x = 32;
        }else if(size>=150 && size<200){
            lb_x = size*4;
            lb_y = 48;
        }else if(size>=200 && size<250){
            lb_x = size*8;
            lb_y = 48;
        }else{
            lb_x = size*5;
            lb_y = 64;
        }
        lb->setGeometry(x,y,lb_x,lb_y);
        v.push_back(lb);
        lb->show(); 
        bullet_chat();
    }else{
        //qDebug()<<"进入了接受图像的函数";
        int size = reciver->pendingDatagramSize();
        //qDebug()<<"接收图像："<<size;
        QByteArray arr;
        arr.resize(size);
        reciver->readDatagram(arr.data(),size);
        QBuffer buf(&arr);
        QImageReader reader(&buf,"JPG");
        QImage image = reader.read();
        QPixmap pic = QPixmap::fromImage(image);
        pic=pic.scaled(ui->label->size());
        ui->label->setPixmap(pic);
    }    
}

void boardcast::bullet_chat()
{
    
    QVector<QLabel*>::iterator it = v.begin();
    for(;it!=v.end();){
        this->moveX(*it);
        QPoint p = (*it)->pos();
        int x = p.x() + (*it)->width();
        if(x<=this->pos().x()){
            //qDebug()<<"弹幕已经消失";
            QLabel* temp = *it;
            v.erase(it);
            delete temp;
            
        }else{
            it++;
        }
    }
}

void boardcast::sendImage(QImage image)//发送图像
{
    //qDebug()<<"进入了发送图像的函数";
    QPixmap pic = QPixmap::fromImage(image);
    pic = pic.scaled(ui->label->size());
    ui->label->setPixmap(pic);
    
    //视频主播自己看自己的图像由本地设置，不由reciver接收
    
    //reciver->close();//自己关闭接收的功能
    
    QByteArray arr;
    QBuffer buf(&arr);
    QImage img = pic.toImage();//摄像头捕获照片之后适应好大小
    img.save(&buf,"JPG");
    sender->writeDatagram(arr,QHostAddress::Broadcast,56789);
    //qDebug()<<sender->error();
}

void boardcast::closeEvent(QCloseEvent*ev)
{
    camera->stop();
    Control::getIntance().deleteRoomRequest();
    boardcast::getInterance().close();
}

void boardcast::on_pushButton_6_clicked()//发送文本
{
    QString str = ui->lineEdit->text();
    if(str == NULL){
        return;
    }
    QString data = QString("【%1】says：%2").arg(selfName).arg(str);
    //qDebug()<<"data:"<<data;
    sender->writeDatagram(data.toLocal8Bit(),addr,56789);
    ui->lineEdit->clear();
    
    //qDebug()<<"文本已发送";
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_SPEAK_REQUEST;
    strcpy(pdu->caData,selfName.toStdString().c_str());
    Widget::getWidget().getQtcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=NULL;
    //qDebug()<<"发言请求已经发送";
}

void boardcast::on_pushButton_clicked()//摄像头开
{
    camera->start();
    //qDebug()<<"摄像头已经打开";
}

void boardcast::on_pushButton_2_clicked()//摄像头关
{
    camera->stop();
    ui->label->clear();
    //qDebug()<<"摄像头已经关闭";
}

void boardcast::on_pushButton_5_clicked()//房间内部成员刷新列表
{
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_MEMBER_IN_ROOM_REQUEST;
    strcpy(pdu->caData,selfName.toStdString().c_str());
    QString str = addr.toString();
    
   // qDebug()<<selfName;
    //qDebug()<<str;
    strcpy(pdu->caData+32,str.toStdString().c_str());
    Widget::getWidget().getQtcpSocket().write((char*)pdu,pdu->uiPDULen);
    //qDebug()<<"刷新成员列表的请求已发送";
    free(pdu);
    pdu=NULL;
}

void boardcast::on_pushButton_8_clicked()//打赏
{
    if(m_pDialog->isHidden()){
        m_pDialog->show();
    }else{
        m_pDialog->hide();
    }
}

void boardcast::moveX(QLabel *lb)
{
    //qDebug()<<"弹幕正在移动";
    QPoint p = lb->pos();
    int x = p.x()-1;
    p.setX(x);//设置好lb的X坐标
    lb->move(p);//移动到设置以后的位置
}

void boardcast::moveY(QLabel *lb)
{
    QPoint p = lb->pos();
    int y = p.y()-1;
    p.setY(y);
    lb->move(p);
}

void boardcast::on_pushButton_9_clicked()//获得金额
{
    //qDebug()<<"进入获得金币余额的函数";
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_GET_GOLD_REQUEST;
    strcpy(pdu->caData,selfName.toStdString().c_str());
    Widget::getWidget().getQtcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

void boardcast::on_pushButton_7_clicked()//充值
{
    int money = QInputDialog::getInt(this,"充值","金额");
    if(money <= 0){
        return;
    }
    //qDebug()<<"money:"<<money;
    QString it = QString("%1").arg(money);
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_RECHARGE_REQUEST;
    strncpy(pdu->caData,selfName.toStdString().c_str(),32);
    strncpy(pdu->caData+32,it.toStdString().c_str(),32);
    Widget::getWidget().getQtcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=NULL;
}

void boardcast::on_pushButton_10_clicked()//私聊按钮
{
    if(ui->listWidget->currentItem() == NULL){
        QMessageBox::information(this,"私聊","请选择私聊对象：");
        return;
    }
    QString perName = ui->listWidget->currentItem()->text();
    privateChat::getPrivateChat().setPerName(perName);
    if(privateChat::getPrivateChat().isHidden()){
        privateChat::getPrivateChat().show();
    }
    
}

void boardcast::on_pushButton_3_clicked()//主播开启麦克风
{
    audioInputIODevice = audioInput->start();
    QObject::connect(audioInputIODevice,SIGNAL(readyRead()),this,SLOT(video_rec()));
}

void boardcast::on_pushButton_4_clicked()//主播关闭麦克风
{
    audioInput->stop();
}

void boardcast::on_pushButton_11_clicked()//用户开启麦克风
{
   // audioOutputIODevice = audioOutput->start();
}

void boardcast::on_pushButton_12_clicked() //用户关闭麦克风
{
    //audioOutput->stop();
}
