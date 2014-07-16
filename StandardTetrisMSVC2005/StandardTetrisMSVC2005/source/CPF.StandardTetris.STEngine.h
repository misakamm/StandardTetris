

#ifndef STEngineHeader
#define STEngineHeader



#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>



#include "CPF.StandardTetris.STGame.h"
#include "CPF.StandardTetris.STFileList.h"
#include "CPF.StandardTetris.STVideoProcessing.h"
#include "CPF.StandardTetris.STConsole.h"



namespace CPF
{
	namespace StandardTetris
	{



		class STEngine
		{

			// WINDOW-RELATED

		private: static char                           mApplicationWindowClassNameANSI[ 256 ];
		private: static char                           mApplicationWindowTitleANSI[ 256 ];
		private: static char                           mApplicationFriendlyNameANSI[ 256 ];

		private: static wchar_t                        mApplicationWindowClassNameWCHAR[ 256 ];
		private: static wchar_t                        mApplicationWindowTitleWCHAR[ 256 ];
		private: static wchar_t                        mApplicationFriendlyNameWCHAR[ 256 ];

		private: static char                           mApplicationPathAndNameANSI[ _MAX_PATH ];
		private: static wchar_t                        mApplicationPathAndNameWCHAR[ _MAX_PATH ];

		private: static char                           mApplicationPathOnlyANSI[ _MAX_PATH ];
		private: static wchar_t                        mApplicationPathOnlyWCHAR[ _MAX_PATH ];

		private: static int                            mClient_Width;
		private: static int                            mClient_Height;

		private: static HWND                           m_HWND;
		private: static HINSTANCE                      m_HINSTANCE;
		private: static HDC                            m_HDC;
		private: static HGLRC                          m_HGLRC;


		private: static STGame                         mSTGame;
		private: static STVideoProcessing              mSTVideoProcessing;
		private: static STFileList                     mSTFileList;
		private: static STConsole                      mSTConsole;



		public: STEngine();


		public: ~STEngine();


		public: static int GeneralInitialization();



		public: static char * GetApplicationWindowClassNameANSI()
				{
					return( (char *)(mApplicationWindowClassNameANSI) );
				}

		public: static char * GetApplicationWindowTitleANSI()
				{
					return( (char *)(mApplicationWindowTitleANSI) );
				}

		public: static char * GetApplicationFriendlyNameANSI()
				{
					return( (char *)(mApplicationFriendlyNameANSI) );
				}


		public: static wchar_t * GetApplicationWindowClassNameWCHAR()
				{
					return( (wchar_t *)(mApplicationWindowClassNameWCHAR) );
				}

		public: static wchar_t * GetApplicationWindowTitleWCHAR()
				{
					return( (wchar_t *)(mApplicationWindowTitleWCHAR) );
				}

		public: static wchar_t * GetApplicationFriendlyNameWCHAR()
				{
					return( (wchar_t *)(mApplicationFriendlyNameWCHAR) );
				}




		public: static char * GetApplicationPathAndNameANSI()
				{
					return( (char *)(mApplicationPathAndNameANSI) );
				}


		public: static wchar_t * GetApplicationPathAndNameWCHAR()
				{
					return( (wchar_t *)(mApplicationPathAndNameWCHAR) );
				}





		public: static char * GetApplicationPathOnlyANSI()
				{
					return( (char *)(mApplicationPathOnlyANSI) );
				}


		public: static wchar_t * GetApplicationPathOnlyWCHAR()
				{
					return( (wchar_t *)(mApplicationPathOnlyWCHAR) );
				}






		public: static int SetUpGraphics();




		public: static void PerformGameIterations
					(
					float f32_DeltaTimeSeconds
					);





		public: static STGame &  GetGame()
				{
					return( mSTGame );
				}


		public: static STVideoProcessing &  GetVideoProcessing()
				{
					return( mSTVideoProcessing );
				}

		public: static STConsole & GetConsole ( )
				{
					return ( mSTConsole );
				}

		public: static STFileList & GetFileList()
				{
					return( mSTFileList );
				}





		public: static int GetClientWidth()
				{
					return( mClient_Width );
				}
		public: static int GetClientHeight()
				{
					return( mClient_Height );
				}

		public: static void SetClientWidth( int clientWidth )
				{
					mClient_Width = clientWidth;
				}
		public: static void SetClientHeight( int clientHeight )
				{
					mClient_Height = clientHeight;
				}


		public: static int GetClientDefaultX();
		public: static int GetClientDefaultY();
		public: static int GetClientDefaultWidth();
		public: static int GetClientDefaultHeight();


		public: static HINSTANCE GetHINSTANCE()
				{
					return( m_HINSTANCE );
				}


		public: static void SetHWND
					(
					HWND HWND_Parameter
					)
				{
					m_HWND = HWND_Parameter;
				}


		public: static void SetHDC
					(
					HDC  HDC_Parameter
					)
				{
					m_HDC = HDC_Parameter;
				}

		public: static HDC GetHDC()
				{
					return(m_HDC);
				}


		public: static HWND GetHWND()
				{
					return( m_HWND );
				}




		public: static void SetHINSTANCE
					(
					HINSTANCE  HINSTANCE_Parameter
					)
				{
					m_HINSTANCE = HINSTANCE_Parameter;
				}




		};



	}
}


#endif






