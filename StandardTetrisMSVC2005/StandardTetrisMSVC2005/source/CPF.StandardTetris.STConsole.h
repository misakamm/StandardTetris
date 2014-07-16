// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STConsoleHeader
#define STConsoleHeader



#include <stdio.h>  // FILE, fopen(), fprintf(), fclose()

// Disable unimportant "symbol too long" error for debug builds of STL string
#pragma warning( disable : 4786 )

#include <string>
#include <vector>
using namespace std;



namespace CPF
{
	namespace StandardTetris
	{


		class STConsole
		{
		private:   vector< string >  mVectorString;
		private:  int               mMaximumLines;

		public: 		STConsole::STConsole();

		public:     void   ClearAllLines( );
		public:     int    GetTotalLines( );
		public:     char * GetLineByIndex( int index );
		public:     void   AddLine( const char *format, ... );
		public:     void   AddToLastLine( const char *format, ... );
		};

	}
}



#endif





