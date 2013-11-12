#define _USE_MATH_DEFINES
#pragma once
#include <math.h>
#include <gl/glut.h>
#include "dStructs.h"
#include <vector>

 

using namespace std;
using namespace dStructs;


namespace Geometry
{

	void drawFilledCircle(point centre, int radius, int numDivisions = 30);
	void drawCircleOutline(point centre, int radius, int numDivisions = 30);
	
};