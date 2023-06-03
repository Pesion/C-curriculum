#include "graphicsscene.h"
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QDebug>

GraphicsScene::GraphicsScene(QObject *parent) : QGraphicsScene(parent)
{
    is_creating_Polygon=false;
}

void GraphicsScene::startCreate()
{
    //开始创建对象
    is_creating_Polygon = true;
    m_list.clear();
}

GraphicsScene::GraphicsScene(int x, int y, int width, int height, QObject *parent)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(width);
    Q_UNUSED(height);
    Q_UNUSED(parent);
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //鼠标按压响应函数
    is_creating_Polygon=false;
    if(is_creating_Polygon){
        QPointF p(event->scenePos().x(),event->scenePos().y());
        switch  (event->buttons())
        {
        case Qt::LeftButton:{
            m_list.push_back(p);
            emit updatePoint(p,m_list,false);
        }break;
        case Qt::RightButton: {
            if (m_list.size() >= 3) {
                emit updatePoint(p, m_list, true);
                emit createFinished();
                is_creating_Polygon = false;
                m_list.clear();
            }
        } break;
        default: break;
        }
    }
    else{
        QGraphicsScene::mousePressEvent(event);
    }
}
