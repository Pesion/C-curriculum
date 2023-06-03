#include "pointshape.h"
#include"shape.h"
PointShape::PointShape(QObject *parent) : QObject(parent)
{

}

PointShape::PointShape(QAbstractGraphicsShapeItem *parent, QPointF p, PointShape::PointType type)
    :QAbstractGraphicsShapeItem(parent), m_point(p), m_type(type)
{
    this->setPos(m_point);
    this->setFlags(QGraphicsItem::ItemIsSelectable |QGraphicsItem::ItemIsMovable |QGraphicsItem::ItemIsFocusable);
    switch (type) {
    case Center:
        this->setCursor(Qt::OpenHandCursor);//表示打开的手形光标
        break;
    case Edge:
        this->setCursor(Qt::PointingHandCursor);//表示指向的手形光标
        break;
    case Special:
        this->setCursor(Qt::PointingHandCursor);
        break;
    default: break;
    }
}

QRectF PointShape::boundingRect() const
{
    return QRectF(-4, -4, 8, 8);
}

void PointShape::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());
    this->setPos(m_point);
    switch (m_type) {
    case Center:
        painter->drawEllipse(-4, -4, 8, 8);
        break;
    case Edge:
        painter->drawRect(QRectF(-4, -4, 8, 8));
        break;
    case Special:
        painter->drawRect(QRectF(-4, -4, 8, 8));
        break;
    default: break;
    }
}

void PointShape::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //移动 更新属性
    if ( event->buttons() == Qt::LeftButton ) {
        qreal dx = event->scenePos().x() - event->lastScenePos().x();
        qreal dy = event->scenePos().y() - event->lastScenePos().y();
        Shape * item = static_cast<Shape *>(this->parentItem());
        Shape::ItemType itemType = item->getType();
        switch (m_type) {
        case Center: {
            item->moveBy(dx, dy);
            this->scene()->update();
        } break;
        case Edge: {
            switch (itemType) {
            case Shape::ItemType::Ellipse: {
                P_Ellipse *ellipse = dynamic_cast<P_Ellipse *>(item);
                ellipse->setEdge(m_point);
            } break;
            case Shape::ItemType::Circle: {
                P_Circle *circle = dynamic_cast<P_Circle *>(item);
                circle->setEdge(m_point);
                circle->updateRadius();
            } break;
            case Shape::ItemType::Rectangle: {
                P_Rectangle *rectangle = dynamic_cast<P_Rectangle *>(item);
                rectangle->setEdge(m_point);
            } break;
            case Shape::ItemType::Polygon: {
                P_Polygon *polygon = dynamic_cast<P_Polygon *>(item);
                polygon->updatePolygon(QPointF(event->lastScenePos().x(), event->lastScenePos().y()),
                                       QPointF(event->scenePos().x(), event->scenePos().y()));
            } break;
            default: break;
            }
        } break;
        default: break;
        }
    }
}

void PointItemList::setRandColor()
{
    //生成一个随机颜色 并将其设置为当前图形项的颜色
    this->setColor(QColor(qrand()%256, qrand()%256, qrand()%256));
}

void PointItemList::setColor(const QColor color)
{
    //将每个元素的画刷设置为指定的颜色
    for (auto &temp : *this)
    {
        temp->setBrush(QBrush(color));
    }
}

void PointItemList::setVisible(bool visible)
{
    //设置每个元素的可见性
    for (auto &temp : *this)
    {
        temp->setVisible(visible);
    }
}
