#include "fluid.h"

Fluid::Fluid(double _density, int _numX, int _numY, double _h, double _overRelaxation, int _numThreads)
{
    density = _density;
    numX = _numX + 2;
    numY = _numY + 2;
    numCells = numX * numY;
    h = _h;
    u.resize(numCells);
    v.resize(numCells);
    newU.resize(numCells);
    newV.resize(numCells);
    Vel.resize(numCells);
    p.resize(numCells);
    s.resize(numCells);
    m.resize(numCells,1.0);
    newM.resize(numCells);
    num = numX * numY;
    cnt=0;
    overRelaxation = _overRelaxation;
    numThreads = _numThreads;
}

void Fluid::integrate(double dt, double gravity)
{
    int n = numY;
    #pragma omp parallel for schedule(static) num_threads(numThreads)
    for (int i = 1; i < numX; i++)
    {
        for (int j = 1; j < numY - 1; j++)
        {
            if (s[i * n + j] != 0.0 && s[i * n + j - 1] != 0.0)
                #pragma omp atomic update
                v[i * n + j] += gravity * dt;
        }
    }
}

void Fluid::solveIncompressibility(int numIters, double dt)
{
    int n = numY;
    double cp = density * h / dt;
    for (int iter = 0; iter < numIters; iter++)
    {
#pragma omp parallel for schedule(static) num_threads(numThreads)
        for (int i = 1; i < numX - 1; i++)
        {
            for (int j = 1; j < numY - 1; j++)
            {
                if (s[i * n + j] == 0.0)
                    continue;

                //                double s_ = s[i * n + j];
                double sx0 = s[(i - 1) * n + j];
                double sx1 = s[(i + 1) * n + j];
                double sy0 = s[i * n + j - 1];
                double sy1 = s[i * n + j + 1];
                double _s = sx0 + sx1 + sy0 + sy1;

                if (_s == 0.0)
                    continue;

                double div = u[(i + 1) * n + j] - u[i * n + j] + v[i * n + j + 1] - v[i * n + j];

                double _p = -div / _s;
                #pragma omp atomic update
                _p *= overRelaxation;
                #pragma omp atomic update
                p[i * n + j] += cp * _p;
                #pragma omp atomic update
                u[i * n + j] -= sx0 * _p;
                #pragma omp atomic update
                u[(i + 1) * n + j] += sx1 * _p;
                #pragma omp atomic update
                v[i * n + j] -= sy0 * _p;
                #pragma omp atomic update
                v[i * n + j + 1] += sy1 * _p;
            }
        }
    }
}

void Fluid::extrapolate()
{
    int n = numY;
    #pragma omp parallel for schedule(static) num_threads(numThreads)
    for (int i = 0; i < numX; i++)
    {
        u[i * n + 0] = u[i * n + 1];
        u[i * n + numY - 1] = u[i * n + numY - 2];
    }
    #pragma omp parallel for schedule(static) num_threads(numThreads)
    for (int j = 0; j < numY; j++)
    {
        v[0 * n + j] = v[1 * n + j];
        v[(numX - 1) * n + j] = v[(numX - 2) * n + j];
    }
}

double Fluid::sampleField(double x, double y, int field)
{
    int n = numY;
    double h1 = 1.0 / h;
    double h2 = 0.5 * h;

    x = fmax(fmin(x, numX * h), h);
    y = fmax(fmin(y, numY * h), h);

    double dx = 0.0;
    double dy = 0.0;

    vector<double> f;

    switch (field)
    {
    case U_FIELD:
        f = u;
        dy = h2;
        break;
    case V_FIELD:
        f = v;
        dx = h2;
        break;
    case S_FIELD:
        f = m;
        dx = h2;
        dy = h2;
        break;
    }

    double x0 = fmin(floor((x - dx) * h1), numX - 1);
    double tx = ((x - dx) - x0 * h) * h1;
    double x1 = fmin(x0 + 1, numX - 1);

    double y0 = fmin(floor((y - dy) * h1), numY - 1);
    double ty = ((y - dy) - y0 * h) * h1;
    double y1 = fmin(y0 + 1, numY - 1);

    double sx = 1.0 - tx;
    double sy = 1.0 - ty;

    double val = sx * sy * f[x0 * n + y0] +
                 tx * sy * f[x1 * n + y0] +
                 tx * ty * f[x1 * n + y1] +
                 sx * ty * f[x0 * n + y1];

    return val;
}

double Fluid::avgU(int i, int j)
{
    int n = numY;
    return (u[i * n + j - 1] + u[i * n + j] +
            u[(i + 1) * n + j - 1] + u[(i + 1) * n + j]) * 0.25;
}

double Fluid::avgV(int i, int j)
{
    int n = numY;
    return (v[(i - 1) * n + j] + v[i * n + j] +
            v[(i - 1) * n + j + 1] + v[i * n + j + 1]) * 0.25;
}

void Fluid::ComputeVelMagnitude()
{
    int n = numY;
    #pragma omp parallel for schedule(static) num_threads(numThreads)
    for (int i = 0; i < numX; i++)
    {
        for (int j = 0; j < numY; j++)
        {
            Vel[i * n + j]=sqrt(pow(u[i * n + j],2)+pow(v[i * n + j],2));
        }
    }
}

void Fluid::advectVel(double dt)
{
    newU = u;
    newV = v;

    int n = numY;
    double h2 = 0.5 * h;
    #pragma omp parallel for schedule(static) num_threads(numThreads)
    for (int i = 1; i < numX; i++)
    {
        for (int j = 1; j < numY; j++)
        {
            #pragma omp atomic update
            cnt++;

            // u component
            if (s[i * n + j] != 0.0 && s[(i - 1) * n + j] != 0.0 && j < numY - 1)
            {
                double x = i * h;
                double y = j * h + h2;
                double _u = u[i * n + j];
                double _v = avgV(i, j);

                #pragma omp atomic update
                x -= dt * _u;
                #pragma omp atomic update
                y -= dt * _v;
                _u = sampleField(x, y, U_FIELD);
                newU[i * n + j] = _u;
            }
            // v component
            if (s[i * n + j] != 0.0 && s[i * n + j - 1] != 0.0 && i < numX - 1)
            {
                double x = i * h + h2;
                double y = j * h;
                double _u = avgU(i, j);
                double _v = v[i * n + j];
                #pragma omp atomic update
                x -= dt * _u;
                #pragma omp atomic update
                y -= dt * _v;
                _v = sampleField(x, y, V_FIELD);
                newV[i * n + j] = _v;
            }
        }
    }

    u=newU;
    v=newV;
}

void Fluid::advectSmoke(double dt)
{
    newM=m;

    int n = numY;
    double h2 = 0.5 * h;
    #pragma omp parallel for schedule(static) num_threads(numThreads)
    for (int i = 1; i < numX - 1; i++)
    {
        for (int j = 1; j < numY - 1; j++)
        {

            if (s[i * n + j] != 0.0)
            {
                double _u = (u[i * n + j] + u[(i + 1) * n + j]) * 0.5;
                double _v = (v[i * n + j] + v[i * n + j + 1]) * 0.5;
                double x = i * h + h2 - dt * _u;
                double y = j * h + h2 - dt * _v;

                newM[i * n + j] = sampleField(x, y, S_FIELD);
            }
        }
    }
    m=newM;
}

void Fluid::simulate(double dt, double gravity, int numIters)
{
    integrate(dt, gravity);
    fill(p.begin(),p.end(),0.0);
    solveIncompressibility(numIters, dt);
    extrapolate();
    advectVel(dt);
    advectSmoke(dt);
    ComputeVelMagnitude();
}

void Fluid::updateFluidParameters()
{
    numCells = numX * numY;
    u.resize(numCells);
    v.resize(numCells);
    newU.resize(numCells);
    newV.resize(numCells);
    p.resize(numCells);
    s.resize(numCells);
    m.resize(numCells,1.0);
    newM.resize(numCells);
    num = numX * numY;
}
