// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#include "CPF.StandardTetris.STStrategy.h"


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


		string STStrategy::GetStrategyName ( )
		{
			return ((string)"unknown");
		}

		void STStrategy::GetBestMoveOncePerPiece
			(
			STBoard & currentBoard,
			STPiece & currentPiece,
			int nextPieceFlag, // false == no next piece available or known
			int nextPieceShape, // None == no piece available or known
			int & bestRotationDelta, // 0 or {0,1,2,3}
			int & bestTranslationDelta // 0 or {...,-2,-1,0,1,2,...}
			)
		{
			bestRotationDelta = 0;
			bestTranslationDelta = 0;
		}

		void STStrategy::GetBestMovePath
			(
			STBoard & currentBoard,
			STPiece & currentPiece,
			int nextPieceFlag, // false == no next piece available or known
			int nextPieceShape, // None == no piece available or known
			char path[]
			)
		{
			int bestRotationDelta = 0;
			int bestTranslationDelta = 0;
			GetBestMoveOncePerPiece(
				currentBoard,
				currentPiece,
				nextPieceFlag,
				nextPieceShape,
				bestRotationDelta,
				bestTranslationDelta
				);
			char* p = path;
			for ( ; bestRotationDelta > 0; --bestRotationDelta )
			{
				*p++ = 'z';
			}
			for ( ; bestTranslationDelta < 0; ++bestTranslationDelta )
			{
				*p++ = 'l';
			}
			for ( ; bestTranslationDelta > 0; --bestTranslationDelta )
			{
				*p++ = 'r';
			}
			p[1] = 0;
		}


	}
}