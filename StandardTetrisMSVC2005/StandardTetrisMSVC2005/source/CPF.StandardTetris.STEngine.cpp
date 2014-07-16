


#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1


#include "CPF.StandardTetris.STEngine.h"



#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>

#include "CPF.StandardTetris.STOpenGLFont.h"
#include "CPF.StandardTetris.STGameDrawing.h"
#include "CPF.StandardTetris.STGame.h"
#include "CPF.StandardTetris.STTime.h"
#include "CPF.StandardTetris.STVideoProcessing.h"
#include "CPF.StandardTetris.STRS232.h"
#include "CPF.StandardTetris.STStrategyManager.h"
#include "CPF.StandardTetris.STConsole.h"
#include "CPF.StandardTetris.STFileList.h"





namespace CPF
{
	namespace StandardTetris
	{



		char                            STEngine::mApplicationWindowClassNameANSI[ 256 ];
		char                            STEngine::mApplicationWindowTitleANSI[ 256 ];
		char                            STEngine::mApplicationFriendlyNameANSI[ 256 ];

		wchar_t                         STEngine::mApplicationWindowClassNameWCHAR[ 256 ];
		wchar_t                         STEngine::mApplicationWindowTitleWCHAR[ 256 ];
		wchar_t                         STEngine::mApplicationFriendlyNameWCHAR[ 256 ];

		char                            STEngine::mApplicationPathAndNameANSI[ _MAX_PATH ];
		wchar_t                         STEngine::mApplicationPathAndNameWCHAR[ _MAX_PATH ];

		char                            STEngine::mApplicationPathOnlyANSI[ _MAX_PATH ];
		wchar_t                         STEngine::mApplicationPathOnlyWCHAR[ _MAX_PATH ];

		int                             STEngine::mClient_Width;
		int                             STEngine::mClient_Height;

		HWND                            STEngine::m_HWND;
		HINSTANCE                       STEngine::m_HINSTANCE;
		HDC                             STEngine::m_HDC;
		HGLRC                           STEngine::m_HGLRC;

		STGame                          STEngine::mSTGame;
		STVideoProcessing               STEngine::mSTVideoProcessing;
		STFileList                      STEngine::mSTFileList;
		STConsole                       STEngine::mSTConsole;





		STEngine::STEngine()
		{
		}




		STEngine::~STEngine()
		{
		}




		int  STEngine::GeneralInitialization( )
		{
			STStrategyManager::Initialize();

			// Clear entire buffers for the application strings, so we are certain to terminate strings.
			memset( (void *)(STEngine::mApplicationWindowClassNameANSI), 0, sizeof(STEngine::mApplicationWindowClassNameANSI) );
			memset( (void *)(STEngine::mApplicationWindowTitleANSI), 0, sizeof(STEngine::mApplicationWindowTitleANSI) );
			memset( (void *)(STEngine::mApplicationFriendlyNameANSI), 0, sizeof(STEngine::mApplicationFriendlyNameANSI) );

			memset( (void *)(STEngine::mApplicationWindowClassNameWCHAR), 0, sizeof(STEngine::mApplicationWindowClassNameWCHAR) );
			memset( (void *)(STEngine::mApplicationWindowTitleWCHAR), 0, sizeof(STEngine::mApplicationWindowTitleWCHAR) );
			memset( (void *)(STEngine::mApplicationFriendlyNameWCHAR), 0, sizeof(STEngine::mApplicationFriendlyNameWCHAR) );


			// Source strings for building ANSI and WCHAR_T strings			
			char * applicationWindowClassNameANSI   = "StandardTetris";
			char * applicationWindowTitleANSI       = "Standard Tetris (2014 0715) mod by Misakamm";
			char * applicationFriendlyNameANSI      = "Standard Tetris (2014 0715) mod by Misakamm";


			// For ANSI, we simply copy from the zero-terminated source strings
			strcpy( (char *)(STEngine::mApplicationWindowClassNameANSI), (const char *)(applicationWindowClassNameANSI) );
			strcpy( (char *)(STEngine::mApplicationWindowTitleANSI), (const char *)(applicationWindowTitleANSI) );
			strcpy( (char *)(STEngine::mApplicationFriendlyNameANSI), (const char *)(applicationFriendlyNameANSI) );


			// For WCHAR_T, we need to convert ANSI-to-WCHAR using MultiByteToWideChar()
			MultiByteToWideChar
				( 
				CP_ACP, // ACP == ANSI CODE PAGE
				MB_PRECOMPOSED, // character-type options
				(LPCSTR)applicationWindowClassNameANSI, // source buffer 
				(int)strlen(applicationWindowClassNameANSI),  // source length in bytes
				(LPWSTR)(STEngine::mApplicationWindowClassNameWCHAR), // wide-char destination buffer
				((int)sizeof(STEngine::mApplicationWindowClassNameWCHAR) / 2) // HACK: divide by 2 to get size destination buffer IN WIDE CHARS
				);

			MultiByteToWideChar
				( 
				CP_ACP, // ACP == ANSI CODE PAGE
				MB_PRECOMPOSED, // character-type options
				(LPCSTR)applicationWindowTitleANSI, // source buffer 
				(int)strlen(applicationWindowTitleANSI),  // source length in bytes
				(LPWSTR)(STEngine::mApplicationWindowTitleWCHAR), // wide-char destination buffer
				((int)sizeof(STEngine::mApplicationWindowTitleWCHAR) / 2) // HACK: divide by 2 to get size destination buffer IN WIDE CHARS
				);

			MultiByteToWideChar
				( 
				CP_ACP, // ACP == ANSI CODE PAGE
				MB_PRECOMPOSED, // character-type options
				(LPCSTR)applicationFriendlyNameANSI, // source buffer 
				(int)strlen(applicationFriendlyNameANSI),  // source length in bytes
				(LPWSTR)(STEngine::mApplicationFriendlyNameWCHAR), // wide-char destination buffer
				((int)sizeof(STEngine::mApplicationFriendlyNameWCHAR) / 2) // HACK: divide by 2 to get size destination buffer IN WIDE CHARS
				);




			// Get the executable path.
			// Clear entire buffers for the application strings, so we are certain to terminate strings.
			memset( (void *)(STEngine::mApplicationPathAndNameANSI), 0, sizeof(STEngine::mApplicationPathAndNameANSI) );
			memset( (void *)(STEngine::mApplicationPathAndNameWCHAR), 0, sizeof(STEngine::mApplicationPathAndNameWCHAR) );

			TCHAR  szEXEPathname[ _MAX_PATH ];
			GetModuleFileName( NULL, szEXEPathname, _MAX_PATH );

			// LPTSTR is LPWSTR (16-bit chars) if UNICODE is defined, otherwise LPSTR (8-bit chars).

#ifdef _UNICODE
			// Copy directly to WCHAR buffer, but convert for ANSI
			wcscpy( (wchar_t *)(STEngine::mApplicationPathAndNameWCHAR), (const wchar_t *)(szEXEPathname) );

			WideCharToMultiByte
				( 
				CP_ACP, // ACP == ANSI CODE PAGE
				WC_NO_BEST_FIT_CHARS, // character-type options
				(LPCWSTR)szEXEPathname, // source buffer 
				(int)wcslen(szEXEPathname), // source length in characters
				(LPSTR)(STEngine::mApplicationPathAndNameANSI), // mult-byte destination buffer
				((int)sizeof(STEngine::mApplicationPathAndNameANSI)), // destination buffer size in bytes
				NULL,
				NULL
				);
#else
			// Copy directly to ANSI, but convert for WCHAR
			strcpy( (char *)(STEngine::mApplicationPathAndNameANSI), (const char *)(szEXEPathname) );

			MultiByteToWideChar
				( 
				CP_ACP, // ACP == ANSI CODE PAGE
				MB_PRECOMPOSED, // character-type options
				(LPCSTR)szEXEPathname, // source buffer 
				(int)strlen(szEXEPathname),  // source length in bytes
				(LPWSTR)(STEngine::mApplicationPathAndNameWCHAR), // wide-char destination buffer
				((int)sizeof(STEngine::mApplicationPathAndNameWCHAR) / 2) // HACK: divide by 2 to get size destination buffer IN WIDE CHARS
				);
#endif



			// Find executable path without name of executable itself      
			// Copy entire strings from path-and-name counterparts
			memcpy( (void *)(STEngine::mApplicationPathOnlyANSI), (const void *)(STEngine::mApplicationPathAndNameANSI), (int)sizeof(STEngine::mApplicationPathOnlyANSI) );
			memcpy( (void *)(STEngine::mApplicationPathOnlyWCHAR), (const void *)(STEngine::mApplicationPathAndNameWCHAR), (int)sizeof(STEngine::mApplicationPathOnlyWCHAR) );
			// Terminate each buffer at first '\' (backslash) character that we encounter
			// when we move backwards through the buffer.
			int n = 0;
			int i = 0;
			int k = 0;
			n = (int)(sizeof(STEngine::mApplicationPathOnlyANSI));
			for ( i = (n-1); ((i >= 0) && (0 == k)); i-- )
			{
				if ('\\' == STEngine::mApplicationPathOnlyANSI[i])
				{
					STEngine::mApplicationPathOnlyANSI[i] = (char)0;
					k = 1;
				}
			}

			n = (int)(sizeof(STEngine::mApplicationPathOnlyWCHAR)) / 2; // divide by 2 for WCHAR
			wchar_t wcharBackslash = L'\\';
			for ( i = (n-1); ((i >= 0) && (0 == k)); i-- )
			{
				if (wcharBackslash == STEngine::mApplicationPathOnlyWCHAR[i])
				{
					STEngine::mApplicationPathOnlyWCHAR[i] = (wchar_t)0;
					k = 1;
				}
			}



			mClient_Width          = (int)(640);
			mClient_Height         = (int)(480);

			m_HWND                      = ((HWND)(0));
			m_HINSTANCE                 = (HINSTANCE)(0);
			m_HDC                       = (HDC)(0);
			m_HGLRC                     = (HGLRC)(0);


			// Initialize random seed of game to current time.
			mSTGame.SeedPieceSequenceGeneratorWithCurrentTime();

			return(0);
		}



		int STEngine::GetClientDefaultX     ( )
		{ 
			return(  40 ); 
		}

		int STEngine::GetClientDefaultY     ( )
		{ 
			return(  50 ); 
		}

		int STEngine::GetClientDefaultWidth ( )
		{ 
			return( 640 ); 
		}

		int STEngine::GetClientDefaultHeight( )
		{ 
			return( 480 ); 
		}






		// Call with StandardTetrisTimeT::GetIntervalDurationSecondsFloat() in main()

		void STEngine::PerformGameIterations
			(
			float f32_DeltaTimeSeconds
			)
		{
			// First, update the game time.
			mSTGame.ConditionalAdvanceGameTimeByDelta( f32_DeltaTimeSeconds );


			int gameSpeed = 0;
			gameSpeed = mSTGame.GetGameSpeedAdjustment();

			if (gameSpeed <= 2)
			{
				// Perform any unforced game iteration.
				mSTGame.ConditionalAdvanceGameIteration( 0 );
			}
			else
			{
				int totalIncrements = 8 * gameSpeed;
				int incrementCounter = 0;
				for ( incrementCounter = 0;
					incrementCounter < totalIncrements;
					incrementCounter++ )
				{
					mSTGame.ConditionalAdvanceGameIteration( 1 );
				}
			}
		}



		int STEngine::SetUpGraphics( )
		{
			static PIXELFORMATDESCRIPTOR pfd = 
			{
				sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
				1,                              // version number
				PFD_DRAW_TO_WINDOW              // support window
				|  PFD_SUPPORT_OPENGL           // support OpenGL
				|  PFD_DOUBLEBUFFER ,           // double buffered
				PFD_TYPE_RGBA,                  // RGBA type
				32,                             // 32-bit color depth
				0, 0, 0, 0, 0, 0,               // color bits ignored
				0,                              // no alpha buffer
				0,                              // shift bit ignored
				0,                              // no accumulation buffer
				0, 0, 0, 0,                     // accum bits ignored
				32,                             // 32-bit z-buffer      
				0,                              // no stencil buffer
				0,                              // no auxiliary buffer
				PFD_MAIN_PLANE,                 // main layer
				0,                              // reserved
				0, 0, 0                         // layer masks ignored
			};

			int    pixelFormat = 0;
			HGLRC  HGLRC_rv        = ((HGLRC)(0));

			pixelFormat = ChoosePixelFormat( m_HDC, &pfd );
			if (0 != pixelFormat) 
			{
				if (0 != SetPixelFormat( m_HDC, pixelFormat, &pfd )) 
				{
					HGLRC_rv = wglCreateContext( m_HDC );
					if ( ((HGLRC)(0)) != HGLRC_rv )
					{
						if (0 == wglMakeCurrent( m_HDC, HGLRC_rv ))
						{
							wglDeleteContext( HGLRC_rv );
							HGLRC_rv = 0;
						}
					}
				}
			}

			// Commit HGLRC to object.
			m_HGLRC = HGLRC_rv;

			if ( ((HGLRC)(0)) != m_HGLRC )
			{
				return(1); // Success
			}

			return(0); // Failed
		}



	}
}

