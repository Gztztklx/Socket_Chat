#include "widget.h"
#include <QApplication>
#include <QDebug>
#include <iostream>
using namespace std;
#include <cstdio>

int main(int argc, char *argv[])
{
    srand(time(0));
    QApplication a(argc, argv);
    Widget::getWidget().show();
    
//    Widget& w1 = Widget::getWidget();
    
    qDebug()<<"sizeof:"<<sizeof(ENUM_MSG_TYPE);
    return a.exec();
}
