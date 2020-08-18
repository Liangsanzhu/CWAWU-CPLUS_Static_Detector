#include "CTabWidget.h"

CTabWidget::CTabWidget(QWidget *parent):QTabWidget(parent)
{
    tabBar = new CTabBar;
    setTabBar(tabBar);     //这里是关键，这样用我们自定义的CTabBar替换原来的QTabBar
}

int CTabWidget::setTab(QWidget *before)
{
}
