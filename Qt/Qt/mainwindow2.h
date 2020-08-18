#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

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
class MainWindow2;
}

class MainWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow2(QWidget *parent = nullptr);
    ~MainWindow2();
    void toShow_w2();

private slots:
    void on_cancel_button_clicked();

    void on_ok_button_clicked();

private:
    Ui::MainWindow2 *ui;
};

#endif // MAINWINDOW2_H
