#include "stdafx.h"
#include "dStructs.h"


dStructs::point::point()
{
}

dStructs::point::point(GLfloat aX, GLfloat aY)
{
	x = aX;
	y = aY;
}

void dStructs::point::operator =(point newVal)
{
	x = newVal.x;
	y = newVal.y;
}

void dStructs::point::operator =(point* newVal)
{
	//I now know this is a bad function
	//should avoid use where possible, but in the mean time, we'll make it as stable as possible and take a...
	//deep copy of values
	x = newVal->x;
	y = newVal->y;
}

dStructs::entity::entity()
{

}

dStructs::entity::entity(GLfloat posX, GLfloat posY, GLfloat sizeX, GLfloat sizeY)
{
	pos = point(posX,posY);
	size = point(sizeX, sizeY);
}

dStructs::entity::entity(point posIn, point sizeIn)
{
	pos = point(posIn.x,posIn.y);
	size = point(sizeIn.x,sizeIn.y);
}

dStructs::entity::entity(point* posIn, point* sizeIn)
{
	pos = *posIn;
	size = *sizeIn;
}

//defunct
void dStructs::copyPoint(point pointToCopy, point& toPoint)
{
	toPoint.x = pointToCopy.x;
	toPoint.y = pointToCopy.y;
}
//defunct
void dStructs::copyEntity(entity entityToCopy, entity& toEntity)
{
	//copy the pos point
	copyPoint(entityToCopy.pos, toEntity.pos);
	//same for the size point
	copyPoint(entityToCopy.size, toEntity.size);
}

string dStructs::replaceCharInString(string myString, char charToReplace, char withChar)
{
	for(int i = 0; i<myString.length(); i++)
	{
		if(myString[i]==charToReplace)//if you find this char at this point
		{
			myString[i] = withChar;//replace what is at this point with the char we are swapping in
		}
	}
	return myString;
}

bool dStructs::mouseIsOverEntity(point* mouse, entity* object)
{
	//if the mouse.x is between these coords
	if(mouse->x >= object->pos.x && mouse->x <= (object->pos.x+object->size.x))
	{
		//and if the mouse.y is between these cords 
		if(mouse->y >= object->pos.y && mouse->y <= (object->pos.y+object->size.y))
		{
			return true;
			printf("\nOVER ENTITY");
		}
		else return false;
		printf("\nNOT COVERED");
	}
	else return false;
	printf("\nNOT COVERED");
}

bool dStructs::mouseIsOverCircle(point* mouse, point* circlePos, int radius)
{
	point diffInXY = point(mouse->x-circlePos->x,mouse->y-circlePos->y);//point representing (x1-x2,y1-y2)
	long double lengthPreSqrt = (pow(diffInXY.x,2) + pow(diffInXY.y,2));//(x1-x2)^2+(y1-y2)^2
	long double magnitude = sqrt(lengthPreSqrt);

	if(magnitude<=radius)//if the distance of the mouse from the centre is less than the radius, we're hovering
	{
		return true;
	}else{
		return false;
	}
}

bool dStructs::mouseIsOverCircle(point mouse, point circlePos, int radius)
{
	point diffInXY = point(mouse.x-circlePos.x,mouse.y-circlePos.y);//point representing (x1-x2,y1-y2)
	long double lengthPreSqrt = (pow(diffInXY.x,2) + pow(diffInXY.y,2));//(x1-x2)^2+(y1-y2)^2
	long double magnitude = sqrt(lengthPreSqrt);

	if(magnitude<=radius)//if the distance of the mouse from the centre is less than the radius, we're hovering
	{
		return true;
	}else{
		return false;
	}
}


bool dStructs::pointIsWithinCenteredBox(entity* frame, point* pointToCheck)
{
	if(pointToCheck->x >= frame->pos.x-(frame->size.x/2) && pointToCheck->x <= frame->pos.x+(frame->size.x/2))
	{
		if(pointToCheck->y >= frame->pos.y-(frame->size.y/2) && pointToCheck->y <= frame->pos.y+(frame->size.y/2))
		{
			return true;
		}else return false;
	}else return false;
}