#pragma once

#include "dStructs.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <LMCons.h>




namespace SpriteyData
{
	using namespace dStructs;
	using namespace std;
	
	struct invalExtException : public exception
	{
		const char* what() const throw()
		{
			return "Invalid file extension. \nSpritey only accepts \".spr\" as a file type for Animations.";
		}
	};

	struct corruptHeaderException : public exception
	{
		const char* what() const throw()
		{
			return "File's header is corrupt or missing.";
		}
	};

	struct corruptAnimDataException : public exception
	{
		const char* what() const throw()
		{
			return "File's animation data is corrupt or missing.";
		}
	};

	struct emptyFileException : public exception
	{
		const char* what() const throw()
		{
			return "File is empty. \nWhat on earth am I meant to do with this?";
		}
	};

	struct HitCircle
	{
		string name;
		
		point loc;
		int radius;

		HitCircle()
		{
			name = "untitled";
			loc = point(0,0);
			radius = 5;
		}

		HitCircle(point locIn, int radIn)
		{
			name= "untitled";
			loc = point(locIn);
			radius = radIn;
		}

		void HitCircle::grow(int growBy = 1)
		{
			radius += growBy;
		}

		void HitCircle::shrink(int shrinkBy = 1)
		{
			if((radius - shrinkBy)>=2)//I'd like to keep radius to >=2, i know it technically won't make a difference, but it can't hurt to keep them positive
			{
				radius -= shrinkBy;
			}
		}

		void HitCircle::move(point moveVector)
		{
			loc.x += moveVector.x;//alter the x value by this amount + right - left
			loc.y += moveVector.y; 
		}

		
	};

	struct Frame
	{
	public:
		point f_loc;
		point f_dim;
		point f_anchor;
		vector<HitCircle> hitBoxes;
		vector<HitCircle> hurtBoxes;

		Frame()//default constructor:: Creates a frame with values initialized to 0, and empty vectors. Won't be used often.
		{
			f_loc = point(0,0);
			f_dim = point(0,0);
			f_anchor = point(0,0);
		}

		Frame(point* loc, point* dim, point* anchor)//the main constructor:: Creates a frame with some specified values
		{
			f_loc = *loc;//use = overload
			f_dim = *dim;
			f_anchor = *anchor;

		}

		Frame(point loc, point dim, point anchor)
		{
			f_loc = loc;
			f_dim = dim;
			f_anchor = anchor;
		}

		void Frame::addHitBox(HitCircle hitBoxToAdd)
		{
			hitBoxes.push_back(hitBoxToAdd);
		}

		void Frame::addHurtBox(HitCircle hurtBoxToAdd)
		{
			hurtBoxes.push_back(hurtBoxToAdd);
		}

	};

	struct Animation
	{
	public:
		vector<Frame*> frameList;//i KNOW this should be private, but it's just more convenient to have it accessible for certain things,
		//such as when we need to iterate through. As the getter works on the currentFrame+1, which can become confusing.
		
		//Always pass in the value currFrame by reference, meaning that we can edit the value of it from in here
		void Animation::newFrame(int &currFrame) //currFrame will always be >0 so we subtract 1
		{
			vector<Frame*>::iterator it;//create a new iterator
			it = frameList.begin()+((int)currFrame);//that new iterator is 

			frameList.insert(it, new Frame(new point(0,0), new point(100,100), new point(50,20)));//add in a default frame after the current frame
			currFrame++;//we are now working on the new Frame

		}

		void Animation::newFrame(int &currFrame, point* withLoc)//an alternative to the above which allows us to pass in a starting location for the frame.
		{
			vector<Frame*>::iterator it;//create a new iterator
			it = frameList.begin()+((int)currFrame);//that new iterator is 

			frameList.insert(it, new Frame(withLoc, new point(100,100), new point(50,20)));//add in a default frame after the current frame
			currFrame++;//we are now working on the new Frame

			
		}

		void Animation::eraseFrame(int &currFrame)
		{
			vector<Frame*>::iterator it;//create a new iterator
			it = frameList.begin()+((int)currFrame-1);

			delete frameList.at(currFrame-1);//delete it from memory, we must do this rather than simply vector::erase. Or else we'll still leave the memory in-use.
			frameList.erase(it);

			////Now if we have just emptied the vector, we need to create a default frame at element 0 
			if(frameList.empty())
			{
				//add a new Frame with default values at the back 
				//(which at this point is actually element 0)
				frameList.push_back(new Frame(new point(0,0), new point(100,100), new point(50,20)));
				//and just to be sure 
				currFrame = 1;
			}

			//If we erase the last frame in the vector, currFrame will eventually point to an out of range 
			//element in the array, so we should decrement currFrame
			
			//OLD WAY OF CHECKING, in fact the check itself throws the exception. Humorous.
			//if(frameList.at(currFrame-1) == nullptr)//
			
			if(currFrame-1 >= frameList.size())//if our currframe-1(or the array element it indirictly implies) is greater than the array size
			{//we're currently looking O.o.Range
				currFrame--;			
			}

		}

		void Animation::cloneFrame(int &currframe)
		{
			vector<Frame*>::iterator it;//create a new iterator
			it = frameList.begin()+((int) currframe);//iterator pointing to the frame after the one being copied

			frameList.insert(it, new Frame(*frameList.at(currframe-1)));//put a copy of the current frame at this point
			currframe++;
		}

		Frame* Animation::getFrame(int frameAdd1)//returns frame at its actual position (e.g. passing 1 will return frame 0)
		{
			if(frameList.at(frameAdd1-1) != nullptr)//if it's a valid frame and not empty space in memory
			{
				return frameList.at(frameAdd1-1);
			}
			else return nullptr;
		}

		int Animation::size()
		{
			return frameList.size();
		}

		Animation(int &currFrame)//The default constructor for animation. Will create a new instance with a single empty frame
		{//don't forget to pass in currFrame by ref
			frameList.push_back(new Frame(new point(0,0), new point(0,0), new point(0,0)));
			currFrame = 1;
		}

		Animation()//A constructor for use only when loading an animation in from a file. Doesn't create a 1st frame, nor set any kind of current frame
		{

		}

		Animation(const Animation& otherAnim)//Copy constructor, creates a deep copy of the framelist, so that temporary instances of Animation can be safely disposed of
		{//without killing in-use pointers
			for(unsigned int i = 0; i<otherAnim.frameList.size();i++)
			{
				frameList.push_back(new Frame(*otherAnim.frameList.at(i)));//create a new frame using the Frame copy constructor, with the Frame pointed to by the element in the framelist 
			}
			
		}

		~Animation()
		{
			/*while (!frameList.empty())
			{
			delete frameList.back();
			frameList.pop_back();
			}*/
		}
		

	};

	struct SpriteySettings
	{
	private:
		//DATA
		string animationName;
		string authorName;
		string associatedSpriteSheet;
		string lastEdited;
		int numberOfFrames;

		//SETTINGS
		bool ANCHOR_DEVIATION_FLAG;
		bool ANIM_LOOPS;

		void SpriteySettings::setAuthor()//gets the username and assigns it to the author var
		{
			TCHAR name[UNLEN+1];
			DWORD size = UNLEN+1;

			if (GetUserName((TCHAR*)name, &size))//if it works
			{
				authorName = string(name);//we're in UNICODE text encoding, so the system is chucking us a wide character string
			}

		}

				
		
	public:
		
		SpriteySettings()
		{
			ANCHOR_DEVIATION_FLAG = true;//defaults to this
			animationName = "untitled animation";//defaults to this 
			setAuthor();//rip the username from the system for our author
		}

		bool SpriteySettings::isAnchorDevAllowed()
		{
			return ANCHOR_DEVIATION_FLAG;
		}

		void SpriteySettings::setAnchorDevAllowed(bool anchorDevAllowed)
		{
			ANCHOR_DEVIATION_FLAG = anchorDevAllowed;
		}

		void SpriteySettings::setSSFileName(string name)
		{
			associatedSpriteSheet = name;
		}

		const char* SpriteySettings::getSSFileName()
		{
			return associatedSpriteSheet.c_str();
		}

		void SpriteySettings::setAuthor(string author)
		{
			authorName = author;
		}
		
		const char* SpriteySettings::getAuthor()
		{
			return authorName.c_str();
		}

		void SpriteySettings::setAnimationName(string animName)
		{
			animationName = animName;
		}

		const char* SpriteySettings::getAnimationName()
		{
			return animationName.c_str();
		}

		void SpriteySettings::setNumFrames(int numFrames)
		{
			numberOfFrames = numFrames;
		}
	};

	class SaveFile
	{

	public:
		enum BOX_TYPE
		{
			HIT, HURT
		};

	private:
		enum ELEM_TYPE
		{
			LOC, DIM, ANC, HB_LOC
		};

		string fileContents;

		void SaveFile::storeHeader(SpriteySettings settings, size_t numFrames)
		{
			fileContents += "<HEADER>";
			storeData(settings, numFrames);
			storeConfig(settings);
			fileContents += "</HEADER>";
		}

		void SaveFile::storeData(SpriteySettings settings, size_t numberOfFrames)
		{
			fileContents += "<DATA>";
			
			//store animation name
			fileContents += "<ANIM-NAME>";
			fileContents += settings.getAnimationName();
			fileContents += "</ANIM-NAME>";

			//store author
			fileContents += "<AUTHOR>";
			fileContents += settings.getAuthor();
			fileContents += "</AUTHOR>";

			//store #frames
			fileContents += "<NUMFRAMES>";
			fileContents += to_string(static_cast<long long>(numberOfFrames));//hope this works, it should!
			fileContents += "</NUMFRAMES>";

			//store ssfilename
			fileContents += "<SPRITESHEET>";
			fileContents += settings.getSSFileName();
			fileContents += "</SPRITESHEET>";

			fileContents += "</DATA>";
		}

		void SaveFile::storeConfig(SpriteySettings settings)
		{
			fileContents += "<CONFIG>";

			//store anchor deviation bool
			fileContents += "<ANC-DEV>";
			
			if(settings.isAnchorDevAllowed())//if true
			{
				fileContents += "t";
			}else{//if false
				fileContents +="f";
			}

			fileContents += "</ANC-DEV>";

			fileContents += "</CONFIG>";
		}

		void SaveFile::storeAnimation(Animation animToStore)
		{
			fileContents+="<ANIM>";
			for(int i = 0; i < animToStore.size(); i++)//loop through and store each Frame contained in the animation
			{
				storeFrame(*animToStore.frameList.at(i));
			}
			fileContents+="</ANIM>";
		}

		void SaveFile::storeFrame(Frame frameToStore)
		{
			fileContents+="<FRAME>";
			storeElement(LOC, frameToStore.f_loc);
			storeElement(DIM, frameToStore.f_dim);
			storeElement(ANC, frameToStore.f_anchor);
			storeBoxLists(frameToStore);
			fileContents+="</FRAME>";
			
		}

		void SaveFile::storeBox(HitCircle hitboxToStore, BOX_TYPE type)
		{
			//A switch to decide the opening-tag
			switch(type)
			{
			case HIT:
				fileContents+="<HITBOX>";
				break;

			case HURT:
				fileContents+="<HURTBOX>";
				break;

			default:
				break;
			}

			//Store the HBinfo 
			//Start with the name
			fileContents+="<HB_NAME>";
			fileContents+=hitboxToStore.name;
			fileContents+="</HB_NAME>";

			//next get the location stored
			storeElement(HB_LOC, hitboxToStore.loc);

			//then on to the radius
			fileContents+="<HB_RADIUS>";
			fileContents+=to_string(static_cast<long long>(hitboxToStore.radius));
			fileContents+="</HB_RADIUS>";

			switch(type)
			{
			case HIT:
				fileContents+="</HITBOX>";
				break;

			case HURT:
				fileContents+="</HURTBOX>";
				break;

			default:
				break;
			}

			

		}

		void SaveFile::storeBoxLists(Frame frame)
		{
			//store the hitbox list
			fileContents+="<HITBOX_LIST>";
			if (!frame.hitBoxes.empty())
			{
				for(int i = 0; i<frame.hitBoxes.size(); i++)
				{
					storeBox(frame.hitBoxes.at(i), HIT);
				}
			}
			fileContents+="</HITBOX_LIST>";

			//store the hitbox list
			fileContents+="<HURTBOX_LIST>";
			if (!frame.hurtBoxes.empty())
			{
				for(int i = 0; i<frame.hurtBoxes.size(); i++)
				{
					storeBox(frame.hurtBoxes.at(i), HURT);
				}
			}
			fileContents+="</HURTBOX_LIST>";
			
		}

		void SaveFile::storeElement(ELEM_TYPE type, point elemToStore)
		{
			//a switch dependant on the 'type' enum
			switch (type)
			{
			case LOC :
				fileContents+="<LOC>";
				storePoint(elemToStore);
				fileContents+="</LOC>";
				break;

			case DIM:
				fileContents+="<DIM>";
				storePoint(elemToStore);
				fileContents+="</DIM>";
				break;

			case ANC:
				fileContents+="<ANC>";
				storePoint(elemToStore);
				fileContents+="</ANC>";
				break;

			case HB_LOC:
				fileContents+="<HB_LOC>";
				storePoint(elemToStore);
				fileContents+="</HB_LOC>";
				break;
			}
		}

		void SaveFile::storePoint(point pointToStore)
		{
			fileContents += to_string(static_cast<long double>(pointToStore.x));
			fileContents += ",";
			fileContents += to_string(static_cast<long double>(pointToStore.y));
		}
		
		void SaveFile::saveToFile(string pathIncludingExt)
		{
			string fileName = pathIncludingExt;  //create the filename
			ofstream SAVESTREAM; //create an output file stream to bind the program to the file we'll be creating/editing
			SAVESTREAM.open(fileName.c_str(), ios::out|ios::trunc); //open the file if it exists and bind with it. If it doesn't exist, it should be created.
			
			if(SAVESTREAM.is_open()&&SAVESTREAM.good())//if the file is open & has given us no errors
			{
				SAVESTREAM << fileContents.c_str();
			}

			SAVESTREAM.close();
			 
		}

	public:
		

		SaveFile(Animation animToStore, SpriteySettings settingsToStore, string pathIncludingExt)
		{
			storeHeader(settingsToStore, animToStore.frameList.size());
			storeAnimation(animToStore);//store the animation in the fileContents
			///////////////////////
			//TEST
			//saveToFile("testfile.spr");
			///////////////////////
			saveToFile(pathIncludingExt);
			printf("\n");
			printf(fileContents.c_str());
		}
		
	};

	class LoadFile
	{
	private:
		string fileContents;//the whole contents of the file as a string
		string headerContents;//the contents of the header as a string
		string animationContents;//the contents of the animation part of the file as a string

		enum ELEM_TYPE
		{
			LOC,DIM,ANC, HB_LOC
		};

		void LoadFile::loadFileContent(string filePath)
		{
			ifstream LOADSTREAM;//here's an input stream to link/interface with the file we have selected and validated
			LOADSTREAM.open(filePath.c_str(), ios::in);

			if(LOADSTREAM.good()&&LOADSTREAM.is_open())
			{
				//OLD METHOD
				//LOADSTREAM>>fileContents;//rip the contents of the file as a whole into the string fileContents
				///////////////////////////

				//New buffer method
				stringstream buffer;
				buffer << LOADSTREAM.rdbuf();
				fileContents = string(buffer.str());
			}

			LOADSTREAM.close();
		}

		bool LoadFile::fileExtIsValid(string filePath)
		{
			//generate a substring containing the last 4 characters of the string "filepath".
			size_t posOfDot = filePath.find_last_of(".");//the position of the last '.' which marks the beginning of our file extension.
			string extension = filePath.substr(posOfDot);//a new string starting from the position of our last '.'
			//which implicitly containts our extension
			
			//string.compare() doesnt return a bool as you'd think it would
			//but an int. 0 means the strings were equal. anything else and they aren't exactly the same
			
			//So now we compare the extension to ".spr", if the file extension is invalid, we throw
			//a HENCH exception in the user's face. Did I say hench? I meant graceful.
			if(extension.compare(".spr")!=0)
			{
				throw invalExtException();
				return false;//I know this does absolutely nothing, but just for my own OCD
			}else{
				//if it does == 0, then our file extension is valid, and we're dealing with a .spr file
				return true;
			}

		}

		point LoadFile::ripPoint(string stringContainingElemNoTags)
		{
			size_t posOfComma = stringContainingElemNoTags.find(",");//the position of the comma that divides the x and y values
			string xAsString = stringContainingElemNoTags.substr(0, posOfComma);//a string cut out from the passed string value, 
			//starting from the beginning, going upto but not including the comma. The X value as a string
			string yAsString = stringContainingElemNoTags.substr(posOfComma+1);//a string cut out from the passed string, beginning
			//from the character after the comma, to the end of the string.
			
			double xAsFloat = atof(xAsString.c_str());//convert it to a double
			double yAsFloat = atof(yAsString.c_str());//convert it to a double

			//create a point containing these two values
			point pointMadeFromString = point(xAsFloat,yAsFloat);

			//TEST///////////////////
			cout<<"\nx = "<< pointMadeFromString.x<<"\ny = "<< pointMadeFromString.y;
			/////////////////////////
			return pointMadeFromString;
		}

		HitCircle LoadFile::ripHitBox(string stringContainingHitbox)
		{
			HitCircle rippedHitBox;
			//first rip the name
			rippedHitBox.name = ripString(stringContainingHitbox,"<HB_NAME>","</HB_NAME>");

			//then rip the location
			rippedHitBox.loc = point(ripElement(HB_LOC, stringContainingHitbox));

			//then rip the radius, by ripping the string for between the tags, then converting to an int
			string rippedRadiusString = ripString(stringContainingHitbox,"<HB_RADIUS>","</HB_RADIUS>");
			if(rippedRadiusString.length()==0){throw corruptAnimDataException();}
			int rippedRadius = atoi(rippedRadiusString.c_str());
			rippedHitBox.radius = rippedRadius;

			return rippedHitBox;
		}

		vector<HitCircle> LoadFile::ripBoxList(string stringContainingBoxesNoTags, SaveFile::BOX_TYPE type)
		{
			//temporary Vector(returned)
			vector<HitCircle> tempVector;
			
			//these change depending on whether we're doing hitlist or hurtlist
			string openTag;
			string closeTag;
			//so we switch to determine them
			switch (type)
			{
			case SaveFile::BOX_TYPE::HIT:
				openTag = "<HITBOX>";
				closeTag = "</HITBOX>";
				break;
			case SaveFile::BOX_TYPE::HURT:
				openTag = "<HURTBOX>";
				closeTag = "</HURTBOX>";
				break;
			default:
				break;
			}

			for(;stringContainingBoxesNoTags.length()>0;)
			{
				size_t posHBTag = stringContainingBoxesNoTags.find(openTag.c_str());//the position of the first opentag
				size_t posHBCTag = stringContainingBoxesNoTags.find(closeTag.c_str());//same for the closetag
				size_t lengthOfBoxString = posHBCTag+closeTag.length();//work out the length to the BACK of the first set of tags

				string firstBoxAsString = stringContainingBoxesNoTags.substr(posHBTag, lengthOfBoxString);
				tempVector.push_back(ripHitBox(firstBoxAsString));//push it to the back of our temp box list


				if(lengthOfBoxString>=stringContainingBoxesNoTags.length())//if this was the last frame. See the ripAnimation func for a more detailed comment on how this works.
				{
					stringContainingBoxesNoTags = "";//this will make the length 0, effectively breaking the loop
				}
				else
				{
					stringContainingBoxesNoTags = stringContainingBoxesNoTags.substr(lengthOfBoxString);
					//if not we chop that first frame off of the string and start again
				}
				
			}

			return tempVector;
		}

		point LoadFile::ripElement(ELEM_TYPE type, string stringContainingElement)
		{
			string openTag;
			string closeTag;
			string stringContainingElemNoTags;//the string with the decided tags removed;
			point elementRipped;
			
			switch(type)
			{
				case LOC:
					openTag = "<LOC>";
					closeTag = "</LOC>";
					break;
				case DIM:
					openTag = "<DIM>";
					closeTag = "</DIM>";
					break;
				case ANC:
					openTag = "<ANC>";
					closeTag = "</ANC>";
					break;
				case HB_LOC:
					openTag = "<HB_LOC>";
					closeTag = "</HB_LOC>";
				default:
					break;
			}

			size_t posOfOpenTag = stringContainingElement.find(openTag.c_str());//the first (and hopefully only) character of the selected open tag
			size_t posOfCloseTag = stringContainingElement.find(closeTag.c_str());//the pos of the first and only occurance of the close tag
			size_t sizeOfContent = posOfCloseTag-(posOfOpenTag+openTag.length());//the length of the string betwen the decided tags
			
			stringContainingElemNoTags = stringContainingElement.substr(posOfOpenTag+openTag.length(),sizeOfContent);//rip the substring
			//TEST////////////
			cout<<"\n"<<openTag.c_str();
			///////////////////
			
			elementRipped = point(ripPoint(stringContainingElemNoTags));//pass this string into the ripPoint function and return the result
			
			//TEST/////////////
			cout<<"\n"<<closeTag.c_str();
			///////////////////
			return elementRipped;

		}

		Frame LoadFile::ripFrame(string stringContainingFrame)
		{
			point rippedLoc;
			point rippedDim;
			point rippedAnc;
			vector<HitCircle> hitList;
			vector<HitCircle> hurtList;
			
			//TEST/////////////////
			//cout<<"\n<FRAME>";
			///////////////////////

			rippedLoc = point(ripElement(LOC, stringContainingFrame));//rip the elements out of the fram into the points created
			rippedDim = point(ripElement(DIM, stringContainingFrame));
			rippedAnc = point(ripElement(ANC, stringContainingFrame));

			//now we'll rip the strings containing our hitboxes, and our hurtboxes
			string hitBoxListAsStringNoTags = ripString(stringContainingFrame,"<HITBOX_LIST>","</HITBOX_LIST>");
			string hurtBoxListAsStringNoTags = ripString(stringContainingFrame,"<HURTBOX_LIST>","</HURTBOX_LIST>");
			
			//rip our hit and hurtbox lists out of these strings
			hitList = ripBoxList(hitBoxListAsStringNoTags, SaveFile::BOX_TYPE::HIT);
			hurtList = ripBoxList(hurtBoxListAsStringNoTags, SaveFile::BOX_TYPE::HURT);
			//TEST////////////////
			//cout<<"\n</FRAME>";
			/////////////////////

			Frame frameRippedFromStr = Frame(rippedLoc,rippedDim,rippedAnc);
			
			//copy those new hit and hurt vectors into the frame
			frameRippedFromStr.hitBoxes = hitList;
			frameRippedFromStr.hurtBoxes = hurtList;

			return frameRippedFromStr;
		}

		Animation LoadFile::ripAnimation(string stringContainingAnim)
		{
			Animation temporaryAnim;
			if(stringContainingAnim.find("<ANIM>")==string::npos || stringContainingAnim.find("</ANIM>")==string::npos || stringContainingAnim.length()<=0)
			{
				cout<<"\nERROR: Animation is corrupt, invalid or empty.\nReturning empty animation.";
				temporaryAnim = Animation();
				return temporaryAnim;
			}else{
				temporaryAnim = Animation();//an animation to push our frames into

				string stringContainingAnimNoTags;//the passed string with the <ANIM> tags removed

				size_t posAnimTag = stringContainingAnim.find("<ANIM>");
				size_t posAnimCTag = stringContainingAnim.find("</ANIM>");

				stringContainingAnimNoTags = stringContainingAnim.substr(posAnimTag+6, posAnimCTag-(posAnimTag+6));//The string between the <ANIM> tags

				//TEST/////////////
				cout<<"\n<ANIM>";
				//////////////////

				for(;stringContainingAnimNoTags.length()>0;)//loop through the <ANIM> string, chop off the first set of <FRAME>content</FRAME>,
				{//push it back into our anim, then remove that first <FRAME> from the string, repeat until the string is empty

					string openTag = "<FRAME>";
					string closetag = "</FRAME>";
					size_t posFrameTag = stringContainingAnimNoTags.find(openTag.c_str());//position of the first <FRAME> tag (SHOULD be 0, but let's make sure)
					size_t posFrameCTag = stringContainingAnimNoTags.find(closetag.c_str());//same as above for the close tag.
					size_t lengthOfFrameString = posFrameCTag+closetag.length();//the length of the string containing this frame
					//ALSO the same as the position of the first character of the next frame (if it's not the last frame). Can be reused for that.

					string firstFrameAsString = stringContainingAnimNoTags.substr(posFrameTag, lengthOfFrameString);//pass this string into the ripFrame.

					temporaryAnim.frameList.push_back(new Frame(ripFrame(firstFrameAsString)));//push back a frame returned by the function that rips a frame from the string

					//CHOP OFF THE ANIM TAGS FIRST, THIS IS IMPORTANT
					if(lengthOfFrameString>=stringContainingAnimNoTags.length())//if our anim string is equal to or shorter than the length of the frame string, that means that this is the last frame and we should break out of the loop.
					{
						stringContainingAnimNoTags = "";//empty the string. this will break the loop.
					}else
					{
						stringContainingAnimNoTags = stringContainingAnimNoTags.substr(lengthOfFrameString);//if not, chop the first <FRAME>content</FRAME>
					}
				}

				//TEST////////////
				cout<<"\n</ANIM>";
				//////////////////

				return temporaryAnim;
			}
		}

		string LoadFile::ripString(string stringContainingElement, string openTag, string closeTag)
		{
			size_t openStart = stringContainingElement.find(openTag);
			size_t stringStart = openStart + openTag.length();//the string we want starts here
			size_t endStart = stringContainingElement.find(closeTag);
			size_t stringLength = endStart - stringStart;

			string result = stringContainingElement.substr(stringStart, stringLength);

			return result;
		
		}

		SpriteySettings LoadFile::ripHeader(string stringContainingHeader)
		{
			SpriteySettings temporarySettings = SpriteySettings();
			//////////////////////////////
			//RIP DATA
			//////////////////////////////

			//rip the anim name, throw an exception if it's corrupt
			string animationName = string(ripString(stringContainingHeader,"<ANIM-NAME>","</ANIM-NAME>"));
			if(animationName.length()==0){throw corruptHeaderException();}
			temporarySettings.setAnimationName(animationName);

			//rip the author name, throw an exception if it's corrupt
			string authorName = string(ripString(stringContainingHeader,"<AUTHOR>","</AUTHOR>"));
			//if(authorName.length()==0){throw corruptHeaderException();}
			temporarySettings.setAuthor(authorName);

			//rip the currently known spritesheet file location, throw an exception if it's corrupt
			string ssFileName = string(ripString(stringContainingHeader, "<SPRITESHEET>","</SPRITESHEET>"));
			if(ssFileName.length()==0){throw corruptHeaderException();}
			temporarySettings.setSSFileName(ssFileName);

			//ripping the number of frames requires a conversion
			string numFramesString = string(ripString(stringContainingHeader, "<NUMFRAMES>", "</NUMFRAMES>"));
			if(numFramesString.length()==0){throw corruptHeaderException();}
			int numFrames = atoi(numFramesString.c_str());
			temporarySettings.setNumFrames(numFrames);

			/////////////////////////////
			//RIP CONFIG
			/////////////////////////////
			string ancDevBoolAsString = string(ripString(stringContainingHeader, "<ANC-DEV>", "</ANC-DEV>"));
			if(ancDevBoolAsString.length()!=1){throw corruptHeaderException();}
			int compareT = ancDevBoolAsString.compare("t"); 
			int compareF = ancDevBoolAsString.compare("f"); 

			if(compareT == 0)
			{
				temporarySettings.setAnchorDevAllowed(true);
			}
			//an else statement here causes terrible undefined behaviour
			if(compareF == 0)
			{
				temporarySettings.setAnchorDevAllowed(false);
			}
			//so we'll check both and make sure it's not corrupt that way.
			if(compareT!=0 && compareF!=0)
			{
				throw corruptHeaderException();
			}
			
			/////////////////////////////////

			return temporarySettings;
		}

		void LoadFile::splitFileComponents(string fileAsString)
		{
			//Throw empty file exception if the string we're passed is empty
			if(fileAsString.length()<=0)
			{
				throw emptyFileException();
			}


			string headerTag = "<HEADER>";
			string headerEndTag = "</HEADER>";
			size_t headerStart = fileAsString.find(headerTag);
			size_t headerLength = (fileAsString.find(headerEndTag) + headerEndTag.length()) - headerStart;
			//Throw corrupt header exception if either of the above is npos
			if(headerStart == string::npos || fileAsString.find(headerEndTag) == string::npos)
			{
				throw corruptHeaderException();
			}
			/////////////////////////////////////////////
			headerContents = fileAsString.substr(headerStart, headerLength);

			string animTag = "<ANIM>";
			string animEndTag = "</ANIM>";
			size_t animStart = fileAsString.find(animTag);
			size_t animLength = (fileAsString.find(animEndTag) + animEndTag.length()) - animStart;
			//Throw corrupt file exception if either of the above is npos
			if(animStart == string::npos || fileAsString.find(animEndTag) == string::npos)
			{
				throw corruptAnimDataException();
			}
			animationContents = fileAsString.substr(animStart,animLength);
		}

	public:

		Animation loadedAnimation;
		SpriteySettings loadedSettings;

		LoadFile(string filePath)
		{
			//first thing we do is check that we are indeed dealing with a .spr file
			if(fileExtIsValid(filePath))//throws an invalExtException if not valid, which we handle elsewhere
			{
				cout<<"\nFile: "<<filePath.c_str()<<"\nFile Valid";
			}

			//then we load the contents of the file into string fileContents
			loadFileContent(filePath);

			//then we split the fileContent string into <HEADER>and<ANIM>
			splitFileComponents(fileContents);//Also throws any exceptions that result from a corrupt file.

			//then we rip in the header data
			//then we initialise loaded animation using the ripAnimation function. 
			loadedAnimation = Animation(ripAnimation(animationContents));
			loadedSettings = SpriteySettings(ripHeader(headerContents));

			//TEST/////
			//string testFrame = "<ANIM><FRAME><LOC>89,-167.5</LOC><ANC>555,898</ANC><DIM>100,100</DIM></FRAME><FRAME><LOC>89,-167.5</LOC><ANC>555,898</ANC><DIM>100,100</DIM></FRAME><FRAME><LOC>89,-167.5</LOC><ANC>555,898</ANC><DIM>100,100</DIM></FRAME></ANIM>";
			//loadedAnimation = Animation(ripAnimation(testFrame));
			///////////

			

		}
	};

	

}