// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STStrategyUserDefinedDllHeader
#define STStrategyUserDefinedDllHeader



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



		class STStrategyUserDefinedDll : public STStrategy
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

		public:
			virtual void GetBestMoveOncePerPiece
				(
				STBoard & currentBoard,
				STPiece & currentPiece,
				int nextPieceFlag, // 0 == no next piece available or known
				int nextPieceShape, // 0 == no piece available or known
				int & bestRotationDelta, // 0 or {0,1,2,3}
				int & bestTranslationDelta // 0 or {...,-2,-1,0,1,2,...}
				);
		public:
			virtual void GetBestMovePath
				(
				STBoard & currentBoard,
				STPiece & currentPiece,
				int nextPieceFlag, // false == no next piece available or known
				int nextPieceShape, // None == no piece available or known
				char path[]
				);



		private:
			int PrivateStrategy
				(
				int                       flagCalledFromParentPly,  // True if called from a parent level
				STBoard &   ref_CurrentBoard,
				STPiece &   ref_CopyOfCurrentPiece,
				int nextPieceFlag, // 0 == no next piece available or known
				int nextPieceShape, // 0 == no piece available or known
				int &                     ref_bestRotationDelta,    // 0 or {0,1,2,3} 
				int &                     ref_bestTranslationDelta  // 0 or {...,-2,-1,0,1,2,...}
				);

		public:
			STStrategyUserDefinedDll();
			bool LoadDll(std::string path);
			bool IsLoad();

		protected:
			void* hInst;
			typedef char* (__stdcall *NameFunc)();
			typedef int (__stdcall *StrategyFunc)(
				int, int,
				char*,
				char,
				int, int, int,
				char,
				int*, int*
				);
			typedef int (__stdcall *StrategyPathFunc)(
				int, int,
				char*,
				char,
				int, int, int,
				char,
				char*
				);
			NameFunc mNameFunc;
			StrategyFunc mStrategyFunc;
			StrategyPathFunc mStrategyPathFunc;
		};

	}
}



#endif






