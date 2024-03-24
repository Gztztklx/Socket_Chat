#include "thread.h"
#include <QDebug>

Thread::Thread(QObject* parent):
    QThread(parent)
{
    
}

void Thread::run()
{
    while(1){
        emit move_label();
        emit move_aircra();
        emit move_flower();
        emit move_rocket();
        this->msleep(10);
        //qDebug()<<"子线程在运行";
    }
}
