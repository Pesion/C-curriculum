#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include"shape.h"
#include"pointshape.h"

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent = nullptr);
    void startCreate();
    explicit GraphicsScene(int x, int y, int width, int height, QObject *parent = nullptr);
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    QList<QPointF> m_list;
    bool is_creating_Polygon;

signals:
    void updatePoint(QPointF p, QList<QPointF> list, bool isCenter);
    void createFinished();

};

#endif // GRAPHICSSCENE_H
