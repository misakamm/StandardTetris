// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STDateTimeHeader
#define STDateTimeHeader



// Disable unimportant "symbol too long" error for debug builds of STL string
#pragma warning( disable : 4786 )
#include <string>
using namespace std;



namespace CPF
{
	namespace StandardTetris
	{


		class STDateTime
		{
		public: int   mYear;        // e.g., 2003
		public: int   mMonth;       // 1..12
		public: int   mDay;         // 1..31
		public: int   mDayOfWeek;   // 0..6 (0==Sunday)
		public: int   mHour;        // 0..23
		public: int   mMinute;      // 0..59
		public: int   mSecond;      // 0..59
		public: int   mMillisecond; // 0..999

		public: STDateTime( );

		public: void GetCurrentDateAndTime( );
		public: void GetStringYYYYMMDD_HHMMSSMMM( string & ref_string );
		public: void GetStringYYYY_MMM_DD_HH_MM_SS_MMM( string & ref_string );
		};


	}
}



#endif





