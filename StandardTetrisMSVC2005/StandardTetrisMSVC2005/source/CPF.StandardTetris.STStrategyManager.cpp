// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.


#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1



#include "CPF.StandardTetris.STStrategyManager.h"



#include "CPF.StandardTetris.STBoard.h"
#include "CPF.StandardTetris.STPiece.h"
#include "CPF.StandardTetris.STStrategyPierreDellacherieOnePiece2003.h"
#include "CPF.StandardTetris.STStrategyRogerLLimaLaurentBercotSebastienBlondeelOnePiece1996.h"
#include "CPF.StandardTetris.STStrategyUserDefinedDll.h"
#include "CPF.StandardTetris.STStrategyColinFaheyTwoPiece2003.h"



#include <string.h>


// Disable unimportant "symbol too long" error for debug builds of STL string
#pragma warning( disable : 4786 )

#include <string>
#include <vector>
using namespace std;




namespace CPF
{
	namespace StandardTetris
	{

	
		vector<STStrategy *> STStrategyManager::mListpSTStrategy;

		string STStrategyManager::mCurrentStrategyName = "";


		void STStrategyManager::AddStrategy( STStrategy * pStrategy )
        {
			// Only add if no strategy in the list has a matching name
			int n = 0;
			int i = 0;
			n = (int)(STStrategyManager::mListpSTStrategy.size());
			for ( i = 0; i < n; i++ )
			{
				if ((STStrategyManager::mListpSTStrategy[i])->GetStrategyName()
					== pStrategy->GetStrategyName())
				{
					return;
				}
			}
			STStrategyManager::mListpSTStrategy.push_back( pStrategy );
        }


		void STStrategyManager::Initialize ( )
        {			
			STStrategyManager::AddStrategy( (STStrategy *)(new STStrategyUserDefinedDll()) );
			STStrategyManager::AddStrategy( (STStrategy *)(new STStrategyPierreDellacherieOnePiece2003()) );
			STStrategyManager::AddStrategy( (STStrategy *)(new STStrategyRogerLLimaLaurentBercotSebastienBlondeelOnePiece1996()) );
			STStrategyManager::AddStrategy( (STStrategy *)(new STStrategyColinFaheyTwoPiece2003()) );
        }


        // The following methods to set and get the current strategy
        // by name intentionally avoid validating the name with the
        // list of strategies.  Only when the strategy is used do we
        // attempt to fix a specified choice.  This is to make sure
        // that loading and saving is not affected by whether or not
        // required strategies are inserted in to the list.

        void STStrategyManager::SetCurrentStrategyByName ( string strategyName )
        {
            STStrategyManager::mCurrentStrategyName = strategyName;
        }

        string STStrategyManager::GetCurrentStrategyName ( )
        {
            // Force mCurrentStrategyName to have a current value
            STStrategyManager::GetCurrentStrategy( );

            return(STStrategyManager::mCurrentStrategyName);
        }





        STStrategy * STStrategyManager::GetCurrentStrategy( )
        { 
			int n = 0;
			n = (int)(STStrategyManager::mListpSTStrategy.size());

			if (n <= 0)
            {
                return( (STStrategy *)0 );
            }

			int i = 0;
			for ( i = 0; i < n; i++ )
			{
				STStrategy * pStrategy = STStrategyManager::mListpSTStrategy[i];

				if (pStrategy->GetStrategyName() == STStrategyManager::mCurrentStrategyName)
				{
					return( pStrategy );
				}
			}

            // The name did not match any strategy in the list, so we
            // arbitrarily choose the first strategy in the list.

			STStrategy * pStrategyDefault = STStrategyManager::mListpSTStrategy[0];

			STStrategyManager::mCurrentStrategyName = pStrategyDefault->GetStrategyName();

            return( pStrategyDefault );
        }




        void STStrategyManager::SelectNextStrategy ( )
        {
			int n = 0;
			n = (int)(STStrategyManager::mListpSTStrategy.size());

			if (n <= 0)
            {
                return;
            }

            // There are at least two strategies; thus, advancing
            // to a next strategy (with wrap-around) is possible.
            int k = -1;
            int i = 0;
            for (i = 0; i < n; i++)
            {
				STStrategy * pStrategy = STStrategyManager::mListpSTStrategy[i];

				if (pStrategy->GetStrategyName( ) == STStrategyManager::mCurrentStrategyName)
                {
                    k = i;
                }
            }
            if ((-1) == k)
            {
                // We failed to find the "current" strategy, so let's
                // choose an arbitrary successor.
                STStrategyManager::mCurrentStrategyName = 
					(STStrategyManager::mListpSTStrategy[0])->GetStrategyName( );
                return;
            }
            // We found the current strategy at index k; so, we increment
            // this variable, with wrapping.
            k++;
            k %= n; // thus, k == n becomes k == 0
            STStrategyManager::mCurrentStrategyName = 
				(STStrategyManager::mListpSTStrategy[k])->GetStrategyName( );
        }




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

        void STStrategyManager::GetBestMoveOncePerPiece
        (
            STBoard & currentBoard,
            STPiece & currentPiece,
            int nextPieceFlag, // 0 == no next piece available or known
            int nextPieceShape, // 0 == no piece available or known
            int & bestRotationDelta, // 0 or {0,1,2,3}
            int & bestTranslationDelta // 0 or {...,-2,-1,0,1,2,...}
        )
        {
            bestRotationDelta = 0;
            bestTranslationDelta = 0;

            STStrategy * pStrategy = STStrategyManager::GetCurrentStrategy();

			if ( (STStrategy *)0 != pStrategy)
			{
				pStrategy->GetBestMoveOncePerPiece
				(
					currentBoard,
					currentPiece,
					nextPieceFlag, // 0 == no next piece available or known
					nextPieceShape, // 0 == no piece available or known
					bestRotationDelta, // 0 or {0,1,2,3}
					bestTranslationDelta // 0 or {...,-2,-1,0,1,2,...}
				);
			}
        }


        void STStrategyManager::GetBestMovePath
        (
            STBoard & currentBoard,
            STPiece & currentPiece,
            int nextPieceFlag, // 0 == no next piece available or known
            int nextPieceShape, // 0 == no piece available or known
            char path[]
        )
        {
            STStrategy * pStrategy = STStrategyManager::GetCurrentStrategy();

			if ( (STStrategy *)0 != pStrategy)
			{
				pStrategy->GetBestMovePath
				(
					currentBoard,
					currentPiece,
					nextPieceFlag, // 0 == no next piece available or known
					nextPieceShape, // 0 == no piece available or known
					path
				);
			}
        }



	}
}



