#ifndef CTEXTBOWSER_H
#define CTEXTBOWSER_H
#include <QTextBrowser>
#include <QMouseEvent>

class CTextBrowser : public QTextBrowser
{
    Q_OBJECT

public:
    CTextBrowser(QWidget *parent = 0);
    ~CTextBrowser();
    void mouseDoubleClickEvent(QMouseEvent *event);
     //bool eventFilter(QObject *obj, QEvent *event);
signals:
    void setQTextBrowser(QMouseEvent *event);
};

#endif // CTEXTBOWSER_H
