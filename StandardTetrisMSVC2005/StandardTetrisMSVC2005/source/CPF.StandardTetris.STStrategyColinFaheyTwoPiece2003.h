// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STStrategyColinFaheyTwoPiece2003Header
#define STStrategyColinFaheyTwoPiece2003Header


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



		class STStrategyColinFaheyTwoPiece2003 : public STStrategy
		{


		public: virtual string GetStrategyName ( );


			// WARNING: Moves requiring rotation must wait until piece has fallen by
			// at least one row!
			// Perform all rotations, and then perform translations.  This
			// avoids the problem of getting the piece jammed on the sides
			// of the board where rotation is impossible. ***
			// Also, the following strategy does not take advantage of the
			// possibility of using free-fall and future movements to
			// slide under overhangs and fill them in.

		public: virtual void GetBestMoveOncePerPiece
        (
            STBoard & board,
            STPiece & piece,
            int nextPieceFlag, // 0 == no next piece available or known
            int nextPieceShape, // 0 == no piece available or known
            int & bestRotationDelta, // 0 or {0,1,2,3}
            int & bestTranslationDelta // 0 or {...,-2,-1,0,1,2,...}
        );



		private: static double PrivateStrategy
					 ( 
					 int         flagCalledFromParentPly,  // True if called from a parent level
					 STBoard &   board,
					 STPiece &   piece,
					 int &       bestRotationDelta,    // 0 or {0,1,2,3} 
					 int &       bestTranslationDelta  // 0 or {...,-2,-1,0,1,2,...}
					 );

		private: static double PrivateStrategyNextPiece
					 ( 
					 STBoard &  board,
					 STPiece &  piece,
					 int        nextPieceKind,  // (-1)==None
					 int &      bestRotationDelta,    // 0 or {0,1,2,3} 
					 int &      bestTranslationDelta  // 0 or {...,-2,-1,0,1,2,...}
					 );

		};





	}
}

#endif






