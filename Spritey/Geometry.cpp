#include "stdafx.h"
#include "Geometry.h"

void Geometry::drawFilledCircle(point centre, int radius, int numDivisions)
{
	if(numDivisions==0)numDivisions=30;//no dividing by zero in my program

	GLfloat twoPi = 2*M_PI;
	vector<GLfloat> vArray;//a vector to hold the vertex array

	vArray.push_back(centre.x);//push in the centre point
	vArray.push_back(centre.y);

	for(int i = 0; i<=numDivisions; i++)
	{
		GLfloat xVal = centre.x +(radius * cos(i*twoPi/numDivisions));
		GLfloat yVal = centre.y +(radius * sin(i*twoPi/numDivisions));

		vArray.push_back(xVal);//push our orbiting values into the v.array
		vArray.push_back(yVal);
	}


	glEnableClientState(GL_VERTEX_ARRAY);//enable Varray mode
	glVertexPointer(2,GL_FLOAT,0,&vArray[0]);

	glDrawArrays(GL_TRIANGLE_FAN,0, vArray.size()/2);//DONT FORGET: "count" is array.size / 2, because 2 values combine to make 1 index

	glDisableClientState(GL_VERTEX_ARRAY);//disable vertex array

}

void Geometry::drawCircleOutline(point centre, int radius, int numDivisions)
{
	if(numDivisions==0)numDivisions=30;//no dividing by zero in my program

	GLfloat twoPi = 2*M_PI;
	vector<GLfloat> vArray;//a vector to hold the vertex array

	for(int i = 0; i<=numDivisions; i++)
	{
		GLfloat xVal = centre.x +(radius * cos(i*twoPi/numDivisions));
		GLfloat yVal = centre.y +(radius * sin(i*twoPi/numDivisions));

		vArray.push_back(xVal);//push our orbiting values into the v.array
		vArray.push_back(yVal);
	}


	glEnableClientState(GL_VERTEX_ARRAY);//enable Varray mode
	glVertexPointer(2,GL_FLOAT,0,&vArray[0]);

	glDrawArrays(GL_LINE_LOOP,0, vArray.size()/2);//DONT FORGET: "count" is array.size / 2, because 2 values combine to make 1 index

	glDisableClientState(GL_VERTEX_ARRAY);//disable vertex array


}