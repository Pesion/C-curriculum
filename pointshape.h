#ifndef POINTSHAPE_H
#define POINTSHAPE_H

#include <QObject>
#include <QAbstractGraphicsShapeItem>
#include <QPointF>
#include <QPen>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QCursor>
#include <QKeyEvent>
#include <QList>

class PointShape : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT
public:
    explicit PointShape(QObject *parent = nullptr);
    enum PointType {
        Center = 0, // 中心点
        Edge,       // 边缘点（可拖动改变图形的形状、大小）
        Special     // 特殊功能点
    };
    PointShape(QAbstractGraphicsShapeItem* parent,QPointF p,PointType type);

    QPointF getPoint() { return m_point; }
    void setPoint(QPointF p) { m_point = p; }

    QPointF m_point;
    PointType m_type;

protected:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

signals:

};

//存放点的容器
class PointItemList: public QList<PointShape *>
{
public:
    void setRandColor();
    void setColor(const QColor color);
    void setVisible(bool visible);
};


#endif // POINTSHAPE_H
