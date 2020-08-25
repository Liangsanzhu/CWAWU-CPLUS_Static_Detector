/********************************************************************************
** Form generated from reading UI file 'mainwindow2.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW2_H
#define UI_MAINWINDOW2_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ErrorInformation
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

    void setupUi(QMainWindow *ErrorInformation)
    {
        if (ErrorInformation->objectName().isEmpty())
            ErrorInformation->setObjectName(QStringLiteral("ErrorInformation"));
        ErrorInformation->resize(800, 600);
        centralwidget = new QWidget(ErrorInformation);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        cancel_button = new QPushButton(centralwidget);
        cancel_button->setObjectName(QStringLiteral("cancel_button"));
        cancel_button->setGeometry(QRect(340, 340, 89, 25));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(140, 120, 111, 17));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(140, 190, 111, 17));
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(140, 220, 151, 17));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(140, 150, 151, 17));
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(150, 60, 67, 17));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(370, 60, 111, 17));
        lineEdit_1 = new QLineEdit(centralwidget);
        lineEdit_1->setObjectName(QStringLiteral("lineEdit_1"));
        lineEdit_1->setGeometry(QRect(350, 120, 181, 25));
        ok_button = new QPushButton(centralwidget);
        ok_button->setObjectName(QStringLiteral("ok_button"));
        ok_button->setGeometry(QRect(450, 340, 89, 25));
        lineEdit_2 = new QLineEdit(centralwidget);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(350, 150, 181, 25));
        lineEdit_4 = new QLineEdit(centralwidget);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(350, 210, 181, 25));
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(140, 250, 131, 17));
        lineEdit_3 = new QLineEdit(centralwidget);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(350, 180, 181, 25));
        lineEdit_5 = new QLineEdit(centralwidget);
        lineEdit_5->setObjectName(QStringLiteral("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(350, 240, 181, 25));
        ErrorInformation->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ErrorInformation);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        ErrorInformation->setMenuBar(menubar);
        statusbar = new QStatusBar(ErrorInformation);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        ErrorInformation->setStatusBar(statusbar);

        retranslateUi(ErrorInformation);

        QMetaObject::connectSlotsByName(ErrorInformation);
    } // setupUi

    void retranslateUi(QMainWindow *ErrorInformation)
    {
        ErrorInformation->setWindowTitle(QApplication::translate("ErrorInformation", "MainWindow", Q_NULLPTR));
        cancel_button->setText(QApplication::translate("ErrorInformation", "\345\217\226\346\266\210", Q_NULLPTR));
        label_3->setText(QApplication::translate("ErrorInformation", "Memory_Leak", Q_NULLPTR));
        label_5->setText(QApplication::translate("ErrorInformation", "Out_Index", Q_NULLPTR));
        label_6->setText(QApplication::translate("ErrorInformation", "Variable_Uninitialized", Q_NULLPTR));
        label_4->setText(QApplication::translate("ErrorInformation", "Null_Pointer_Deref", Q_NULLPTR));
        label->setText(QApplication::translate("ErrorInformation", "\346\212\245\351\224\231\344\277\241\346\201\257", Q_NULLPTR));
        label_2->setText(QApplication::translate("ErrorInformation", "\350\207\252\345\256\232\344\271\211\346\212\245\351\224\231\346\226\207\345\255\227", Q_NULLPTR));
        ok_button->setText(QApplication::translate("ErrorInformation", "\347\241\256\350\256\244", Q_NULLPTR));
        label_7->setText(QApplication::translate("ErrorInformation", "Buffer_Overflow", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ErrorInformation: public Ui_ErrorInformation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW2_H
