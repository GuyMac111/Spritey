#include "StdAfx.h"
#include "FViewer.h"


FViewer::FViewer(System::Windows::Forms::Form ^ parentForm, int xPos, int yPos, GLsizei iWidth, GLsizei iHeight) : COpenGL(parentForm,iWidth,iHeight)
{
	printf("\nSuper inherited frameViewer constructor");
	HWND hWnd = static_cast<HWND> (this->Handle.ToPointer());
	SetWindowPos(hWnd, NULL,xPos,yPos,iWidth,iHeight,SWP_NOZORDER);
	loadedMode = FV_LOADED_MODE::NO_SHEET;
	mode = FV_MODE::IDLE;
	hovered = FV_SELECTED::NONE;

	positionOfHoveredHB = nullptr;
	selectedHitBoxList = gcnew List<int>;
	selectedHurtBoxList = gcnew List<int>;

	size = new point(iWidth,iHeight);
	drawAnchorAt = new point(size->x/2,20);//we will draw the bottom left corner of the sprite here, and then translate it by -AnchorPos

	//set the hit&hurt box draw flags
	DRAW_HITBOXES_FLAG = true;
	DRAW_HURTBOXES_FLAG = true;

	//A managed array defined to represent a colour HITBOXES
	hbColour = gcnew array<GLfloat>(4); 
	hbColour[0] = 1.0f;//reflects all red
	hbColour[1] = 0.2f;
	hbColour[2] = 0.5f;
	hbColour[3] = 0.4f;

	//A managed array defined to represent a colour HITBOXES_LINES
	hbLineColour = gcnew array<GLfloat>(4); 
	hbLineColour [0] = 1.0f;
	hbLineColour [1] = 0.2f;
	hbLineColour [2] = 0.5f;
	hbLineColour [3] = 0.8f;

	//A managed array defined to represent a colour HURTBOXES
	hurbColour = gcnew array<GLfloat>(4); 
	hurbColour[0] = 0.2f;//reflects all red
	hurbColour[1] = 0.8f;
	hurbColour[2] = 0.5f;
	hurbColour[3] = 0.4f;

	//A managed array defined to represent a colour HURTBOXES_LINES
	hurbLineColour = gcnew array<GLfloat>(4); 
	hurbLineColour [0] = 0.2f;
	hurbLineColour [1] = 0.8f;
	hurbLineColour [2] = 0.4f;
	hurbLineColour [3] = 0.8f;

	//Highlighted border is solid white
	hlhbLineColour = gcnew array<GLfloat>(4);
	hlhbLineColour [0] = 1.0f;
	hlhbLineColour [1] = 1.0f;
	hlhbLineColour [2] = 1.0f;
	hlhbLineColour [3] = 1.0f;

	//Highlighted border is solid white
	selecLineColour = gcnew array<GLfloat>(4);
	selecLineColour [0] = 0.0f;
	selecLineColour [1] = 0.0f;
	selecLineColour [2] = 1.0f;
	selecLineColour [3] = 1.0f;
	
}

void FViewer::loadSpriteSheet(string filename)
{
	try
	{	
		wglMakeCurrent(m_hDC, m_hglrc);
		myTex = new Texture(filename);
		//If we get here, no exceptions were thrown, and therefore the initialising was complete and legal
		loadSSdims(myTex);//load the dimensions of this texture into the spritesheet variable
		loadedMode = FV_LOADED_MODE::SHEET_LOADED;
	}
	catch(Texture::nonPOTException& e)
	{
		System::String^ err = gcnew System::String(e.what());
		MessageBox::Show(err, "Sprite Sheet Error", MessageBoxButtons::OK, MessageBoxIcon::Stop);
		//no need to set loadedmode here as loadinitialtex sets it as necessary.
		if(myTex != nullptr)
		{
			delete myTex;
		}
	}
	catch(Texture::InvalidSizeException& e)
	{
		System::String^ err = gcnew System::String(e.what());
		MessageBox::Show(err, "Sprite Sheet Error", MessageBoxButtons::OK, MessageBoxIcon::Stop);
		//no need to set loadedmode here as loadinitialtex sets it as necessary.
		if(myTex != nullptr)
		{
			delete myTex;
		}
	}

}

void FViewer::loadSSdims(Texture* texture)
{
	if (texture != nullptr)//if texture doesn't point to empty space (to make sure we don't assign the value of a hanging pointer to spriteSheet
	{
		delete spriteSheet; //delete what spriteSheet points to if it already points to something
		spriteSheet = new entity(0,0,myTex->w,myTex->h);
	}
	
}

void FViewer::drawFrame(Frame* frame)
{
	if(frame!=nullptr)
	{
		point* drawAt = new point(drawAnchorAt->x - frame->f_anchor.x, drawAnchorAt->y - frame->f_anchor.y); //a way for us to decide where we are drawing the frame on the viewer.
		//don't forget to flip to Y-coords
		//the anchor point will be drawn at (size.x/2, 20) every single time
				
		myTex->drawInFrame(frame->f_loc, frame->f_dim, drawAt);
		delete drawAt;
	}
}

void FViewer::drawHitBox(point centre, int radius, bool isHovered, bool isSelected)
{
	//translate the centre values of the hitbox from the anchor-based coordinates, back to the world coords
	centre.x = centre.x + drawAnchorAt->x;
	centre.y = centre.y + drawAnchorAt->y;

	//set up all the drawing colours
	GLfloat previousColour[4];
	glGetFloatv(GL_CURRENT_COLOR, previousColour);
	
	pin_ptr<GLfloat> p = &hbColour[0];//stick a pin ptr in this bad boy, stop him from going anywhere
	GLfloat* glP = p;//a non-moveable pointer to elem 0

	pin_ptr<GLfloat> lineP = &hbLineColour[0];//stick a pin_ptr in this bad boy, stop him from going anywhere
	pin_ptr<GLfloat> hLineP = &hlhbLineColour[0];//create a pin_ptr in the highlight line colour array. If it needs drawing we'll point to this instead of the regular one
	pin_ptr<GLfloat> sLineP = &selecLineColour[0];

	GLfloat* glLineP; //declare it outside the scope of the if statement = lineP;//a non-moveable pointer to elem 0
	if(isHovered)//if this hitbox is hovered
	{
		glLineP = hLineP;//set the line colour to the highlighted version
	} 
	else//if not
	{
		if (isSelected)
		{
			glLineP = sLineP;
		} 
		else
		{
			glLineP = lineP;//the regular line colour
		}
		
	}
	glColor4fv(glP);
	drawFilledCircle(centre, radius);
	glColor4fv(glLineP);
	drawCircleOutline(centre, radius);

	glColor4fv(previousColour);

	//delete glP; NOTE: Pin_ptr objects do not need deleting as they are MANAGED
	//delete glLineP;
	
}

void FViewer::drawHitBox(HitCircle hitBox, bool isHovered, bool isSelected)
{
	drawHitBox(hitBox.loc, hitBox.radius, isHovered, isSelected);
}

void FViewer::drawHurtBox(point centre, int radius, bool isHovered, bool isSelected)
{
	//translate the centre values of the hitbox from the anchor-based coordinates, back to the world coords
	centre.x = centre.x + drawAnchorAt->x;
	centre.y = centre.y + drawAnchorAt->y;

	//set up all the drawing colours
	GLfloat previousColour[4];
	glGetFloatv(GL_CURRENT_COLOR, previousColour);

	pin_ptr<GLfloat> p = &hurbColour[0];//stick a pin ptr in this bad boy, stop him from going anywhere
	GLfloat* glP = p;//a non-moveable pointer to elem 0

	pin_ptr<GLfloat> lineP = &hurbLineColour[0];//stick a pin ptr in this bad boy, stop him from going anywhere
	pin_ptr<GLfloat> hLineP = &hlhbLineColour[0];//create a pin_ptr in the highlight line colour array. If it needs drawing we'll point to this instead of the regular one
	pin_ptr<GLfloat> sLineP = &selecLineColour[0];

	GLfloat* glLineP; //declare it outside the scope of the if statement = lineP;//a non-moveable pointer to elem 0
	if(isHovered)//if this hitbox is hovered
	{
		glLineP = hLineP;//set the line colour to the highlighted version
	} 
	else//if not
	{
		if (isSelected)
		{
			glLineP = sLineP;
		} 
		else
		{
			glLineP = lineP;//the regular line colour
		}

	}
	
	

	glColor4fv(glP);
	drawFilledCircle(centre, radius);
	glColor4fv(glLineP);
	drawCircleOutline(centre, radius);

	glColor4fv(previousColour);

	//delete glP;  NOTE: Pin_ptr objects do not need deleting as they are MANAGED
	//delete glLineP;
}

void FViewer::drawHurtBox(HitCircle hurtBox, bool isHovered, bool isSelected)
{
	drawHurtBox(hurtBox.loc, hurtBox.radius, isHovered, isSelected);
}

void FViewer::drawHitBoxList(Frame frame)
{
	if(!frame.hitBoxes.empty())//only bother doing this stuff if the list actually has something in it
	{
		for(int i = 0; i<frame.hitBoxes.size();i++)
		{
			if(hovered==FV_SELECTED::IN_HITBOXES&&positionOfHoveredHB!=nullptr&&i==*positionOfHoveredHB)
			{//if one has been hovered and is in this hitboxlist
				drawHitBox(frame.hitBoxes.at(i), true, false);
			}
			else
			{
				if(selectedHitBoxList->Contains(i))//if it's in the selected list, 
				{
					drawHitBox(frame.hitBoxes.at(i), false, true);//draw it selected
				}
				else
				{
					drawHitBox(frame.hitBoxes.at(i), false, false);
				}
				
			}
			
		}
	}
}

void FViewer::drawHurtBoxList(Frame frame)
{
	if(!frame.hurtBoxes.empty())//only bother doing this stuff if the list actually has something in it
	{
		for(int i = 0; i<frame.hurtBoxes.size();i++)
		{
			if(hovered==FV_SELECTED::IN_HURTBOXES&&positionOfHoveredHB!=nullptr&&i==*positionOfHoveredHB)
			{//if one has been hovered and is in this hitboxlist
				drawHurtBox(frame.hurtBoxes.at(i), true, false);
			}
			else
			{
				if(selectedHurtBoxList->Contains(i))
				{
					drawHurtBox(frame.hurtBoxes.at(i), false, true);
				} 
				else
				{
					drawHurtBox(frame.hurtBoxes.at(i), false, false);
				}
				
			}

		}
	}
}

void FViewer::Render(Frame* frame)
{
	wglMakeCurrent(m_hDC, m_hglrc); //Make current before we render it!
	//printf("\nRender: FViewer instance");
	glClearColor(0.5f,1.0f,0.5f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	if(loadedMode == FV_LOADED_MODE::SHEET_LOADED && frame != nullptr)
	{
		drawFrame(frame);//draws the frame
		if(DRAW_HURTBOXES_FLAG){drawHurtBoxList(*frame);}//if DHF is true draw the hurtboxes
		if(DRAW_HITBOXES_FLAG){drawHitBoxList(*frame);}//if DHF is true draw the hitboxes
	}
	
	////TEST
	//point* testPos = new point(size->x/2, 20);
	//drawHitbox(*testPos, 30);
	//delete testPos;
}

void FViewer::Render()
{
	wglMakeCurrent(m_hDC, m_hglrc);
	//printf("\nRender: FViewer instance");
	glClearColor(0.5f,1.0f,0.5f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	
	////TEST
	//point* testPos = new point(size->x/2, 20);
	//drawHitbox(*testPos, 30);
	//delete testPos;
}

void FViewer::checkHovered(point mousePos, Frame currentFrame)
{
	//at the beginning of this function the hovered enum and positionofhoveredhb int should always be their default NON & nullptr values
	hovered = FV_SELECTED::NONE;
	delete positionOfHoveredHB;
	positionOfHoveredHB = nullptr;

	checkHitBoxesHov(mousePos, currentFrame);//check hitboxes first as they are always drawn on top
	checkHurtBoxesHov(mousePos, currentFrame);

}

void FViewer::checkHitBoxesHov(point mousePos, Frame currentFrame)
{
	if(hovered == FV_SELECTED::NONE && positionOfHoveredHB == nullptr)//if there hasn't been one hovered yet
	{
		//unlike most loops we're gonna start at the back. 
		for(int i = currentFrame.hitBoxes.size()-1; i>=0 && (hovered == FV_SELECTED::NONE && positionOfHoveredHB == nullptr);i--)
		{
			//create a point to rep the world pos of the hitbox centre
			point* hitboxWorldPos = new point(currentFrame.hitBoxes.at(i).loc.x+drawAnchorAt->x, currentFrame.hitBoxes.at(i).loc.y+drawAnchorAt->y);
			//loop will keep iterating until it hits the end of the array or a hovered box is found
			if(mouseIsOverCircle(mousePos, *hitboxWorldPos, currentFrame.hitBoxes.at(i).radius))
			{
				//if it(the mouse) is within this circle
				//KEEP AN EYE ON THIS. Not sure how the scope will be affected
				positionOfHoveredHB = new int(i); //use int's copy constructor to avoid 'i' going out of scope and leaving us with a hanging pointer
				hovered = FV_SELECTED::IN_HITBOXES;//let the enum know that the hovered box is in this array of boxes
			}
			delete hitboxWorldPos;
		}
	}
}

void FViewer::checkHurtBoxesHov(point mousePos, Frame currentFrame)
{
	if(hovered == FV_SELECTED::NONE && positionOfHoveredHB == nullptr)//if there hasn't been one hovered yet
	{
		//unlike most loops we're gonna start at the back. 
		for(int i = currentFrame.hurtBoxes.size()-1; i>=0 && (hovered == FV_SELECTED::NONE && positionOfHoveredHB == nullptr);i--)
		{
			//create a point to rep the world pos of the hitbox centre
			point* hurtboxWorldPos = new point(currentFrame.hurtBoxes.at(i).loc.x+drawAnchorAt->x, currentFrame.hurtBoxes.at(i).loc.y+drawAnchorAt->y);
			//loop will keep iterating until it hits the end of the array or a hovered box is found
			if(mouseIsOverCircle(mousePos, *hurtboxWorldPos, currentFrame.hurtBoxes.at(i).radius))
			{
				//if it(the mouse) is within this circle
				//KEEP AN EYE ON THIS. Not sure how the scope will be affected
				positionOfHoveredHB = new int(i); //use int's copy constructor to avoid 'i' going out of scope and leaving us with a hanging pointer
				hovered = FV_SELECTED::IN_HURTBOXES;//let the enum know that the hovered box is in this array of boxes
			}
			delete hurtboxWorldPos;
		}
	}
}

void FViewer::addToSelectedList(int posOfHB, FV_SELECTED whichList)
{
	switch(whichList)
	{
	case FV_SELECTED::IN_HITBOXES://if HITBOX add to hitbox
		if(!selectedHitBoxList->Contains(posOfHB))//if that box isnt already in there, add it.
		{
			selectedHitBoxList->Add(posOfHB);
		}
		else//if it's already in there, remove it
		{
			selectedHitBoxList->Remove(posOfHB);
		}
		
		break;

	case FV_SELECTED::IN_HURTBOXES://and vice versa
		if(!selectedHurtBoxList->Contains(posOfHB))//if that box isnt already in there, add it.
		{
			selectedHurtBoxList->Add(posOfHB);
		}
		else//if it's already in there, remove it
		{
			selectedHurtBoxList->Remove(posOfHB);
		}
		break;

	default:
		break;
	}

}

void FViewer::unselectAllHB()
{
	//just hard-clear both Lists
	selectedHitBoxList->Clear();
	selectedHurtBoxList->Clear();
}

void FViewer::checkIfSelecting(point mousePos, Frame currentFrame, bool shiftHeld)
{
	checkHovered(mousePos, currentFrame);//get the latest info on which hit/hurtbox is being hovered over
	
	if(hovered!=FV_SELECTED::NONE && positionOfHoveredHB!=nullptr)//if we have anything hovered-over
	{
		if (!shiftHeld)//if shift isnt held
		{
			unselectAllHB();//clear the selected lists before adding to it, so that we're operating a 1-at-a-time selection process
		}

		addToSelectedList(*positionOfHoveredHB, hovered);//Adds the currently hovered hb's location
		//this functions isnt very indicative of this, but it also removes the HB from the selected list if it's already select,
		//meaning we can toggle
	}
	else//if there's nothing to add to the lists
	{
		unselectAllHB();//unselect them all
		
	}
}

void FViewer::resizeBox(HitCircle& box, int defaulStep)
{
	if((box.radius+defaulStep)>1)//we don't want to shrink HB's to below 2
	{
		box.radius += defaulStep;//we add on the change in size
	}
}

void FViewer::resizeAllSelectedBoxes(Frame& currentFrame, int defaultStep)
{
	if(DRAW_HITBOXES_FLAG && selectedHitBoxList->Count>0)//if we're even drawing the Hitboxes & it isn't empty
	{
		for(int i = 0; i<selectedHitBoxList->Count; i++)
		{
			if(currentFrame.hitBoxes.size() >= selectedHitBoxList[i] && selectedHitBoxList[i] >= 0)//make sure the object in the selected list is within the range of the hitbox vector
			{
				resizeBox(currentFrame.hitBoxes.at(selectedHitBoxList[i]), defaultStep);
			}
		}
	}

	if(DRAW_HURTBOXES_FLAG && selectedHurtBoxList->Count>0)//if we're even drawing the Hitboxes & it isn't empty
	{
		for(int i = 0; i<selectedHurtBoxList->Count; i++)
		{
			if(currentFrame.hurtBoxes.size() >= selectedHurtBoxList[i] && selectedHurtBoxList[i] >= 0)//make sure the object in the selected list is within the range of the hitbox vector
			{
				resizeBox(currentFrame.hurtBoxes.at(selectedHurtBoxList[i]),defaultStep);
			}
		}
	}

	
}

void FViewer::moveBox(HitCircle& box, int defaultStep, bool shiftHeld, DIRECTION direction)
{
		switch (direction)
		{
		case DIRECTION::UP: 
			if(!shiftHeld)
			{
				box.loc.y+=defaultStep;
			}
			else
			{
				box.loc.y+=(defaultStep*2);
			}
			break;

		case DIRECTION::DOWN:
			if(!shiftHeld)
			{
				box.loc.y-=defaultStep;
			}
			else
			{
				box.loc.y-=(defaultStep*2);
			}
			break;

		case DIRECTION::LEFT:
			if(!shiftHeld)
			{
				box.loc.x-=defaultStep;
			}
			else
			{
				box.loc.x-=(defaultStep*2);
			}
			break;

		case DIRECTION::RIGHT:
			if(!shiftHeld)
			{
				box.loc.x+=defaultStep;
			}
			else
			{
				box.loc.x+=(defaultStep*2);
			}
			break;
		
		default:
			break;
		}


}

void FViewer::moveAllSelectedBoxes(Frame& currentFrame, int defaultStep, bool shiftHeld, DIRECTION direction)
{
	if(DRAW_HITBOXES_FLAG && selectedHitBoxList->Count>0)//if we're even drawing the Hitboxes & it isn't empty
	{
		for(int i = 0; i<selectedHitBoxList->Count; i++)
		{
			if(currentFrame.hitBoxes.size() >= selectedHitBoxList[i] && selectedHitBoxList[i] >= 0)//make sure the object in the selected list is within the range of the hitbox vector
			{
				moveBox(currentFrame.hitBoxes.at(selectedHitBoxList[i]),defaultStep, shiftHeld, direction);
			}
		}
	}

	if(DRAW_HURTBOXES_FLAG && selectedHurtBoxList->Count>0)//if we're even drawing the Hitboxes & it isn't empty
	{
		for(int i = 0; i<selectedHurtBoxList->Count; i++)
		{
			if(currentFrame.hurtBoxes.size() >= selectedHurtBoxList[i] && selectedHurtBoxList[i] >= 0)//make sure the object in the selected list is within the range of the hitbox vector
			{
				moveBox(currentFrame.hurtBoxes.at(selectedHurtBoxList[i]),defaultStep, shiftHeld, direction);
			}
		}
	}
	
}

void FViewer::deleteAllSelectedBoxes(Frame& currentFrame)
{
	if(DRAW_HITBOXES_FLAG)//if the hitboxes are even visible and operable at this time
	{
		deleteAllInList(currentFrame, FV_SELECTED::IN_HITBOXES);
	}
	if(DRAW_HURTBOXES_FLAG)//same for hurtboxes
	{
		deleteAllInList(currentFrame, FV_SELECTED::IN_HURTBOXES);
	}
}

void FViewer::deleteAllInList(Frame& currentFrame, FV_SELECTED whichList)
{
	switch (whichList)
	{
	case FV_SELECTED::IN_HITBOXES:
		{
			if (selectedHitBoxList->Count>0)//if we even need to bother
			{
				vector<HitCircle>* savedBoxes = new vector<HitCircle>;

				for (int i = 0; i<currentFrame.hitBoxes.size(); i++)
				{
					//go through each position in the HBlist and check if that pos also exists in the selected list
					if(!selectedHitBoxList->Contains(i))
					{
						//if it is NOT on the selected list, then we don't wanna delete it,
						//and therefore should add it to the saved list
						savedBoxes->push_back(currentFrame.hitBoxes.at(i));
					}

				}
				//great now our savedBoxes vector is full of all the HB's that 
				//DONT appear in the selected list
				//now to clear the currentFrames hitbox list
				currentFrame.hitBoxes.clear();
				//and copy the hitCircles we wanted to keep BACK IN
				for(int i = 0; i<savedBoxes->size(); i++)
				{
					currentFrame.hitBoxes.push_back(savedBoxes->at(i));
				}

				//cleanup
				delete savedBoxes;
			}
			break;
		}


	case FV_SELECTED::IN_HURTBOXES:
		{
			if (selectedHurtBoxList->Count>0)//if we even need to bother
			{
				vector<HitCircle>* savedBoxes = new vector<HitCircle>;

				for (int i = 0; i<currentFrame.hurtBoxes.size(); i++)
				{
					//go through each position in the HBlist and check if that pos also exists in the selected list
					if(!selectedHurtBoxList->Contains(i))
					{
						//if it is NOT on the selected list, then we don't wanna delete it,
						//and therefore should add it to the saved list
						savedBoxes->push_back(currentFrame.hurtBoxes.at(i));
					}

				}
				//great now our savedBoxes vector is full of all the HB's that 
				//DONT appear in the selected list
				//now to clear the currentFrames hitbox list
				currentFrame.hurtBoxes.clear();
				//and copy the hitCircles we wanted to keep BACK IN
				for(int i = 0; i<savedBoxes->size(); i++)
				{
					currentFrame.hurtBoxes.push_back(savedBoxes->at(i));
				}

				//cleanup
				delete savedBoxes;
			}
			break;
		}
	}

}
