#ifndef REGION_H
#define REGION_H
#include <iostream>
#include "fluid.h"

#define DEBUG 0

enum OBJ {CYLINDER, SQUARE, DIAMOND, NACA, ROTOR};

// #define CALL_SET_OBSTACLE(object,ptrToMember)  (object.*ptrToMember)

// class Region;
// typedef   void (Region::*SET_OBSTACLE)(double, double, bool);



using namespace std;


class Region
{
public:
    Region(int _height, int _width, double _simHeight = 1.1);
//    Region(shared_ptr<Region> r){this=r;}

    double cX(double x)
    {
        return x * cScale;
    }

    double cY(double y)
    {
        return height - y * cScale;
    }

    void setupRegion(int _RegionNr = 0, double _overRelaxation=1.9, int _resolution=50, double _density=1000, int _numThreads=4);
    void setObstacle(double x, double y, bool reset);
    void setObstacleCylinder(double x, double y, bool reset);
    void setObstacleSquare(double x, double y, bool reset);
    void setObstacleDiamond(double x, double y, bool reset);
    void setObstacleNaca(double x, double y, bool reset);
    void setObstacleRotor(double x, double y, bool reset);
    void updateRegionSize(int _height, int _width);
    void update();

    void sayhello(){cout<<"hello! "<<text<<endl;}

    string text="";
    double gravity = -9.81;
    double dt =  1.0 / 60.0;
    int numIters= 40;
    int frameNr= 0;
    double overRelaxation= 1.9;
    double obstacleX= 0.0;
    double obstacleY= 0.0;
    double characteristic_length= 0.15;
    bool paused= false;
    int RegionNr= 0;
    bool showObstacle= false;
    bool showObstaclePosition=false;
    bool showStreamlines= false;
    bool showVelocity= false;
    bool showXVelocity= false;
    bool showYVelocity= false;
    bool showVelocityVectors= false;
    bool showPressure= false;
    bool showTracer= true;
    shared_ptr<Fluid> fluid;

    int height;
    int width;
    double simHeight;
    double cScale;
    double simWidth;
    int resolution;
    int numThreads;

    OBJ obstacle;
};

OBJ indexToOBJ(int index);

#endif // Region_H
