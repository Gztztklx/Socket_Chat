#include "widget.h"
#include <QApplication>
#include "opedb.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OpeDB::getInstance().init();
    Widget w;
    w.show();
    
    return a.exec();
}
