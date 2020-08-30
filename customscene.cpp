#include "customscene.h"
#include <QDebug>

CustomScene::CustomScene(QObject* parent): QGraphicsScene(parent)
{
    sceneMode = NoMode;
    itemToDraw = 0;
}

void CustomScene::setMode(Mode mode){
    sceneMode = mode;
    QGraphicsView::DragMode vMode =
            QGraphicsView::NoDrag;
    if(mode == DrawLine){
        makeItemsControllable(false);
        vMode = QGraphicsView::NoDrag;
    }
    else if(mode == SelectObject){
        makeItemsControllable(true);
        vMode = QGraphicsView::RubberBandDrag;
    }
    QGraphicsView* mView = views().at(0);
    if(mView)
        mView->setDragMode(vMode);
}

void CustomScene::makeItemsControllable(bool areControllable){
    foreach(QGraphicsItem* item, items()){
        if (item->type() != 7)  // Check to disable movement of Image render
        {
            item->setFlag(QGraphicsItem::ItemIsSelectable,
                          areControllable);
            item->setFlag(QGraphicsItem::ItemIsMovable,
                          areControllable);
        }
    }
}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    try {
        if(sceneMode == DrawLine){
            origPoint = event->scenePos();
        }
        QGraphicsScene::mousePressEvent(event);
    } catch (std::exception &e) {
        qDebug() << e.what();
    }

}

void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(sceneMode == DrawLine){
        if(!itemToDraw){
            itemToDraw = new QGraphicsLineItem;
            this->addItem(itemToDraw);
            itemToDraw->setPen(QPen(Qt::red, 3, Qt::SolidLine));
            itemToDraw->setPos(origPoint);
        }
        itemToDraw->setLine(0,0,
                            event->scenePos().x() - origPoint.x(),
                            event->scenePos().y() - origPoint.y());
    }
    else
        QGraphicsScene::mouseMoveEvent(event);
}

void CustomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if (sceneMode == DrawLine){
        auto *currentLine = new QLine();
        currentLine->setLine(origPoint.x(), origPoint.y(), itemToDraw->line().x2()+origPoint.x(), itemToDraw->line().y2()+origPoint.y());
        lines.insert(itemToDraw, currentLine);
    }
    itemToDraw = 0;
    QGraphicsScene::mouseReleaseEvent(event);
}

void CustomScene::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Delete){
        foreach(QGraphicsItem* item, selectedItems()){
            removeItem(item);
            lines.remove(itemToDraw);
            delete item;
        }
    }
    else
        QGraphicsScene::keyPressEvent(event);
}

