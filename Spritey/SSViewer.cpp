#include "StdAfx.h"
#include "SSViewer.h"


//GLfloat SSViewer::cbColour[] = {0.7f, 0.2f ,0.6f , 0.4f}; //colour of the capturebox

SSViewer::SSViewer(System::Windows::Forms::Form ^ parentForm, GLsizei iWidth, GLsizei iHeight) : COpenGL(parentForm,iWidth,iHeight)
{
	printf("\nSuper inherited COGL const func GO");
	
	//A managed array defined to represent a colour
	cbColour = gcnew array<GLfloat>(4); 
	cbColour[0] = 0.7f;
	cbColour[1] = 0.2f;
	cbColour[2] = 0.6f;
	cbColour[3] = 0.4f;

	//A managed array defined to represent a colour
	hbColour = gcnew array<GLfloat>(4); 
	hbColour[0] = 0.6f;
	hbColour[1] = 0.7f;
	hbColour[2] = 0.2f;
	hbColour[3] = 0.4f;

	//A managed array defined to represent a colour
	hbLineColour = gcnew array<GLfloat>(4); 
	hbLineColour [0] = 0.6f;
	hbLineColour [1] = 0.7f;
	hbLineColour [2] = 0.2f;
	hbLineColour [3] = 0.8f;

	//A managed array defined to represent a colour
	hbDragColour = gcnew array<GLfloat>(4); 
	hbDragColour [0] = 0.4f;
	hbDragColour [1] = 0.8f;
	hbDragColour [2] = 0.2f;
	hbDragColour [3] = 0.4f;

	//A managed array defined to represent a colour
	hbDragLineColour = gcnew array<GLfloat>(4); 
	hbDragLineColour[0] = 0.4f;
	hbDragLineColour[1] = 0.8f;
	hbDragLineColour[2] = 0.2f;
	hbDragLineColour[3] = 0.8f;

	//let us know that the spritesheet is not loaded
	loadedMode = SS_LOADED_MODE::NO_SHEET;
	mode = SSV_MODE::IDLE;
	dragState = DRAG_STATE::NOT_DRAGGING;

	//Holder Texture
	//This test texture is a safety image, that we know works. If exceptions are thrown by the reloadTexture function, we revert to this image.
	//It's also contained in the project directory, so it's always there
	loadInitialTex();
	loadAnchorTex();

	DRAW_HB_FLAG = false;//initialise this flag
	MOUSE_HOVERED = false;//initialise this flag
	

}

void SSViewer::changeSpriteSheet(string filename)
{
	try
	{
		//delete myTex;
		////Test Texture
		//myTex = new Texture(filename);
		wglMakeCurrent(m_hDC, m_hglrc);
		myTex->reloadTexture(filename);
		loadSSDims(myTex);//load the dimensions of this texture into the spritesheet variable
		//If we get here, no exceptions were thrown, and therefore the initialising was complete and legal
		loadedMode = SS_LOADED_MODE::SHEET_LOADED;
	}
	catch(Texture::nonPOTException& e)
	{
		System::String^ err = gcnew System::String(e.what());
		MessageBox::Show(err, "Sprite Sheet Error", MessageBoxButtons::OK, MessageBoxIcon::Stop);
		loadInitialTex();
		//no need to set loadedmode here as loadinitialtex sets it as necessary.
		
	}
	catch(Texture::InvalidSizeException& e)
	{
		System::String^ err = gcnew System::String(e.what());
		MessageBox::Show(err, "Sprite Sheet Error", MessageBoxButtons::OK, MessageBoxIcon::Stop);
		loadInitialTex();
		//no need to set loadedmode here as loadinitialtex sets it as necessary.
	}

}

void SSViewer::loadInitialTex()
{
	try
	{
		//Test Texture
		myTex = new Texture("resources/images/holder.png");
		//If we get here, no exceptions were thrown, and therefore the initialising was complete and legal
		loadSSDims(myTex);//load the dimensions of this texture into the spritesheet variable
		loadedMode = SS_LOADED_MODE::HOLDER;
	}
	catch(Texture::nonPOTException& e)
	{
		System::String^ err = gcnew System::String(e.what());
		MessageBox::Show(err, "Sprite Sheet Error", MessageBoxButtons::OK, MessageBoxIcon::Stop);
		loadedMode = SS_LOADED_MODE::NO_SHEET;
	}
	catch(Texture::InvalidSizeException& e)
	{
		System::String^ err = gcnew System::String(e.what());
		MessageBox::Show(err, "Sprite Sheet Error", MessageBoxButtons::OK, MessageBoxIcon::Stop);
		loadedMode = SS_LOADED_MODE::NO_SHEET;
	}
}

void SSViewer::drawCaptureBox(point cbSize)
{
	//store the current colour buffer
	GLfloat previousColour[4];
	glGetFloatv(GL_CURRENT_COLOR, previousColour);

	pin_ptr<GLfloat> p = &cbColour[0];//pin pointer to elem 0 in this array
	GLfloat* glP = p;//a non-moveable pointer to elem 0
	//switch to the capture box's colour, in the colour buffer 
	glColor4fv(glP);

	point BL = point((getCurrentWidth()/2)-(cbSize.x/2), (getCurrentHeight()/2)-(cbSize.y/2));
	point TL = point((getCurrentWidth()/2)-(cbSize.x/2), (getCurrentHeight()/2)+(cbSize.y/2));
	point TR = point((getCurrentWidth()/2)+(cbSize.x/2), (getCurrentHeight()/2)+(cbSize.y/2));
	point BR = point((getCurrentWidth()/2)+(cbSize.x/2), (getCurrentHeight()/2)-(cbSize.y/2));

	glBegin(GL_QUADS);
	glVertex2f(BL.x,BL.y);
	glVertex2f(TL.x,TL.y);
	glVertex2f(TR.x,TR.y);
	glVertex2f(BR.x,BR.y);
	glEnd();

	//switch the colours back (put the previous colour back into the colour buffer)
	glColor4fv(previousColour);
}

void SSViewer::drawAnchorPoint(point frameAnchorLoc, point* cbSize)
{
	point* worldAnchorPos = new point();
	//Anchor is drawn at the bottom left corner of the CB + Anchor vector
	worldAnchorPos->x = (getCurrentWidth()/2)-(cbSize->x/2)+frameAnchorLoc.x;
	worldAnchorPos->y =	(getCurrentHeight()/2)-(cbSize->y/2)+frameAnchorLoc.y;//DONT Y-Flip the vertical location. No-clue why. (I hate you TR Coords)
	//Draw the anchor-point
	anchorTex->draw(worldAnchorPos, Texture::ANCHOR::CENTER);//draw it from the center as it is a centered texture

	//we'll delete this pointer
	delete worldAnchorPos;
}

void SSViewer::mouseHoverCheck(point* mouse)
{
	if(mouseIsOverEntity(mouse, spriteSheet))
	{
		DRAW_HB_FLAG = true;//tell the renderer that we need to draw the HB
		MOUSE_HOVERED = true;
	}else{//if false
		DRAW_HB_FLAG = false;
		MOUSE_HOVERED = false;
	}
}

void SSViewer::drawHighlightBox()
{
	//store the current colour buffer
	GLfloat previousColour[4];
	glGetFloatv(GL_CURRENT_COLOR, previousColour);

	//we'll create GLfloat * for both colours here and then select which one at RT
	pin_ptr<GLfloat> p = &hbColour[0];//pin pointer to elem 0 in this array
	GLfloat* glP = p;//a non-moveable pointer to elem 0
	//switch to the capture box's colour, in the colour buffer 
	pin_ptr<GLfloat> dragging_p = &hbDragColour[0];
	GLfloat* glDragP = dragging_p;
	
	switch(dragState)
	{
	case DRAG_STATE::NOT_DRAGGING:
		glColor4fv(glP);
		break;
	case DRAG_STATE::DRAGGING:
		glColor4fv(glDragP);
		break;
	default:
		glColor4fv(glP);
		break;
	}
	

	

	point* BL;
	point* TL;
	point* TR;
	point* BR;

	//we will be highlighting dependant on which corner the spritesheet is being anchored from
	switch(myTex->Draw_From_Corner)
	{
	case Texture::ANCHOR::BOTTOMLEFT:
		BL = new point(spriteSheet->pos.x, spriteSheet->pos.y);
		TL = new point(spriteSheet->pos.x, spriteSheet->pos.y+spriteSheet->size.y);
		TR = new point(spriteSheet->pos.x+spriteSheet->size.x, spriteSheet->pos.y+spriteSheet->size.y);
		BR = new point(spriteSheet->pos.x+spriteSheet->size.x, spriteSheet->pos.y);
		break;

	case Texture::ANCHOR::BOTTOMRIGHT:
		BL = new point(spriteSheet->pos.x-spriteSheet->size.x, spriteSheet->pos.y);
		TL = new point(spriteSheet->pos.x-spriteSheet->size.x, spriteSheet->pos.y+spriteSheet->size.y);
		TR = new point(spriteSheet->pos.x, spriteSheet->pos.y+spriteSheet->size.y);
		BR = new point(spriteSheet->pos.x, spriteSheet->pos.y);
		break;
	case Texture::ANCHOR::TOPLEFT:
		BL = new point(spriteSheet->pos.x, spriteSheet->pos.y-spriteSheet->size.y);
		TL = new point(spriteSheet->pos.x, spriteSheet->pos.y);
		TR = new point(spriteSheet->pos.x+spriteSheet->size.x, spriteSheet->pos.y);
		BR = new point(spriteSheet->pos.x+spriteSheet->size.x, spriteSheet->pos.y-spriteSheet->size.y);
		break;

	case Texture::ANCHOR::TOPRIGHT:
		BL = new point(spriteSheet->pos.x-spriteSheet->size.x, spriteSheet->pos.y-spriteSheet->size.y);
		TL = new point(spriteSheet->pos.x-spriteSheet->size.x, spriteSheet->pos.y);
		TR = new point(spriteSheet->pos.x, spriteSheet->pos.y);
		BR = new point(spriteSheet->pos.x, spriteSheet->pos.y-spriteSheet->size.y);
		break;
	
	case Texture::ANCHOR::CENTER:
		BL = new point(spriteSheet->pos.x-(spriteSheet->size.x/2),spriteSheet->pos.y-(spriteSheet->size.y/2));
		TL = new point(spriteSheet->pos.x-(spriteSheet->size.x/2),spriteSheet->pos.y+(spriteSheet->size.y/2));
		TR = new point(spriteSheet->pos.x+(spriteSheet->size.x/2),spriteSheet->pos.y+(spriteSheet->size.y/2));
		BR = new point(spriteSheet->pos.x+(spriteSheet->size.x/2),spriteSheet->pos.y-(spriteSheet->size.y/2));
		break;
	
	default:
		break;//just in case
	
	}


	//draw the box
	glBegin(GL_QUADS);
	glVertex2f(BL->x,BL->y);
	glVertex2f(TL->x,TL->y);
	glVertex2f(TR->x,TR->y);
	glVertex2f(BR->x,BR->y);
	glEnd();

	//DRAW THE OUTLINE
	//as with the colours above, we'll create 2 colours and choose between them at RT with a switch
	pin_ptr<GLfloat> p2 = &hbLineColour[0];//pin pointer to elem 0 in this array
	GLfloat* glP2 = p2;//a non-moveable pointer to elem 0
	//switch to the capture box's colour, in the colour buffer 
	pin_ptr<GLfloat> p2Drag = &hbDragLineColour[0];
	GLfloat* glDragP2 = p2Drag;
	
	switch(dragState)
	{
	case DRAG_STATE::NOT_DRAGGING:
		glColor4fv(glP2);
		break;
	case DRAG_STATE::DRAGGING:
		glColor4fv(glDragP2);
		break;
	default:
		glColor4fv(glP2);
		break;
	}
	
	
	glLineWidth(2);//Make the outline a bit thicker

	glBegin(GL_LINE_LOOP);
	glVertex2f(BL->x,BL->y);
	glVertex2f(TL->x,TL->y);
	glVertex2f(TR->x,TR->y);
	glVertex2f(BR->x,BR->y);
	glEnd();

	//switch the colours back (put the previous colour back into the colour buffer)
	glColor4fv(previousColour);
	glLineWidth(1);
	//free the memory of the coord points
	delete BL;
	delete TL;
	delete TR;
	delete BR;
}

void SSViewer::loadSSDims(Texture* texture)
{
	if(texture!=nullptr)
	{
		delete spriteSheet;
		spriteSheet = new entity(point(0,0),point(myTex->w,myTex->h));
	}
}

void SSViewer::beginDrag(point* mouse)
{
	if(mode == SSV_MODE::DRAG && loadedMode == SS_LOADED_MODE::SHEET_LOADED  && mouseIsOverEntity(mouse,spriteSheet))//check that we're in fact set to DRAG mode
	{
		dragState = DRAG_STATE::DRAGGING;//we are now dragging
		prevMousePos = new point(mouse->x,mouse->y);//prevMousePos is now set to the mouse's position at this moment in time
		prevSheetPos = new point(spriteSheet->pos.x,spriteSheet->pos.y);
		printf("\nDRAG START");
	}
}

void SSViewer::drag(point* mouse)
{
	if(mode == SSV_MODE::DRAG && loadedMode == SS_LOADED_MODE::SHEET_LOADED && dragState == DRAG_STATE::DRAGGING)
	{
		currentMouseGrowth = new point(mouse->x-prevMousePos->x, mouse->y-prevMousePos->y);//set how much the mouse's position has changed and in what direction
		//since we first clicked
		spriteSheet->pos.x = prevSheetPos->x + currentMouseGrowth->x;
		spriteSheet->pos.y = prevSheetPos->y + currentMouseGrowth->y;

		delete currentMouseGrowth;
	}
}

void SSViewer::endDrag()
{
	if(mode == SSV_MODE::DRAG && loadedMode == SS_LOADED_MODE::SHEET_LOADED)
	{
		dragState = DRAG_STATE::NOT_DRAGGING;//we're no longer dragging
		
		//we need to make sure we aren't deleting nullptrs here. Because apparently that IS dangerous after all
		if(prevMousePos != nullptr)
		{
			delete prevMousePos;//purge prevMousePos ready for the next time
			prevMousePos = nullptr;
		}
		if(prevSheetPos != nullptr)
		{
			delete prevSheetPos;//same for the prevsheetpos
			prevSheetPos = nullptr;
		}
		printf("\nDRAG END");
	}
}

void SSViewer::loadAnchorTex()
{
	try
	{
		//Test Texture
		anchorTex = new Texture("Resources/anchor.png");
		//If we get here, no exceptions were thrown, and therefore the anchor tex is loaded
	}
	catch(Texture::nonPOTException& e)
	{
		System::String^ err = gcnew System::String(e.what());
		MessageBox::Show(err, "Sprite Sheet Error", MessageBoxButtons::OK, MessageBoxIcon::Stop);
		
	}
	catch(Texture::InvalidSizeException& e)
	{
		System::String^ err = gcnew System::String(e.what());
		MessageBox::Show(err, "Sprite Sheet Error", MessageBoxButtons::OK, MessageBoxIcon::Stop);
		
	}
}

void SSViewer::Render()
{
	wglMakeCurrent(m_hDC, m_hglrc);
	//printf("\nRender: SSViewer instance");
	glClearColor(0.5f,0.5f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void SSViewer::Render(point* captureBoxSizePointer)
{
	wglMakeCurrent(m_hDC, m_hglrc);
	//printf("\nRender: SSViewer instance");
	glClearColor(0.5f,0.5f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//draw the spritesheet texture
	if(loadedMode == SS_LOADED_MODE::HOLDER || loadedMode == SS_LOADED_MODE::SHEET_LOADED)
	{
		//point* texPos = new point(0,0);
		point* sheetPos = new point(spriteSheet->pos.x, spriteSheet->pos.y);
		myTex->draw(sheetPos, Texture::ANCHOR::BOTTOMLEFT);
		//delete texPos;
		delete sheetPos;
	}


	//draw the highlightbox if necessary 

	if(DRAW_HB_FLAG == true && loadedMode == SS_LOADED_MODE::SHEET_LOADED)
	{
		drawHighlightBox();
	}

	//set it's size and draw the capbox 
	point captureBoxSize = point(captureBoxSizePointer->x,captureBoxSizePointer->y);
	drawCaptureBox(captureBoxSize);

	
	//glutPostRedisplay();

	printf("\nSSVIEWER RENDER");
}

void SSViewer::Render(point* captureBoxSizePointer, Frame* currentFrame)
{
	wglMakeCurrent(m_hDC, m_hglrc);
	//printf("\nRender: SSViewer instance");
	glClearColor(0.5f,0.5f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//draw the spritesheet texture
	if(loadedMode == SS_LOADED_MODE::HOLDER || loadedMode == SS_LOADED_MODE::SHEET_LOADED)
	{
		//point* texPos = new point(0,0);
		point* sheetPos = new point(spriteSheet->pos.x, spriteSheet->pos.y);
		myTex->draw(sheetPos, Texture::ANCHOR::BOTTOMLEFT);
		//delete texPos;
		delete sheetPos;
	}


	//draw the highlightbox if necessary 

	if(DRAW_HB_FLAG == true && loadedMode == SS_LOADED_MODE::SHEET_LOADED)
	{
		drawHighlightBox();
	}

	//set it's size and draw the capbox 
	point captureBoxSize = point(captureBoxSizePointer->x,captureBoxSizePointer->y);
	drawCaptureBox(captureBoxSize);

	//Here we draw the anchor symbol, but only if there is a current frame in valid existance, and there is a sheet loaded
	if(loadedMode == SS_LOADED_MODE::SHEET_LOADED && currentFrame!=nullptr && currentFrame!=NULL)//check against both nullptr and null, just for certainty
	{
		drawAnchorPoint(currentFrame->f_anchor, captureBoxSizePointer);
	}
	//glutPostRedisplay();

	printf("\nSSVIEWER RENDER");
}



