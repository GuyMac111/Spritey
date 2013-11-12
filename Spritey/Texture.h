#pragma once

#include <gl/glew.h>
#include <iostream>
#include <vector>
#include <GL/glut.h>
#include "lodepng.h"
#include <exception>
#include "dStructs.h"
#include <gl/glext.h>


using namespace std;
using namespace dStructs;


class Texture
{


public:

	enum ANCHOR
	{
		CENTER,
		TOPLEFT,
		BOTTOMLEFT,
		TOPRIGHT,
		BOTTOMRIGHT
	};

	Texture(string filename);//initialises our texture and loads its pixeldata into a buffer
	Texture(void);
	~Texture(void);
	
	void reloadTexture(string filename);//initialises our texture with new image data from a new file

	void draw(point centerPoint, point dimensions);//draws a texture on a quad of specified dimensions 
	void draw(point origin);//draws a texture on a quad that is the same size as the image
	void draw(point* origin);//The same as above but accepts a pointer. Draws from the center by default, unless Draw_From_Corner previously set otherwise.
	void draw(point* origin, Texture::ANCHOR drawFrom); //As above but allows us to change the Draw_From_Corner first

	void drawInFrame(point locIntex, point dimInTex, point* origin); //draws a frame from this instance of texture. //ATM draws from bottom left only

	unsigned w, h;//width & height of our imagefile

	struct nonPOTException : public exception
	{
		const char* what() const throw()
		{
			return "Sprite Sheet dimensions must be power of 2. (8,16,32,64,128,256,512,1024) \nFind a Sprite Sheet with compatible dimensions, or edit this one using image editing software. \n(Just pad out the edges to the nearest compatible size)";
		}
	};

	struct InvalidSizeException : public exception
	{
		const char* what() const throw()
		{
			return "Sprite Sheet dimensions are either 0 (invalid) or greater than 1024px.\n Spritey accepts only power of 2 values (8,16,32,64,128,256,512,1024) between 2px-1024px.";
		}
	};
	
	

	ANCHOR Draw_From_Corner;


	
	//GLuint texID;//the ID we will give OGL for this particular texture.
	
private:
	vector<unsigned char> buffer; 
	vector<unsigned char> image;
	void texGLInit();
	void texGLSecondaryInit();
	GLuint textureID[1];

	bool checkPOT(int num);

};

