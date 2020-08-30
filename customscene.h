#ifndef CUSTOMSCENE_H
#define CUSTOMSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QAction>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QList>

class Line{
public:
    QGraphicsItem *item;
    QPointF origPoint;
    QPointF endPoint;
};

class CustomScene : public QGraphicsScene
{
public:
    enum Mode {NoMode, SelectObject, DrawLine};
    CustomScene(QObject* parent = 0);
    void setMode(Mode mode);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    Mode sceneMode;
    QPointF origPoint;
    QPointF endPoint;
    QMap<QGraphicsItem*, QLine*> lines;
    QGraphicsLineItem* itemToDraw;
    void makeItemsControllable(bool areControllable);
};

#endif // CUSTOMSCENE_H
