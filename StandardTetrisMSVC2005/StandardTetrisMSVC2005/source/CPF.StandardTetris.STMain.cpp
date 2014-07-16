// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#include "CPF.StandardTetris.STMain.h"



#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>

#include "CPF.StandardTetris.STOpenGLFont.h"
#include "CPF.StandardTetris.STGameDrawing.h"
#include "CPF.StandardTetris.STGame.h"
#include "CPF.StandardTetris.STTime.h"
#include "CPF.StandardTetris.STVideoProcessing.h"
#include "CPF.StandardTetris.STRS232.h"
#include "CPF.StandardTetris.STEngine.h"
#include "CPF.StandardTetris.STUserInterface.h"




#define IDR_ICON1                       101




// RECORD_EXCEPTION_INFO
int __cdecl record_exception_info( PEXCEPTION_POINTERS data, const char *message )
{ 
	CPF::StandardTetris::STEngine::GetVideoProcessing().Terminate();

	if (((HWND)(0)) != CPF::StandardTetris::STEngine::GetHWND())
	{
		DestroyWindow( CPF::StandardTetris::STEngine::GetHWND() );
	}

	PostQuitMessage( 0 );

	return( 0 ); // Make this 1 if we are in release mode
}





// Main entry point for this application
// (Note: This function must be outside any namespace, so that its symbol
// can be found during the linking phase; e.g., "_WinMain".)

int WINAPI 
WinMain
( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	int result = -1;

	__try
	{
		result = CPF::StandardTetris::STMain::HandledWinMain( hInstance, hPrevInstance, lpCmdLine, nCmdShow );
	}

	__except(record_exception_info(GetExceptionInformation(),"main thread"))
	{
	}

	return( result );
}










namespace CPF
{
	namespace StandardTetris
	{





		LRESULT CALLBACK 
			STMain::WndProc
			(
			HWND hwnd, 
			UINT iMsg, 
			WPARAM wParam, 
			LPARAM lParam
			)
		{

			switch(iMsg)
			{
			case WM_KEYDOWN:
				{
					STUserInterface::HandleKeyPress( (unsigned int)wParam, (unsigned int)lParam );
				}
				break;

			case WM_CLOSE:
				{
					// Disable the video stream callback
					STEngine::GetVideoProcessing().Terminate();
				}
				break;

			case WM_DESTROY:
				{
					PostQuitMessage( 0 );
				}
				break;
			}

			long val = 0;
			val = (long)DefWindowProc( hwnd, iMsg, wParam, lParam );
			return val;
		} 













		int WINAPI 
			STMain::HandledWinMain
			(
			HINSTANCE  HINSTANCE_This,
			HINSTANCE  hPrevInstance,
			LPSTR     szCmdLine,
			int        iCmdShow
			)
		{
			STTime::SetReferenceTimeToNow();

			STEngine::GeneralInitialization();  // Do this AFTER time has been initialized.



			int    relativeFrameCount = 0;

			MSG msg;

			STEngine::SetHINSTANCE( HINSTANCE_This );

			
			// Define a window class.
			
			WNDCLASSEX   WNDCLASSEX_Properties;
			::memset( (void *)(&(WNDCLASSEX_Properties)), 0, sizeof(WNDCLASSEX_Properties) );

			WNDCLASSEX_Properties.cbSize        = sizeof(WNDCLASSEX);
			WNDCLASSEX_Properties.style         = (CS_HREDRAW | CS_VREDRAW | CS_OWNDC);
			WNDCLASSEX_Properties.lpfnWndProc   = STMain::WndProc;
			WNDCLASSEX_Properties.cbClsExtra    = 0;
			WNDCLASSEX_Properties.cbWndExtra    = 0;
			WNDCLASSEX_Properties.hInstance     = HINSTANCE_This;
			WNDCLASSEX_Properties.hIcon         = LoadIcon( HINSTANCE_This, MAKEINTRESOURCE(IDR_ICON1) ); // Large icon
			WNDCLASSEX_Properties.hCursor       = LoadCursor (NULL, IDC_ARROW);
			WNDCLASSEX_Properties.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);

#ifdef _UNICODE
			WNDCLASSEX_Properties.lpszMenuName  = L"menu";
#else
			WNDCLASSEX_Properties.lpszMenuName  = "menu";
#endif

#ifdef _UNICODE
			WNDCLASSEX_Properties.lpszClassName = STEngine::GetApplicationWindowClassNameWCHAR();
#else
			WNDCLASSEX_Properties.lpszClassName = STEngine::GetApplicationWindowClassNameANSI();
#endif

			WNDCLASSEX_Properties.hIconSm       = NULL; // LoadIcon( NULL, MAKEINTRESOURCE(IDI_ICON1) ); // Large icon ////NULL; // Use small version of large icon
			


			
			// Register the window class
			
			ATOM   ATOM_Result;
			::memset( (void *)(&(ATOM_Result)), 0, sizeof(ATOM_Result) );

			ATOM_Result = 
				::RegisterClassEx( (&(WNDCLASSEX_Properties)) );

			if (((ATOM)(0)) == ATOM_Result)
			{
				// Failed to register the window class.
				::MessageBoxA( 0, "Failed to register the window class", STEngine::GetApplicationFriendlyNameANSI(), (MB_ICONERROR | MB_OK) );
				return( 0 );
			}
			





			
			// Compute initial window dimensions
			
			HWND  HWND_DesktopWindow = ::GetDesktopWindow();

			RECT  RECT_DesktopWindow;

			::GetWindowRect( HWND_DesktopWindow, (&(RECT_DesktopWindow)) );

			int targetWidth  = 640;
			int targetHeight = 480;

			int initialWindowWidth  = targetWidth;
			int initialWindowHeight = targetHeight;
			int initialWindowX      = 0;
			int initialWindowY      = 0;

			initialWindowWidth  = targetWidth;
			initialWindowHeight = targetHeight;
			initialWindowX      = (((RECT_DesktopWindow.right  - RECT_DesktopWindow.left) + 1) - initialWindowWidth ) / 2;
			initialWindowY      = (((RECT_DesktopWindow.bottom - RECT_DesktopWindow.top ) + 1) - initialWindowHeight) / 2;
			





			
			// Create a window
			
			HWND        HWND_Window = (HWND)0;

			int          fullScreen   = 0;
			unsigned int windowStyle = 0;

			fullScreen = 0;

			if (0 == fullScreen)
			{
				windowStyle = WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;
			}
			else
			{
				windowStyle = WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP;
				initialWindowX      = 0;
				initialWindowY      = 0;
				initialWindowWidth  = ((RECT_DesktopWindow.right  - RECT_DesktopWindow.left) + 1);
				initialWindowHeight = ((RECT_DesktopWindow.bottom - RECT_DesktopWindow.top ) + 1);
			}

#ifdef _UNICODE
			HWND_Window = 
				::CreateWindow
				(
				STEngine::GetApplicationWindowClassNameWCHAR(), // Name of window class
				STEngine::GetApplicationWindowTitleWCHAR(), // Title
				windowStyle,
				initialWindowX,
				initialWindowY,
				initialWindowWidth,
				initialWindowHeight,
				NULL,                     // No parent window
				NULL,                     // No menu
				STEngine::GetHINSTANCE(), // Instance handle
				NULL                      // No additional arguments
				);
#else
			HWND_Window = 
				::CreateWindow
				(
				STEngine::GetApplicationWindowClassNameANSI(), // Name of window class
				STEngine::GetApplicationWindowTitleANSI(), // Title
				windowStyle,
				initialWindowX,
				initialWindowY,
				initialWindowWidth,
				initialWindowHeight,
				NULL,                     // No parent window
				NULL,                     // No menu
				STEngine::GetHINSTANCE(), // Instance handle
				NULL                      // No additional arguments
				);
#endif

			if (((HWND)(0)) == HWND_Window)
			{
				// Failed to create a window.
				::MessageBoxA( 0, "Failed to create a window of specified class", STEngine::GetApplicationFriendlyNameANSI(), (MB_ICONERROR | MB_OK) );
				return( 0 );
			}
			




			STEngine::SetHWND( HWND_Window );

			STEngine::SetHDC( GetDC( HWND_Window ) );

			STEngine::SetUpGraphics();

			if (0 == STEngine::SetUpGraphics())
			{
				MessageBoxA(0, "Failed to Create OpenGL Rendering Context.", 
					STEngine::GetApplicationFriendlyNameANSI(), MB_ICONERROR );
				DestroyWindow( STEngine::GetHWND() );
			}


			glEnable    ( GL_DEPTH_TEST );
			glDepthFunc ( GL_LEQUAL     );
			glEnable    ( GL_TEXTURE_2D );

			ShowWindow( HWND_Window, iCmdShow );
			UpdateWindow( HWND_Window );



			int done = 0;
			int messageCount = 0;

			while( ! done )
			{
				int message = 0;

				message = PeekMessage( &msg, NULL, 0, 0, PM_REMOVE );

				if (message)
				{
					if (msg.message == WM_QUIT)
					{
						done = 1;
					}
				}

				if ((0 == message) || (messageCount > 32))
				{
					// Since we're drawing, reset message count
					messageCount = 0;
					try
					{
						STTime::NextIntervalBeginsNow();
						STEngine::PerformGameIterations( STTime::GetIntervalDurationSecondsFloat() );
						STGameDrawing::DrawScreen
							( 
							STEngine::GetVideoProcessing(),
							STEngine::GetGame(),
							STEngine::GetConsole()
							);
						relativeFrameCount++;
					}
					catch(...)
					{
					}
				}

				if (message)
				{
					messageCount++;
					try
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
					catch(...)
					{
					}
				}

				if (relativeFrameCount > 5)
				{
					double frameDurationSeconds = 0.0;
					frameDurationSeconds = (double) STTime::GetIntervalDurationSecondsFloat();

					double framesPerSecond = 0.0;
					if (frameDurationSeconds > 0.0001)
						framesPerSecond = (1.0 / frameDurationSeconds);
					if (framesPerSecond <   0.0) framesPerSecond =   0.0;
					if (framesPerSecond > 999.0) framesPerSecond = 999.0;
					STEngine::GetGame().SetReportedFrameRate( framesPerSecond );
					relativeFrameCount = 0;
				}

			} // while( ! done )

			return( (int)(msg.wParam) );
		}





	}
}





