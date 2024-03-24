#include "thread_3.h"

thread_3::thread_3(QObject* parent):
    QThread(parent)
{
    
}

void thread_3::run()
{
    while(1){
        emit flush();
        sleep(3);
    }
}
