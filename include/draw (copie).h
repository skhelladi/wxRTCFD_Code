#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPen>
#include <QTimer>
#include <QMimeData>
#include <QDrag>
#include <math.h>
#include <vector>
#include <iostream>
#include "region.h"

//QT_BEGIN_NAMESPACE
//class QDragEnterEvent;
//class QDropEvent;
//QT_END_NAMESPACE

using namespace std;

class Draw: public QOpenGLWidget
{
    Q_OBJECT

public:
    Draw(QWidget *parent);
    ~Draw();
    void setRegion(shared_ptr<Region> s){region=s;}
    void paint(QPainter *painter, QPaintEvent *event);
    shared_ptr<Region> region;
public slots:
    void animate();

protected:    
//    void dragEnterEvent(QDragEnterEvent *event) override;
//    void dragLeaveEvent(QDragLeaveEvent *event) override;
//    void dragMoveEvent(QDragMoveEvent *event) override;
//    void dropEvent(QDropEvent *event) override;
//    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    int elapsed;

    QBrush background;
    QBrush circleBrush;
    QFont textFont;
    QPen circlePen;
    QPen textPen;

    QPainter *painter;
    bool mouseDown;
    bool dragOver=false;

};

vector<int> getSciColor(double val, double minVal, double maxVal);
void drawText(QPainter *painter, qreal x, qreal y, Qt::Alignment flags, const QString & text, QRectF * boundingRect = 0);
void drawText(QPainter *painter, const QPointF & point, Qt::Alignment flags, const QString & text, QRectF * boundingRect = {});
#endif // DRAW_H
