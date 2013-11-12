#pragma once
#include "oglform.h"
#include "dStructs.h"
#pragma managed(push, off)
#include "Texture.h"
#include "SpriteyData.h"
#include "Geometry.h"
#pragma managed(pop)

using namespace dStructs;
using namespace SpriteyData;
using namespace Geometry;
using System::Collections::Generic::List;

ref class FViewer :public OpenGLForm::COpenGL
{
public:

	enum class FV_MODE
	{
		IDLE,ADD_HITBOX,ADD_HURTBOX,EDIT_BOXES
	};

	enum class FV_LOADED_MODE
	{
		NO_SHEET, SHEET_LOADED
	};

	enum class FV_SELECTED
	{
		NONE, IN_HITBOXES, IN_HURTBOXES
	};

	enum class DIRECTION
	{
		UP, DOWN , LEFT, RIGHT
	};

	FViewer(System::Windows::Forms::Form ^ parentForm, int xPos, int yPos, GLsizei iWidth, GLsizei iHeight);
	
	virtual void Render() override;
	void Render(Frame* frame);
	void loadSpriteSheet(string filename);
	void drawFrame(Frame* frame);
	void drawHitBox(point centre, int radius, bool isHovered, bool isSelected);
	void drawHitBox(HitCircle hitBox, bool isHovered, bool isSelected);
	void drawHurtBox(point centre, int radius, bool isHovered, bool isSelected);
	void drawHurtBox(HitCircle hurtBox, bool isHovered, bool isSelected);
	void drawHitBoxList(Frame frame);
	void drawHurtBoxList(Frame frame);
	void checkHovered(point mousePos, Frame currentFrame);//goes through both lists in the current frame and checks all boxes against the pos of the mouse. 
	//the first time it finds that a box is selected, it breaks the loop and stops. Loading that list the box is in into "selected" and the position of that box in the array into the 
	void addToSelectedList(int posOfHB, FV_SELECTED whichList);
	void checkIfSelecting(point mousePos, Frame currentFrame, bool shiftHeld);
	void unselectAllHB();
	void moveAllSelectedBoxes(Frame& currentFrame, int defaultStep, bool shiftHeld, DIRECTION direction);//cycles through both lists (providing they're selected) and moves the boxes by the described ammount, in the specified direction using the above method
	void deleteAllSelectedBoxes(Frame& currentFrame);
	void resizeAllSelectedBoxes(Frame& currentFrame, int defaultStep);


	FV_LOADED_MODE loadedMode;
	FV_MODE mode;
	FV_SELECTED hovered;

	
	point* drawAnchorAt;
	Texture* myTex;//the currently loaded texture we will be using to draw our frames

private:
	entity* spriteSheet;
	point* size;//a simple soft coded value representing the fviewers size
	
	int* positionOfHoveredHB;//this is a pointer because at the end of it's use, it can be deleted and set to nullptr. 
	//We can then check for nullptr to make sure we don't try to draw a hovered box if we aren't hovering anything.

	List<int>^ selectedHitBoxList;//A list to contain the locations of the selected nodes
	List<int>^ selectedHurtBoxList;

	/////////
	//FLAGS//
	/////////
	bool DRAW_HITBOXES_FLAG;
	bool DRAW_HURTBOXES_FLAG;

	void loadSSdims(Texture* texture);
	void checkHitBoxesHov(point mousePos, Frame currentFrame);
	void checkHurtBoxesHov(point mousePos, Frame currentFrame);
	//void deleteBox(int boxPos, Frame& currentFrame);
	void deleteAllInList(Frame& currentFrame, FV_SELECTED whichList);
	void moveBox(HitCircle& box, int defaultStep, bool shiftHeld, DIRECTION direction);//moves ONE box
	void resizeBox(HitCircle& box, int defaulStep);
	///////////////////
	///COLOUR ARRAYS///
	///////////////////
	array<GLfloat>^ hbColour;
	array<GLfloat>^ hbLineColour;
	array<GLfloat>^ hurbColour;
	array<GLfloat>^ hurbLineColour;
	//array<GLfloat>^ hlhbColour;
	array<GLfloat>^ hlhbLineColour;//when highlighted only the HB's lines will change colour
	array<GLfloat>^ selecLineColour;



};

