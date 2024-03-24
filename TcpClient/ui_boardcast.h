/********************************************************************************
** Form generated from reading UI file 'boardcast.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BOARDCAST_H
#define UI_BOARDCAST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_boardcast
{
public:
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLabel *label_3;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QLabel *label_4;
    QPushButton *pushButton_5;
    QTextEdit *textEdit_2;
    QLabel *label_5;
    QLineEdit *lineEdit;
    QPushButton *pushButton_6;
    QListWidget *listWidget;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QLabel *label_6;
    QTextEdit *textEdit;
    QLabel *label_7;
    QLabel *label_9;
    QPushButton *pushButton_9;
    QLabel *lb_flower;
    QLabel *lb_rocket;
    QLabel *lb_air;
    QPushButton *pushButton_10;
    QPushButton *pushButton_11;
    QPushButton *pushButton_12;

    void setupUi(QWidget *boardcast)
    {
        if (boardcast->objectName().isEmpty())
            boardcast->setObjectName(QStringLiteral("boardcast"));
        boardcast->setEnabled(true);
        boardcast->resize(533, 457);
        label = new QLabel(boardcast);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 271, 281));
        label_2 = new QLabel(boardcast);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(290, 10, 41, 16));
        pushButton = new QPushButton(boardcast);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setEnabled(true);
        pushButton->setGeometry(QRect(290, 40, 41, 23));
        pushButton->setMouseTracking(false);
        pushButton->setTabletTracking(false);
        pushButton->setAcceptDrops(false);
        pushButton->setAutoFillBackground(false);
        pushButton_2 = new QPushButton(boardcast);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setEnabled(true);
        pushButton_2->setGeometry(QRect(290, 70, 41, 23));
        label_3 = new QLabel(boardcast);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(290, 160, 41, 16));
        pushButton_3 = new QPushButton(boardcast);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setEnabled(true);
        pushButton_3->setGeometry(QRect(290, 180, 41, 41));
        pushButton_4 = new QPushButton(boardcast);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setEnabled(true);
        pushButton_4->setGeometry(QRect(290, 222, 41, 41));
        label_4 = new QLabel(boardcast);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(350, 10, 91, 21));
        pushButton_5 = new QPushButton(boardcast);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(440, 10, 71, 23));
        textEdit_2 = new QTextEdit(boardcast);
        textEdit_2->setObjectName(QStringLiteral("textEdit_2"));
        textEdit_2->setEnabled(false);
        textEdit_2->setGeometry(QRect(10, 320, 331, 101));
        textEdit_2->setAcceptDrops(true);
        label_5 = new QLabel(boardcast);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 300, 54, 12));
        lineEdit = new QLineEdit(boardcast);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(11, 432, 421, 20));
        pushButton_6 = new QPushButton(boardcast);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setEnabled(true);
        pushButton_6->setGeometry(QRect(440, 430, 75, 23));
        listWidget = new QListWidget(boardcast);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(350, 40, 91, 221));
        pushButton_7 = new QPushButton(boardcast);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        pushButton_7->setGeometry(QRect(370, 270, 75, 41));
        pushButton_8 = new QPushButton(boardcast);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));
        pushButton_8->setGeometry(QRect(450, 100, 51, 31));
        label_6 = new QLabel(boardcast);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(450, 50, 54, 31));
        textEdit = new QTextEdit(boardcast);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setEnabled(false);
        textEdit->setGeometry(QRect(350, 320, 161, 101));
        label_7 = new QLabel(boardcast);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(453, 290, 51, 20));
        label_9 = new QLabel(boardcast);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(460, 210, 31, 31));
        pushButton_9 = new QPushButton(boardcast);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));
        pushButton_9->setGeometry(QRect(450, 150, 51, 41));
        lb_flower = new QLabel(boardcast);
        lb_flower->setObjectName(QStringLiteral("lb_flower"));
        lb_flower->setGeometry(QRect(90, 380, 61, 71));
        lb_flower->setStyleSheet(QStringLiteral("image: url(:/res/flo.jpg);"));
        lb_rocket = new QLabel(boardcast);
        lb_rocket->setObjectName(QStringLiteral("lb_rocket"));
        lb_rocket->setGeometry(QRect(100, 330, 121, 121));
        lb_rocket->setStyleSheet(QStringLiteral("image: url(:/res/rocket.png);"));
        lb_air = new QLabel(boardcast);
        lb_air->setObjectName(QStringLiteral("lb_air"));
        lb_air->setEnabled(true);
        lb_air->setGeometry(QRect(23, 240, 191, 211));
        lb_air->setStyleSheet(QStringLiteral("border-image: url(:/res/air.png);"));
        pushButton_10 = new QPushButton(boardcast);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));
        pushButton_10->setGeometry(QRect(460, 260, 41, 23));
        pushButton_11 = new QPushButton(boardcast);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));
        pushButton_11->setGeometry(QRect(290, 270, 31, 41));
        pushButton_12 = new QPushButton(boardcast);
        pushButton_12->setObjectName(QStringLiteral("pushButton_12"));
        pushButton_12->setGeometry(QRect(330, 270, 31, 41));

        retranslateUi(boardcast);

        QMetaObject::connectSlotsByName(boardcast);
    } // setupUi

    void retranslateUi(QWidget *boardcast)
    {
        boardcast->setWindowTitle(QApplication::translate("boardcast", "Form", nullptr));
        label->setText(QString());
        label_2->setText(QApplication::translate("boardcast", "\346\221\204\345\203\217\345\244\264", nullptr));
        pushButton->setText(QApplication::translate("boardcast", "\345\274\200", nullptr));
        pushButton_2->setText(QApplication::translate("boardcast", "\345\205\263", nullptr));
        label_3->setText(QApplication::translate("boardcast", "\351\272\246\345\205\213\351\243\216", nullptr));
        pushButton_3->setText(QApplication::translate("boardcast", "\345\274\200\345\247\213\n"
"\347\233\264\346\222\255", nullptr));
        pushButton_4->setText(QApplication::translate("boardcast", "\345\201\234\346\255\242\n"
"\347\233\264\346\222\255", nullptr));
        label_4->setText(QApplication::translate("boardcast", "\350\201\212\345\244\251\345\256\244\346\210\220\345\221\230\345\210\227\350\241\250", nullptr));
        pushButton_5->setText(QApplication::translate("boardcast", "\345\210\267\346\226\260", nullptr));
        label_5->setText(QApplication::translate("boardcast", "\350\201\212\345\244\251\350\256\260\345\275\225", nullptr));
        pushButton_6->setText(QApplication::translate("boardcast", "send", nullptr));
        pushButton_7->setText(QApplication::translate("boardcast", "\345\205\205\345\200\274\344\270\255\345\277\203", nullptr));
        pushButton_8->setText(QApplication::translate("boardcast", "\346\211\223\350\265\217", nullptr));
        label_6->setText(QString());
        label_7->setText(QApplication::translate("boardcast", "\347\263\273\347\273\237\346\266\210\346\201\257", nullptr));
        label_9->setText(QString());
        pushButton_9->setText(QApplication::translate("boardcast", "\344\275\231\351\242\235", nullptr));
        lb_flower->setText(QString());
        lb_rocket->setText(QString());
        lb_air->setText(QString());
        pushButton_10->setText(QApplication::translate("boardcast", "\347\247\201\350\201\212", nullptr));
        pushButton_11->setText(QApplication::translate("boardcast", "\345\274\200\345\247\213\n"
"\346\224\266\345\220\254", nullptr));
        pushButton_12->setText(QApplication::translate("boardcast", "\345\201\234\346\255\242\n"
"\346\224\266\345\220\254", nullptr));
    } // retranslateUi

};

namespace Ui {
    class boardcast: public Ui_boardcast {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOARDCAST_H
