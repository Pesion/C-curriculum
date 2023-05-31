#include "graphicedit.h"
#include "ui_graphicedit.h"

#include<QGraphicsRectItem>
#include<QInputDialog>
#include<QColorDialog>
#include<QFontDialog>
#include<QTime>
#include<QKeyEvent>
#include<QMessageBox>
#include<cmath>

template<class T> void setBrushColor(T *item)
{
    //函数模板
    QColor color=item->brush().color();
    color=QColorDialog::getColor(color,NULL,"选择填充颜色");
    if (color.isValid())
        item->setBrush(QBrush(color));
}

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
    connect(ui->action_5,&QAction::triggered,this,&GraphicEdit::slotStart);
    connect(ui->pushButton,&QPushButton::clicked,this,&GraphicEdit::slotStart);
}

GraphicEdit::~GraphicEdit()
{
    delete ui;
}

void GraphicEdit::slotStart()
{
    QMessageBox::about(this,"程序使用说明","空格键可以控制图形旋转，上下左右键可以控制图形移动");
}


void GraphicEdit::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    //创建状态栏标签
    labViewCord=new QLabel("View 坐标：");
    labViewCord->setMinimumWidth(150);
    ui->statusbar->addWidget(labViewCord);
    labSceneCord=new QLabel("Scene 坐标：");
    labSceneCord->setMinimumWidth(150);
    ui->statusbar->addWidget(labSceneCord);
    labStart=new QLabel("Start 坐标：");
    labStart->setMinimumWidth(150);
    ui->statusbar->addWidget(labStart);
    labEnd=new QLabel("End 坐标: ");
    labEnd->setMinimumWidth(200);
    ui->statusbar->addWidget(labEnd);
    //创建QGraphicsScene
    scene=new QGraphicsScene(-300,-200,600,200);
    ui->View->setScene(scene); //与view关联
//    ui->View->setDragMode(QGraphicsView::RubberBandDrag);

    ui->View->setCursor(Qt::CrossCursor); //设置鼠标
    ui->View->setMouseTracking(true); //设置鼠标追踪
    ui->View->setDragMode(QGraphicsView::RubberBandDrag);//设置拖拽模式

//    this->setCentralWidget(ui->View);   //填充主窗口的中央区域
    connect(ui->View,&QWGraphicsView::mouseMovePoint,this,&GraphicEdit::on_mouseMovePoint);
    connect(ui->View,&QWGraphicsView::mouseClicked,this,&GraphicEdit::on_mouseClicked);
    connect(ui->View,&QWGraphicsView::mouseDoubleClick,this,&GraphicEdit::on_mouseDoubleClick);
    connect(ui->View,&QWGraphicsView::keyPress,this,&GraphicEdit::on_keyPress);
    connect(ui->View,&QWGraphicsView::mouseRelease,this,&GraphicEdit::on_mouseRelease);
    qsrand(QTime::currentTime().second());
}

void GraphicEdit::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void GraphicEdit::on_action_3_triggered()
{
    //图片插入
}

void GraphicEdit::on_mouseMovePoint(QPoint point)
{
    //鼠标移动事件，point是 GraphicsView的坐标,物理坐标
    labViewCord->setText(QString::asprintf("View 坐标：%d,%d",point.x(),point.y()));
//    QPointF pointScene=ui->View->mapToScene(point); //转换到Scene坐标
//    labSceneCord->setText(QString::asprintf("Scene 坐标：%.0f,%.0f",pointScene.x(),pointScene.y()));
}

void GraphicEdit::on_mouseClicked(QPoint point)
{
    //鼠标单击事件
    labViewCord->setText(QString::asprintf("View 坐标：%d,%d",point.x(),point.y()));
    QPointF pointScene=ui->View->mapToScene(point); //转换到Scene坐标
    labSceneCord->setText(QString::asprintf("Scene 坐标：%.0f,%.0f",pointScene.x(),pointScene.y()));
    labStart->setText(QString::asprintf("Start 坐标：%.0f,%.0f",pointScene.x(),pointScene.y()));
    startX=pointScene.x();
    startY=pointScene.y();
}

void GraphicEdit::on_mouseDoubleClick(QPoint point)
{
    //鼠标双击事件 调用相应的对话框 设置填充颜色 线条颜色或字体
    QPointF pointScene=ui->View->mapToScene(point); //转换到Scene坐标
    QGraphicsItem  *item=NULL;
    item=scene->itemAt(pointScene,ui->View->transform()); //获取光标下的绘图项
    if (item == NULL) //没有绘图项
    {
        QMessageBox::about(this,"错误","请先选择图形");
        return;
    }
    switch (item->type())  //绘图项的类型
    {
      case    QGraphicsRectItem::Type: //矩形框
      { //强制类型转换
        QGraphicsRectItem *theItem=qgraphicsitem_cast<QGraphicsRectItem*>(item);
        setBrushColor(theItem);
        break;
      }
      case    QGraphicsEllipseItem::Type: //椭圆和圆都是QGraphicsEllipseItem
      {
        QGraphicsEllipseItem *theItem;
        theItem=qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
        setBrushColor(theItem);
        break;
      }
      case    QGraphicsPolygonItem::Type: //梯形、三角形
      {
        QGraphicsPolygonItem *theItem=qgraphicsitem_cast<QGraphicsPolygonItem*>(item);
        setBrushColor(theItem);
        break;
      }
      case    QGraphicsLineItem::Type: //直线，设置线条颜色
      {
        QGraphicsLineItem *theItem=qgraphicsitem_cast<QGraphicsLineItem*>(item);
        QPen    pen=theItem->pen();
        QColor  color=theItem->pen().color();
        color=QColorDialog::getColor(color,this,"选择线条颜色");
        if (color.isValid())
        {
            pen.setColor(color);
            theItem->setPen(pen);
        }
        break;
      }
    }
}

void GraphicEdit::on_keyPress(QKeyEvent *event)
{
    //按键事件
    if (scene->selectedItems().count()!=1)
        return; //没有选中的绘图项，或选中的多于1个
    QGraphicsItem   *item=scene->selectedItems().at(0);
    if (event->key()==Qt::Key_Space) //顺时针旋转90度
        item->setRotation(90+item->rotation());
    else if (event->key()==Qt::Key_Left)  //左移
        item->setX(-1+item->x());
    else if (event->key()==Qt::Key_Right) //右移
        item->setX(1+item->x());
    else if (event->key()==Qt::Key_Up) //上移
        item->setY(-1+item->y());
    else if (event->key()==Qt::Key_Down) //下移
        item->setY(1+item->y());
}

void GraphicEdit::on_mouseRelease(QPoint point)
{
    //鼠标释放事件
    labViewCord->setText(QString::asprintf("View 坐标：%d,%d",point.x(),point.y()));
    QPointF pointScene=ui->View->mapToScene(point); //转换到Scene坐标
    labEnd->setText(QString::asprintf("End 坐标：%.0f,%.0f",pointScene.x(),pointScene.y()));
    endX=pointScene.x();
    endY=pointScene.y();
}

void GraphicEdit::on_pushButton_16_clicked()
{
    //添加梯形
    QGraphicsPolygonItem   *item=new QGraphicsPolygonItem;
    QPolygonF   points;
    points.append(QPointF(-40,-40));
    points.append(QPointF(40,-40));
    points.append(QPointF(100,40));
    points.append(QPointF(-100,40));
    item->setPolygon(points);
    item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));
    item->setFlags(QGraphicsItem::ItemIsMovable| QGraphicsItem::ItemIsSelectable| QGraphicsItem::ItemIsFocusable);//可移动 可选择 可交互
    item->setBrush(QBrush(Qt::green));
    item->setZValue(++frontZ);
    item->setData(ItemId,++seqNum);
    item->setData(ItemDesciption,"梯形");
    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

void GraphicEdit::on_pushButton_3_clicked()
{
    //添加线
    QGraphicsLineItem   *item=new QGraphicsLineItem(-100,0,100,0);//x,y 为左上角的图元局部坐标，图元中心点为0,0
    item->setFlags(QGraphicsItem::ItemIsMovable| QGraphicsItem::ItemIsSelectable| QGraphicsItem::ItemIsFocusable);
    QPen    pen(Qt::red);
    pen.setWidth(3);
    item->setPen(pen);
    item->setZValue(++frontZ);
    item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));
    item->setData(ItemId,++seqNum);
    item->setData(ItemDesciption,"直线");
    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);

}

void GraphicEdit::on_pushButton_17_clicked()
{
    //添加圆
    QGraphicsEllipseItem   *item=new QGraphicsEllipseItem(-50,-50,100,100);
    item->setFlags(QGraphicsItem::ItemIsMovable| QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    item->setBrush(QBrush(Qt::cyan));
    item->setZValue(++frontZ);
    item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));
    item->setData(ItemId,++seqNum);
    item->setData(ItemDesciption,"圆形");
    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

void GraphicEdit::on_pushButton_18_clicked()
{
    //添加矩形
//    QGraphicsRectItem   *item=new QGraphicsRectItem(-50,-25,100,50);//x,y 为左上角的图元局部坐标，图元中心点为0,0
//    item->setFlags(QGraphicsItem::ItemIsMovable| QGraphicsItem::ItemIsSelectable| QGraphicsItem::ItemIsFocusable);
//    item->setBrush(QBrush(Qt::yellow));
//    item->setZValue(++frontZ);
//    item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));
//    item->setData(ItemId,++seqNum);
//    item->setData(ItemDesciption,"矩形");
//    scene->addItem(item);
//    scene->clearSelection();
//    item->setSelected(true);

    int x=abs(endX-startX);
    int y=abs(endY-startY);
    QGraphicsRectItem   *item=new QGraphicsRectItem(startX,startY,x,y);//x,y 为左上角的图元局部坐标，图元中心点为0,0
    item->setFlags(QGraphicsItem::ItemIsMovable| QGraphicsItem::ItemIsSelectable| QGraphicsItem::ItemIsFocusable);
    item->setBrush(QBrush(Qt::yellow));
    item->setZValue(++frontZ);
    item->setPos(startX+(qrand() % 100),startY+(qrand() % 100));
    item->setData(ItemId,++seqNum);
    item->setData(ItemDesciption,"矩形");
    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

void GraphicEdit::on_pushButton_19_clicked()
{
    //添加三角形
    QGraphicsPolygonItem   *item=new QGraphicsPolygonItem;
    QPolygonF   points;
    points.append(QPointF(0,-40));
    points.append(QPointF(60,40));
    points.append(QPointF(-60,40));
    item->setPolygon(points);
    item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));
    item->setFlags(QGraphicsItem::ItemIsMovable| QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    item->setBrush(QBrush(Qt::magenta));
    item->setZValue(++frontZ);
    item->setData(ItemId,++seqNum);
    item->setData(ItemDesciption,"三角形");
    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

void GraphicEdit::on_pushButton_7_clicked()
{
    //添加椭圆
    QGraphicsEllipseItem   *item=new QGraphicsEllipseItem(-50,-30,100,60);
    item->setFlags(QGraphicsItem::ItemIsMovable| QGraphicsItem::ItemIsSelectable| QGraphicsItem::ItemIsFocusable);
    item->setBrush(QBrush(Qt::blue)); //填充颜色
    item->setZValue(++frontZ); //用于叠放顺序
    item->setPos(-50+(qrand() % 100),-50+(qrand() % 100)); //初始位置
    item->setData(ItemId,++seqNum);  //自定义数据，ItemId键
    item->setData(ItemDesciption,"椭圆"); //自定义数据，ItemDesciption键
    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

void GraphicEdit::on_action_triggered()
{
    //颜色设置 调用相应的对话框 设置填充颜色 线条颜色
    QPointF pointScene;
    pointScene.setX(startX);
    pointScene.setY(startY);
    QGraphicsItem  *item=NULL;
    item=scene->itemAt(pointScene,ui->View->transform()); //获取光标下的绘图项
    if (item == NULL) //没有绘图项
    {
        QMessageBox::about(this,"错误","请先选择图形");
        return;
    }
    switch (item->type())  //绘图项的类型
    {
      case    QGraphicsRectItem::Type: //矩形框
      { //强制类型转换
        QGraphicsRectItem *theItem=qgraphicsitem_cast<QGraphicsRectItem*>(item);
        setBrushColor(theItem);
        break;
      }
      case    QGraphicsEllipseItem::Type: //椭圆和圆都是 QGraphicsEllipseItem
      {
        QGraphicsEllipseItem *theItem;
        theItem=qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
        setBrushColor(theItem);
        break;
      }
      case    QGraphicsPolygonItem::Type: //梯形和三角形
      {
        QGraphicsPolygonItem *theItem=qgraphicsitem_cast<QGraphicsPolygonItem*>(item);
        setBrushColor(theItem);
        break;
      }
      case    QGraphicsLineItem::Type: //直线，设置线条颜色
      {
        QGraphicsLineItem *theItem=qgraphicsitem_cast<QGraphicsLineItem*>(item);
        QPen    pen=theItem->pen();
        QColor  color=theItem->pen().color();
        color=QColorDialog::getColor(color,this,"选择线条颜色");
        if (color.isValid())
        {
            pen.setColor(color);
            theItem->setPen(pen);
        }
        break;
      }
    }
}

void GraphicEdit::on_action_2_triggered()
{
    //样式设置 设置图形的大小、边长等
    QPointF pointScene;
    pointScene.setX(startX);
    pointScene.setY(startY);
    QGraphicsItem  *item=NULL;
    item=scene->itemAt(pointScene,ui->View->transform()); //获取光标下的绘图项
    if (item == NULL) //没有绘图项
    {
        QMessageBox::about(this,"错误","请先选择图形");
        return;
    }

}

void GraphicEdit::on_pushButton_22_clicked()
{
    //逆时针旋转
    int cnt=scene->selectedItems().count();
    if (cnt==1)
    {
        QGraphicsItem* item=scene->selectedItems().at(0);
        item->setRotation(-30+item->rotation());
    }
    else
        ui->View->rotate(-30);
}

void GraphicEdit::on_pushButton_23_clicked()
{
    //顺时针旋转
    int cnt=scene->selectedItems().count();
    if (cnt==1)
    {
        QGraphicsItem* item=scene->selectedItems().at(0);
        item->setRotation(+30+item->rotation());
    }
    else
        ui->View->rotate(+30);
}

void GraphicEdit::on_pushButton_21_clicked()
{
    //放大
    int cnt=scene->selectedItems().count();
    if (cnt==1)
    {
        QGraphicsItem   *item;
        item=scene->selectedItems().at(0);
        item->setScale(0.1+item->scale());
    }
    else
        ui->View->scale(1.1,1.1);
}

void GraphicEdit::on_pushButton_20_clicked()
{
    //缩小
    int cnt=scene->selectedItems().count();
    if (cnt==1)
    {
        QGraphicsItem   *item;
        item=scene->selectedItems().at(0);
        item->setScale(item->scale()-0.1);
    }
    else
        ui->View->scale(0.9,0.9);
}

void GraphicEdit::on_pushButton_24_clicked()
{
    //删除选中项
    int cnt=scene->selectedItems().count();
    if (cnt>0)
        for (int i=0;i<cnt;i++)
        {
            QGraphicsItem*  item=scene->selectedItems().at(0);
            scene->removeItem(item); //删除绘图项
        }
}

void GraphicEdit::on_pushButton_25_clicked()
{
    //向上
    if (scene->selectedItems().count()!=1)
        return; //没有选中的绘图项，或选中的多于1个
    QGraphicsItem   *item=scene->selectedItems().at(0);
    item->setY(-10+item->y());
}

void GraphicEdit::on_pushButton_26_clicked()
{
    //向下
    if (scene->selectedItems().count()!=1)
        return; //没有选中的绘图项，或选中的多于1个
    QGraphicsItem   *item=scene->selectedItems().at(0);
    item->setY(10+item->y());
}

void GraphicEdit::on_pushButton_27_clicked()
{
    //向左
    if (scene->selectedItems().count()!=1)
        return; //没有选中的绘图项，或选中的多于1个
    QGraphicsItem   *item=scene->selectedItems().at(0);
    item->setX(-10+item->x());
}

void GraphicEdit::on_pushButton_28_clicked()
{
    //向右
    if (scene->selectedItems().count()!=1)
        return; //没有选中的绘图项，或选中的多于1个
    QGraphicsItem   *item=scene->selectedItems().at(0);
    item->setX(10+item->x());
}

void GraphicEdit::on_pushButton_29_clicked()
{
    //前置
    int cnt=scene->selectedItems().count();
    if (cnt>0)
    {
        //只处理选中的第1个绘图项
        QGraphicsItem* item=scene->selectedItems().at(0);
        item->setZValue(++frontZ);
    }
}

void GraphicEdit::on_pushButton_30_clicked()
{
    //后置
    int cnt=scene->selectedItems().count();
    if (cnt>0)
    {
        //只处理选中的第1个绘图项
        QGraphicsItem* item=scene->selectedItems().at(0);
        item->setZValue(--backZ);
    }
}

void GraphicEdit::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this); // 创建菜单栏
    QAction *action1 = menu.addAction("颜色设置");
    QAction *action2 = menu.addAction("样式设置");
    // 连接菜单项的槽函数
    connect(action1, &QAction::triggered, this, &GraphicEdit::onAction1Triggered);
    connect(action2, &QAction::triggered, this, &GraphicEdit::onAction2Triggered);
    menu.exec(event->globalPos()); // 在全局坐标位置显示菜单栏
}

void GraphicEdit::onAction1Triggered()
{
    on_action_triggered();//调用颜色设置
}

void GraphicEdit::onAction2Triggered()
{
    on_action_2_triggered();//调用样式设置
}

void GraphicEdit::on_pushButton_31_clicked()
{
    //组合
    int cnt=scene->selectedItems().count();
    if (cnt>1)
    {
        QGraphicsItemGroup* group =new QGraphicsItemGroup; //创建组合
        scene->addItem(group); //组合添加到场景中

        for (int i=0;i<cnt;i++)
        {
            QGraphicsItem* item=scene->selectedItems().at(0);
            item->setSelected(false); //清除选择虚线框
            item->clearFocus();
            group->addToGroup(item); //添加到组合
        }
        group->setFlags(QGraphicsItem::ItemIsMovable| QGraphicsItem::ItemIsSelectable| QGraphicsItem::ItemIsFocusable);
        group->setZValue(++frontZ);
//        group->clearFocus();
        scene->clearSelection();
        group->setSelected(true);
    }
    else{
        QMessageBox::about(this,"错误","请选择两个及以上图形进行组合！");
        return;
    }
}

void GraphicEdit::on_pushButton_32_clicked()
{
    //打散
    int cnt=scene->selectedItems().count();
    if (cnt==1)
    {
        QGraphicsItemGroup  *group;
        group=(QGraphicsItemGroup*)scene->selectedItems().at(0);
        scene->destroyItemGroup(group);
    }
}
