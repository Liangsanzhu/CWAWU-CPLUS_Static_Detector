#include "CTabBar.h"
#include <QtGui>

CTabBar::CTabBar(QWidget *parent):QTabBar(parent),pressFlag(false)
{

}

void CTabBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton)
    {
        pressFlag = true;
    }
    QTabBar::mousePressEvent(event);
}

void CTabBar::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton &&pressFlag )
    {
        pressFlag = false;
        if(tabRect(currentIndex()).contains( event->pos()))
            return;
        emit sig_tabDrag(currentIndex(),event->pos());
    }
}
