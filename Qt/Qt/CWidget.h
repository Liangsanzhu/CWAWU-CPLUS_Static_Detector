#ifndef CWIDGET_H
#define CWIDGET_H

#include <QtGui>
#include <QWidget>

class CWidget:public QWidget
{
    Q_OBJECT
public:
    CWidget(QWidget* = 0);
    ~CWidget();
protected:
    bool event(QEvent *);
signals:
    void sig_doubleClickedTitleBar();  //被双击时发射的信号
};

#endif // CWIDGET_H
