#ifndef SHAPE_H
#define SHAPE_H

#include <QObject>
#include<QAbstractGraphicsShapeItem>
#include<QGraphicsEllipseItem>
#include"pointshape.h"

#define PI 3.1415926

//图形基类
class Shape : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT
public:
    explicit Shape(QObject *parent = nullptr);
    enum ItemType {
        Circle = 0,         // 圆
        Ellipse,            // 椭圆
        Rectangle,          // 矩形
        Polygon,            // 多边形
        Triangle,           //三角形
        Trapezoid,          //梯形
    };
    QPointF getCenter() { return m_center; }
    void setCenter(QPointF p) { m_center = p; }

    QPointF getEdge() { return m_edge; }
    void setEdge(QPointF p) { m_edge = p; }

    ItemType getType() { return m_type; }

    QPointF m_center;
    QPointF m_edge;
    ItemType m_type;
    PointItemList m_pointList;

    QPen m_pen_isSelected;
    QPen m_pen_noSelected;

protected:
    Shape(QPointF center, QPointF edge, ItemType type);

    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;

signals:

};

//椭圆
class P_Ellipse : public Shape
{
    Q_OBJECT

public:
    P_Ellipse(qreal x, qreal y, qreal width, qreal height, ItemType type);

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
};

//圆
class P_Circle : public P_Ellipse
{
public:
    P_Circle(qreal x, qreal y, qreal radius, ItemType type);

    void updateRadius();

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

public:
    qreal m_radius;
};

// 矩形
class P_Rectangle : public Shape
{
public:
    P_Rectangle(qreal x, qreal y, qreal width, qreal height, ItemType type);

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
};

// 多边形
class P_Polygon : public Shape
{
    Q_OBJECT

public:
    P_Polygon(ItemType type);

    QPointF getCentroid(QList<QPointF> list);
    void getMaxLength();
    void updatePolygon(QPointF origin, QPointF end);

public slots:
    void pushPoint(QPointF p, QList<QPointF> list, bool isCenter);

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

public:
    qreal m_radius;
    bool is_create_finished;
};

//梯形
class P_Trapezoid :public Shape
{
public:

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
};

//
class P_Triangle :public Shape
{
public:
    P_Triangle(qreal x1,qreal y1,qreal x2,qreal y2, ItemType type);
protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
};

#endif // SHAPE_H
