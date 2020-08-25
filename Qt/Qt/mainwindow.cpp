#include "mainwindow.h"
#include "ui_mainwindow.h"
//wyh below
#include "CTabWidget.h"
#include "CWidget.h"
#include <QtGui>
#include<QGridLayout>

#include <fstream>
#include <stdlib.h>
#include <QtScript>
#include <QDebug>
using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSplitter *leftSplitter = new QSplitter(Qt::Vertical);
    QSplitter *leftSplitter1 = new QSplitter(Qt::Vertical);
    leftSplitter1->addWidget(ui->label_3);

    QPixmap pixmap(":/new/prefix1/Icons/none.png");
    ui->label_3->setPixmap(pixmap);

    QSplitter *bottomSplitter = new QSplitter(Qt::Vertical);
    bottomSplitter->addWidget(ui->label_2);

    bottomSplitter->addWidget(ui->textBrowser);
    ui->textBrowser->setFixedWidth(300);
   //  ui->textBrowser->setFixedHeight(500);

    bottomSplitter->setStretchFactor(1, 1);


    leftSplitter1->addWidget(ui->label);


    leftSplitter1->addWidget(ui->pushButton);
    leftSplitter1->addWidget(ui->pushButton_2);
    leftSplitter->addWidget(leftSplitter1);
    leftSplitter->addWidget(bottomSplitter);
    leftSplitter->setStretchFactor(1, 1);

    ui->label->setText("请选择.cpp文件");
    QSplitter *rightSplitter = new QSplitter(Qt::Horizontal);
   // rightSplitter->addWidget(leftSplitter);
    ui->tabWidget->clear();
    rightSplitter->addWidget(ui->tabWidget);        // delete after imerge with wyh

    //rightSplitter->addWidget(ui->tabWidget1);
    rightSplitter->setStretchFactor(1, 1);

    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal);
    mainSplitter->addWidget( leftSplitter);

    mainSplitter->addWidget(rightSplitter);


    setCentralWidget(mainSplitter);
    mFileName = "";

    //wyh below
     tabWidget = new CTabWidget(ui->tabWidget);
     tabWidget->setMovable(true);
     tabWidget->setTabsClosable(true);
     tabWidget->setFixedHeight(643);
     tabWidget->setFixedWidth(988);
     //添加2个tab页
     t1 = new CTextBrowser;
     t2 = new CTextBrowser;



     tabWidget->addTab(t1,"查看源码");           //need to load data and info here
     tabWidget->addTab(t2,"错误报告");           //same as above
     //setCentralWidget(tabWidget);

     connect(t1, SIGNAL(setQTextBrowser(QMouseEvent*)), this, SLOT(txtbrowser_click_t1(QMouseEvent*)));
 connect(t2, SIGNAL(setQTextBrowser(QMouseEvent*)), this, SLOT(txtbrowser_click_t2(QMouseEvent*)));
     connect(tabWidget->tabBar,SIGNAL(sig_tabDrag(int,QPoint)),this,SLOT(slot_tabDrag(int,QPoint)));
     connect(tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(slot_closeTab(int)));
     connect(tabWidget,SIGNAL(currentChanged(int)),tabWidget,SLOT(setCurrentIndex(int)));


     connect(ui->action_Checker, SIGNAL(triggered()), this, SLOT(build_llvm()));



 }
void MainWindow::txtbrowser_click_t1(QMouseEvent *event){
 int lineno_cur=t1->textCursor().blockNumber()+1;
 //qDebug()<<"lineno_cur"<<lineno_cur<<endl;
 if(errorReportInfo.find(lineno_cur)!=errorReportInfo.end())
 {
   // qDebug()<<errorReportInfo[lineno_cur]<<endl;
    QTextBlock block;
    tabWidget->tabBar->setCurrentIndex(1);
  //  qDebug()<<t2->document()->blockCount()<<endl;
    if(errorReportInfo[lineno_cur]+20>=t2->document()->blockCount())
    {
         block = t2->document()->findBlockByLineNumber(errorReportInfo[lineno_cur]);
    }
    else if(errorReportInfo[lineno_cur]<=30)
    {
         block = t2->document()->findBlockByLineNumber(errorReportInfo[lineno_cur]);
    }
    else
    {
         block = t2->document()->findBlockByLineNumber(errorReportInfo[lineno_cur]);
    }

    t2->setTextCursor(QTextCursor(block));
   QTextBlock ori;
   ori=t2->document()->findBlockByLineNumber(errorReportInfo[lineno_cur]);
    QTextBlockFormat blockFormat = QTextCursor(ori).blockFormat();

     QTextBlockFormat blockFormatremain = QTextCursor(ori).blockFormat();
    blockFormat.setBackground(QColor(255,64,64));
    QTextCursor(ori).setBlockFormat(blockFormat);
    msecSleep(2000);
     QTextCursor(ori).setBlockFormat(blockFormatremain);
    //t2->moveCursor(QTextCursor(block).EndOfBlock);
    //qDebug()<<t2->textCursor().LineUnderCursor<<endl;
    //t2->document()->find
 }
 //t1->
}
void MainWindow::msecSleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() <dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void MainWindow::txtbrowser_click_t2(QMouseEvent *event){
 int lineno_cur=t2->textCursor().blockNumber()+1;
// qDebug()<<"lineno_cur t2"<<lineno_cur<<endl;

 //t1->
}
 MainWindow::~MainWindow()
 {
     delete ui;
 }

 void MainWindow::on_pushButton_clicked()
 {

     QString fileName = QFileDialog::getOpenFileName(
         this, tr("打开文件"),
         "../../tests/TemplateChecker", tr("cpp files(*.cpp);;"));

     if (fileName.isEmpty())
     {
         return;
     }
     else
     {
         if (fileName != "")
         {
             QStringList t = fileName.split("/");

             mFileName = t.back();
             ui->label->setText(mFileName);
             QPixmap pixmap(":/new/prefix1/Icons/file.png");
             ui->label_3->setPixmap(pixmap);
         }
         else
         {
             QMessageBox mesg;
             mesg.warning(this, "警告", "文件名为空!");
         }
     }
 }
 void MainWindow::build_llvm()
 {
     QProcess p(0);
      QString strTemp="";
      QDateTime current_date_time =QDateTime::currentDateTime();
      QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
       ui->textBrowser->append(current_date+"\n../../cmake-build-debug$ ninja\n");
     p.setWorkingDirectory("../../cmake-build-debug");
         p.start("ninja");
         p.waitForStarted();
         p.waitForFinished();

        strTemp=QString::fromLocal8Bit(p.readAllStandardOutput());
        QDateTime current_date_time1 =QDateTime::currentDateTime();
        QString current_date1 =current_date_time1.toString("yyyy.MM.dd hh:mm:ss.zzz");
         ui->textBrowser->append(current_date1+"\n"+strTemp+"\n");
            p.close();
            //run_test();
 }
 void MainWindow::run_test()
 {
     QString strTemp="";
     QProcess p3(0);
     QDateTime current_date_time =QDateTime::currentDateTime();
     QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
   p3.setWorkingDirectory("../../tests/TemplateChecker");
   ui->textBrowser->append(current_date+"\nclang++ -emit-ast -c "+mFileName+"\n");
  // p3.start("clang++ -emit-ast -c "+mFileName);
      p3.waitForStarted();
      p3.waitForFinished();
     strTemp=QString::fromLocal8Bit(p3.readAllStandardOutput());
     QDateTime current_date_time3 =QDateTime::currentDateTime();
     QString current_date3 =current_date_time3.toString("yyyy.MM.dd hh:mm:ss.zzz");

     if(strTemp.count()!=0)
      ui->textBrowser->append(current_date3+"\n"+strTemp+"\n");


       p3.close();
     QProcess p2(0);
     QDateTime current_date_time1 =QDateTime::currentDateTime();
     QString current_date1 =current_date_time1.toString("yyyy.MM.dd hh:mm:ss.zzz");
     ui->textBrowser->append(current_date1+"\n../../cmake-build-debug/tools/TemplateChecker/TemplateChecker astList.txt config.txt\n");
                     p2.setWorkingDirectory("../../tests/TemplateChecker");
                     p2.start("../../cmake-build-debug/tools/TemplateChecker/TemplateChecker astList.txt config.txt");
                        p2.waitForStarted();
                        p2.waitForFinished();
                       strTemp=QString::fromLocal8Bit(p2.readAllStandardOutput());
                       QDateTime current_date_time2 =QDateTime::currentDateTime();
                       QString current_date2 =current_date_time2.toString("yyyy.MM.dd hh:mm:ss.zzz");
                        strTemp.append(QString::fromLocal8Bit(p2.readAllStandardError()));
                        if(strTemp.count()!=0)
                        {
                         ui->textBrowser->append(current_date2+"\n"+strTemp+"\n");
                        }


                         p2.close();

 }
 void MainWindow::on_pushButton_2_clicked()
 {
     if (mFileName != "")
     {
         QString strTemp = "";
         //加入界面现实代码。设置 t1_tb和t2_tb的settext。

         /************************读取被测试源码******************/
         QMap<int,QString> example_code;

         QFile file_1("../../tests/TemplateChecker/"+mFileName);
         if (!file_1.open(QFile::ReadOnly | QFile::Text))
             qDebug() << "Can not open";

         QTextStream in_1(&file_1);
         example_code[0]=in_1.readLine();     //读取一行放到字符串里
         int count = 0;
         while (!example_code[count].isNull()) //字符串有内容
         {
             count++;
             example_code[count] = in_1.readLine(); //循环读取下行
         }
         file_1.close();


         /**********************读取报错信息*********************/
         QFile file_2("../../tests/TemplateChecker/error_result.txt");
         if (!file_2.open(QFile::ReadOnly | QFile::Text))
             qDebug() << "Can not open";

         QTextStream in_2(&file_2);
         QString result_all=in_2.readAll();
         file_2.close();
         //t2->findChild<QTextBrowser *>("tab_2_tb")->append(result_all);
        // t2->findChild<QTextBrowser*>("tab_2_tb")->show();


         /******************拆分报错信息*************************/
         QMap<int,int> linenum;
         QMap<int,QString> errorlineinfo;

         int report_line=1;

         QStringList result=result_all.split("--------------------------------------\n");
         QStringList result1=result_all.split("\n");
         for(int i=1;i<result.size();i++)
         {

             QStringList temp_1=result[i].split("\n");   //逐行拆分
             // qDebug()<<temp_1.size()<<endl;

             QString temp_2=temp_1[0];
             QStringList temp_3=temp_2.split(":");       //逐词拆分

            // qDebug()<<temp_3[1]<<endl;
             if(temp_3[3]==" error")
             {
                 linenum.insert(temp_3[1].toInt(),1);
                 if(errorlineinfo.find(temp_3[1].toInt())!=errorlineinfo.end())
                 {
                    errorlineinfo.insert(temp_3[1].toInt(),"");

                 }
                 errorReportInfo.insert(temp_3[1].toInt(),report_line);
                 //qDebug()<<"report "<<temp_3[1].toInt()<<" "<<report_line<<endl;
                 errorlineinfo[temp_3[1].toInt()]+=temp_1[0];
             }
            report_line+=temp_1.size();

         }

         /************读取自定义报错信息*************/
         QMap<int,QString> user_def;

         //打开文件
         QFile file_alter("../../tests/TemplateChecker/alter_info.txt");
         if (!file_alter.open(QFile::ReadOnly | QFile::Text))
             qDebug() << "Can not open";

         //读入自定义信息
         QTextStream in_3(&file_alter);
         QString all_info=in_3.readAll();
         QStringList error_info=all_info.split("***\n");     //逐条拆分
         //qDebug()<<error_info.size();
         for(int i=0;i<error_info.size();i++)
         {
             QStringList temp=error_info[i].split("\n");     //逐行拆分
             //qDebug()<<temp;
             if(temp.size()>1)
             {
                 QString alter_info;
                 for(int j=1;j<temp.size();j++)
                     alter_info+=temp[j];
                 //qDebug()<<alter_info;
                 user_def[i]=alter_info;
             }
         }
         file_alter.close();

         /************逐条输出被报错信息，输出对应自定义信息并标红*************/

         QString resultstring="";

         //t2->append(result[0]);
         //t2_tb->append("--------------------------------------");
         QString scount1=QString::number(result1.size());
         int digit1=scount1.size();
         QString space1="  ";

         for(int i=0;i<result1.size();i++)
         {
              QString snum=QString::number(i+1);      //将行号转为字符串并右对齐
                 int temp=snum.size();
                 while(temp<digit1)
                 {
                     snum=space1+snum;
                     temp++;
                 }
                 QString snum_space="      ";
                  QString snum_space1="  ";
                  QString snum_temp=snum;
                  stringToHtmlFilter(snum_temp);
                  stringToHtmlFilter(snum_space);
                  stringToHtmlFilter(snum_space1);
                   stringToHtmlFilter(result1[i]);
             if(i%2==0)
            {
             resultstring+="<div style=\"overflow:visible;float:left;display:inline;background:#F8F8F8;\"><span style=\"background:black;color:white; background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\
                    stop:0 #F0F0F0, stop:0.1 #DDD,stop:0.5 #888,stop:0.9 #DDD, stop:1 #F0F0F0)\">"+snum_space1+snum_temp+snum_space+"</span><span>"+result1[i]+"</span></div>";

             }
             else if(i%2==1)
            {
             resultstring+="<div style=\"overflow:visible;float:left;display:inline;background:#F0F0F0;\"><span style=\"background:black;color:white; background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\
                    stop:0 #F0F0F0, stop:0.1 #DDD,stop:0.5 #888,stop:0.9 #DDD, stop:1 #F0F0F0)\">"+snum_space1+snum_temp+snum_space+"</span><span>"+result1[i]+"</span></div>";

             }


         }
         t1->setFontFamily("Microsoft YaHei");
         t2->setText(resultstring);
         t2->show();
         /*
          *
             QStringList temp_1=result[i].split("\n");   //逐行拆分
             QString temp_2=temp_1[0];
             QStringList temp_3=temp_2.split(":");       //逐词拆分


         if(temp_3[4].contains("Miss memory realese")&&user_def[0]!="")
         {
             QString output="User-defined error reporting: "+user_def[0];
             stringToHtmlFilter(output);
             t2->append("<font color = 'red'>" + output+"<font>");
         }
         else if(temp_3[4].contains("Find Null Pointer Dereference Fault")&&user_def[1]!="")
         {
             QString output="User-defined error reporting: "+user_def[1];
             stringToHtmlFilter(output);
             t2->append("<font color = 'red'>" + output+"<font>");
         }
         else if(temp_3[4].contains("out of range")&&user_def[2]!="")
         {
             QString output="User-defined error reporting: "+user_def[2];
             stringToHtmlFilter(output);
             t2->append("<font color = 'red'>" + output+"<font>");
         }
         else if(temp_3[4].contains("Unintialized Varirbale")&&user_def[3]!="")
         {
             QString output="User-defined error reporting: "+user_def[3];
             stringToHtmlFilter(output);
             t2->append("<font color = 'red'>" + output+"<font>");
         }
         else if((temp_3[4].contains("format data")||temp_3[4].contains("copy data"))&&user_def[4]!="")
         {
             QString output="User-defined error reporting: "+user_def[4];
             stringToHtmlFilter(output);
             t2->append("<font color = 'red'>" + output+"<font>");
         }*/

         /************输出被测源码，并对错误行标红*************/
         QString scount=QString::number(count);
         int digit=scount.size();
         QString space="  ";
         //i_linenum=0;
         QString total="";
         for(int i=0;i<count;i++)    //分别对每一行源码进行处理
         {
             QString snum=QString::number(i+1);      //将行号转为字符串并右对齐
             int temp=snum.size();
             while(temp<digit)
             {
                 snum=space+snum;
                 temp++;
             }

            // example_code[i]=snum+"      "+example_code[i];
             stringToHtmlFilter(example_code[i]);
             QString snum_space="      ";
              QString snum_space1="  ";
             QString snum_temp=snum;
             stringToHtmlFilter(snum_temp);
             stringToHtmlFilter(snum_space);
              stringToHtmlFilter(snum_space1);
              // "<span style=\"float:left;font-family:Segoe UI;background:#888888;\">"+snum_space+
             if(linenum.find(i+1)!=linenum.end())   //错误源码，标红
             {
                  stringToHtmlFilter(errorlineinfo[i+1]);

                  if(i%2==0)
                 {total+="\
                         <div style=\"overflow:visible;float:left;display:inline;background:#F8F8F8;\"><span style=\"background:black;color:white; background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\
                              stop:0 #F0F0F0, stop:0.1 #DDD,stop:0.5 #888,stop:0.9 #DDD, stop:1 #F0F0F0)\">"+snum_space1+snum_temp+snum_space+"</span><span style=\"text-decoration: underline;color: red;\" title="+ errorlineinfo[i+1]+">"+example_code[i]+"</span></div>";

                   }
                  else if(i%2==1)
                 {total+="\
                         <div style=\"overflow:visible;float:left;display:inline;background:#F0F0F0;\"><span style=\"color:white;\
                              background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\
                                                stop:0 #F0F0F0, stop:0.1 #DDD,stop:0.5 #888,stop:0.9 #DDD, stop:1 #F0F0F0)\">"+snum_space1+snum_temp+snum_space+"</span><span style=\"text-decoration: underline;color: red;\" title="+ errorlineinfo[i+1]+">"+example_code[i]+"</span></div>";

                   }
                 //t1_tb->append(example_code[i]);

             }
             else
             {
                 if(i%2==0)
                {
                 total+="<div style=\"overflow:visible;float:left;display:inline;background:#F8F8F8;\"><span style=\"background:black;color:white; background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\
                        stop:0 #F0F0F0, stop:0.1 #DDD,stop:0.5 #888,stop:0.9 #DDD, stop:1 #F0F0F0)\">"+snum_space1+snum_temp+snum_space+"</span><span>"+example_code[i]+"</span></div>";

                 }
                 else if(i%2==1)
                {
                 total+="<div style=\"overflow:visible;float:left;display:inline;background:#F0F0F0;\"><span style=\"background:black;color:white; background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\
                        stop:0 #F0F0F0, stop:0.1 #DDD,stop:0.5 #888,stop:0.9 #DDD, stop:1 #F0F0F0)\">"+snum_space1+snum_temp+snum_space+"</span><span>"+example_code[i]+"</span></div>";

                 }
             }
             //if(i%2==1){
               //  qDebug()<<"hi"<<endl;
                 //t1->append(total);
                 //total="";
             //}

//t1->setText();
         }
         t1->setFontFamily("Microsoft YaHei");
         t1->setText(total);
        t1->show();
        //int lineno_cur=t1->textCursor().blockNumber();

       // t1->moveCursor();
           //QTextBlock block = t1->document()->findBlockByNumber(nLineNum);
        QTextBlock block = t2->document()->findBlockByNumber(60);

        t2->setTextCursor(QTextCursor(block));
         //  p->setTextCursor(QTextCursor(block));
          //t1->moveCursor(QTextCursor::);
        // QString test="a test!";
         //t1->setToolTip(test);

     }
     else
     {
         QMessageBox mesg;
         mesg.warning(this, "警告", "文件名为空!");
     }
 }

 void MainWindow::stringToHtmlFilter(QString &str)
 {
     str.replace("&","&amp;");
     str.replace(">","&gt;");
     str.replace("<","&lt;");
     str.replace("\"","&quot;");
     str.replace("\'","&#39;");
     str.replace(" ","&nbsp;");
     str.replace("\n","<br>");
     str.replace("\r","<br>");
 }


 void MainWindow::slot_tabDrag(int index,QPoint point)
 {
     CWidget *widget = new CWidget;
     QWidget *draged = tabWidget->widget(index);
     QString windowName = tabWidget->tabText(index);
     //draged->hide();

     connect(widget,SIGNAL(sig_doubleClickedTitleBar()),this,SLOT(slot_tabBarDoubleClicked()));

     QGridLayout *layout = new QGridLayout;
     layout->addWidget(draged);
     widget->setLayout(layout);
     widget->resize(600,400);
     widget->move(point+pos()+tabWidget->pos());
     widget->setWindowTitle(windowName);
     widget->setWindowFlag(Qt::WindowStaysOnTopHint);
     widget->show();

    draged->show();
 }

 void MainWindow::slot_tabBarDoubleClicked()
 {
     CWidget *widget = qobject_cast<CWidget*>(sender());
     QObjectList list = widget->children();
     QTextEdit *edit = NULL;

     for(int i = 0;i<list.count();++i)
     {
         if(list[i]->inherits("QTextEdit"))
         {
             edit = qobject_cast<QTextEdit*>(list[i]);
             break;
         }
     }
     if(edit == NULL)
     {
         return ;
     }

     edit->setParent(tabWidget);
     tabWidget->addTab(edit,widget->windowTitle());

 }

 void MainWindow::slot_closeTab(int index)
 {

     tabWidget->removeTab(index);

 }

// void MainWindow::on_action_alterinfo_triggered()
// {
//     w2.toShow_w2();
// }

 void MainWindow::on_action_alter_triggered()
 {
    // w2.toShow_w2();
     QString strTemp="";
     QProcess p3(0);
     QDateTime current_date_time =QDateTime::currentDateTime();
     QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
   p3.setWorkingDirectory("../..");
   ui->textBrowser->append(current_date+"\ngedit error_detail.txt\n");
   p3.start("gedit error_detail.txt");
      p3.waitForStarted();
      p3.waitForFinished();
     strTemp=QString::fromLocal8Bit(p3.readAllStandardOutput());
     QDateTime current_date_time3 =QDateTime::currentDateTime();
     QString current_date3 =current_date_time3.toString("yyyy.MM.dd hh:mm:ss.zzz");

     if(strTemp.count()!=0)
      ui->textBrowser->append(current_date3+"\n"+strTemp+"\n");
 }
 void MainWindow::on_action_about_triggered()
 {
  QMessageBox::information(NULL, "关于", "C++静态检测器 v1.0\nCWAWU小组 版权所有\n");
 }
 void MainWindow::on_action_code_triggered()
 {
  tabWidget->clear();
  tabWidget->addTab(t1,"查看源码");           //need to load data and info here
  tabWidget->addTab(t2,"错误报告");           //same as above
 }

