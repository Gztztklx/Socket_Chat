#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QIcon>
#include "protocol.h"
#include "widget.h"


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    
private slots:
    void on_pushButton_4_clicked();
    
    void on_pushButton_5_clicked();
    
    void on_pushButton_6_clicked();
    
private:
    Ui::Dialog *ui;
    QString selfName;
    QHostAddress addr;
};

#endif // DIALOG_H
