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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void split_result(QString a);
    void split_code(QString a);
    ~MainWindow();




private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void build_llvm();


private:
    Ui::MainWindow *ui;
    QString mFileName;
    QStringList source_code;
};

#endif // MAINWINDOW_H
