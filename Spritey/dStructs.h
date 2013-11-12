#pragma once

#include <Windows.h>
#include <GL/GL.h>
#include <string>
#include <math.h>


using namespace std;

namespace dStructs 
{
	struct point
	{
		GLfloat x, y;

		point();
		point(GLfloat aX,GLfloat aY); //constructor for point
		void operator= (point newVal); //overload equals operator for assignment
		void operator= (point* newVal); //overload so that we can work with point pointers (no pun intended) as weell
	};

	struct entity
	{
		point pos, size;

		entity();
		entity(GLfloat posX, GLfloat posY, GLfloat sizeX, GLfloat sizeY);
		entity(point posIn, point sizeIn);
		entity(point* posIn, point* sizeIn);
	
	};


string replaceCharInString(string myString, char charToReplace, char withChar);
bool mouseIsOverEntity(point* mouse, entity* object);
bool pointIsWithinCenteredBox(entity* frame, point* pointToCheck);
bool mouseIsOverCircle(point* mouse, point* circlePos, int radius);
bool mouseIsOverCircle(point mouse, point circlePos, int radius);

//defunct
static void copyPoint(point pointToCopy, point& toPoint);
static void copyEntity(entity entityToCopy, entity& toEntity);




};


