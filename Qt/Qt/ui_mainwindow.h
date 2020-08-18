/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_llvm;
    QAction *action_alter;
    QAction *action_Checker;
    QWidget *centralWidget;
    QLabel *label;
    QPushButton *pushButton_2;
    QTextBrowser *textBrowser;
    QTabWidget *tabWidget;
    QWidget *tab;
    QTextBrowser *tab_1_tb;
    QWidget *tab_2;
    QTextBrowser *tab_2_tb;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(480, 371);
        action_llvm = new QAction(MainWindow);
        action_llvm->setObjectName(QString::fromUtf8("action_llvm"));
        action_alter = new QAction(MainWindow);
        action_alter->setObjectName(QString::fromUtf8("action_alter"));
        action_Checker = new QAction(MainWindow);
        action_Checker->setObjectName(QString::fromUtf8("action_Checker"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(80, 150, 67, 17));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(40, 200, 89, 25));
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(130, 220, 256, 192));
        textBrowser->setMinimumSize(QSize(256, 192));
        textBrowser->setMaximumSize(QSize(16777215, 16777215));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(110, 20, 351, 311));
        tabWidget->setStyleSheet(QString::fromUtf8(""));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tab_1_tb = new QTextBrowser(tab);
        tab_1_tb->setObjectName(QString::fromUtf8("tab_1_tb"));
        tab_1_tb->setGeometry(QRect(0, 0, 570, 280));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tab_1_tb->sizePolicy().hasHeightForWidth());
        tab_1_tb->setSizePolicy(sizePolicy1);
        tab_1_tb->setMinimumSize(QSize(256, 192));
        tab_1_tb->setStyleSheet(QString::fromUtf8("QTabBar::tab{ min-height: 60px; min-width:60px;}"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tab_2_tb = new QTextBrowser(tab_2);
        tab_2_tb->setObjectName(QString::fromUtf8("tab_2_tb"));
        tab_2_tb->setGeometry(QRect(0, 0, 570, 280));
        sizePolicy1.setHeightForWidth(tab_2_tb->sizePolicy().hasHeightForWidth());
        tab_2_tb->setSizePolicy(sizePolicy1);
        tab_2_tb->setMinimumSize(QSize(256, 192));
        tabWidget->addTab(tab_2, QString());
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(40, 220, 67, 17));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(50, 50, 100, 100));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(40, 160, 89, 25));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 480, 28));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        menu_4 = new QMenu(menuBar);
        menu_4->setObjectName(QString::fromUtf8("menu_4"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menuBar->addAction(menu_4->menuAction());
        menu->addAction(action_llvm);
        menu->addAction(action_alter);
        menu_3->addAction(action_Checker);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        action_llvm->setText(QCoreApplication::translate("MainWindow", "\350\256\276\347\275\256llvm\350\267\257\345\276\204", nullptr));
        action_alter->setText(QCoreApplication::translate("MainWindow", "\350\256\276\347\275\256\346\212\245\351\224\231\346\226\207\345\255\227", nullptr));
        action_Checker->setText(QCoreApplication::translate("MainWindow", "\347\274\226\350\257\221Checker", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\346\243\200\346\265\213", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "\346\237\245\347\234\213\346\272\220\347\240\201", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "\345\210\206\346\236\220\347\273\223\346\236\234", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\350\276\223\345\207\272\346\227\245\345\277\227\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\350\256\276\347\275\256", nullptr));
        menu_2->setTitle(QCoreApplication::translate("MainWindow", "\350\247\206\345\233\276", nullptr));
        menu_3->setTitle(QCoreApplication::translate("MainWindow", "\347\274\226\350\257\221", nullptr));
        menu_4->setTitle(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
