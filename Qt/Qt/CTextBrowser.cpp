#include"CTextBowser.h"
#include <qdebug.h>
CTextBrowser::CTextBrowser(QWidget *parent) : QTextBrowser(parent){
//qDebug()<<"init"<<endl;
}

CTextBrowser::~CTextBrowser(){}

void CTextBrowser::mouseDoubleClickEvent(QMouseEvent *event){
    //qDebug()<<"hihi"<<endl;
    emit  setQTextBrowser(event);
}
