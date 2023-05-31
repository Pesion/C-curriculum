#ifndef GRAPHICEDIT_H
#define GRAPHICEDIT_H

#include <QMainWindow>
#include<QString>
#include<QFile>
#include<QTextStream>
#include<QFileDialog>
#include<QDebug>
#include<QThread>
#include<QMouseEvent>
#include<QGraphicsScene>
#include<QLabel>
#include "qmenu.h"
#include "qevent.h"

namespace Ui {
class GraphicEdit;
}

class GraphicEdit : public QMainWindow
{
    Q_OBJECT

public:
    explicit GraphicEdit(QWidget *parent = nullptr);
    ~GraphicEdit();

public slots:
    void slotStart();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_action_3_triggered();
    void on_pushButton_16_clicked();
    void on_mouseMovePoint(QPoint point); //鼠标移动
    void on_mouseClicked(QPoint point); //鼠标单击
    void on_mouseDoubleClick(QPoint point); //鼠标双击
    void on_keyPress(QKeyEvent *event); //按键
    void on_mouseRelease(QPoint point);//鼠标释放
    void on_pushButton_3_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_7_clicked();

    void on_action_triggered();

    void on_pushButton_22_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_30_clicked();

    void on_action_2_triggered();

    void on_pushButton_31_clicked();

    void on_pushButton_32_clicked();

private:
    static const int ItemId = 1;   //绘图项自定义数据的key
    static const int ItemDesciption = 2;   //绘图项自定义数据的key

    int seqNum=0;//用于给每个图形项编号
    int backZ=0;//设置图形项目叠放顺序 数值越小 越在后面显示
    int frontZ=0;//设置图形项目叠放顺序 数值越大 越在前面显示
    int startX,startY,endX,endY;//鼠标点击时坐标和松开后的坐标
    QGraphicsScene  *scene;

    QLabel  *labViewCord;
    QLabel  *labSceneCord;
    QLabel  *labStart;
    QLabel  *labEnd;

    void contextMenuEvent(QContextMenuEvent *event);    //创建菜单
    void onAction1Triggered();//调取颜色设置
    void onAction2Triggered();//调取样式设置
private:
    Ui::GraphicEdit *ui;

};

#endif // GRAPHICEDIT_H
