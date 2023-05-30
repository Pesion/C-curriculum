#include "graphicedit.h"
#include "ui_graphicedit.h"

GraphicEdit::GraphicEdit(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GraphicEdit)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/new/picture/产品设置.png"));
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->action_5,&QAction::triggered,this,[=](){
            ui->stackedWidget->setCurrentIndex(1);      //切换至图形编辑子窗口
    });
    connect(ui->action_3,&QAction::triggered,this,[=](){
        ui->stackedWidget->setCurrentIndex(2);      //切换至图片编辑子窗口
    });
    connect(ui->pushButton_16,&QPushButton::clicked,this,&GraphicEdit::on_pushButton_16_clicked);
}

GraphicEdit::~GraphicEdit()
{
    delete ui;
}

void GraphicEdit::paintEvent(QPaintEvent *event)
{

}

void GraphicEdit::on_pushButton_clicked()
{
    p=new PaintGraph;
    ui->stackedWidget->setCurrentIndex(1);
}

void GraphicEdit::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}



void GraphicEdit::on_pushButton_16_clicked()
{
    //绘制点
    ui->widget_11->update();
    QPainter painter(ui->widget_11);
    painter.drawPoint(10,10);

}
