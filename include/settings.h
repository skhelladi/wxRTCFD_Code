#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include <wx/wx.h>
#include <vector>
#include <algorithm>
// #include <execution>

using namespace std;

// Structure pour représenter un point en 2D
struct Point {
    double x;
    double y;
};

// Fonction de comparaison de points selon leur abscisse
// bool comparePoints(const Point &a, const Point &b);

wxArrayString getCaseList();
wxArrayString getObstacleList();
wxArrayString getScalarList();

vector<wxPoint> getSquarePoints(wxPoint pos, double length);
vector<Point> getSquarePoints(Point pos, double length);

vector<wxPoint> getDiamondPoints(wxPoint pos, double length);
vector<Point> getDiamondPoints(Point pos, double length);

vector<wxPoint> getNacaPoints(wxPoint pos, double length);
vector<Point> getNacaPoints(Point pos, double length); 
vector<Point> generateNacaProfile(Point pos, double chord, double thickness, int nb_points, double incidence=M_PI/12);
vector<wxPoint> generateNacaProfile(wxPoint pos, double chord, double thickness, int nb_points, double incidence=M_PI/12);

vector<vector<Point>> generateRotorPoints(Point pos, double length);
vector<vector<wxPoint>> generateRotorPoints(wxPoint pos, double length);
vector<vector<Point>> generateRotor(Point center, double radius, double chord, double thickness, int nb_points, int Z);
vector<vector<wxPoint>> generateRotor(wxPoint center, double radius, double chord, double thickness, int nb_points, int Z);




wxPoint *fromVectorToPtr(vector<wxPoint> pt);

// bool isPointInPolygon(vector<wxPoint> P, wxPoint M);

// bool isInside(vector<Point> polygon, Point P);
bool isInsidePolygon(vector<Point> polygon, Point P);
vector<Point> rotatePolygon(vector<Point> polygon, Point center, double theta);
vector<wxPoint> rotatePolygon(vector<wxPoint> polygon, wxPoint center, double theta);
vector<vector<Point>> generateCircularRepeats(vector<Point> polygon, Point center, int n);



#endif // SETTINGS_H_INCLUDED
