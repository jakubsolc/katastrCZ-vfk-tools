


#include <cmath>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

#include "rect.h"
#include "colors.h"
#include "safestring.h"


Rect::Rect(double aX1, double aX2, double aY1, double aY2) : X1(aX1), X2(aX2), Y1(aY1), Y2(aY2)
{
    if (X1 > X2) std::swap(X1, X2);
    if (Y1 > Y2) std::swap(Y1, Y2);
}


bool Rect::ok()
{
    return (X1 < X2) && (Y1 < Y2);
}

void Rect::pr()
{
    std::cout << std::fixed << std::setprecision(2) << "Rect X:[" << X1 << "..." << X2 << "]  Y:[" << Y1 << "..." << Y2 << "]\n";
}


Rect& Rect::expand_round(double ex, double u)
{
//    double dist = X2 - X1;
    X1 -= ex;
    X2 += ex;
    Y1 -= ex;
    Y2 += ex;
    X1 = floor(X1/u) * u;
    Y1 = floor(Y1/u) * u;
    X2 = ceil(X2/u) * u;
    Y2 = ceil(Y2/u) * u;
    
    return *this;
}

Rect& Rect::expand_round(double exx, double exy, double u)
{
//    double dist = X2 - X1;
    X1 -= exx;
    X2 += exx;
    Y1 -= exy;
    Y2 += exy;
    X1 = floor(X1/u) * u;
    Y1 = floor(Y1/u) * u;
    X2 = ceil(X2/u) * u;
    Y2 = ceil(Y2/u) * u;
    
    return *this;
}


void Rect::load_rect(std::string aX1, std::string aX2, std::string aY1, std::string aY2)
{
//    double dist = X2 - X1;
    try{
    X1 = safestod(aX1, 0, 1);
    X2 = safestod(aX2, 0, 1);
    Y1 = safestod(aY1, 0, 1);
    Y2 = safestod(aY2, 0, 1);
    }
    catch(...)
    {
        std::cout << FRED("Error load_rect: pri nacitani cisel doslo k chybe formatu.\n");
    }
    if (X1>X2) std::swap(X1, X2);
    if (Y1>Y2) std::swap(Y1, Y2);
}

bool Rect::is_inside(double X, double Y)
{
    return (X1 <= X) && (X <= X2) && (Y1 <= Y) && (Y <= Y2);
}
