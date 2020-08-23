#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/windowstyle/stylesheet.qss");
        qss.open(QFile::ReadOnly);
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    MainWindow w;
    w.setWindowTitle("CWAWU C++静态检测器");
    w.showMaximized();

    return a.exec();
}
