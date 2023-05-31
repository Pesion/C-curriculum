#include "explain.h"
#include "ui_explain.h"

Explain::Explain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Explain)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/new/picture/设置.png"));
    ui->stackedWidget->setCurrentWidget(ui->page);
    connect(ui->pushButton,&QPushButton::clicked,this,[=](){
        //切换显示第一个子窗口
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->pushButton_2,&QPushButton::clicked,this,[=](){
        //切换显示第二个子窗口
        ui->stackedWidget->setCurrentIndex(1);
    });

}

Explain::~Explain()
{
    delete ui;
}

void Explain::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Explain::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
