#pragma once

#include <Windows.h>
#pragma managed(push, off)
#include <gl/glew.h> //this must be included before any instance of glut.h or gl.h. Not sure about glu.h
#include <GL/gl.h>
#include <GL/GLU.h>
#include <gl/glut.h>
#pragma managed(pop)
#include <tchar.h>
#include <string>

using namespace System::Windows::Forms;
using namespace System::ComponentModel;

namespace OpenGLForm
{


	public ref class COpenGL:
		public System::Windows::Forms::NativeWindow
		{
		public:
			COpenGL(System::Windows::Forms::Form ^ parentForm, GLsizei iWidth, GLsizei iHeight)
			{
				CreateParams^ cp = gcnew CreateParams;

				

				//set it's position on the parent form
				cp->X = 12;
				cp->Y = 27;
				cp->Height = iHeight;
				cp->Width = iWidth;

				//set the height and width for the benefit of the derived classes,
				//so that they can find out their own size using the getHeight & width funcs
				currentWidth = cp->Width;
				currentHeight = cp->Height;

				//Specify the form as the parent
				cp->Parent = parentForm->Handle;

				//create as a child of the specified parent
				//and make OPENGL compliant
				cp->Style = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN; //This originally included 
				//WS_CLIPSIBLINGS, which caused the phantom panel to be drawn over the top.
				//We are only using the panel to capture mouse input ;) 

				//create the window
				this->CreateHandle(cp);

				m_hDC = GetDC((HWND)this->Handle.ToPointer());

				if(m_hDC)
				{
					wglMakeCurrent(m_hDC,NULL);
					MySetPixelFormat(m_hDC);
					ReSizeGLScene(iWidth,iHeight);
					InitGL();
					printf("\nOpenGL Version: %s\n", glGetString(GL_VERSION));

				}


			}

			virtual void Render()
			{
				//simple render, just refresh the buffers for now
				glClearColor(1.0f,0.5f,0.5f,1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
			}

			void SwapOpenGLBuffers()
			{
				SwapBuffers(m_hDC);
			}

		protected:
			HDC m_hDC;
			HGLRC m_hglrc;

			GLfloat getCurrentWidth()
			{
				if(currentWidth)
				{
					return currentWidth;
				}
				else return 0;
			}

			GLfloat getCurrentHeight()
			{
				if(currentHeight)
				{
					return currentHeight;
				}
				else return 0;
			}

			~COpenGL()
			{
				this->DestroyHandle();
			}

			GLint MySetPixelFormat(HDC hdc)
			{
				PIXELFORMATDESCRIPTOR pfd = { 
					sizeof(PIXELFORMATDESCRIPTOR),    // size of this pfd 
					1,                                // version number 
					PFD_DRAW_TO_WINDOW |              // support window 
					PFD_SUPPORT_OPENGL |              // support OpenGL 
					PFD_DOUBLEBUFFER,                 // double buffered 
					PFD_TYPE_RGBA,                    // RGBA type 
					24,                               // 24-bit color depth 
					0, 0, 0, 0, 0, 0,                 // color bits ignored 
					0,                                // no alpha buffer 
					0,                                // shift bit ignored 
					0,                                // no accumulation buffer 
					0, 0, 0, 0,                       // accum bits ignored 
					32,                               // 32-bit z-buffer     
					0,                                // no stencil buffer 
					0,                                // no auxiliary buffer 
					PFD_MAIN_PLANE,                   // main layer 
					0,                                // reserved 
					0, 0, 0                           // layer masks ignored 
				}; 

				GLint  iPixelFormat; 

				// get the device context's best, available pixel format match 
				if((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
				{
					MessageBox::Show("ChoosePixelFormat Failed");
					return 0;
				}

				// make that match the device context's current pixel format 
				if(SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
				{
					MessageBox::Show("SetPixelFormat Failed");
					return 0;
				}

				if((m_hglrc = wglCreateContext(m_hDC)) == NULL)
				{
					MessageBox::Show("wglCreateContext Failed");
					//
					//wchar_t lpBuffer[256];
					//
					//	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,               // It´s a system error
					//	nullptr,                                      // No string to be formatted needed
					//	GetLastError(),                               // Hey Windows: Please explain this error!
					//	MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),  // Do it in the standard language
					//	(LPTSTR)&lpBuffer,								   // Put the message here
					//	0,                     // Number of bytes to store the message
					//	nullptr);
					//
					//MessageBox::Show( nullptr, gcnew System::String(lpBuffer), "LastRrror",MessageBoxButtons::OK);
					
					//if(lpBuffer)LocalFree(lpBuffer);


					return 0;
				}

				if((wglMakeCurrent(m_hDC, m_hglrc)) == NULL)
				{
					MessageBox::Show("wglMakeCurrent Failed");
					return 0;
				}

				return 1;
			}

			
			
			bool InitGL(GLvoid)										// All setup for opengl goes here
			{
					
				glMatrixMode(GL_PROJECTION);	
				glLoadIdentity();
				gluOrtho2D(0,currentWidth,0,currentHeight); //these will be set be now so it's safe to use them
															//also makes sure we're drawing from the bottom left
				//glShadeModel(GL_SMOOTH);							// Enable smooth shading
				glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black background
				//glClearDepth(1.0f);									// Depth buffer setup
				glDepthFunc(GL_LEQUAL);								// The type of depth testing to do
				glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really nice perspective calculations
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//activate the alpha blending functionality
				glLineWidth(2);			// Width of the drawing line
				glDisable(GL_DEPTH_TEST);
				glMatrixMode(GL_MODELVIEW);

				return TRUE;										// Initialisation went ok
			}

			GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize and initialise the gl window
			{
				

				if (height==0)										// Prevent A Divide By Zero By
				{
					height=1;										// Making Height Equal One
				}

				glViewport(0,0,width,height);						// Reset The Current Viewport

												
				
				//change the width and height values for the benefit of derived classes.
				currentWidth = width;
				currentHeight = height;

				glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
				glLoadIdentity();									// Reset The Projection Matrix
				gluOrtho2D(0,currentWidth,0,currentHeight);
				glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
				glLoadIdentity();									// Reset The Modelview Matrix
			}

			
			
			
		private:
			GLfloat currentWidth;
			GLfloat currentHeight;
				
				

};



}

