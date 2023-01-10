#include "region.h"

Region::Region(int _height, int _width, double _simHeight)
{
    height = _height;
    width = _width;
    simHeight = _simHeight;
    cScale = height / simHeight;
    simWidth = width / cScale;
    obstacle = CYLINDER;

#if (DEBUG)
    cout << "------------------" << endl;
    cout << "Region constructor" << endl;
    cout << "------------------" << endl;
    cout << "height = " << height << endl;
    cout << "width = " << width << endl;
    cout << "simHeight = " << simHeight << endl;
    cout << "cScale = " << cScale << endl;
    cout << "simWidth = " << simWidth << endl;
#endif
}

void Region::setupRegion(int _RegionNr, double _overRelaxation, int _resolution, double _density, int _numThreads)
{
    this->RegionNr = _RegionNr;
    this->characteristic_length = 0.15;
    this->overRelaxation = _overRelaxation;
    this->numThreads = _numThreads;

    this->dt = 1.0 / 60.0;
    this->numIters = 40;

    this->obstacleX = 0;
    this->obstacleY = 0;
    this->resolution = _resolution;

    int res = this->resolution;

    if (RegionNr == 0)
        res = this->resolution;
    else if (RegionNr == 3)
        res = 2 * this->resolution;

    double domainHeight = 1.0;
    double domainWidth = domainHeight / simHeight * simWidth;
    double h = domainHeight / res;

    int numX = floor(domainWidth / h);
    int numY = floor(domainHeight / h);

    double density = _density;

    this->fluid = make_shared<Fluid>(density, numX, numY, h, overRelaxation, numThreads);
    shared_ptr<Fluid> f = this->fluid;

    int n = f->numY;
    Point pos;
    if(obstacle!=ROTOR)
    {
      pos = {0.4,0.5};
    }

    if (RegionNr == 0)
    { // tank
#pragma omp parallel for schedule(static) num_threads(numThreads)
        for (int i = 0; i < f->numX; i++)
        {
            for (int j = 0; j < f->numY; j++)
            {
                double s = 1.0; // fluid
                if (i == 0 || i == f->numX - 1 || j == 0)
                    s = 0.0; // solid
                f->s[i * n + j] = s;
            }
        }
        setObstacle(pos.x, pos.y, true);
        // CALL_SET_OBSTACLE(this,setObstacle)(0.4, 0.5, true);
        this->gravity = -9.81;
        this->showPressure = true;
        this->showTracer = false;
        this->showStreamlines = false;
        this->showVelocity = false;
        this->showXVelocity = false;
        this->showYVelocity = false;
        this->showVelocityVectors = false;
        this->showObstacle = true;
        this->showObstaclePosition = false;
    }
    else if (RegionNr == 1 || RegionNr == 3)
    { // vortex shedding

        double inVel = 2.0;
#pragma omp parallel for schedule(static) num_threads(numThreads)
        for (int i = 0; i < f->numX; i++)
        {
            for (int j = 0; j < f->numY; j++)
            {
                double s = 1.0; // fluid
                if (i == 0 || j == 0 || j == f->numY - 1)
                    s = 0.0; // solid

                f->s[i * n + j] = s;

                if (i == 1)
                {
                    f->u[i * n + j] = inVel;
                }
            }
        }

        double pipeH = 0.1 * f->numY;
        int minJ = floor(0.5 * f->numY - 0.5 * pipeH);
        int maxJ = floor(0.5 * f->numY + 0.5 * pipeH);

        for (int j = minJ; j < maxJ; j++)
            f->m[j] = 0.0;

        setObstacle(pos.x, pos.y, true);

        this->gravity = 0.0;
        this->showPressure = false;
        this->showTracer = true;
        this->showStreamlines = false;
        this->showVelocity = false;
        this->showXVelocity = false;
        this->showYVelocity = false;
        this->showVelocityVectors = false;

        if (RegionNr == 3)
        {
            this->dt = 1.0 / 60.0; // 1/120
            this->numIters = 40;   // 100
            this->showPressure = true;
        }
    }
    else if (RegionNr == 2)
    { // paint
        this->gravity = 0.0;
        this->overRelaxation = 1.0;
        this->showPressure = false;
        this->showTracer = true;
        this->showStreamlines = false;
        this->showVelocity = false;
        this->showXVelocity = false;
        this->showYVelocity = false;
        this->showVelocityVectors = false;
        this->characteristic_length = 0.075;
        this->showObstacle = true;
        this->showObstaclePosition = false;
        setObstacle(pos.x, pos.y, true);
    }

    //    document.getElementById("streamButton").checked = this->showStreamlines;
    //    document.getElementById("velocityButton").checked = this->showVelocity;
    //    document.getElementById("pressureButton").checked = this->showPressure;
    //    document.getElementById("smokeButton").checked = this->showTracer;
    //    document.getElementById("overrelaxButton").checked = this->overRelaxation > 1.0;
}

void Region::setObstacle(double x, double y, bool reset)
{
    switch (obstacle)
    {
    case CYLINDER:
        setObstacleCylinder(x, y, reset);
        break;
    case SQUARE:
        setObstacleSquare(x, y, reset);
        break;
    case DIAMOND:
        setObstacleDiamond(x, y, reset);
        break;
    case NACA:
        setObstacleNaca(x, y, reset);
        break;
    case ROTOR:
        setObstacleRotor(x, y, reset);
        break;
    default:
        setObstacleCylinder(x, y, reset);
    }
}

void Region::setObstacleCylinder(double x, double y, bool reset)
{

    double vx = 0.0;
    double vy = 0.0;

    if (!reset)
    {
        vx = (x - this->obstacleX) / this->dt;
        vy = (y - this->obstacleY) / this->dt;
    }

    this->obstacleX = x;
    this->obstacleY = y;
    double r = this->characteristic_length;
    shared_ptr<Fluid> f = this->fluid;
    int n = f->numY;
    //    double cd = sqrt(2) * f->h;
#pragma omp parallel for schedule(static) num_threads(fluid->numThreads)
    for (int i = 1; i < f->numX - 2; i++)
    {
        for (int j = 1; j < f->numY - 2; j++)
        {

            f->s[i * n + j] = 1.0;

            double dx = (i + 0.5) * f->h - x;
            double dy = (j + 0.5) * f->h - y;

            if (dx * dx + dy * dy < r * r)
            {
                f->s[i * n + j] = 0.0;
                if (this->RegionNr == 2)
                    f->m[i * n + j] = 0.5 + 0.5 * sin(0.1 * this->frameNr);
                else
                    f->m[i * n + j] = 1.0;

                f->u[i * n + j] = vx;
                f->u[(i + 1) * n + j] = vx;
                f->v[i * n + j] = vy;
                f->v[i * n + j + 1] = vy;
            }
        }
    }

    this->showObstacle = true;
    //    this->showObstaclePosition=true;
}

void Region::setObstacleSquare(double x, double y, bool reset)
{

    double vx = 0.0;
    double vy = 0.0;

    if (!reset)
    {
        vx = (x - this->obstacleX) / this->dt;
        vy = (y - this->obstacleY) / this->dt;
    }

    this->obstacleX = x;
    this->obstacleY = y;
    double r = this->characteristic_length;
    shared_ptr<Fluid> f = this->fluid;

    vector<Point> P = getSquarePoints(Point({x, y}), r);

    int n = f->numY;
    #pragma omp parallel for schedule(static) num_threads(f->numThreads)
    for (int i = 1; i < f->numX - 2; i++)
    {
        for (int j = 1; j < f->numY - 2; j++)
        {

            f->s[i * n + j] = 1.0;

            // double dx = (i + 0.5) * f->h - x;
            // double dy = (j + 0.5) * f->h - y;
            
            // if (fabs(dx)<r&&fabs(dy)<r)
            if (isInsidePolygon(P, Point({(i + 0.5) * f->h, (j + 0.5) * f->h})))
            {
                f->s[i * n + j] = 0.0;
                if (this->RegionNr == 2)
                    f->m[i * n + j] = 0.5 + 0.5 * sin(0.1 * this->frameNr);
                else
                    f->m[i * n + j] = 1.0;

                f->u[i * n + j] = vx;
                f->u[(i + 1) * n + j] = vx;
                f->v[i * n + j] = vy;
                f->v[i * n + j + 1] = vy;
            }
        }
    }

    this->showObstacle = true;
}

void Region::setObstacleDiamond(double x, double y, bool reset)
{
    double vx = 0.0;
    double vy = 0.0;

    if (!reset)
    {
        vx = (x - this->obstacleX) / this->dt;
        vy = (y - this->obstacleY) / this->dt;
    }

    this->obstacleX = x;
    this->obstacleY = y;
    double r = this->characteristic_length;
    shared_ptr<Fluid> f = this->fluid;
    int n = f->numY;
    Point center = {x, y};
    vector<Point> P = getDiamondPoints(center, r);
    //    double cd = sqrt(2) * f->h;
#pragma omp parallel for schedule(static) num_threads(f->numThreads)
    for (int i = 1; i < f->numX - 2; i++)
    {
        for (int j = 1; j < f->numY - 2; j++)
        {

            f->s[i * n + j] = 1.0;

            // double dx = (i + 0.5) * f->h - x;
            // double dy = (j + 0.5) * f->h - y;

            // //! axis change by a rotation of theta=pi/4
            // double dxb = sqrt(2) / 2 * (dx + dy);
            // double dyb = sqrt(2) / 2 * (-dx + dy);

            // if (fabs(dxb) < r && fabs(dyb) < r)
            // Point M = {(i + 0.5) * f->h ,(j + 0.5) * f->h };
            
            if (isInsidePolygon(P, Point({(i + 0.5) * f->h, (j + 0.5) * f->h})))
            {
                f->s[i * n + j] = 0.0;
                if (this->RegionNr == 2)
                    f->m[i * n + j] = 0.5 + 0.5 * sin(0.1 * this->frameNr);
                else
                    f->m[i * n + j] = 1.0;

                f->u[i * n + j] = vx;
                f->u[(i + 1) * n + j] = vx;
                f->v[i * n + j] = vy;
                f->v[i * n + j + 1] = vy;
            }
        }
    }

    this->showObstacle = true;
}

void Region::setObstacleNaca(double x, double y, bool reset)
{
    double vx = 0.0;
    double vy = 0.0;

    if (!reset)
    {
        vx = (x - this->obstacleX) / this->dt;
        vy = (y - this->obstacleY) / this->dt;
    }

    this->obstacleX = x;
    this->obstacleY = y;
    double r = this->characteristic_length;
    shared_ptr<Fluid> f = this->fluid;
    int n = f->numY;
    Point center = {x, y};
    vector<Point> P = getNacaPoints(center, r);
    //    double cd = sqrt(2) * f->h;
#pragma omp parallel for schedule(static) num_threads(f->numThreads)
    for (int i = 1; i < f->numX - 2; i++)
    {
        for (int j = 1; j < f->numY - 2; j++)
        {

            f->s[i * n + j] = 1.0;

            // double dx = (i + 0.5) * f->h - x;
            // double dy = (j + 0.5) * f->h - y;

            // //! axis change by a rotation of theta=pi/4
            // double dxb = sqrt(2) / 2 * (dx + dy);
            // double dyb = sqrt(2) / 2 * (-dx + dy);

            // if (fabs(dxb) < r && fabs(dyb) < r)
            // Point M = {(i + 0.5) * f->h ,(j + 0.5) * f->h };
            
            if (isInsidePolygon(P, Point({(i + 0.5) * f->h, (j + 0.5) * f->h})))
            {
                f->s[i * n + j] = 0.0;
                if (this->RegionNr == 2)
                    f->m[i * n + j] = 0.5 + 0.5 * sin(0.1 * this->frameNr);
                else
                    f->m[i * n + j] = 1.0;

                f->u[i * n + j] = vx;
                f->u[(i + 1) * n + j] = vx;
                f->v[i * n + j] = vy;
                f->v[i * n + j + 1] = vy;
            }
        }
    }

    this->showObstacle = true;
}

void Region::setObstacleRotor(double x, double y, bool reset)
{
}

void Region::updateRegionSize(int _height, int _width)
{

    height = _height;
    width = _width;
    cScale = height / simHeight;
    simWidth = width / cScale;

#if (DEBUG)
    cout << "----------------" << endl;
    cout << "updateRegionSize" << endl;
    cout << "----------------" << endl;
    cout << "height = " << height << endl;
    cout << "width = " << width << endl;
    cout << "simHeight = " << simHeight << endl;
    cout << "cScale = " << cScale << endl;
    cout << "simWidth = " << simWidth << endl;
#endif
}

void Region::update()
{
    if (!paused)
    {
        fluid->simulate(dt, gravity, numIters);
    }
}

OBJ indexToOBJ(int index)
{
    switch (index)
    {
    case 0 : return CYLINDER;
    case 1 : return SQUARE;
    case 2 : return DIAMOND;
    case 3 : return NACA;
    case 4 : return ROTOR;
    default: return CYLINDER;
    }
    
}