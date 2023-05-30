#include "mainwindow.h"
#include "ui_mainwindow.h"

#include"explain.h"
#include"graphicedit.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/new/picture/主页.png"));
    //显示当前日期时间
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(time_update()));
    timer->start(1000); //1s执行一次,定时器
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::time_update() //显示系统时间的功能
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd");
    ui->label->setText(str);
}

void MainWindow::on_action_1_triggered()
{
    QString fileName=QFileDialog::getOpenFileName(this,"添加文件","D:\\个人文件\\课程文件\\面向对象文件");
    //读取文件内容
    QFile file(fileName);//创建一个文件的对象 并绑定到给定的文件上 参数为路径
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Unable to open file";
            return;
        }
    //进行文件操作：

    file.close();
}

void MainWindow::on_action_2_triggered()
{
    g=new GraphicEdit;
    this->close();
    g->show();
}

void MainWindow::on_action_3_triggered()
{
    e=new Explain;
    e->show();
}
