/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
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
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(480, 371);
        action_llvm = new QAction(MainWindow);
        action_llvm->setObjectName(QStringLiteral("action_llvm"));
        action_alter = new QAction(MainWindow);
        action_alter->setObjectName(QStringLiteral("action_alter"));
        action_Checker = new QAction(MainWindow);
        action_Checker->setObjectName(QStringLiteral("action_Checker"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(80, 150, 67, 17));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(40, 200, 89, 25));
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(130, 220, 256, 192));
        textBrowser->setMinimumSize(QSize(256, 192));
        textBrowser->setMaximumSize(QSize(16777215, 16777215));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setEnabled(true);
        tabWidget->setGeometry(QRect(110, 20, 351, 311));
        tabWidget->setStyleSheet(QStringLiteral(""));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 220, 67, 17));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(50, 50, 100, 100));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(40, 160, 89, 25));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 480, 28));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        menu_4 = new QMenu(menuBar);
        menu_4->setObjectName(QStringLiteral("menu_4"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menuBar->addAction(menu_4->menuAction());
        menu->addAction(action_llvm);
        menu->addAction(action_alter);
        menu_3->addAction(action_Checker);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        action_llvm->setText(QApplication::translate("MainWindow", "\350\256\276\347\275\256llvm\350\267\257\345\276\204", Q_NULLPTR));
        action_alter->setText(QApplication::translate("MainWindow", "\350\256\276\347\275\256\346\212\245\351\224\231\346\226\207\345\255\227", Q_NULLPTR));
        action_Checker->setText(QApplication::translate("MainWindow", "\347\274\226\350\257\221Checker", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\346\243\200\346\265\213", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "\350\276\223\345\207\272\346\227\245\345\277\227\357\274\232", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\346\226\207\344\273\266", Q_NULLPTR));
        menu->setTitle(QApplication::translate("MainWindow", "\350\256\276\347\275\256", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("MainWindow", "\350\247\206\345\233\276", Q_NULLPTR));
        menu_3->setTitle(QApplication::translate("MainWindow", "\347\274\226\350\257\221", Q_NULLPTR));
        menu_4->setTitle(QApplication::translate("MainWindow", "\345\270\256\345\212\251", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
