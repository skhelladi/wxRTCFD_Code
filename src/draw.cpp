#include "draw.h"

//
// wxBEGIN_EVENT_TABLE(Draw, wxPanel)
//    EVT_PAINT(Draw::paintEvent)
// wxEND_EVENT_TABLE()

BEGIN_EVENT_TABLE(Draw, wxPanel)
// some useful events

EVT_MOTION(Draw::onMouseMoved)
EVT_LEFT_DOWN(Draw::onMouseDown)
EVT_LEFT_UP(Draw::onMouseReleased)
/*  EVT_RIGHT_DOWN(Draw::rightClick)
 EVT_LEAVE_WINDOW(Draw::mouseLeftWindow)
 EVT_KEY_DOWN(Draw::keyPressed)
 EVT_KEY_UP(Draw::keyReleased)
 EVT_MOUSEWHEEL(Draw::mouseWheelMoved)
 */
 //     EVT_TIMER(Draw::m_timer,Draw::animate)

 // catch paint events
    EVT_PAINT(Draw::paintEvent)

    END_EVENT_TABLE()

    Draw::Draw(wxWindow* parent, shared_ptr<Region> _region)
    : wxPanel(parent), region(_region)
{
    //  m_timer = new wxTimer(this);
    //  m_timer->SetOwner( this, wxID_ANY );
    //  m_timer->Bind(wxEVT_TIMER, &animate, this);
    loop = 0;
}

void Draw::animate(wxTimerEvent& event)
{
    //    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    //    cout<<loop++<<endl;

    Refresh();
    //    Update();
}

void Draw::paintEvent(wxPaintEvent& event)
{
    region->update();
    wxPaintDC dc(this);
    paint(dc);
    //    wxPaintDC dc(this);
    //    if(m_Bitmap.IsOk())
    //    {
    //        dc.DrawBitmap(m_Bitmap,0,0);
    //    }
}

void Draw::getSize(int* width, int* height)
{
    wxPaintDC dc(this);
    dc.GetSize(width, height);
}

void Draw::paint(wxDC& dc)
{
    int height, width;

    dc.GetSize(&width, &height);

    //    cout<<height<<" - "<<width<<endl;

    //    region->sayhello();

    region->updateRegionSize(height, width);
    shared_ptr<Fluid> f = region->fluid;

    int n = f->numY;

    double cellScale = 1.1;

    double h = f->h;

    double minP = *min_element(f->p.begin(), f->p.end());
    double maxP = *max_element(f->p.begin(), f->p.end());

    double minVel = *min_element(f->Vel.begin(), f->Vel.end());
    double maxVel = *max_element(f->Vel.begin(), f->Vel.end());

    double minXVel = *min_element(f->u.begin(), f->u.end());
    double maxXVel = *max_element(f->u.begin(), f->u.end());

    double minYVel = *min_element(f->v.begin(), f->v.end());
    double maxYVel = *max_element(f->v.begin(), f->v.end());

    vector<int> color = { 255, 255, 255, 255 };

    m_pixelData = new unsigned char[3 * (width) * (height)];
    //    wxBitmap m_Bitmap(width,height);
    //    wxMemoryDC memDC;
    //    memDC.SelectObject(m_Bitmap);
    //    memDC.SetBackground(*wxWHITE_BRUSH);
    //    memDC.Clear();
    //    memDC.SetPen(*wxRED_PEN);
    //    memDC.SetBrush(*wxTRANSPARENT_BRUSH);
    //    memDC.DrawRectangle(wxRect(10, 10, 200, 200));
    //    memDC.SelectObject(wxNullBitmap);

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
                    color[2] = 255;//*s
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
                    int Xpos(x + xi);
                    int Ypos(yi);
                    if (Xpos >= 0 && Xpos < width && Ypos >= 0 && Ypos < height)
                    {
                        m_pixelData[3 * Xpos + 3 * Ypos * width] = r;
                        m_pixelData[3 * Xpos + 3 * Ypos * width + 1] = g;
                        m_pixelData[3 * Xpos + 3 * Ypos * width + 2] = b;
                    }
                }
            }
        }
    }

    wxBitmap b(width, height, 24);
    wxNativePixelData data(b);

    wxNativePixelData::Iterator p(data);

    int curPixelDataLoc = 0;

    //    #pragma omp parallel for reduction(+:curPixelDataLoc)
    for (int x = 0; x < height; ++x)
    {
        wxNativePixelData::Iterator rowStart = p;
        for (int y = 0; y < width; ++y, ++p)
        {
            p.Red() = m_pixelData[curPixelDataLoc++];
            p.Green() = m_pixelData[curPixelDataLoc++];
            p.Blue() = m_pixelData[curPixelDataLoc++];
        }
        p = rowStart;
        p.OffsetY(data, 1);
    }

    m_Bitmap = b;

    if (m_Bitmap.IsOk())
    {
        dc.DrawBitmap(m_Bitmap, 0, 0);
    }

    //      Refresh();
    //    Update();

    //        _painter->drawImage(0, 0, image);

    //        cout<<region->showVelocity<<endl;
    if (region->showVelocityVectors)
    {
        double scale = 0.025;
        //            #pragma omp parallel for

        for (int i = 0; i < f->numX; i += 5)
        {
            for (int j = 0; j < f->numY; j += 5)
            {

                double u = f->u[i * n + j];
                double v = f->v[i * n + j];

                double x0 = region->cX(i * h);
                double x1 = region->cX(i * h + u * scale);
                double y0 = region->cY(j * h);
                double y1 = region->cY(j * h + v * scale);

                dc.DrawLine(x0, y0, x1, y1);
                //                    QPainterPath velPath;
                //                    velPath.moveTo(x0, y0);
                //                    velPath.lineTo(x1, y1);
                //                    _painter->drawPath(velPath);
            }
        }
    }

    if (region->showStreamlines)
    {
        //        double segLen = f->h * 1.0;
        int numSegs = 10;
        for (int i = 1; i < f->numX - 1; i += 5)
        {
            for (int j = 1; j < f->numY - 1; j += 5)
            {

                double x = (i + 0.5) * f->h;
                double y = (j + 0.5) * f->h;

                //                    QPainterPath velPath;
                //                    velPath.moveTo(region->cX(x), region->cY(y));
                wxPoint pt[numSegs];
                pt[0] = wxPoint(region->cX(x), region->cY(y));
                int n;
                for (n = 1; n < numSegs; n++)
                {
                    double u = f->sampleField(x, y, U_FIELD);
                    double v = f->sampleField(x, y, V_FIELD);
                    //                        double l = sqrt(u * u + v * v);
                    //                         x += u/l * segLen;
                    //                         y += v/l * segLen;
                    x += u * 0.01;
                    y += v * 0.01;

                    //                    if (x > f->numX * f->h)
                    //                    {
                    //                        numSegs;
                    //                        break;
                    //                    }

                    pt[n] = wxPoint(region->cX(x), region->cY(y));
                    //                        velPath.lineTo(region->cX(x), region->cY(y));
                }

                dc.DrawLines(numSegs, pt);

                //                    _painter->drawPath(velPath);
            }
        }
    }

    if (region->showObstacle)
    {
        //! c.strokeW;
        double r = region->characteristic_length + 0.5 * f->h;

        if (region->obstacle == CYLINDER)
            dc.DrawCircle(region->cX(region->obstacleX), region->cY(region->obstacleY), region->cScale * r);
        else if (region->obstacle == SQUARE)
        {
            vector<wxPoint> ppt = getSquarePoints(wxPoint(region->cX(region->obstacleX), region->cY(region->obstacleY)), region->cScale * r);
            wxPoint* pt = fromVectorToPtr(ppt);
            dc.DrawPolygon(4, pt);
            // dc.DrawRectangle(region->cX(region->obstacleX - r), region->cY(region->obstacleY + r), 2.0 * region->cScale * r, 2.0 * region->cScale * r);
        }
        else if (region->obstacle == DIAMOND)
        {
            vector<wxPoint> ppt = getDiamondPoints(wxPoint(region->cX(region->obstacleX), region->cY(region->obstacleY)), region->cScale * r);
            wxPoint* pt = fromVectorToPtr(ppt);
            dc.DrawPolygon(4, pt);
        }
        else if (region->obstacle == NACA)
        {
            vector<wxPoint> ppt = getNacaPoints(wxPoint(region->cX(region->obstacleX), region->cY(region->obstacleY)), region->cScale * r);
            wxPoint* pt = fromVectorToPtr(ppt);
            dc.DrawPolygon(ppt.size(), pt);
        }
        else if (region->obstacle == ROTOR)
        {
            wxPoint center(region->cX(region->obstacleX), region->cY(region->obstacleY));
            wxPoint pos(region->cX(region->obstacleX - 0.1), region->cY(region->obstacleY - 0.3));

            pos = rotatePolygon({ pos }, center, -rotation_angle)[0];
            region->setObstacle(region->obstacleX, region->obstacleY, true);

            // cout << "Draw - rotation_angle = " << rotation_angle << endl;

            vector<wxPoint> ppt = getNacaPoints(pos, region->cScale * r * 0.5);
            wxPoint* pt = fromVectorToPtr(ppt);
            dc.DrawPolygon(ppt.size(), pt);
            wxGraphicsContext* gc = wxGraphicsContext::Create(&dc);

            vector<wxPoint> ppt2 = rotatePolygon(ppt, center, 2.0 * M_PI / 3);
            wxPoint* pt2 = fromVectorToPtr(ppt2);
            dc.DrawPolygon(ppt.size(), pt2);

            vector<wxPoint> ppt3 = rotatePolygon(ppt, center, 4.0 * M_PI / 3);
            wxPoint* pt3 = fromVectorToPtr(ppt3);
            dc.DrawPolygon(ppt.size(), pt3);



        }
        else if (region->obstacle == MULTI_OBJECTS)
        {
            vector<wxPoint> ppt = getSquarePoints(wxPoint(region->cX(region->obstacleX), region->cY(region->obstacleY)), region->cScale * r * 0.5);
            wxPoint* pt = fromVectorToPtr(ppt);
            dc.DrawPolygon(4, pt);

            vector<wxPoint> ppt2 = getSquarePoints(wxPoint(region->cX(region->obstacleX + 0.4), region->cY(region->obstacleY)), region->cScale * r * 0.5);
            wxPoint* pt2 = fromVectorToPtr(ppt2);
            dc.DrawPolygon(4, pt2);

            vector<wxPoint> ppt3 = getSquarePoints(wxPoint(region->cX(region->obstacleX + 0.8), region->cY(region->obstacleY)), region->cScale * r * 0.5);
            wxPoint* pt3 = fromVectorToPtr(ppt3);
            dc.DrawPolygon(ppt3.size(), pt3);
        }

    }

    if (region->showPressure)
    {
        wxString s = "pressure: " + to_string(minP) + " - " + to_string(maxP) + " Pa";
        dc.DrawText(s, region->width / 2, 25);
    }

    if (region->showVelocity)
    {
        wxString s = "velocity: " + to_string(minVel) + " - " + to_string(maxVel) + " m/s";
        dc.DrawText(s, region->width / 2, 25);
    }

    if (region->showXVelocity)
    {
        wxString s = "x-velocity: " + to_string(minXVel) + " - " + to_string(maxXVel) + " m/s";
        dc.DrawText(s, region->width / 2, 25);
    }

    if (region->showYVelocity)
    {
        wxString s = "y-velocity: " + to_string(minYVel) + " - " + to_string(maxYVel) + " m/s";
        dc.DrawText(s, region->width / 2, 25);
    }

    if (region->showObstaclePosition)
    {
        wxString s = "Obstacle position: (" + to_string(region->obstacleX) + " , " + to_string(region->obstacleY) + ")";
        dc.DrawText(s, region->width / 2, region->height - 25);
    }
}

//
//
//// some useful events
///*
void Draw::onMouseMoved(wxMouseEvent& event)
{
    wxCoord sx, sy;
    event.GetPosition(&sx, &sy);

    int mx = sx; // - bounds.left - region->canvas.clientLeft;
    int my = sy; // - bounds.top - region->clientTop;

    double x = mx / region->cScale;
    double y = (region->height - my) / region->cScale;

    if (mouseDown)
        region->setObstacle(x, y, false);

    Refresh();
}
void Draw::onMouseDown(wxMouseEvent& event)
{
    wxCoord sx, sy;
    event.GetPosition(&sx, &sy);
    //
    //    cout<<"("<<x<<","<<y<<")"<<endl;
    int mx = sx; // - bounds.left - region->canvas.clientLeft;
    int my = sy; // - bounds.top - region->clientTop;

    double x = mx / region->cScale;
    double y = (region->height - my) / region->cScale;

    region->setObstacle(x, y, false);

    mouseDown = true;

    Refresh();
}

void Draw::onMouseReleased(wxMouseEvent& event)
{
    mouseDown = false;
}

// void Draw::mouseWheelMoved(wxMouseEvent& event) {}
//
// void Draw::rightClick(wxMouseEvent& event) {}
// void Draw::mouseLeftWindow(wxMouseEvent& event) {}
// void Draw::keyPressed(wxKeyEvent& event) {}
// void Draw::keyReleased(wxKeyEvent& event) {}
// */

vector<int> getSciColor(double val, double minVal, double maxVal)
{
    val = fmin(fmax(val, minVal), maxVal - 0.0001);
    double d = maxVal - minVal;
    val = (d == 0.0) ? 0.5 : (val - minVal) / d;
    double m = 0.25;
    int num = floor(val / m);
    double s = (val - num * m) / m;
    double r = 0, g = 0, b = 0;

    switch (num)
    {
    case 0:
        r = 0.0;
        g = s;
        b = 1.0;
        break;
    case 1:
        r = 0.0;
        g = 1.0;
        b = 1.0 - s;
        break;
    case 2:
        r = s;
        g = 1.0;
        b = 0.0;
        break;
    case 3:
        r = 1.0;
        g = 1.0 - s;
        b = 0.0;
        break;
    }

    return { int(255 * r), int(255 * g), int(255 * b), 255 };
}
