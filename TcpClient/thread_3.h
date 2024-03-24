#ifndef THREAD_3_H
#define THREAD_3_H

#include <QWidget>
#include <QThread>
#include <QVector>
#include <QLabel>
#include <QPoint>

class thread_3 : public QThread
{
    Q_OBJECT
public:
    thread_3(QObject* parent = 0);
    
protected:
    virtual void run();
    
signals:
    flush();
};

#endif // THREAD_3_H
