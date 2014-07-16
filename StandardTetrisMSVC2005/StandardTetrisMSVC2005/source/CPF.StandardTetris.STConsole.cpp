// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1



#include "CPF.StandardTetris.STConsole.h"


#include <stdio.h>    // FILE, fopen(), fprintf(), fclose()
#include <stdlib.h>   // stroul() : Converts string to unsigned long int
#include <stdarg.h>

// Disable unimportant "symbol too long" error for debug builds of STL string
#pragma warning( disable : 4786 )

#include <string>
#include <vector>
using namespace std;



namespace CPF
{
	namespace StandardTetris
	{


		STConsole::STConsole()
		{
			mMaximumLines = 24;
		}


		void   STConsole::ClearAllLines( )
		{
			mVectorString.clear();
		}



		int    STConsole::GetTotalLines( )
		{
			int totalLines = 0;

			totalLines = (int)(mVectorString.size());

			return( totalLines );
		}



		char * STConsole::GetLineByIndex( int index )
		{
			int totalLines = 0;

			totalLines = (int)(mVectorString.size());

			if ((index < 0) || (index >= totalLines))
			{
				return( ((char *)(0)) );
			}

			return( (char *)(mVectorString[index].c_str()) );
		}



		void   STConsole::AddLine( const char *format, ... )
		{
			if (((char *)(0)) == format)
			{
				return;
			}

			if (((char)(0)) == format[0])
			{
				return;
			}

			// Format the string
			va_list       ap;          // Argument pointer
			char s[ 8192 ];   // Output string              
			va_start( ap, format );
			vsprintf( (char *)s, format, ap );
			va_end  ( ap );

			if (((char)(0)) == s[0]) 
			{
				return;
			}

			int totalLines = 0;
			totalLines = (int)(mVectorString.size());

			while (totalLines >= mMaximumLines)
			{
				// Clobber a few lines!
				mVectorString.erase(mVectorString.begin());
				totalLines = (int)(mVectorString.size());
			}

			string  string_Text;
			string_Text += (string)(s);
			mVectorString.push_back( string_Text );
		}



		void   STConsole::AddToLastLine( const char *format, ... )
		{
			if (((char *)(0)) == format)
			{
				return;
			}

			if (((char)(0)) == format[0])
			{
				return;
			}

			// Format the string
			va_list       ap;          // Argument pointer
			char s[ 8192 ];   // Output string              
			va_start( ap, format );
			vsprintf( (char *)s, format, ap );
			va_end  ( ap );

			if (((char)(0)) == s[0]) 
			{
				return;
			}

			int totalLines = 0;
			totalLines = (int)(mVectorString.size());

			while (totalLines >= mMaximumLines)
			{
				// Clobber a few lines!
				mVectorString.erase(mVectorString.begin());
				totalLines = (int)(mVectorString.size());
			}

			string  string_Text;
			string_Text += (string)(s);

			// Get string
			if (totalLines > 0)
			{
				// Can I do this?
				mVectorString[ totalLines-1 ] += string_Text;
			}
			else
			{
				// No line exists, so this text becomes a new first line.
				mVectorString.push_back( string_Text );
			}
		}




	}
}