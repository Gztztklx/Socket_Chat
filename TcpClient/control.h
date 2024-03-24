#ifndef CONTROL_H
#define CONTROL_H

#include <QWidget>
#include "protocol.h"
#include <QHostAddress>
#include "thread_3.h"


namespace Ui {
class Control;
}

class Control : public QWidget
{
    Q_OBJECT
    
public:
    explicit Control(QWidget *parent = 0);
    ~Control();
    static Control& getIntance();
    void setName();
    void createRoomRespond(PDU*pdu);
    void flushRoomRespond(PDU* pdu);
    QHostAddress getAddr();
    void deleteRoomRequest();
    void getIntoRoomRespond(PDU* pdu);

public slots:
    
    
private slots:
    void on_pushButton_2_clicked();
    
    
    void on_pushButton_clicked();
    
    void on_pushButton_3_clicked();
    
    void on_pushButton_4_clicked();
    
    void on_pushButton_5_clicked();
    
private:
    Ui::Control *ui;
    QString strLoginName;
    QHostAddress addr;
    thread_3* t_3;   
};

#endif // CONTROL_H
