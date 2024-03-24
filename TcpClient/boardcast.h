#ifndef BOARDCAST_H
#define BOARDCAST_H

#include <QWidget>
#include <QUdpSocket>
#include <QHostAddress>
#include <QCamera>
#include <QCameraInfo>
#include <QBuffer>
#include <QByteArray>
#include <QImageReader>
#include "abstractvideosyrface.h"
#include "protocol.h"
#include "dialog.h"
#include <QDialog>
#include <QVector>
#include <QLabel>
#include <QPoint>
#include <QInputDialog>
#include <QAudioInput>
#include <QAudioOutput>
#include <ctime>
#include <cstdlib>

#include "thread.h"

namespace Ui {
class boardcast;
}

class boardcast : public QWidget
{
    Q_OBJECT
    
public:
    explicit boardcast(QWidget *parent = 0);
    ~boardcast();
    static boardcast& getInterance();
    void flushMemberInRoom(PDU* pdu);
    void speakRespond(PDU* pdu);
    void setgoldCoin(PDU* pdu);
    void rechargeRespond(PDU* pdu);
    void giveFlowerRespond(PDU* pdu);
    void giveRocketRespond(PDU* pdu);
    void giveAircraftRespond(PDU* pdu);
    void closeOnCameraButton();//关闭开启摄像机的按钮
    void useOnCameraButton();//使用开启摄像机的按钮
    
    void joinDifferentGroup();

    
public slots:
    void onReadyReadVideo();
    
    void video_rec();
    
    void receive();
    
    void bullet_chat();//弹幕移动，注销
    
    void moveFlower();
    
    void moveAircra();
    
    void moveRocket();

    void sendImage(QImage image);
    
    
protected:
    virtual void closeEvent(QCloseEvent*ev);

    
private slots:
    void on_pushButton_6_clicked();
    
    void on_pushButton_clicked();
    
    void on_pushButton_2_clicked();
    
    void on_pushButton_5_clicked();
    
    void on_pushButton_8_clicked();
    
    void on_pushButton_9_clicked();
    
    void on_pushButton_7_clicked();
    
    void on_pushButton_10_clicked();
    
    void on_pushButton_3_clicked();
    
    void on_pushButton_4_clicked();
    
    void on_pushButton_11_clicked();
    
    void on_pushButton_12_clicked();
    
private:
    Ui::boardcast *ui;
    QUdpSocket* sender; //发送文本和视频
    QUdpSocket* reciver; //接收文本和视频
    
    QUdpSocket* send_video;//发送音频
    QUdpSocket* reci_video;//接收音频
    
    QCamera* camera;
    AbstractVideoSurface* abs;
    
    QAudioInput *audioInput;
    QAudioOutput *audioOutput;
    QIODevice* audioInputIODevice;
    QIODevice* audioOutputIODevice;
    
    
    QHostAddress addr;
    QString selfName;
    Dialog* m_pDialog;
    void moveX(QLabel* lb);
    void moveY(QLabel* lb);
    Thread* t;
    Thread* gift_thread_flower;
    Thread* gift_thread_rocket;
    Thread* gift_thread_aircra;
    QVector<QLabel*> v;
    
    QPoint pos_lb_flower;
    QPoint pos_lb_rocket;
    QPoint pos_lb_air;
};

#endif // BOARDCAST_H
