#include "settings.h"

wxArrayString getCaseList()
{
    wxArrayString arrCase;
    arrCase.Add("Tank");
    arrCase.Add("Wind tunnel");
    arrCase.Add("Paint");
    arrCase.Add("Wind tunnel (high resolution)");

    return arrCase;
}

wxArrayString getObstacleList()
{
    wxArrayString arrObject;
    arrObject.Add("Cylinder");
    arrObject.Add("Square");
    arrObject.Add("Diamond");
    arrObject.Add("NACA");
    arrObject.Add("Rotor");

    return arrObject;
}

wxArrayString getScalarList()
{
    wxArrayString arrScalar;
    arrScalar.Add("None");
    arrScalar.Add("Pressure");
    arrScalar.Add("Velocity");
    arrScalar.Add("x-Velocity");
    arrScalar.Add("y-Velocity");

    return arrScalar;
}
