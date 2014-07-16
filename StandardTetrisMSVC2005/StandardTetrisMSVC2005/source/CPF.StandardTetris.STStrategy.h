// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STStrategyHeader
#define STStrategyHeader


#include "CPF.StandardTetris.STBoard.h"
#include "CPF.StandardTetris.STPiece.h"


// Disable unimportant "symbol too long" error for debug builds of STL string
#pragma warning( disable : 4786 )

#include <string>
#include <vector>
using namespace std;



namespace CPF
{
	namespace StandardTetris
	{



		class STStrategy
		{
	        // This is a base class from which specific strategies are derived

		public: virtual string GetStrategyName ( );

		public: virtual void GetBestMoveOncePerPiece
			(
				STBoard & currentBoard,
				STPiece & currentPiece,
				int nextPieceFlag, // false == no next piece available or known
				int nextPieceShape, // None == no piece available or known
				int & bestRotationDelta, // 0 or {0,1,2,3}
				int & bestTranslationDelta // 0 or {...,-2,-1,0,1,2,...}
			);

		public: virtual void GetBestMovePath
			(
				STBoard & currentBoard,
				STPiece & currentPiece,
				int nextPieceFlag, // false == no next piece available or known
				int nextPieceShape, // None == no piece available or known
				char path[]
			);


		};



	}
}




#endif






