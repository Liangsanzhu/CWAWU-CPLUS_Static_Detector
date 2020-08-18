/********************************************************************************
** Form generated from reading UI file 'mainwindow2.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW2_H
#define UI_MAINWINDOW2_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow2
{
public:
    QWidget *centralwidget;
    QPushButton *cancel_button;
    QLabel *label_3;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_4;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit_1;
    QPushButton *ok_button;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_4;
    QLabel *label_7;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_5;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow2)
    {
        if (MainWindow2->objectName().isEmpty())
            MainWindow2->setObjectName(QString::fromUtf8("MainWindow2"));
        MainWindow2->resize(800, 600);
        centralwidget = new QWidget(MainWindow2);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        cancel_button = new QPushButton(centralwidget);
        cancel_button->setObjectName(QString::fromUtf8("cancel_button"));
        cancel_button->setGeometry(QRect(340, 340, 89, 25));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(140, 120, 111, 17));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(140, 190, 111, 17));
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(140, 220, 151, 17));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(140, 150, 151, 17));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(150, 60, 67, 17));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(370, 60, 111, 17));
        lineEdit_1 = new QLineEdit(centralwidget);
        lineEdit_1->setObjectName(QString::fromUtf8("lineEdit_1"));
        lineEdit_1->setGeometry(QRect(350, 120, 181, 25));
        ok_button = new QPushButton(centralwidget);
        ok_button->setObjectName(QString::fromUtf8("ok_button"));
        ok_button->setGeometry(QRect(450, 340, 89, 25));
        lineEdit_2 = new QLineEdit(centralwidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(350, 150, 181, 25));
        lineEdit_4 = new QLineEdit(centralwidget);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(350, 210, 181, 25));
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(140, 250, 131, 17));
        lineEdit_3 = new QLineEdit(centralwidget);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(350, 180, 181, 25));
        lineEdit_5 = new QLineEdit(centralwidget);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(350, 240, 181, 25));
        MainWindow2->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow2);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 28));
        MainWindow2->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow2);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow2->setStatusBar(statusbar);

        retranslateUi(MainWindow2);

        QMetaObject::connectSlotsByName(MainWindow2);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow2)
    {
        MainWindow2->setWindowTitle(QCoreApplication::translate("MainWindow2", "MainWindow", nullptr));
        cancel_button->setText(QCoreApplication::translate("MainWindow2", "\345\217\226\346\266\210", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow2", "Memory_Leak", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow2", "Out_Index", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow2", "Variable_Uninitialized", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow2", "Null_Pointer_Deref", nullptr));
        label->setText(QCoreApplication::translate("MainWindow2", "\346\212\245\351\224\231\344\277\241\346\201\257", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow2", "\350\207\252\345\256\232\344\271\211\346\212\245\351\224\231\346\226\207\345\255\227", nullptr));
        ok_button->setText(QCoreApplication::translate("MainWindow2", "\347\241\256\350\256\244", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow2", "Buffer_Overflow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow2: public Ui_MainWindow2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW2_H
