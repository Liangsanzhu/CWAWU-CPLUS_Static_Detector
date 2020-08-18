#include "mainwindow.h"
#include "ui_mainwindow.h"
//wyh below
#include "CTabWidget.h"
#include "CWidget.h"
#include <QtGui>
#include<QGridLayout>

#include <fstream>
#include <stdlib.h>
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
     t1 = new QTextBrowser;
     t2 = new QTextBrowser;
     tabWidget->addTab(t1,"Review Source Code");           //need to load data and info here
     tabWidget->addTab(t2,"Error Report");           //same as above
     //setCentralWidget(tabWidget);

     connect(tabWidget->tabBar,SIGNAL(sig_tabDrag(int,QPoint)),this,SLOT(slot_tabDrag(int,QPoint)));
     connect(tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(slot_closeTab(int)));
     connect(tabWidget,SIGNAL(currentChanged(int)),tabWidget,SLOT(setCurrentIndex(int)));


     connect(ui->action_Checker, SIGNAL(triggered()), this, SLOT(build_llvm()));

     connect(ui->action_llvm,SIGNAL(triggered()),this,SLOT(on_action_alter_triggered()));


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

 void MainWindow::on_pushButton_2_clicked()
 {
     if (mFileName != "")
     {
         QString strTemp = "";

         //加入界面现实代码。设置 t1_tb和t2_tb的settext。

         /************************读取被测试源码******************/
         QMap<int,QString> example_code;

         QFile file_1("../../tests/TemplateChecker/example.cpp");
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
         int i_linenum=0;
         QStringList result=result_all.split("--------------------------------------\n");
         for(int i=1;i<result.size();i++)
         {

             QStringList temp_1=result[i].split("\n");   //逐行拆分
              errorlineinfo[i-1]=temp_1[0];
             QString temp_2=temp_1[0];
             QStringList temp_3=temp_2.split(":");       //逐词拆分

             if(temp_3[3]==" error")
             {
                 linenum[i_linenum]=temp_3[1].toInt();
                 i_linenum++;
             }
             else if(temp_3[3]==" warning")
             {
                 linenum[i]=0-temp_3[1].toInt();
                 i_linenum++;
             }
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

         t2->append(result[0]);
         //t2_tb->append("--------------------------------------\n");
         for(int i=1;i<result.size();i++)
         {
             t2->append("--------------------------------------\n");
             t2->append(result[i]);

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
             }


         }


         /************输出被测源码，并对错误行标红*************/
         QString scount=QString::number(count);
         int digit=scount.size();
         QString space="  ";
         i_linenum=0;
         for(int i=0;i<count;i++)    //分别对每一行源码进行处理
         {
             QString snum=QString::number(i+1);      //将行号转为字符串并右对齐
             int temp=snum.size();
             while(temp<digit)
             {
                 snum=space+snum;
                 temp++;
             }

             example_code[i]=snum+"      "+example_code[i];
             stringToHtmlFilter(example_code[i]);

             if(i_linenum<linenum.size()&&i+1==linenum[i_linenum])   //错误源码，标红
             {
                  stringToHtmlFilter(errorlineinfo[i_linenum]);
                 t1->append("<span style=\"text-decoration: underline;color: red;\"><p title="+ errorlineinfo[i_linenum]+">"+example_code[i]+"</p></span>");


                 //t1_tb->append(example_code[i]);
                 i_linenum++;
             }
             else if(i_linenum<linenum.size()&&i+1==-linenum[i_linenum])     //警告源码，标黄
             {
                 t1->append("<span style='text-decoration-line: underline; text-decoration-style: wavy;'><font color = 'yellow'>" + example_code[i]+"<font></span>");
                 //t1_tb->append(example_code[i]);
                 i_linenum++;
             }
             else
                 t1->append("<font color = 'black'>" + example_code[i]+"<font>");
             t1->show();
         }

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
     tabWidget->removeTab(index);
     connect(widget,SIGNAL(sig_doubleClickedTitleBar()),this,SLOT(slot_tabBarDoubleClicked()));

     QGridLayout *layout = new QGridLayout;
     layout->addWidget(draged);
     widget->setLayout(layout);
     widget->resize(600,400);
     widget->move(point+pos()+tabWidget->pos());
     widget->setWindowTitle(windowName);
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
     delete widget;
 }

 void MainWindow::slot_closeTab(int index)
 {
     QWidget *draged = tabWidget->widget(index);
     tabWidget->removeTab(index);
     delete draged;
 }

// void MainWindow::on_action_alterinfo_triggered()
// {
//     w2.toShow_w2();
// }

 void MainWindow::on_action_alter_triggered()
 {
     w2.toShow_w2();
 }
