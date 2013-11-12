#pragma once
#include "OGLForm.h"
#include "dStructs.h"
#pragma managed(push, off)
#include "Texture.h"
#include "SpriteyData.h"
#pragma managed(pop)


using namespace dStructs;
using namespace SpriteyData;

ref class SSViewer : public OpenGLForm::COpenGL
{
public:
	enum class SSV_MODE
	{
		IDLE, DRAG, CB_EDIT,MOVE_ANCHOR
	};

	enum class DRAG_STATE
	{
		NOT_DRAGGING, DRAGGING
	};

	enum class SS_LOADED_MODE
	{
		NO_SHEET, SHEET_LOADED, HOLDER
	};

	enum class ANCHOR_MOVE_STATE
	{
		IDLE, UP, DOWN, LEFT, RIGHT
	};

	//bool ssLoadedFlag; defunct. We're useing the enum ssLoadedMode instead

	SSViewer(System::Windows::Forms::Form ^ parentForm, GLsizei iWidth, GLsizei iHeight);
	virtual void Render() override;
	void Render(point* captureBoxSizePointer);
	void Render(point* captureBoxSizePointer, Frame* currentFrame);
	void loadInitialTex();
	void changeSpriteSheet(string filename);//changes myTex to a new texture from file
	void mouseHoverCheck(point* mouse);
	
	/////////////
	//dragging
	////////////
	void beginDrag(point* mouse);//begins a dragging phase
	void drag(point* mouse);//moves the texture
	void endDrag();//ends a dragging phase


	/////////////
	//enums
	////////////
	SS_LOADED_MODE loadedMode;
	SSV_MODE mode;
	DRAG_STATE dragState;
	//The below two bool flags do EXACTLY the same thing. Bad practice i guess, but they are
	//both more readable for their purpose
	bool DRAW_HB_FLAG;//gets set by mousehovercheck and checked by render
	bool MOUSE_HOVERED;//gets set the same as above
	entity* spriteSheet;
	

private:
	void drawCaptureBox(point cbSize);
	void drawHighlightBox();
	void loadSSDims(Texture* texture);
	void loadAnchorTex();
	void drawAnchorPoint(point frameAnchorLoc, point* cbSize); // draws the anchor symbol

	Texture* myTex;
	Texture* anchorTex;
	array<GLfloat>^ cbColour;//the colour of the capture box
	array<GLfloat>^ hbColour;//the colour of the box that highlights the spritesheet when hovered
	array<GLfloat>^ hbLineColour;//like above but a bit darker, for the outline of the box
	array<GLfloat>^ hbDragColour;//same as hbColour but for when the box is being dragged
	array<GLfloat>^ hbDragLineColour;//as above
	
	
	///////////////////
	//Dragging vars
	///////////////////
	point* prevMousePos;//When dragging begins, themouse position is frozen here where......
	point* currentMouseGrowth;//this is worked out each frame of mouse-motion by subtracting
	////prevMousePos from the current mouse position. When the dragging phase ends, this is reset to 0.
	point* prevSheetPos;//Stores the value of the spritesheet
	

};

