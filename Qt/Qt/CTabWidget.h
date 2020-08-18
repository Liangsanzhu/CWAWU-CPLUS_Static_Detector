#ifndef CTABWIDGET_H
#define CTABWIDGET_H

#include <QTabWidget>
#include <QtGui>
#include "CTabBar.h"

class CTabWidget :public QTabWidget
{
    Q_OBJECT
public:
    CTabWidget(QWidget* =0);
    int setTab(QWidget *before);
    virtual ~CTabWidget(){};
public:
    CTabBar *tabBar;
};

#endif // CTABWIDGET_H
