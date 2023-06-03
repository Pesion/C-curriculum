#include "shape.h"
#include <QVector>
#include <QDebug>
#include <QMenu>
#include <QSpinBox>
#include <QWidgetAction>

Shape::Shape(QObject *parent) : QObject(parent)
{

}

Shape::Shape(QPointF center, QPointF edge, Shape::ItemType type)
    : m_center(center), m_edge(edge), m_type(type)
{
    m_pen_noSelected.setColor(QColor(0, 160, 230));
    m_pen_noSelected.setWidth(2);
    m_pen_isSelected.setColor(QColor(255, 0, 255));
    m_pen_isSelected.setWidth(2);

    this->setPen(m_pen_noSelected);
    this->setFlags(QGraphicsItem::ItemIsSelectable |QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);
}

void Shape::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    this->setPen(m_pen_isSelected);
}

void Shape::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    this->setPen(m_pen_noSelected);
}

P_Ellipse::P_Ellipse(qreal x, qreal y, qreal width, qreal height, Shape::ItemType type)
    : Shape(QPointF(x,y), QPointF(x+width/2,y+height/2), type)
{
    PointShape *point = new PointShape(this, m_edge, PointShape::Edge);
    point->setParentItem(this);
    m_pointList.append(point);
    m_pointList.append(new PointShape(this, m_center, PointShape::Center));
    m_pointList.setRandColor();
}

QRectF P_Ellipse::boundingRect() const
{
    return QRectF(m_center.x() - abs(m_edge.x()), m_center.y() - abs(m_edge.y()), abs(m_edge.x()) * 2, abs(m_edge.y()) * 2);
}

void P_Ellipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());
    QRectF ret(m_center.x() - abs(m_edge.x()), m_center.y() - abs(m_edge.y()), abs(m_edge.x()) * 2, abs(m_edge.y()) * 2);
    painter->drawEllipse(ret);
}

void P_Ellipse::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if ( !this->isSelected() )
        return;
    QMenu* menu = new QMenu();
    menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }");
    QSpinBox* width_spinBox = new QSpinBox(menu);
    width_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    width_spinBox->setRange(0, 1000);
    width_spinBox->setPrefix("w: ");
    width_spinBox->setSuffix(" mm");
    width_spinBox->setSingleStep(1);
    width_spinBox->setValue(2 * abs(m_edge.x()));
    connect(width_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        if (m_edge.x() < 0) {
            m_edge.setX((-1) * v/2);
        } else {
            m_edge.setX(v/2);
        }
        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });
    QSpinBox* height__spinBox = new QSpinBox(menu);
    height__spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    height__spinBox->setRange(0, 1000);
    height__spinBox->setPrefix("h: ");
    height__spinBox->setSuffix(" mm");
    height__spinBox->setSingleStep(1);
    height__spinBox->setValue(2 * abs(m_edge.y()));
    connect(height__spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        if (m_edge.y() < 0) {
            m_edge.setY((-1) * v/2);
        }
        else {
            m_edge.setY(v/2);
        }
        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    QWidgetAction* width_widgetAction = new QWidgetAction(menu);
    width_widgetAction->setDefaultWidget(width_spinBox);
    menu->addAction(width_widgetAction);

    QWidgetAction* height_widgetAction = new QWidgetAction(menu);
    height_widgetAction->setDefaultWidget(height__spinBox);
    menu->addAction(height_widgetAction);

    menu->exec(QCursor::pos());
    delete menu;

    QGraphicsItem::contextMenuEvent(event);
}


P_Circle::P_Circle(qreal x, qreal y, qreal radius, Shape::ItemType type)
    : P_Ellipse(x, y, radius*sqrt(2), radius*sqrt(2), type)
{
    updateRadius();
}

void P_Circle::updateRadius()
{
    m_radius = sqrt(pow(m_center.x() - m_edge.x(), 2) + pow(m_center.y() - m_edge.y(), 2));
}

QRectF P_Circle::boundingRect() const
{
    return QRectF(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
}

void P_Circle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());
    QRectF ret(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
    painter->drawEllipse(ret);
}

void P_Circle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu* menu = new QMenu();
    menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }");
    QSpinBox* radius_spinBox = new QSpinBox(menu);
    radius_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    radius_spinBox->setRange(0, 1000);
    radius_spinBox->setPrefix("r: ");
    radius_spinBox->setSuffix(" mm");
    radius_spinBox->setSingleStep(1);
    radius_spinBox->setValue(m_radius);
    connect(radius_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        m_radius = v;

        if (m_edge.x() < 0) {
            m_edge.setX(m_center.x() - m_radius * sqrt(2)/2);
        } else {
            m_edge.setX(m_center.x() + m_radius * sqrt(2)/2);
        }

        if (m_edge.y() < 0) {
            m_edge.setY(m_center.y() - m_radius * sqrt(2)/2);
        } else {
            m_edge.setY(m_center.y() + m_radius * sqrt(2)/2);
        }

        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    QWidgetAction* radius_widgetAction = new QWidgetAction(menu);
    radius_widgetAction->setDefaultWidget(radius_spinBox);
    menu->addAction(radius_widgetAction);

    menu->exec(QCursor::pos());
    delete menu;

    QGraphicsItem::contextMenuEvent(event);
}

P_Rectangle::P_Rectangle(qreal x, qreal y, qreal width, qreal height, Shape::ItemType type)
    : Shape(QPointF(x,y), QPointF(width/2,height/2), type)
{
    PointShape *point = new PointShape(this, m_edge, PointShape::Edge);
    point->setParentItem(this);
    m_pointList.append(point);
    m_pointList.append(new PointShape(this, m_center, PointShape::Center));
    m_pointList.setRandColor();
}

QRectF P_Rectangle::boundingRect() const
{
    return QRectF(m_center.x() - abs(m_edge.x()), m_center.y() - abs(m_edge.y()), abs(m_edge.x()) * 2, abs(m_edge.y()) * 2);
}

void P_Rectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QRectF ret(m_center.x() - abs(m_edge.x()), m_center.y() - abs(m_edge.y()), abs(m_edge.x()) * 2, abs(m_edge.y()) * 2);
    painter->drawRect(ret);
}

void P_Rectangle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if ( !this->isSelected() )
        return;

    QMenu* menu = new QMenu();
    menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }");

    QSpinBox* width_spinBox = new QSpinBox(menu);
    width_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    width_spinBox->setRange(0, 1000);
    width_spinBox->setPrefix("w: ");
    width_spinBox->setSuffix(" mm");
    width_spinBox->setSingleStep(1);
    width_spinBox->setValue(2 * abs(m_edge.x()));
    connect(width_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        if (m_edge.x() < 0) {
            m_edge.setX((-1) * v/2);
        } else {
            m_edge.setX(v/2);
        }
        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    QSpinBox* height__spinBox = new QSpinBox(menu);
    height__spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    height__spinBox->setRange(0, 1000);
    height__spinBox->setPrefix("h: ");
    height__spinBox->setSuffix(" mm");
    height__spinBox->setSingleStep(1);
    height__spinBox->setValue(2 * abs(m_edge.y()));
    connect(height__spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        if (m_edge.y() < 0) {
            m_edge.setY((-1) * v/2);
        } else {
            m_edge.setY(v/2);
        }
        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    QWidgetAction* width_widgetAction = new QWidgetAction(menu);
    width_widgetAction->setDefaultWidget(width_spinBox);
    menu->addAction(width_widgetAction);

    QWidgetAction* height_widgetAction = new QWidgetAction(menu);
    height_widgetAction->setDefaultWidget(height__spinBox);
    menu->addAction(height_widgetAction);

    menu->exec(QCursor::pos());
    delete menu;

    QGraphicsItem::contextMenuEvent(event);
}

P_Polygon::P_Polygon(Shape::ItemType type)
    : Shape(QPointF(0,0), QPointF(0,0), type)
{
    is_create_finished = false;
}

QPointF P_Polygon::getCentroid(QList<QPointF> list)
{
    qreal x = 0;
    qreal y = 0;
    for (auto &temp : list)
    {
        x += temp.x();
        y += temp.y();
    }
    x = x/list.size();
    y = y/list.size();
    return QPointF(x,y);
}

void P_Polygon::getMaxLength()
{
    QVector<qreal> vec;
    for (auto &temp : m_pointList)
    {
        qreal dis = sqrt(pow(m_center.x() - temp->x(), 2) + pow(m_center.y() - temp->y(), 2));
        vec.append(dis);
    }

    qreal ret = 0;
    for (auto &temp : vec)
    {
        if (temp > ret) {
            ret = temp;
        }
    }
    m_radius = ret;
}

void P_Polygon::updatePolygon(QPointF origin, QPointF end)
{
    QList<QPointF> list;

    for (auto &temp : m_pointList) {
        if (temp->getPoint() == origin) {
            temp->setPoint(end);
        }
        list.append(temp->getPoint());
    }

    m_center = getCentroid(list);
    m_pointList.at(m_pointList.size()-1)->setPoint(m_center);

    getMaxLength();
}

void P_Polygon::pushPoint(QPointF p, QList<QPointF> list, bool isCenter)
{
    if (!is_create_finished) {
        m_center = getCentroid(list);
        getMaxLength();

        if (isCenter) {
            m_pointList.append(new PointShape(this, m_center, PointShape::Center));
            m_pointList.setRandColor();
            is_create_finished = true;
        } else {
            PointShape *point = new PointShape(this, p, PointShape::Edge);
            point->setParentItem(this);
            m_pointList.append(point);
            m_pointList.setColor(QColor(0, 255, 0));
        }

        this->update();
    }
}

QRectF P_Polygon::boundingRect() const
{
    return QRectF(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
}

void P_Polygon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    if (is_create_finished) {
        for (int i = 1; i < m_pointList.size() - 1; i++)
        {
            painter->drawLine(m_pointList.at(i-1)->getPoint(), m_pointList.at(i)->getPoint());
        }

        painter->drawLine(m_pointList.at(m_pointList.size()-2)->getPoint(), m_pointList.at(0)->getPoint());
    } else {
        for (int i = 1; i < m_pointList.size(); i++)
        {
            painter->drawLine(m_pointList.at(i-1)->getPoint(), m_pointList.at(i)->getPoint());
        }
    }
}
