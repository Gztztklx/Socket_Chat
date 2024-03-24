#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QVector>
#include <QLabel>
#include <QPoint>//

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread(QObject* parent = 0);
protected:
    virtual void run();
    
signals:
    void move_label();
    
    void move_flower();
    
    void move_rocket();
    
    void move_aircra();
};

#endif // THREAD_H
