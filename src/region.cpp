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
    else if (RegionNr == 4)
        res = 2 * this->resolution;

    if (RegionNr == 2)
	this->FSI = true;
    else
	this->FSI = false;

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
    double theta = getTheta0();
    this->theta = theta;
    cout << "init :" << theta << endl;

    obstacleGeometricalFeatures();

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
        setObstacle(pos.x, pos.y, theta, true);
        // CALL_SET_OBSTACLE(this,setObstacle)(0.4, 0.5, true);
        //this->gravity = -9.81;
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
    else if (RegionNr == 1 || RegionNr == 2 || RegionNr == 4)
    { // vortex shedding

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
                    f->u[i * n + j] = this->inVel;
                }
            }
        }

        double pipeH = 0.1 * f->numY;
        int minJ = floor(0.5 * f->numY - 0.5 * pipeH);
        int maxJ = floor(0.5 * f->numY + 0.5 * pipeH);

        for (int j = minJ; j < maxJ; j++)
            f->m[j] = 0.0;

        setObstacle(pos.x, pos.y, theta, true);

        //this->gravity = 0.0;
        this->showPressure = false;
        this->showTracer = true;
        this->showStreamlines = false;
        this->showVelocity = false;
        this->showXVelocity = false;
        this->showYVelocity = false;
        this->showVelocityVectors = false;

	if (RegionNr == 2) this->showPressure = true;

        if (RegionNr == 4)
        {
            this->dt = 1.0 / 60.0; // 1/120
            this->numIters = 40;   // 100
            this->showPressure = true;
        }
    }
    else if (RegionNr == 3)
    { // paint
        //this->gravity = 0.0;
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
        setObstacle(pos.x, pos.y, theta, true);
    }

    //    document.getElementById("streamButton").checked = this->showStreamlines;
    //    document.getElementById("velocityButton").checked = this->showVelocity;
    //    document.getElementById("pressureButton").checked = this->showPressure;
    //    document.getElementById("smokeButton").checked = this->showTracer;
    //    document.getElementById("overrelaxButton").checked = this->overRelaxation > 1.0;
}

void Region::setObstacle(double x, double y, double theta, bool reset)
{
    switch (obstacle)
    {
    case CYLINDER:
        setObstacleCylinder(x, y, theta, reset);
        break;
    case SQUARE:
        setObstacleSquare(x, y, theta, reset);
        break;
    case NACA:
        setObstacleNaca(x, y, theta, reset);
        break;
    case ROTOR:
        setObstacleRotor(x, y, theta, reset);
        break;
    default:
        setObstacleCylinder(x, y, theta, reset);
    }
}


double Region::getTheta0()
{
    switch (obstacle)
    {
    case CYLINDER:
	return 0.0;
        break;
    case SQUARE:
	return -M_PI/6;
        break;
    case NACA:
	return -M_PI/10;
        break;
    case ROTOR:
	return 0.0;
        break;
    default:
	return 0.0;
    }
}

void Region::obstacleGeometricalFeatures()
{

    double cv, cs, ci;
    switch (obstacle)
    {
    case CYLINDER:
        cv = M_PI;
	cs = 2.0*M_PI;
	ci = 1.0/2.0;
        break;
    case SQUARE:
        cv = 4.0;
	cs = 8.0;
	ci = 1.0/12.0;
        break;
    case NACA:
        cv = 4.0; // TO BE MODIFIED
	cs = 8.0; // TO BE MODIFIED
	ci = 1.0/12.0; // TO BE MODIFIED
        break;	
    case ROTOR:
        cv = 4.0;
	cs = 8.0;
	ci = 1.0/12.0;
        break;
    default:
        cv = M_PI;
	cs = 2.0*M_PI;
	ci = 1.0/2.0;
    }
    this->volume = cv * pow(this->characteristic_length,2.0) * 1.0;
    this->surface = cs * this->characteristic_length * 1.0;	

    this->mass = this->volume * this->density;
    this->Ip = ci * this->mass * pow(this->characteristic_length,2.0); 
}

void Region::setObstacleCylinder(double x, double y, double theta, bool reset)
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
    this->theta = theta;
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
            double dist = sqrt(dx * dx + dy * dy) - r;
            if (this->FSI) f->phi[i * n + j] = dist;

            if (dx * dx + dy * dy < r * r)
            {
                f->s[i * n + j] = 0.0;
                if (this->RegionNr == 3)
                    f->m[i * n + j] = 0.5 + 0.5 * sin(0.1 * this->frameNr);
                else
                    f->m[i * n + j] = 1.0;

                f->u[i * n + j] = vx - this->omega * dy;
                f->u[(i + 1) * n + j] = vx - this->omega * dy;
                f->v[i * n + j] = vy + this->omega * dx;
                f->v[i * n + j + 1] = vy + this->omega * dx;

		if (this->FSI) f->phi[i * n + j] *= -1;
	    }
        }
    }

    if (this->FSI) 
    {
	getIbCells();
	computeNormals();
    }

}

void Region::setObstacleSquare(double x, double y, double theta, bool reset)
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
    this->theta = theta;
    double r = this->characteristic_length;
    shared_ptr<Fluid> f = this->fluid;

    vector<Point> P;
    P = getSquarePoints(Point({x, y}), r, theta);
    vector<Point> Pfsi;
    Pfsi = getSquarePoints(Point({x, y}), r, f->h, theta);

    int n = f->numY;
#pragma omp parallel for schedule(static) num_threads(f->numThreads)
    for (int i = 1; i < f->numX - 2; i++)
    {
        for (int j = 1; j < f->numY - 2; j++)
        {

	    double xx = (i + 0.5) * f->h;
	    double yy = (j + 0.5) * f->h;

	    if (this->FSI) f->phi[i * n + j] = computeDistToObstacle(Pfsi, Point({xx, yy}));

            f->s[i * n + j] = 1.0;

            if (isInsidePolygon(P, Point({xx, yy})))
            {
                f->s[i * n + j] = 0.0;
                if (this->RegionNr == 3)
                    f->m[i * n + j] = 0.5 + 0.5 * sin(0.1 * this->frameNr);
                else
                    f->m[i * n + j] = 1.0;

                f->u[i * n + j] = vx - this->omega * (yy - y);
                f->u[(i + 1) * n + j] = vx - this->omega * (yy - y);
                f->v[i * n + j] = vy + this->omega * (xx - x);
                f->v[i * n + j + 1] = vy + this->omega * (xx - x);

		if (this->FSI) f->phi[i * n + j] *= -1;
            }
        }
    }

    if (this->FSI)
    {
        getIbCells();
        computeNormals();
    }

}

void Region::setObstacleNaca(double x, double y, double theta, bool reset)
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
    this->theta = theta;
    double r = this->characteristic_length;
    shared_ptr<Fluid> f = this->fluid;
    int n = f->numY;
    Point center = {x, y};
    vector<Point> P;
    if (!this->FSI)
    {
    	P = getNacaPoints(center, r, theta);
    }
    else
    {
	P = getNacaPoints(center, r, f->h, theta);
    }

#pragma omp parallel for schedule(static) num_threads(f->numThreads)
    for (int i = 1; i < f->numX - 2; i++)
    {
        for (int j = 1; j < f->numY - 2; j++)
        {

            double xx = (i + 0.5) * f->h;
            double yy = (j + 0.5) * f->h;

            if (this->FSI) f->phi[i * n + j] = computeDistToObstacle(P, Point({xx, yy}));

            f->s[i * n + j] = 1.0;

            if (isInsidePolygon(P, Point({xx, yy})))
            {
                f->s[i * n + j] = 0.0;
                if (this->RegionNr == 3)
                    f->m[i * n + j] = 0.5 + 0.5 * sin(0.1 * this->frameNr);
                else
                    f->m[i * n + j] = 1.0;

                f->u[i * n + j] = vx - this->omega * (yy - y);
                f->u[(i + 1) * n + j] = vx - this->omega * (yy - y);
                f->v[i * n + j] = vy + this->omega * (xx - x);
                f->v[i * n + j + 1] = vy + this->omega * (xx - x);
		
		if (this->FSI) f->phi[i * n + j] *= -1;
	    }
        }
    }

    if (this->FSI) 
    {       
        getIbCells();
        computeNormals();
    }

}

void Region::setObstacleRotor(double x, double y, double theta, bool reset)
{
}

double Region::computeDistToObstacle(vector<Point> Pfsi, Point P)
{
    Point Pp;
    Pp = findNearest(Pfsi, P);
    double dist = sqrt(pow(P.x-Pp.x,2.0)+pow(P.y-Pp.y,2.0));

    return dist;

}

void Region::getIbCells()
{
    shared_ptr<Fluid> f = this->fluid;
    int n = f->numY;

    //vector<double> gradsx, gradsy;
    //gradsx.resize(f->numX*f->numY);
    //gradsy.resize(f->numX*f->numY);

    //for (int i = 2; i < f->numX - 2; i++)
    //{
    //    for (int j = 2; j < f->numY - 2; j++)
    //    {
    //        gradsx[i * n + j] = f->s[(i+1) * n + j] - f->s[(i-1) * n + j];
    //        gradsy[i * n + j] = f->s[i * n + j + 1] - f->s[i * n + j - 1];
    //    }
    //}	

    this->nib = 0;
#pragma omp parallel for schedule(static) num_threads(f->numThreads)
    for (int i = 2; i < f->numX - 3; i++)
    {
        for (int j = 2; j < f->numY - 3; j++)
        {
	    f->ib[i * n + j] = 0.0;
            if (f->s[i * n + j] == 1.0)
            {
                f->ib[i * n + j] = 0.0;
                if (f->s[(i-1) * n + j] == 0.0 || f->s[(i+1) * n + j] == 0.0 || f->s[i * n + (j-1)] == 0.0 || f->s[i * n + (j+1)] == 0.0)
                {
                    f->ib[i * n + j] = 1.0;
                    this->nib++;
                }
		//double norm = sqrt(pow(gradsx[i * n + j],2.0) + pow(gradsy[i * n + j],2.0));
		//if(norm>1e-09)
		//{	
		//	f->ib[i * n + j] = 1.0; 
		//	this->nib++;
		//}
    	    }
    	}
    }
}

void Region::computeNormals()
{
    shared_ptr<Fluid> f = this->fluid;
    int n = f->numY;

    vector<double> gradsx, gradsy;
    gradsx.resize(f->numX*f->numY);
    gradsy.resize(f->numX*f->numY);
    double SMALL = 1e-012;

#pragma omp parallel for schedule(static) num_threads(f->numThreads)
    for (int i = 1; i < f->numX - 2; i++)
    {
        for (int j = 1; j < f->numY - 2; j++)
        {
	    gradsx[i * n + j] = f->phi[(i+1) * n + j] - f->phi[(i-1) * n + j];
            gradsy[i * n + j] = f->phi[i * n + j + 1] - f->phi[i * n + j - 1];
	    double norm = sqrt(pow(gradsx[i * n + j],2.0) + pow(gradsy[i * n + j],2.0));
	    f->nx[i * n + j] = gradsx[i * n + j] / (norm + SMALL);
	    f->ny[i * n + j] = gradsy[i * n + j] / (norm + SMALL);
	}
    }
}

void Region::writeCdinFile()
{
    computeForce();

    double Cd;
    double Cl;
    double crossArea = 2.0*this->characteristic_length;

    if (RegionNr == 1 || RegionNr == 2 || RegionNr == 4)
    {
    	Cd = 2*this->Fx / (this->fluid->density*pow(this->inVel,2.0)*crossArea);
    	Cl = 2*this->Fy / (this->fluid->density*pow(this->inVel,2.0)*crossArea);
    }
    else
    {
	Cd = 0.0;
	Cl = 0.0;
    }

    Cdfile.open("Drag_and_Lift.txt", fstream::app);
    //Cdfile << time.GetInterval()  << " " << Cd << " " << Cl << " \n";
    Cdfile << Cd << " " << Cl << " \n";
    Cdfile.close();
}


void Region::moveObstacle(double dt, bool reset)
{
    computeForce();

    double rhobar = (this->density - this->fluid->density) / this->density;
    double d_omega;

    if (this->nib != 0)
    {
	this->ax = (1.0/this->mass) * this->Fx + rhobar * this->gravity[0];
        this->ay = (1.0/this->mass) * this->Fy + rhobar * this->gravity[1];
	d_omega = (1.0/this->Ip) * this->Mz;
    }
    else
    {
        this->ax = 0.0;
        this->ay = 0.0;
	d_omega = 0.0;
    }

    double x_new = this->obstacleX + this->vx * dt + 0.5 * this->ax * pow(dt,2.0);
    double y_new = this->obstacleY + this->vy * dt + 0.5 * this->ay * pow(dt,2.0);
    double theta_new = this->theta + this->omega * dt + 0.5 * d_omega * pow(dt,2.0);
    cout << "NEW: " << x_new << " " << y_new << " " << theta_new << endl;
    setObstacle(x_new, y_new, theta_new, reset);
    this->omega = (theta_new - theta) / dt; 
    this->vx = (x_new - this->obstacleX) / dt;
    this->vy = (y_new - this->obstacleY) / dt;
}


void Region::computeForce()
{
    shared_ptr<Fluid> f = this->fluid;
    int n = f->numY;

    double dS = this->surface/this->nib;
    double _Fx = 0.0;
    double _Fy = 0.0;
    double _Mz = 0.0;

#pragma omp parallel for schedule(static) num_threads(f->numThreads)
    for (int i = 1; i < f->numX - 2; i++)
    {
    	for (int j = 1; j < f->numY - 2; j++)
    	{
    	    _Fx -= f->ib[i * n + j] * f->p[i * n + j] * f->nx[i * n + j] * dS;
    	    _Fy -= f->ib[i * n + j] * f->p[i * n + j] * f->ny[i * n + j] * dS;

	    double dx = (i + 0.5) * f->h - this->obstacleX;
            double dy = (j + 0.5) * f->h - this->obstacleY;

	    _Mz -= f->ib[i * n + j] * f->p[i * n + j] * (dx * f->ny[i * n + j] - dy * f->nx[i * n + j]) * dS;

	}
    }

    this->Fx = _Fx;
    this->Fy = _Fy;
    this->Mz = _Mz;

    //cout<<"Force: "<< _Fx << " " << _Fy <<endl;
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
	if (this->FSI) moveObstacle(this->dt, false);
	if (this->writeCd) writeCdinFile();
    }
}

OBJ indexToOBJ(int index)
{
    switch (index)
    {
    case 0 : return CYLINDER;
    case 1 : return SQUARE;
    case 2 : return NACA;
    case 3 : return ROTOR;
    default: return CYLINDER;
    }

}
