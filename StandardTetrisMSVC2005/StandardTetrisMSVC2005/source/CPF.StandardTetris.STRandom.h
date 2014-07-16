// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STRandomHeader
#define STRandomHeader

#include "random.h"


namespace CPF
{
	namespace StandardTetris
	{



		class STRandom
		{
		private: __int64 mState;
		private: AI::Random  mRandom;

		public: STRandom ( );

		public: __int64 GetState ( );

		public: void SetState ( __int64 state );

		public: void Advance ( );

		public: __int64 GetRandomBits ( );

		public: int GetIntegerInRangeUsingCurrentState
					(
					int minimum,
					int maximum
					);

		};




	}
}



#endif






