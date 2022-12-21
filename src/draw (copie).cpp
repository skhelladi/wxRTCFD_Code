#include "draw.h"


Draw::Draw(QWidget *parent)
    : QOpenGLWidget(parent)
{
    elapsed = 0;

    setAutoFillBackground(false);

    background = QBrush(QColor(200, 200, 200));
    circlePen = QPen(Qt::black);
    circlePen.setWidth(1);
    textPen = QPen(Qt::black);
    textFont.setPixelSize(12);

    painter = new QPainter();
    setAcceptDrops(true);
}

Draw::~Draw()
{
    delete painter;
}

void Draw::paint(QPainter *_painter, QPaintEvent *event)
{
    _painter->setRenderHint(QPainter::Antialiasing);
    _painter->fillRect(event->rect(), background);
//    _painter->translate(QPoint(50,-10));

//    region->width = width();
//    region->height = height();

    _painter->save();
    {

//        region->updateRegionSize(region->height,region->width);
        region->updateRegionSize(height(),width());
        shared_ptr<Fluid> f = region->fluid;
        int n = f->numY;

        double cellScale = 1.1;

        double h = f->h;

        double minP = *min_element(f->p.begin(),f->p.end());
        double maxP = *max_element(f->p.begin(),f->p.end());

        double minVel = *min_element(f->Vel.begin(),f->Vel.end());
        double maxVel = *max_element(f->Vel.begin(),f->Vel.end());

        double minXVel = *min_element(f->u.begin(),f->u.end());
        double maxXVel = *max_element(f->u.begin(),f->u.end());

        double minYVel = *min_element(f->v.begin(),f->v.end());
        double maxYVel = *max_element(f->v.begin(),f->v.end());


        vector<int> color = {255, 255, 255, 255};

        QImage image(width(),height(), QImage::Format_ARGB32);//QImage::Format_RGB888
//        QImage image(region->width,region->height, QImage::Format_ARGB32);
        image.fill(QColor(Qt::white).rgb());

        for (int i = 0; i < f->numX; i++)
        {
            for (int j = 0; j < f->numY; j++)
            {

                if (region->showPressure)
                {
                    double p = f->p[i * n + j];
                    double s = f->m[i * n + j];
                    color = getSciColor(p, minP, maxP);
                    if (region->showTracer)
                    {
                        color[0] = fmax(0.0, color[0] - 255 * s);
                        color[1] = fmax(0.0, color[1] - 255 * s);
                        color[2] = fmax(0.0, color[2] - 255 * s);
                    }
                }
                else if (region->showVelocity)
                {
                    double s = f->m[i * n + j];
                    double Vel = f->Vel[i * n + j];
                    color = getSciColor(Vel, minVel, maxVel);
                    if (region->showTracer)
                    {
                        color[0] = fmax(0.0, color[0] - 255 * s);
                        color[1] = fmax(0.0, color[1] - 255 * s);
                        color[2] = fmax(0.0, color[2] - 255 * s);
                    }
                }
                else if (region->showXVelocity)
                {
                    double s = f->m[i * n + j];
                    double Vel = f->u[i * n + j];
                    color = getSciColor(Vel, minXVel, maxXVel);
                    if (region->showTracer)
                    {
                        color[0] = fmax(0.0, color[0] - 255 * s);
                        color[1] = fmax(0.0, color[1] - 255 * s);
                        color[2] = fmax(0.0, color[2] - 255 * s);
                    }
                }
                else if (region->showYVelocity)
                {
                    double s = f->m[i * n + j];
                    double Vel = f->v[i * n + j];
                    color = getSciColor(Vel, minYVel, maxYVel);
                    if (region->showTracer)
                    {
                        color[0] = fmax(0.0, color[0] - 255 * s);
                        color[1] = fmax(0.0, color[1] - 255 * s);
                        color[2] = fmax(0.0, color[2] - 255 * s);
                    }
                }
                else if (region->showTracer)
                {
                    double s = f->m[i * n + j];

                    if (region->RegionNr == 2)
                    {
                        color = getSciColor(s, 0.0, 1.0);
                    }
                    else
                    {
                        color[0] = 255 * s;
                        color[1] = 255 * s;
                        color[2] = 255 * s;
                    }
                }
                else if (f->s[i * n + j] == 0.0)
                {
                    color[0] = 255;
                    color[1] = 255;
                    color[2] = 255;
                }

                int x = floor(region->cX(i * h));
                int y = floor(region->cY((j + 1) * h));
                int cx = floor(region->cScale * cellScale * h) + 1;
                int cy = floor(region->cScale * cellScale * h) + 1;

                int r = color[0];
                int g = color[1];
                int b = color[2];

                for (int yi = y; yi < y + cy; yi++)
                {
                    for (int xi = 0; xi < cx; xi++)
                    {
                        int Xpos(x+xi);
                        int Ypos(yi);
                      if(Xpos>=0&&Xpos<width()&&Ypos>=0&&Ypos<height())
                          image.setPixel(x+xi, yi, qRgba(r,g,b,255));
//                      else
//                      {
//                          cout<<"("<<Xpos<<","<<width()<<") -- ("<<Ypos<<","<<height()<<")"<<endl;
//                      }
                    }

                }

            }
        }
        _painter->drawImage(0, 0, image);

//        cout<<region->showVelocity<<endl;
        if (region->showVelocityVectors)
        {
            double scale = 0.025;
            for (int i = 0; i < f->numX; i++)
            {
                for (int j = 0; j < f->numY; j++)
                {

                    double u = f->u[i * n + j];
                    double v = f->v[i * n + j];

                    double x0 = region->cX(i * h);
                    double x1 = region->cX(i * h + u * scale);
                    double y0 = region->cY(j * h);
                    double y1 = region->cY(j * h + v * scale);


                    QPainterPath velPath;
                    velPath.moveTo(x0, y0);
                    velPath.lineTo(x1, y1);
                    _painter->drawPath(velPath);

                }
            }

        }

        if (region->showStreamlines)
        {
//            double segLen = f->h * 1.0;
            int numSegs = 15;

            for (int i = 1; i < f->numX - 1; i += 5)
            {
                for (int j = 1; j < f->numY - 1; j += 5)
                {

                    double x = (i + 0.5) * f->h;
                    double y = (j + 0.5) * f->h;

                    QPainterPath velPath;
                    velPath.moveTo(region->cX(x), region->cY(y));

                    for (int n = 0; n < numSegs; n++)
                    {
                        double u = f->sampleField(x, y, U_FIELD);
                        double v = f->sampleField(x, y, V_FIELD);
//                        double l = sqrt(u * u + v * v);
//                         x += u/l * segLen;
//                         y += v/l * segLen;

                        x += u * 0.01;
                        y += v * 0.01;
                        if (x > f->numX * f->h)
                            break;

                        velPath.lineTo(region->cX(x), region->cY(y));
                    }

                    _painter->drawPath(velPath);

//                    repaint();

                }
            }
        }

        if (region->showObstacle)
        {
            //! c.strokeW;
            double r = region->obstacleRadius + f->h;
//            if (region->showPressure)
//            {
//                circleBrush = QBrush(QColor(0,0,0));
//            }
//            else
//            {
//                circleBrush = QBrush(QColor(255,255,255));
//            }
             circleBrush = QBrush(QColor(255,255,255));

            _painter->setBrush(circleBrush);
            _painter->setPen(circlePen);
            _painter->drawEllipse(QPointF(region->cX(region->obstacleX),region->cY(region->obstacleY)),region->cScale * r, region->cScale * r);
//            repaint();
        }

        //        cout<<region->cX(region->obstacleX)<<" -- "<<region->cY(region->obstacleY)<<endl;
        if (region->showPressure)
        {
            string s = "pressure: " + to_string(minP) + " - " + to_string(maxP) + " Pa";
            _painter->setPen(textPen);
            _painter->setFont(textFont);
            QPointF pt(region->width/2,25);
            drawText(_painter,pt,Qt::AlignHCenter,QString::fromStdString(s));
        }

        if (region->showVelocity)
        {
            string s = "velocity: " + to_string(minVel) + " - " + to_string(maxVel) + " m/s";
            _painter->setPen(textPen);
            _painter->setFont(textFont);
            QPointF pt(region->width/2,25);
            drawText(_painter,pt,Qt::AlignHCenter,QString::fromStdString(s));
        }

        if (region->showXVelocity)
        {
            string s = "x-velocity: " + to_string(minXVel) + " - " + to_string(maxXVel) + " m/s";
            _painter->setPen(textPen);
            _painter->setFont(textFont);
            QPointF pt(region->width/2,25);
            drawText(_painter,pt,Qt::AlignHCenter,QString::fromStdString(s));
        }

        if (region->showYVelocity)
        {
            string s = "y-velocity: " + to_string(minYVel) + " - " + to_string(maxYVel) + " m/s";
            _painter->setPen(textPen);
            _painter->setFont(textFont);
            QPointF pt(region->width/2,25);
            drawText(_painter,pt,Qt::AlignHCenter,QString::fromStdString(s));
        }

        if(region->showObstaclePosition)
        {
            string s = "Obstacle position: (" + to_string(region->obstacleX) + " , " + to_string(region->obstacleY) + ")";
            painter->setPen(textPen);
            painter->setFont(textFont);
            QPointF p(region->width/2,region->height-25);
            drawText(painter,p,Qt::AlignHCenter,QString::fromStdString(s));
        }

    }



    _painter->restore();

}

void Draw::animate()
{
//    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    update();
}



void Draw::paintEvent(QPaintEvent *event)
{
    painter->begin(this);
    {
        region->update();
        painter->setRenderHint(QPainter::Antialiasing);
        paint(painter, event);
    }
    painter->end();
}

void Draw::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pt=event->pos();
//    cout<<pt.x()<<" - "<<pt.y()<<endl;

    int mx = pt.x();// - bounds.left - region->canvas.clientLeft;
    int my = pt.y();// - bounds.top - region->clientTop;

    double x = mx / region->cScale;
    double y = (region->height - my) / region->cScale;

    region->setObstacle(x, y, false);


//    region->update();

}



vector<int> getSciColor(double val, double minVal, double maxVal)
{
    val = fmin(fmax(val, minVal), maxVal - 0.0001);
    double d = maxVal - minVal;
    val = (d == 0.0)? 0.5 : (val - minVal) / d;
    double m = 0.25;
    int num = floor(val / m);
    double s = (val - num * m) / m;
    double  r=0, g=0, b=0;

    switch (num)
    {
    case 0: r = 0.0; g = s; b = 1.0; break;
    case 1: r = 0.0; g = 1.0; b = 1.0 - s; break;
    case 2: r = s; g = 1.0; b = 0.0; break;
    case 3: r = 1.0; g = 1.0 - s; b = 0.0; break;
    }

    return {int(255 * r), int(255 * g), int(255 * b), 255};
}

void drawText(QPainter *painter, qreal x, qreal y, Qt::Alignment flags, const QString & text, QRectF * boundingRect)
{
    const qreal size = 32767.0;
    QPointF corner(x, y - size);
    if (flags & Qt::AlignHCenter) corner.rx() -= size/2.0;
    else if (flags & Qt::AlignRight) corner.rx() -= size;
    if (flags & Qt::AlignVCenter) corner.ry() += size/2.0;
    else if (flags & Qt::AlignTop) corner.ry() += size;
    else flags |= Qt::AlignBottom;
    QRectF rect{corner.x(), corner.y(), size, size};
    painter->drawText(rect, flags, text, boundingRect);
}

void drawText(QPainter *painter, const QPointF & point, Qt::Alignment flags, const QString & text, QRectF * boundingRect)
{
    drawText(painter, point.x(), point.y(), flags, text, boundingRect);
}
