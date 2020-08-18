#include "mainwindow2.h"
#include "ui_mainwindow2.h"

MainWindow2::MainWindow2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow2)
{
    ui->setupUi(this);
}

MainWindow2::~MainWindow2()
{
    delete ui;
}

void MainWindow2::on_cancel_button_clicked()
{
    this->close();
}

void MainWindow2::on_ok_button_clicked()
{
    QVector<QLineEdit*> alter_text={ui->lineEdit_1,ui->lineEdit_2,ui->lineEdit_3,ui->lineEdit_4,ui->lineEdit_5};
    QVector<QLabel*> error_label={ui->label_3,ui->label_4,ui->label_5,ui->label_6,ui->label_7};

    //打开文件

    QFile file_alter("../../tests/TemplateChecker/alter_info.txt");
    if (!file_alter.open(QFile::WriteOnly | QFile::Text|QFile::Truncate))
        qDebug() << "Can not open";

    //将lineEdit中自定义信息读入文件
    QTextStream out(&file_alter);
    for(int i=0;i<alter_text.size();i++)
    {
        QString temp_1=error_label[i]->text();
        QString temp_2=alter_text[i]->text();
        out<<temp_1<<endl;

        if(temp_2!="")
        {
            out<<temp_2<<endl;
        }
        if(i+1!=alter_text.size())
        {
            out<<"***"<<endl;
        }
    }
    out.flush();
    file_alter.close();
    this->close();
}

void MainWindow2::toShow_w2()
{
    this->show();
    QVector<QLineEdit*> error_text={ui->lineEdit_1,ui->lineEdit_2,ui->lineEdit_3,ui->lineEdit_4,ui->lineEdit_5};
    for(int i=0;i<error_text.size();i++)
    {
        error_text[i]->clear();
    }

    //打开文件
    QFile file_alter("../../tests/TemplateChecker/alter_info.txt");
    if (!file_alter.open(QFile::ReadOnly | QFile::Text))
        qDebug() << "Can not open";

    // 将自定义信息读入lineEdit
    QTextStream in(&file_alter);
    QString all_info=in.readAll();
    QStringList error_info=all_info.split("***\n");
    //qDebug()<<error_info.size();
    for(int i=0;i<error_info.size();i++)
    {
        QStringList temp=error_info[i].split("\n");
        //qDebug()<<temp;
        if(temp.size()>1)
        {
            QString alter_info;
            for(int j=1;j<temp.size();j++)
                alter_info+=temp[j];
            //qDebug()<<alter_info;
            error_text[i]->setText(alter_info);
            error_text[i]->show();
        }
        else
        {
            error_text[i]->clear();
            error_text[i]->show();
        }
    }
    file_alter.close();
}
