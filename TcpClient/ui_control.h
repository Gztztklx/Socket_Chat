/********************************************************************************
** Form generated from reading UI file 'control.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROL_H
#define UI_CONTROL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Control
{
public:
    QLabel *label;
    QPushButton *pushButton;
    QLabel *label_2;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QListWidget *listWidget;
    QPushButton *pushButton_5;

    void setupUi(QWidget *Control)
    {
        if (Control->objectName().isEmpty())
            Control->setObjectName(QStringLiteral("Control"));
        Control->resize(197, 232);
        label = new QLabel(Control);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(11, 11, 48, 16));
        pushButton = new QPushButton(Control);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(110, 140, 75, 41));
        label_2 = new QLabel(Control);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(110, 11, 36, 16));
        pushButton_2 = new QPushButton(Control);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(110, 40, 75, 34));
        pushButton_3 = new QPushButton(Control);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(110, 90, 75, 34));
        pushButton_4 = new QPushButton(Control);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(110, 200, 75, 23));
        listWidget = new QListWidget(Control);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(10, 30, 81, 192));
        pushButton_5 = new QPushButton(Control);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(150, 10, 31, 23));

        retranslateUi(Control);

        QMetaObject::connectSlotsByName(Control);
    } // setupUi

    void retranslateUi(QWidget *Control)
    {
        Control->setWindowTitle(QApplication::translate("Control", "Form", nullptr));
        label->setText(QApplication::translate("Control", "\346\210\277\351\227\264\345\210\227\350\241\250", nullptr));
        pushButton->setText(QApplication::translate("Control", "\345\210\267\346\226\260\n"
"\346\210\277\351\227\264", nullptr));
        label_2->setText(QApplication::translate("Control", "\347\224\250\346\210\267\345\220\215", nullptr));
        pushButton_2->setText(QApplication::translate("Control", "\345\210\233\345\273\272\n"
"\346\210\277\351\227\264", nullptr));
        pushButton_3->setText(QApplication::translate("Control", "\350\277\233\345\205\245\n"
"\346\210\277\351\227\264", nullptr));
        pushButton_4->setText(QApplication::translate("Control", "\346\263\250\351\224\200", nullptr));
        pushButton_5->setText(QApplication::translate("Control", "\345\205\263\351\227\255", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Control: public Ui_Control {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROL_H
