#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void loadConfig();
    void address();
    
private:
    Ui::Widget *ui;
    QString m_strIp;
    quint16 m_usPort;

};

#endif // WIDGET_H
