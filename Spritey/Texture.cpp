#include "StdAfx.h"
#include "Texture.h"
#include <math.h>


//static int texFactoryID = 1;//the texture ID we assign each specific texture. Factory style.

using namespace lodepng;

Texture::Texture(string filename)
{
	//textureID[0]=0;

	const char* fnPtr = filename.c_str(); //our image loader accepts a ptr to a char, not a string
	//printf(fnPtr);
	
	lodepng::load_file(buffer, fnPtr);//load the file into a buffer

	unsigned error = lodepng::decode(image,w,h,buffer);//lodepng's decode function will load the pixel data into image vector from the buffer
	//display any errors with the texture
	if(error)
	{
		cout << "\ndecoder error " << error << ": " << lodepng_error_text(error) <<endl;
	}
	//execute the code that'll throw exceptions to do with the images size
	checkPOT(w);
	checkPOT(h);


	//loop through and //printf our pixel data
	/*for(GLuint i = 0; i<image.size(); i+=4)
	{
		//printf("\n%i,%i,%i,%i,", image.at(i),image.at(i+1),image.at(i+2),image.at(i+3));
	
	}*/

	////printf("\nImage size is %i", image.size());

	//image now contains our pixeldata. All ready for OpenGL to do its thing
	
	//let's get this texture up in the video memory
	texGLInit();

	Draw_From_Corner = CENTER;	
}

void Texture::reloadTexture(string filename)
{
	//first and foremost clear the image and buffer vectors back down to nothing so we can start afresh 
	buffer.clear();
	image.clear();
	w = 0;
	h = 0;
	//also delete the texture name we were using before
	glDeleteTextures(1, &textureID[0]);

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("OpenGL error: %u", err);
	}


	const char* fnPtr = filename.c_str(); //our image loader accepts a ptr to a char, not a string
	//printf(fnPtr);

	lodepng::load_file(buffer, fnPtr);//load the file into a buffer

	unsigned error = lodepng::decode(image,w,h,buffer);//lodepng's decode function will load the pixel data into image vector from the buffer
	//display any errors with the texture
	if(error)
	{
		cout << "\ndecoder error " << error << ": " << lodepng_error_text(error) <<endl;
	}
	//execute the code that'll throw exceptions to do with the images size
	checkPOT(w);
	checkPOT(h);
	


	//loop through and //printf our pixel data
	/*for(GLuint i = 0; i<image.size(); i+=4)
	{
	//printf("\n%i,%i,%i,%i,", image.at(i),image.at(i+1),image.at(i+2),image.at(i+3));

	}*/

	////printf("\nImage size is %i", image.size());

	//image now contains our pixeldata. All ready for  to do its thing

	//let's get this texture up in the video memoryOpenGL
	texGLSecondaryInit();

	Draw_From_Corner = CENTER;
}

void Texture::texGLInit()
{

	glGenTextures(1, &textureID[0]);
	////printf("\ntextureID = %u", textureID[0]);
	
	glBindTexture(GL_TEXTURE_2D, textureID[0]);//evrything we're about to do is about this texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,w,h,0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	//we COULD free the image vectors memory right about now.
	image.clear();
	
	

}

void Texture::texGLSecondaryInit()
{
	PFNGLBINDBUFFERARBPROC glBindBuffer = NULL;                  // VBO Bind Procedure
	

	glGenTextures(1, &textureID[0]);
	////printf("\ntextureID = %u", textureID[0]);

	glBindTexture(GL_TEXTURE_2D, textureID[0]);//evrything we're about to do is about this texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	//glDisable(GL_COLOR_MATERIAL);
	//glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,w,h,0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("OpenGL error: %u", err);
	}
	//we COULD free the image vectors memory right about now.
	image.clear();



}

void Texture::draw(point centerPoint, point dimensions)
{
	glEnable(GL_TEXTURE_2D);
	//printf("\nDrawing texture at (%f, %f)",centerPoint.x, centerPoint.y);
	glBindTexture(GL_TEXTURE_2D, textureID[0]);//bind the texture
	//create a quick vertex array for the primitive we're going to bind the texture to
	////printf("TexID = %u",textureID[0]);
	GLfloat vArray[8] = 
	{
		centerPoint.x-(dimensions.x/2), centerPoint.y-(dimensions.y/2),//bottom left i0
		centerPoint.x-(dimensions.x/2), centerPoint.y+(dimensions.y/2),//top left i1
		centerPoint.x+(dimensions.x/2), centerPoint.y+(dimensions.y/2),//top right i2
		centerPoint.x+(dimensions.x/2), centerPoint.y-(dimensions.y/2)//bottom right i3
	};
	
	//create a quick texture array (we COULD create this on the heap rather than creating/destoying every cycle)
	GLfloat tArray[8] = 
	{
		//this has been tinkered with from my normal order. I think LodePNG is bringing the PD upside down. SO A QUICK FIX HERE WAS NECESSARY.
		0.0f,1.0f,//0
		0.0f,0.0f,//1
		1.0f,0.0f,//2
		1.0f,1.0f//3
	};

	//and finally.. the index array...remember, we draw in triangles....(and we'll go CW)
	GLubyte iArray[6] =
	{
		0,1,2,
		0,2,3
	};

	//Activate arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	//Give openGL a pointer to our vArray and tArray
	glVertexPointer(2, GL_FLOAT, 0, &vArray[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &tArray[0]);
	
	//Draw it all
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, &iArray[0]);
	
	//glDrawArrays(GL_TRIANGLES,0,6);
	
	//Disable the vertex arrays
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	//done!

	/*glBegin(GL_QUADS);
	glTexCoord2f(0.0f,0.0f);
		glVertex2f(centerPoint.x-(dimensions.x/2), centerPoint.y-(dimensions.y/2));
	glTexCoord2f(0.0f,1.0f);
		glVertex2f(centerPoint.x-(dimensions.x/2), centerPoint.y+(dimensions.y/2));
	glTexCoord2f(1.0f,1.0f);
		glVertex2f(centerPoint.x+(dimensions.x/2), centerPoint.y+(dimensions.y/2));
	glTexCoord2f(1.0f,0.0f);
		glVertex2f(centerPoint.x+(dimensions.x/2), centerPoint.y-(dimensions.y/2));
	glEnd();*/
}

void Texture::draw(point origin)
{
	glEnable(GL_TEXTURE_2D);
	//printf("\nDrawing texture at (%f, %f)",centerPoint.x, centerPoint.y);
	glBindTexture(GL_TEXTURE_2D, textureID[0]);//bind the texture
	//create a quick vertex array for the primitive we're going to bind the texture to
	////printf("TexID = %u",textureID[0]);
	
	GLfloat vArray[8];

#pragma region anchor switch
	switch (Draw_From_Corner)
	{
	case CENTER:
		
			vArray[0] = origin.x-(w/2); vArray[1] = origin.y-(h/2);//bottom left i0
			vArray[2] = origin.x-(w/2); vArray[3] = origin.y+(h/2);//top left i1
			vArray[4] = origin.x+(w/2); vArray[5] = origin.y+(h/2);//top right i2
			vArray[6] = origin.x+(w/2); vArray[7] = origin.y-(h/2);//bottom right i3
		break;

	case BOTTOMLEFT:
		
			vArray[0] = origin.x; vArray[1] = origin.y;//bottom left i0
			vArray[2] = origin.x; vArray[3] = origin.y+h;//top left i1
			vArray[4] = origin.x+w; vArray[5] = origin.y+h;//top right i2
			vArray[6] = origin.x+w; vArray[7] = origin.y;//bottom right i3
		
		break;

	case TOPLEFT:
		
			vArray[0] = origin.x; vArray[1] = origin.y-h;//bottom left i0
			vArray[2] = origin.x; vArray[3] = origin.y;//top left i1
			vArray[4] = origin.x+w; vArray[5] = origin.y;//top right i2
			vArray[6] = origin.x+w; vArray[7] = origin.y-h;//bottom right i3
		
		break;

	case TOPRIGHT:
		
			vArray[0] = origin.x-w; vArray[1] = origin.y-h;//bottom left i0
			vArray[2] = origin.x-w; vArray[3] = origin.y;//top left i1
			vArray[4] = origin.x; vArray[5] = origin.y;//top right i2
			vArray[6] = origin.x; vArray[7] = origin.y-h;//bottom right i3
		
		break;

	case BOTTOMRIGHT:
		
			vArray[0] = origin.x-w; vArray[1] = origin.y;//bottom left i0
			vArray[2] = origin.x-w; vArray[3] = origin.y+h;//top left i1
			vArray[4] = origin.x-h; vArray[5] = origin.y;//top right i2
			vArray[6] = origin.x; vArray[7] = origin.y;//bottom right i3
		
		break;

	default: //same as center
		
			vArray[0] = origin.x-(w/2); vArray[1] = origin.y-(h/2);//bottom left i0
			vArray[2] = origin.x-(w/2); vArray[3] = origin.y+(h/2);//top left i1
			vArray[4] = origin.x+(w/2); vArray[5] = origin.y+(h/2);//top right i2
			vArray[6] = origin.x+(w/2); vArray[7] = origin.y-(h/2);//bottom right i3
		
		break;


	}
#pragma  endregion	


	//create a quick texture array (we COULD create this on the heap rather than creating/destoying every cycle)
	GLfloat tArray[8] = 
	{
		//this has been tinkered with from my normal order. I think LodePNG is bringing the PD upside down. SO A QUICK FIX HERE WAS NECESSARY.
		0.0f,1.0f,//0
		0.0f,0.0f,//1
		1.0f,0.0f,//2
		1.0f,1.0f//3
	};

	//and finally.. the index array...remember, we draw in triangles....(and we'll go CW)
	GLubyte iArray[6] =
	{
		0,1,2,
		0,2,3
	};

	//Activate arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//Give openGL a pointer to our vArray and tArray
	glVertexPointer(2, GL_FLOAT, 0, &vArray[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &tArray[0]);

	//Draw it all
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, &iArray[0]);

	//glDrawArrays(GL_TRIANGLES,0,6);

	//Disable the vertex arrays
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

void Texture::draw(point* origin)
{
	glEnable(GL_TEXTURE_2D);
	//printf("\nDrawing texture at (%f, %f)",centerPoint.x, centerPoint.y);
	glBindTexture(GL_TEXTURE_2D, textureID[0]);//bind the texture
	//create a quick vertex array for the primitive we're going to bind the texture to
	////printf("TexID = %u",textureID[0]);
	GLfloat vArray[8];

#pragma region anchor switch
	switch (Draw_From_Corner)
	{
	case CENTER:

		vArray[0] = origin->x-(w/2); vArray[1] = origin->y-(h/2);//bottom left i0
		vArray[2] = origin->x-(w/2); vArray[3] = origin->y+(h/2);//top left i1
		vArray[4] = origin->x+(w/2); vArray[5] = origin->y+(h/2);//top right i2
		vArray[6] = origin->x+(w/2); vArray[7] = origin->y-(h/2);//bottom right i3
		break;

	case BOTTOMLEFT:

		vArray[0] = origin->x; vArray[1] = origin->y;//bottom left i0
		vArray[2] = origin->x; vArray[3] = origin->y+h;//top left i1
		vArray[4] = origin->x+w; vArray[5] = origin->y+h;//top right i2
		vArray[6] = origin->x+w; vArray[7] = origin->y;//bottom right i3

		break;

	case TOPLEFT:

		vArray[0] = origin->x; vArray[1] = origin->y-h;//bottom left i0
		vArray[2] = origin->x; vArray[3] = origin->y;//top left i1
		vArray[4] = origin->x+w; vArray[5] = origin->y;//top right i2
		vArray[6] = origin->x+w; vArray[7] = origin->y-h;//bottom right i3

		break;

	case TOPRIGHT:

		vArray[0] = origin->x-w; vArray[1] = origin->y-h;//bottom left i0
		vArray[2] = origin->x-w; vArray[3] = origin->y;//top left i1
		vArray[4] = origin->x; vArray[5] = origin->y;//top right i2
		vArray[6] = origin->x; vArray[7] = origin->y-h;//bottom right i3

		break;

	case BOTTOMRIGHT:

		vArray[0] = origin->x-w; vArray[1] = origin->y;//bottom left i0
		vArray[2] = origin->x-w; vArray[3] = origin->y+h;//top left i1
		vArray[4] = origin->x-h; vArray[5] = origin->y;//top right i2
		vArray[6] = origin->x; vArray[7] = origin->y;//bottom right i3

		break;

	default: //same as center

		vArray[0] = origin->x-(w/2); vArray[1] = origin->y-(h/2);//bottom left i0
		vArray[2] = origin->x-(w/2); vArray[3] = origin->y+(h/2);//top left i1
		vArray[4] = origin->x+(w/2); vArray[5] = origin->y+(h/2);//top right i2
		vArray[6] = origin->x+(w/2); vArray[7] = origin->y-(h/2);//bottom right i3

		break;


	}
#pragma  endregion	

	//create a quick texture array (we COULD create this on the heap rather than creating/destoying every cycle)
	GLfloat tArray[8] = 
	{
		//this has been tinkered with from my normal order. I think LodePNG is bringing the PD upside down. SO A QUICK FIX HERE WAS NECESSARY.
		0.0f,1.0f,//0
		0.0f,0.0f,//1
		1.0f,0.0f,//2
		1.0f,1.0f//3
	};

	//and finally.. the index array...remember, we draw in triangles....(and we'll go CW)
	GLubyte iArray[6] =
	{
		0,1,2,
		0,2,3
	};

	//Activate arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//Give openGL a pointer to our vArray and tArray
	glVertexPointer(2, GL_FLOAT, 0, &vArray[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &tArray[0]);

	//Draw it all
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, &iArray[0]);

	//glDrawArrays(GL_TRIANGLES,0,6);

	//Disable the vertex arrays
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

void Texture::draw(point* origin, ANCHOR drawFrom)
{
	//let us set the DFC enum here.
	Draw_From_Corner = drawFrom;

	glEnable(GL_TEXTURE_2D);
	//printf("\nDrawing texture at (%f, %f)",centerPoint.x, centerPoint.y);
	glBindTexture(GL_TEXTURE_2D, textureID[0]);//bind the texture
	//create a quick vertex array for the primitive we're going to bind the texture to
	////printf("TexID = %u",textureID[0]);
	GLfloat vArray[8];

#pragma region anchor switch
	switch (Draw_From_Corner)
	{
	case CENTER:

		vArray[0] = origin->x-(w/2); vArray[1] = origin->y-(h/2);//bottom left i0
		vArray[2] = origin->x-(w/2); vArray[3] = origin->y+(h/2);//top left i1
		vArray[4] = origin->x+(w/2); vArray[5] = origin->y+(h/2);//top right i2
		vArray[6] = origin->x+(w/2); vArray[7] = origin->y-(h/2);//bottom right i3
		break;

	case BOTTOMLEFT:

		vArray[0] = origin->x; vArray[1] = origin->y;//bottom left i0
		vArray[2] = origin->x; vArray[3] = origin->y+h;//top left i1
		vArray[4] = origin->x+w; vArray[5] = origin->y+h;//top right i2
		vArray[6] = origin->x+w; vArray[7] = origin->y;//bottom right i3

		break;

	case TOPLEFT:

		vArray[0] = origin->x; vArray[1] = origin->y-h;//bottom left i0
		vArray[2] = origin->x; vArray[3] = origin->y;//top left i1
		vArray[4] = origin->x+w; vArray[5] = origin->y;//top right i2
		vArray[6] = origin->x+w; vArray[7] = origin->y-h;//bottom right i3

		break;

	case TOPRIGHT:

		vArray[0] = origin->x-w; vArray[1] = origin->y-h;//bottom left i0
		vArray[2] = origin->x-w; vArray[3] = origin->y;//top left i1
		vArray[4] = origin->x; vArray[5] = origin->y;//top right i2
		vArray[6] = origin->x; vArray[7] = origin->y-h;//bottom right i3

		break;

	case BOTTOMRIGHT:

		vArray[0] = origin->x-w; vArray[1] = origin->y;//bottom left i0
		vArray[2] = origin->x-w; vArray[3] = origin->y+h;//top left i1
		vArray[4] = origin->x-h; vArray[5] = origin->y;//top right i2
		vArray[6] = origin->x; vArray[7] = origin->y;//bottom right i3

		break;

	default: //same as center

		vArray[0] = origin->x-(w/2); vArray[1] = origin->y-(h/2);//bottom left i0
		vArray[2] = origin->x-(w/2); vArray[3] = origin->y+(h/2);//top left i1
		vArray[4] = origin->x+(w/2); vArray[5] = origin->y+(h/2);//top right i2
		vArray[6] = origin->x+(w/2); vArray[7] = origin->y-(h/2);//bottom right i3

		break;


	}
#pragma  endregion			

	//create a quick texture array (we COULD create this on the heap rather than creating/destoying every cycle)
	GLfloat tArray[8] = 
	{
		//this has been tinkered with from my normal order. I think LodePNG is bringing the PD upside down. SO A QUICK FIX HERE WAS NECESSARY.
		0.0f,1.0f,//0
		0.0f,0.0f,//1
		1.0f,0.0f,//2
		1.0f,1.0f//3
	};

	//and finally.. the index array...remember, we draw in triangles....(and we'll go CW)
	GLubyte iArray[6] =
	{
		0,1,2,
		0,2,3
	};

	//Activate arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	

	//Give openGL a pointer to our vArray and tArray
	glVertexPointer(2, GL_FLOAT, 0, &vArray[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &tArray[0]);

	//Draw it all
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, &iArray[0]);

	//glDrawArrays(GL_TRIANGLES,0,6);

	//Disable the vertex arrays
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

void Texture::drawInFrame(point locInTex, point dimInTex, point* origin)
{
	//////////////NOTE ON THIS FUNCTION
	////The vert and tex coord's in this function are ordered differently to the other drawing functions. I don't know why, but doing them the same was as
	//in the draw funcs above, the verteces were upside-down. So we had to re-order(flip) those......
	//AND THEN Y-Flip(1-yVal) the Texture coords, as the above also flipped the Texture.
	//
	GLint repeatValS;
	GLint repeatValT;
	glGetIntegerv(GL_TEXTURE_WRAP_S, &repeatValS);
	glGetIntegerv(GL_TEXTURE_WRAP_T, &repeatValT);
	//before we bind this texture, we need to set the texture wrap to CLAMP
	//We set texture wrap to CLAMP_TO_BORDER, as this sets any excess space to the coloure set in GL_TEXTURE BORDER (0,0,0,0 by default which is what we want)
	//Whereas CLAMP_TO_EDGE, sets any excess space to the colour of the last texel, which because of blending can cause unpredicted colours to appear in the space
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//when we're done switch them back, this is why we have stored them above.

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID[0]);//bind the texture
	//create a quick vertex array for the primitive we're going to bind the texture to
	////printf("TexID = %u",textureID[0]);
	
	point* locTexCoords = new point();
	point* dimTexCoords = new point();

	//Turn the frames coords and dimensions into values that are Texture-Coordinate-System usable
	locTexCoords->x = (1.0f/w)*locInTex.x;
	locTexCoords->y = 1.0f -((1.0f/h)*locInTex.y);
	dimTexCoords->x = (1.0f/w)*(locInTex.x+dimInTex.x);
	dimTexCoords->y = 1.0f -((1.0f/h)*(locInTex.y+dimInTex.y));

	GLfloat vArray[8] =
	{
		vArray[0] = origin->x, vArray[1] = origin->y+dimInTex.y,//top left i1
		vArray[2] = origin->x, vArray[3] = origin->y,//bottom left i0
		vArray[4] = origin->x+dimInTex.x, vArray[5] = origin->y,//bottom right i3		
		vArray[6] = origin->x+dimInTex.x, vArray[7] = origin->y+dimInTex.y//top right i2
	};

	//create a quick texture array (we COULD create this on the heap rather than creating/destroying every cycle)
	GLfloat tArray[8] = 
	{
		//this has been tinkered with from my normal order. I think LodePNG is bringing the PixelData upside down. SO A QUICK FIX HERE WAS NECESSARY.
		
		locTexCoords->x,dimTexCoords->y,//0 TL
		
		locTexCoords->x,locTexCoords->y,//1 BL
		
		dimTexCoords->x,locTexCoords->y,//2 BR
		
		dimTexCoords->x,dimTexCoords->y//3 TR
	
	};

	//and finally.. the index array...remember, we draw in triangles....(and we'll go CW)
	GLubyte iArray[6] =
	{
		0,1,2,
		0,2,3
	};

	//Activate arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//Give openGL a pointer to our vArray and tArray
	glVertexPointer(2, GL_FLOAT, 0, &vArray[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &tArray[0]);

	//Draw it all
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, &iArray[0]);

	//glDrawArrays(GL_TRIANGLES,0,6);

	//Disable the vertex arrays
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	
	//switch the WRAP settings back
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatValS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatValT);

	//delete repeatValS;
	//delete repeatValT;
	delete locTexCoords;
	delete dimTexCoords;
}

bool Texture::checkPOT(int num)
{
	string error; //holds an error message to return should we need one.
	if(num>=0 && num<=1024)
	{
		for(int i=1;num >= pow(2.0f,i); i++)//while num is still larger than or equal to the current POT val being compared
		{
			if (num == pow(2.0f,i)) return true;//if they match, it is POT and return NULL, (no error needed)

		}
		//if we get HERE, the image was not POT and we should return false. We should also generate an error message box here,
		//telling the user "This texture is not a power of two" or something
		/*error = "Sprite Sheet dimensions must be power of 2. (8,16,32,64,128,256,512,1024) \nFind a Sprite Sheet with compatible dimensions, or edit this one using image editing software. \n(Just pad out the edges to the nearest compatible size)"
		return error.c_str();*/
		//Change of plan, we're throwing an exception
		throw nonPOTException();
		return false;
	}
	//if we get HERE, the number was either negative, 0 or greater than the max texture size we will allow. We should therefore return false.
	//We should also generate an error message box here,
	//telling the user their issue
	/*error = "Sprite Sheet dimensions are either 0 (invalid) or greater than 1024px.\n Spritey accepts only power of 2 values (8,16,32,64,128,256,512,1024) between 2px-1024px."
	return error.c_str();*/
	//Change of plan, we're throwing an exception
	throw InvalidSizeException();
	return false;
}

Texture::Texture(void)
{
}
Texture::~Texture(void)
{
}
