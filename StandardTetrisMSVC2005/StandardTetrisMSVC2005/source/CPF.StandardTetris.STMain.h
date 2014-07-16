// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STMainHeader
#define STMainHeader



#include "CPF.StandardTetris.STEngine.h"



namespace CPF
{
	namespace StandardTetris
	{



		class STMain
		{

		public: static LRESULT CALLBACK WndProc
					(
					HWND hwnd, 
					UINT iMsg, 
					WPARAM wParam, 
					LPARAM lParam
					);

		public: static int WINAPI HandledWinMain
					(
					HINSTANCE  HINSTANCE_This,
					HINSTANCE  hPrevInstance,
					LPSTR     szCmdLine,
					int        iCmdShow
					);


		};


	}
}



#endif






