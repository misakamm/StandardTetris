// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STTimeHeader
#define STTimeHeader



#include <windows.h>
#include <time.h>
#include <winnt.h>



namespace CPF
{
	namespace StandardTetris
	{


		class STTime
		{

			// Performance Counter Variables

		private: static int                 m_iCounterExists;
		private: static LARGE_INTEGER       m_LICounterFrequency;
		private: static LARGE_INTEGER       m_LICounterReferenceValue;


				 // time(NULL) Variable

		private: static int                 m_iSecondsSince1970ReferenceValue;



				 // Interval Duration

		private: static LARGE_INTEGER       m_LICounterIntervalReferenceValue;
		private: static double              m_double_IntervalSeconds;
		private: static float               m_float_IntervalSeconds;



				 // The following sets the reference time to the time of the call.
				 // All future calls to GetTimeSecondsXXXXX() will be relative to
				 //   this reference time.

		public: static void   SetReferenceTimeToNow( );  // Defines the reference time



				// All of the following are derived from the Performance Counter values.
				// All of the values are determined at the instant of the call, and
				//   the time values are relative to the reference time.

		public: static double  GetRelativeTimeSecondsDouble( );
		public: static float   GetRelativeTimeSecondsFloat( );
		public: static int     GetRelativeTimeSecondsInteger( );



				// The following is based on time(NULL)

		public: static int    GetTimeWallClockSecondsSince1970( );
		public: static int    GetWallClockReferenceValueSeconds( );
		public: static int    GetRelativeTimeWallClockSeconds( );



				// Interval Duration Methods

		public: static double  GetIntervalDurationSecondsDouble( );
		public: static float   GetIntervalDurationSecondsFloat( );
		public: static void    NextIntervalBeginsNow( );



				// Trivia information that might be useful.

		public: static int    PerformanceCounterExists( );
		public: static void   GetCounterFrequencyHighLow( unsigned int &refHigh32, unsigned int &refLow32 );
		public: static void   GetCounterReferenceHighLow( unsigned int &refHigh32, unsigned int &refLow32 );
		public: static void   GetCounterCurrentHighLow  ( unsigned int &refHigh32, unsigned int &refLow32 );

		};



	}
}



#endif






