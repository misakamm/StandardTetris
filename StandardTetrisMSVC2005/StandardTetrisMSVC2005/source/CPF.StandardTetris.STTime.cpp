// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#include "CPF.StandardTetris.STTime.h"



#include <windows.h>
#include <time.h>
#include <winnt.h>



namespace CPF
{
	namespace StandardTetris
	{



		// Performance Counter Variables
		
		int                 STTime::m_iCounterExists          = 0;
		LARGE_INTEGER       STTime::m_LICounterFrequency      = {0,0};
		LARGE_INTEGER       STTime::m_LICounterReferenceValue = {0,0};
		

		// time(NULL) Variable
		
		int                 STTime::m_iSecondsSince1970ReferenceValue = 0;
		

		
		// Interval Duration
		
		LARGE_INTEGER       STTime::m_LICounterIntervalReferenceValue = {0,0};
		double              STTime::m_double_IntervalSeconds          = (0.0);
		float               STTime::m_float_IntervalSeconds           = (0.0f);
		


		
		// The following sets the reference time to the time of the call.
		// All future calls to GetTimeSecondsXXXXX() will be relative to
		//   this reference time.
		

		void   STTime::SetReferenceTimeToNow()
		{
			
			// Clear values
			
			STTime::m_iCounterExists                           = 0;
			STTime::m_LICounterFrequency.QuadPart              = 0;
			STTime::m_LICounterReferenceValue.QuadPart         = 0;
			
			STTime::m_iSecondsSince1970ReferenceValue          = 0;
			
			STTime::m_LICounterIntervalReferenceValue.QuadPart = 0;
			STTime::m_double_IntervalSeconds                   = (0.0);
			STTime::m_float_IntervalSeconds                    = (0.0f);
			

			
			// Determine if we have a performance counter.
			// We do this by requesting the frequency.
			
			STTime::m_iCounterExists = 
				(int) QueryPerformanceFrequency( (&(STTime::m_LICounterFrequency)) );
			

			
			// If we have a performance counter, get the current
			//   value for our starting value.  This will be our
			//   initial "current" value, too.
			
			if (STTime::m_iCounterExists)
			{
				QueryPerformanceCounter( (&(STTime::m_LICounterReferenceValue)) );
			}
			

			
			// Set the time(NULL) reference value, too.
			
			STTime::m_iSecondsSince1970ReferenceValue   = (int)(time(NULL));
			

			
			// Set the Interval Duration Reference Time
			
			STTime::m_LICounterIntervalReferenceValue.QuadPart = 
				(STTime::m_LICounterReferenceValue.QuadPart);
			STTime::m_double_IntervalSeconds                   = (0.0);
			STTime::m_float_IntervalSeconds                    = (0.0f);
			
		}



		double  STTime::GetRelativeTimeSecondsDouble(  )
		{
			
			// If we don't have a performance counter, we can't honor this request.
			
			if (0 == (STTime::m_iCounterExists))
			{
				// We don't have a performance counter.
				return( 0.0 );
			}
			

			LARGE_INTEGER   LICounterDeltaValue;
			QueryPerformanceCounter( (&(LICounterDeltaValue)) );

			LICounterDeltaValue.QuadPart -= 
				((STTime::m_LICounterReferenceValue).QuadPart);

			double dSeconds = 0.0f;
			if ((STTime::m_LICounterFrequency.QuadPart) > 0)
			{
				dSeconds  = (double)(LICounterDeltaValue.QuadPart)
					/ (double)((STTime::m_LICounterFrequency.QuadPart));
			}

			return( dSeconds );
		}




		float   STTime::GetRelativeTimeSecondsFloat(  )
		{
			return(   ((float)(GetRelativeTimeSecondsDouble()))   );
		}




		int     STTime::GetRelativeTimeSecondsInteger(  )
		{
			return(   ((int)(GetRelativeTimeSecondsDouble()))   );
		}




		int    STTime::GetTimeWallClockSecondsSince1970(  )
		{
			int   iSecondsSince1970 = 0;
			iSecondsSince1970 = (int)time( NULL );
			return ( iSecondsSince1970 );
		}




		int    STTime::GetWallClockReferenceValueSeconds(  )
		{
			return ( (STTime::m_iSecondsSince1970ReferenceValue) );
		}



		int    STTime::GetRelativeTimeWallClockSeconds(  )
		{
			int   iSecondsSince1970 = 0;
			iSecondsSince1970 = (int)time( NULL );
			return ( iSecondsSince1970 - (STTime::m_iSecondsSince1970ReferenceValue) );
		}



		double  STTime::GetIntervalDurationSecondsDouble(  )
		{
			// Return cached interval duration in seconds, as a 'double'.
			// ( 60fps --> 0.0166 sec/frame;  85fps --> 0.0117 sec/frame )
			return( STTime::m_double_IntervalSeconds );
		}



		float   STTime::GetIntervalDurationSecondsFloat(  )
		{
			// Return cached interval duration in seconds, as a 'float'.
			// ( 60fps --> 0.0166 sec/frame;  85fps --> 0.0117 sec/frame )
			return( STTime::m_float_IntervalSeconds );
		}



		void    STTime::NextIntervalBeginsNow(  )
		{
			
			// If we don't have a performance counter, we can't honor this request.
			
			if (0 == (STTime::m_iCounterExists))
			{
				// We don't have a performance counter.
				return;
			}
			

			// Conter Value for "Now"
			LARGE_INTEGER   LICounterValue;
			QueryPerformanceCounter( (&(LICounterValue)) );

			// Difference between previous counter value and "Now"
			LARGE_INTEGER   LICounterDeltaValue;
			LICounterDeltaValue.QuadPart = LICounterValue.QuadPart;
			LICounterDeltaValue.QuadPart -= 
				((STTime::m_LICounterIntervalReferenceValue).QuadPart);

			// Compute difference in seconds (double precision).
			double dSeconds = 0.0f;
			if ((STTime::m_LICounterFrequency.QuadPart) > 0)
			{
				dSeconds  = (double)(LICounterDeltaValue.QuadPart)
					/ (double)((STTime::m_LICounterFrequency.QuadPart));
			}

			// Cast duration time to float (single precision).
			float fSeconds = ((float)(dSeconds));

			// Cache both representations of interval duration value.
			STTime::m_double_IntervalSeconds = (dSeconds);
			STTime::m_float_IntervalSeconds  = (fSeconds);

			// Set the interval reference value to "now".
			STTime::m_LICounterIntervalReferenceValue.QuadPart = 
				LICounterValue.QuadPart;
		}



		int    STTime::PerformanceCounterExists(  )
		{
			return( (STTime::m_iCounterExists) );
		}



		void   STTime::GetCounterFrequencyHighLow
			( 
			unsigned int &refHigh32, 
			unsigned int &refLow32 
			)
		{
			refHigh32 = ((STTime::m_LICounterFrequency).HighPart);
			refLow32  = ((STTime::m_LICounterFrequency).LowPart);
		}



		void   STTime::GetCounterReferenceHighLow
			( 
			unsigned int &refHigh32, 
			unsigned int &refLow32 
			)
		{
			refHigh32 = ((STTime::m_LICounterReferenceValue).HighPart);
			refLow32  = ((STTime::m_LICounterReferenceValue).LowPart);
		}



		void   STTime::GetCounterCurrentHighLow
			( 
			unsigned int &refHigh32, 
			unsigned int &refLow32 
			)
		{
			refHigh32 = 0;
			refLow32  = 0;

			if (0 == (STTime::m_iCounterExists))
				return;

			LARGE_INTEGER  LICounterCurrent;
			QueryPerformanceCounter( (&(LICounterCurrent)) );

			refHigh32 = (LICounterCurrent.HighPart);
			refLow32  = (LICounterCurrent.LowPart);
		}




	}
}
