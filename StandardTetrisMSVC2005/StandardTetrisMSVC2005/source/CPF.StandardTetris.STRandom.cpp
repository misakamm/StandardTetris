// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#include "CPF.StandardTetris.STRandom.h"



#include "CPF.StandardTetris.STMD5.h"

#define MT19937 1


namespace CPF
{
	namespace StandardTetris
	{



	STRandom::STRandom ( )
		{
		(*this).mState = 0;
		}


	__int64 STRandom::GetState ( )
        {
            return ((*this).mState);
        }


	void STRandom::SetState ( __int64 state )
        {
#if MT19937
			if ( mState == 0 )
			{
				mRandom.seed((unsigned)state);
			}
#endif
            (*this).mState = state;
        }


	void STRandom::Advance ( )
        {
            (*this).mState += (__int64)1;
        }

        __int64 STRandom::GetRandomBits ( )
        {
            unsigned char input[8];
            int i = 0;
            for ( i = 0; i < 8; i++ )
            {
                input[i] = (unsigned char)(( (*this).mState >> (8 * i)) & 0xff);
            }

			STMD5::MD5_CTX context;
			unsigned char digest[16];
			STMD5::MD5Init (&context);
			STMD5::MD5Update (&context, (unsigned char *)input, 8);
			STMD5::MD5Final (digest, &context);

            __int64 outputValue = 0;
            for (i = 0; i < 8; i++)
            {
                outputValue |= ((__int64)digest[i]) << (8 * i);
            }
            return (outputValue);
        }

        int STRandom::GetIntegerInRangeUsingCurrentState
        (
            int minimum,
            int maximum
        )
        {
#if MT19937
			if (minimum >= maximum)
			{
				return(minimum);
			}
			int r = maximum - minimum + 1;
			if ( r == 7 )
			{
				AI::uint32 n = mRandom.rand() & 0x7;
				while ( n >= 7 ) n = mRandom.rand() & 0x7;
				return (int)n + minimum;
			}
			else
			{
				return mRandom.randint(r) + minimum;
			}
#endif
			//--------------------------------------------------
            if (minimum == maximum)
            {
                // Trivial case: no range at all.
                return(minimum);
            }

            // If the specified range is not in order, then determine
            // the actual minimum and maximum.  (We will always return
            // a random value in the specified interval, even if the
            // specified interval is not in an ascending order.)
            int actualMinimum = 0;
            int actualMaximum = 0;
            if (minimum <= maximum)
            {
                actualMinimum = minimum;
                actualMaximum = maximum;
            }
            else
            {
                actualMinimum = maximum;
                actualMaximum = minimum;
            }

            int totalValues = ((actualMaximum - actualMinimum) + 1);
            // e.g., a range of 3,4,5,6,7 has ((7-3)+1) = 5 values


            __int64 randomBits = (*this).GetRandomBits( );

            __int64 mask = 0x7fffffffffffffff;
            __int64 maskedValue = (randomBits & mask);
            if (maskedValue == mask)
            {
                maskedValue = 0; // prevent fraction from hitting 1.0
            }
            double fraction = (double)maskedValue / (double)mask; // 0.0 ... 0.9999...
            double relativeValue = (double)totalValues * fraction; // 0.0 ... (0.999... * totalValues)

            int value = actualMinimum + (int)relativeValue;

            // Clip to minimum in the unexpected event that the computed
            // value is less than the desired minimum.  This shouldn't
            // happen, but paranoia is awesome.
            if (value < actualMinimum)
            {
                value = actualMinimum;
            }

            // Clip to maximum in the unexpected event that the computed
            // value is greater than the desired maximum.  This shouldn't
            // happen, but paranoia is awesome.
            if (value > actualMaximum)
            {
                value = actualMaximum;
            }

            return (value);
        }






	}
}

