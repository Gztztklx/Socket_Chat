#ifndef PRIVATECHAT_H
#define PRIVATECHAT_H

#include <QWidget>
#include "protocol.h"

namespace Ui {
class privateChat;
}

class privateChat : public QWidget
{
    Q_OBJECT
    
public:
    explicit privateChat(QWidget *parent = 0);
    ~privateChat();
    static privateChat& getPrivateChat();
    void setPerName(QString data);
    void updateMsg(const PDU* pdu);
    
private slots:
    void on_pushButton_clicked();
    
private:
    Ui::privateChat *ui;
    QString perName;
    QString selfName;
};

#endif // PRIVATECHAT_H
