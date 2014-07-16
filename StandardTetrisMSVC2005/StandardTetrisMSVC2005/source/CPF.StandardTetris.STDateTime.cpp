// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1



#include "CPF.StandardTetris.STDateTime.h"

#include <windows.h>
#include <stdio.h>

// Disable unimportant "symbol too long" error for debug builds of STL string
#pragma warning( disable : 4786 )
#include <string>
using namespace std;



namespace CPF
{
	namespace StandardTetris
	{


		STDateTime::STDateTime( )
		{
			memset( (void *)(this), ((int)(0)), sizeof(*this) );
		}



		void STDateTime::GetCurrentDateAndTime( )
		{
			SYSTEMTIME   st;
			GetLocalTime( &st );

			mYear         = (int)(st.wYear);         // e.g., 2003
			mMonth        = (int)(st.wMonth);        // 1..12
			mDay          = (int)(st.wDay);          // 1..31
			mDayOfWeek    = (int)(st.wDayOfWeek);    // 0..6 (0==Sunday)
			mHour         = (int)(st.wHour);         // 0..23
			mMinute       = (int)(st.wMinute);       // 0..59
			mSecond       = (int)(st.wSecond);       // 0..59
			mMillisecond  = (int)(st.wMilliseconds); // 0..999
		}



		void STDateTime::GetStringYYYYMMDD_HHMMSSMMM( string & ref_string )
		{
			ref_string = "";

			char temp[128];
			// YYYY
			sprintf( temp, "%04d", mYear );
			ref_string += (string)(temp);
			// MM
			sprintf( temp, "%02d", mMonth );
			ref_string += (string)(temp);
			// DD
			sprintf( temp, "%02d", mDay );
			ref_string += (string)(temp);
			// _
			ref_string += (string)("_");
			// HH
			sprintf( temp, "%02d", mHour );
			ref_string += (string)(temp);
			// MM
			sprintf( temp, "%02d", mMinute );
			ref_string += (string)(temp);
			// SS
			sprintf( temp, "%02d", mSecond );
			ref_string += (string)(temp);
			// MMM (milliseconds)
			sprintf( temp, "%03d", mMillisecond );
			ref_string += (string)(temp);  
		}



		void STDateTime::GetStringYYYY_MMM_DD_HH_MM_SS_MMM( string & ref_string )
		{
			ref_string = "";

			char temp[128];
			// YYYY
			sprintf( temp, "%04d", mYear );
			ref_string += (string)(temp);

			ref_string += (string)(" ");

			// MMM (Month)
			switch( mMonth )
			{
			case  1: ref_string += (string)("Jan"); break;
			case  2: ref_string += (string)("Feb"); break;
			case  3: ref_string += (string)("Mar"); break;
			case  4: ref_string += (string)("Apr"); break;
			case  5: ref_string += (string)("May"); break;
			case  6: ref_string += (string)("Jun"); break;
			case  7: ref_string += (string)("Jul"); break;
			case  8: ref_string += (string)("Aug"); break;
			case  9: ref_string += (string)("Sep"); break;
			case 10: ref_string += (string)("Oct"); break;
			case 11: ref_string += (string)("Nov"); break;
			case 12: ref_string += (string)("Dec"); break;
			default: ref_string += (string)("???"); break;
			}

			ref_string += (string)(" ");

			// DD
			sprintf( temp, "%02d", mDay );
			ref_string += (string)(temp);

			ref_string += (string)(" ");

			// HH
			sprintf( temp, "%02d", mHour );
			ref_string += (string)(temp);

			ref_string += (string)(":");

			// MM
			sprintf( temp, "%02d", mMinute );
			ref_string += (string)(temp);

			ref_string += (string)(":");

			// SS
			sprintf( temp, "%02d", mSecond );
			ref_string += (string)(temp);

			ref_string += (string)(".");

			// MMM (milliseconds)
			sprintf( temp, "%03d", mMillisecond );
			ref_string += (string)(temp);  
		}



	}
}

