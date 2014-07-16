// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STStrategyManagerHeader
#define STStrategyManagerHeader



#include "CPF.StandardTetris.STBoard.h"
#include "CPF.StandardTetris.STPiece.h"
#include "CPF.StandardTetris.STStrategy.h"



// Disable unimportant "symbol too long" error for debug builds of STL string
#pragma warning( disable : 4786 )

#include <string>
#include <vector>
using namespace std;



namespace CPF
{
	namespace StandardTetris
	{


		class STStrategyManager
		{
		private: static vector<STStrategy *> mListpSTStrategy;

		private: static string mCurrentStrategyName;

		public: static void AddStrategy( STStrategy * pStrategy );

		public: static void Initialize();


				// The following methods to set and get the current strategy
				// by name intentionally avoid validating the name with the
				// list of strategies.  Only when the strategy is used do we
				// attempt to fix a specified choice.  This is to make sure
				// that loading and saving is not affected by whether or not
				// required strategies are inserted in to the list.

		public: static void SetCurrentStrategyByName ( string strategyName );

		public: static string GetCurrentStrategyName ( );



		public: static STStrategy * GetCurrentStrategy( );


		public: static void SelectNextStrategy ( );



				// WARNING: When you get the "best" rotation and translation
				// from the following function, you must wait until the piece has
				// its origin at least as low as row 0 (zero) instead of its initial
				// row -1 (negative one) if any rotations (1,2,3) are required.
				// Perform all rotations, and then perform translations.  This
				// avoids the problem of getting the piece jammed on the sides
				// of the board where rotation is impossible. ***
				// Also, the following strategy does not take advantage of the
				// possibility of using free-fall and future movements to
				// slide under overhangs and fill them in.

		public: static void GetBestMoveOncePerPiece
					(
					STBoard & currentBoard,
					STPiece & currentPiece,
					int nextPieceFlag, // 0 == no next piece available or known
					int nextPieceShape, // 0 == no piece available or known
					int & bestRotationDelta, // 0 or {0,1,2,3}
					int & bestTranslationDelta // 0 or {...,-2,-1,0,1,2,...}
					);

		public: static void GetBestMovePath
					(
					STBoard & currentBoard,
					STPiece & currentPiece,
					int nextPieceFlag, // 0 == no next piece available or known
					int nextPieceShape, // 0 == no piece available or known
					char path[]
					);



		};



	}
}


#endif






