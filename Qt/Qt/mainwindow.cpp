#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

        QSplitter *leftSplitter=new QSplitter(Qt::Vertical);
        leftSplitter->addWidget(ui->label_3);

        QPixmap pixmap(":/new/prefix1/Icons/none.png");
        ui->label_3->setPixmap(pixmap);

        leftSplitter->addWidget(ui->label);

        leftSplitter->addWidget(ui->pushButton);
        leftSplitter->addWidget(ui->pushButton_2);

        leftSplitter->setStretchFactor(1,1);
         ui->label->setText("请选择.cpp文件");
         QSplitter *rightSplitter=new QSplitter(Qt::Horizontal);
         rightSplitter->addWidget(leftSplitter);
         rightSplitter->addWidget(ui->tabWidget);


         rightSplitter->setStretchFactor(1,1);
         QSplitter *bottomSplitter=new QSplitter(Qt::Vertical);
         bottomSplitter->addWidget(ui->label_2);
         bottomSplitter->addWidget(ui->textBrowser);

         bottomSplitter->setStretchFactor(1,1);
        QSplitter *mainSplitter=new QSplitter(Qt::Vertical);
        mainSplitter->addWidget(rightSplitter);

        mainSplitter->addWidget(bottomSplitter);

        mainSplitter->setStretchFactor(0,2);
         mainSplitter->setStretchFactor(1,1);

        setCentralWidget(mainSplitter);
        mFileName="";

        connect(ui->action_Checker,SIGNAL(triggered()), this, SLOT(build_llvm()));
ui->tabWidget_3->clear();


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

       if(fileName.isEmpty())
       {
           return;
       }
       else
       {
           if(fileName!="")
           {
              QStringList t= fileName.split("/");

              mFileName=t.back();
              ui->label->setText(mFileName);
                QPixmap pixmap(":/new/prefix1/Icons/file.png");
               ui->label_3->setPixmap(pixmap);
           }
           else
           {
               QMessageBox mesg;
               mesg.warning(this,"警告","文件名为空!");

           }

       }


}
void MainWindow::split_result(QString a)
{
    QStringList t=a.split("****\n");
    int i=0;
    ui->tabWidget_2->clear();

    for(auto it=t.begin();it!=t.end();it++)
    {
        QTextBrowser*qb=new QTextBrowser();
        qb->setText(*it);
        QString temp;

        if(i!=0)
        ui->tabWidget_2->addTab(qb,"路径"+ temp.number(i));
        else
              ui->tabWidget_2->addTab(qb,"所有结果");
        i++;
    }
}
void MainWindow::split_code(QString a)
{
    QStringList t=a.split("****\n");
    ui->tabWidget_3->clear();
    QMap<int,QMap<int,int>> map;

    int i=0;
        auto it=t.begin();
        it++;
    for(;it!=t.end();it++)
    {
        i++;

        QString temp=*it;
        QStringList temp_t=temp.split("\n");
        QString strTemp;
        for(auto at=temp_t.begin();at!=temp_t.end();at++)
        {
            QString p=*at;
            int num=p.toInt()-1;
            qDebug()<<i<<" "<<num<<endl;
            map[i][num]=1;

        }

        int j=0;
        auto at=source_code.begin();

        for(;at!=source_code.end();at++)
        {

            QString q=*at;

            q.replace("<","&lt;");
            q.replace(">","&gt;");

            if(map[i].find(j)==map[i].end())
            {

                strTemp.append("<br><font color=\"#000000\">" + q +"</font></br>");

            }
            else
            {
                strTemp.append("<br><font color=\"#FFFF00\">" + q +"</font></br> ");

            }
            j++;
        }
        QTextBrowser*qb=new QTextBrowser();
        qb->setText(strTemp);
        ui->tabWidget_3->addTab(qb,"路径"+ temp.number(i));

    }
}
 void MainWindow::build_llvm()
 {
     QProcess p(0);
      QString strTemp="";
     p.setWorkingDirectory("../../cmake-build-debug");
         p.start("ninja");
         p.waitForStarted();
         p.waitForFinished();

        strTemp=QString::fromLocal8Bit(p.readAllStandardOutput());
        ui->textBrowser->append("../../cmake-build-debug$ ninja");
         ui->textBrowser->append(strTemp);
            p.close();
 }
void MainWindow::on_pushButton_2_clicked()
{
    if(mFileName!="")
    {

         QString strTemp="";

            /*QProcess p1(0);
             p1.setWorkingDirectory("../../SE-Experiment-master/tests/TemplateChecker");
             p1.execute("clang++ -emit-ast -c "+mFileName);
                p1.waitForStarted();
                p1.waitForFinished();
               strTemp=QString::fromLocal8Bit(p1.readAllStandardError());
                ui->textBrowser->append(strTemp);
                p1.close();*/

         QProcess p3(0);
       p3.setWorkingDirectory("../../tests/TemplateChecker");
       p3.start("cat "+mFileName);
          p3.waitForStarted();
          p3.waitForFinished();
         strTemp=QString::fromLocal8Bit(p3.readAllStandardOutput());

         ui->tab->findChild<QTextBrowser*>("tab_1_tb")->setText(strTemp);
           p3.close();
           source_code=strTemp.split("\n");

                  QProcess p2(0);
                p2.setWorkingDirectory("../../tests/TemplateChecker");
                p2.start("../../cmake-build-debug/tools/TemplateChecker/TemplateChecker astList.txt config.txt");
                   p2.waitForStarted();
                   p2.waitForFinished();
                  strTemp=QString::fromLocal8Bit(p2.readAllStandardOutput());

                   strTemp.append(QString::fromLocal8Bit(p2.readAllStandardError()));
                 QString strTemp1=strTemp.split("&&&&\n")[0];
                    QString strTemp2=strTemp.split("&&&&\n")[1];
                    split_code(strTemp1);
                   split_result(strTemp2);


                    p2.close();


    }
    else
    {
        QMessageBox mesg;
        mesg.warning(this,"警告","文件名为空!");

     }
}
