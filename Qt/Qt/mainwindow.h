#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QSplitter>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QTime>
#include <QDir>
#include <QFile>
#include<QProcess>
#include <QMainWindow>
#include <QTextEdit>


#include "mainwindow2.h"

//wyh below
#include "CTabWidget.h"
#include "CTabBar.h"
#include "ui_mainwindow.h"

#include <fstream>
#include <stdlib.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QMap<int,QStringList> split_result(QString a);
    void split_code(QString a,QString b,QMap<int,QStringList>);
    ~MainWindow();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    //void build_llvm();

    void stringToHtmlFilter(QString &str);

    // wyh below
    void slot_tabBarDoubleClicked();            //响应双击弹回的槽函数
    void slot_tabDrag(int index,QPoint point);  //响应拖动动作的槽函数
    void slot_closeTab(int);                    //关闭tab的槽函数

   // void on_action_alterinfo_triggered();

    void on_action_alter_triggered();

    void txtbrowser_click_t1(int);

private:
    Ui::MainWindow *ui;
    QString mFileName;
    QStringList source_code;
    QTextBrowser* t1;
    QTextBrowser* t2;

protected:
    MainWindow2 w2;

    //wyh below
    CTabWidget *tabWidget;
    CTabWidget *tabWidget1;
};

#endif // MAINWINDOW_H
