/********************************************************************************
** Form generated from reading UI file 'privatechat.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRIVATECHAT_H
#define UI_PRIVATECHAT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_privateChat
{
public:
    QTextEdit *textEdit;
    QLineEdit *lineEdit;
    QPushButton *pushButton;

    void setupUi(QWidget *privateChat)
    {
        if (privateChat->objectName().isEmpty())
            privateChat->setObjectName(QStringLiteral("privateChat"));
        privateChat->resize(400, 300);
        textEdit = new QTextEdit(privateChat);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setEnabled(false);
        textEdit->setGeometry(QRect(10, 10, 381, 241));
        lineEdit = new QLineEdit(privateChat);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(10, 260, 301, 31));
        pushButton = new QPushButton(privateChat);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(320, 260, 75, 31));

        retranslateUi(privateChat);

        QMetaObject::connectSlotsByName(privateChat);
    } // setupUi

    void retranslateUi(QWidget *privateChat)
    {
        privateChat->setWindowTitle(QApplication::translate("privateChat", "Form", nullptr));
        pushButton->setText(QApplication::translate("privateChat", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class privateChat: public Ui_privateChat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRIVATECHAT_H
