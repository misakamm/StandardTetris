// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#include "CPF.StandardTetris.STStrategyColinFaheyTwoPiece2003.h"


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



		string STStrategyColinFaheyTwoPiece2003::GetStrategyName ( )
        {
            return ((string)"Colin Fahey (two-piece, 2003)");
        }



		// WARNING: Moves requiring rotation must wait until piece has fallen by
		// at least one row!
		// Perform all rotations, and then perform translations.  This
		// avoids the problem of getting the piece jammed on the sides
		// of the board where rotation is impossible. ***
		// Also, the following strategy does not take advantage of the
		// possibility of using free-fall and future movements to
		// slide under overhangs and fill them in.

		void STStrategyColinFaheyTwoPiece2003::GetBestMoveOncePerPiece
        (
            STBoard & board,
            STPiece & piece,
            int nextPieceFlag, // 0 == no next piece available or known
            int nextPieceShape, // 0 == no piece available or known
            int & bestRotationDelta, // 0 or {0,1,2,3}
            int & bestTranslationDelta // 0 or {...,-2,-1,0,1,2,...}
        )
		{
			bestRotationDelta    = 0;
			bestTranslationDelta = 0;

			// We are given the current board, and the current piece
			// configuration.  Our goal is to evaluate various possible
			// moves and return the best move we explored.


			if (0 != nextPieceFlag)
			{
				// two-piece analysis (current piece and next piece)
				STStrategyColinFaheyTwoPiece2003::PrivateStrategyNextPiece
					(
					board,
					piece,
					nextPieceShape,  // (-1)==None
					bestRotationDelta,     // 0 or {0,1,2,3}
					bestTranslationDelta   // 0 or {...,-2,-1,0,1,2,...}
					);
			}
			else
			{
				// one-piece analysis
				STStrategyColinFaheyTwoPiece2003::PrivateStrategy
					(
					0, // not called from parent ply; Just this one ply.
					board,
					piece,
					bestRotationDelta,     // 0 or {0,1,2,3}
					bestTranslationDelta   // 0 or {...,-2,-1,0,1,2,...}
					);
			}

		}






		double STStrategyColinFaheyTwoPiece2003::PrivateStrategy
			( 
			int  flagCalledFromParentPly,  // True if called from a parent level
			STBoard &  board,
			STPiece &  piece,
			int & bestRotationDelta,    // 0 or {0,1,2,3} 
			int & bestTranslationDelta  // 0 or {...,-2,-1,0,1,2,...}
			)
		{

			if (0 == board.IsValid()) 
			{
				return(0.0);
			}

			if (0 == piece.IsValid()) 
			{
				return(0.0);
			}


			int width  = 0;
			int height = 0;
			width  = board.GetWidth();
			height = board.GetHeight();


			int    currentBestTranslationDelta  = 0;
			int    currentBestRotationDelta = 0;
			double  currentBestMerit = (-1.0e20);
			int    currentBestPriority = 0;

			int    trialTranslationDelta = 0;
			int    trialRotationDelta = 0;
			double  trialMerit = 0.0;
			int    trialPriority = 0;

			int    maxOrientations = 0;
			int    moveAcceptable = 0;
			int    count = 0;


			STBoard  tempBoard;
			STPiece  tempPiece;



			maxOrientations = STPiece::GetMaxOrientations( piece.GetKind() );



			for 
				( 
				trialRotationDelta = 0; 
				trialRotationDelta < maxOrientations;
				trialRotationDelta++ 
				)
			{
				
				// Make temporary copy of piece, and rotate the copy.
				
				tempPiece.CopyFromPiece( piece );

				for ( count = 0; count < trialRotationDelta; count++ )
				{
					tempPiece.Rotate();
				}
				


				
				// Determine the translation limits for this rotated piece.
				
				int moveIsPossible = 0;
				int minDeltaX      = 0;
				int maxDeltaX      = 0;
				board.DetermineAccessibleTranslationsForPieceOrientation
					(
					tempPiece,
					moveIsPossible, // OUT: 0==NONE POSSIBLE
					minDeltaX,    // Left limit
					maxDeltaX     // Right limit
					);
				


				
				// Consider all allowed translations for the current rotation.
				
				if (0 != moveIsPossible)
				{
					for 
						( 
						trialTranslationDelta = minDeltaX;
						trialTranslationDelta <= maxDeltaX;
						trialTranslationDelta++ 
						)
					{
						// Evaluate this move

						// Copy piece to temp and rotate and translate
						
						tempPiece.CopyFromPiece( piece );
						
						for ( count = 0; count < trialRotationDelta; count++ )
						{
							tempPiece.Rotate();
						}

						tempPiece.Translate( trialTranslationDelta, 0 );

						moveAcceptable = board.IsGoalAcceptable( tempPiece );

						if (0 != moveAcceptable)
						{
							// Since the piece can be (not necessarily GET) at the goal
							// horizontal translation and orientation, it's worth trying
							// out a drop and evaluating the move.
							tempBoard.CopyFromBoard( board );


							tempBoard.FullDropAndAddPieceToBoard( tempPiece	);


							//       trialPriority = -100 * abs( trialTranslationDelta );
							//       if (trialTranslationDelta > 0)  trialPriority -= 10;
							//       trialPriority += trialRotationDelta;


							trialPriority = 0;




							if (0 != flagCalledFromParentPly)
							{
								int rowsEliminated = 0;
								rowsEliminated = tempBoard.CollapseAnyCompletedRows();

								double weightTotalShadowedHoles      = (-0.65);
								double weightPileHeightWeightedCells = (-0.10);
								double weightSumOfWellHeights        = (-0.20);

								trialMerit =  (weightTotalShadowedHoles)      * (double)(tempBoard.TotalShadowedHoles());
								trialMerit += (weightPileHeightWeightedCells) * (double)(tempBoard.PileHeightWeightedCells());
								trialMerit += (weightSumOfWellHeights)        * (double)(tempBoard.SumOfWellHeights());          
							}
							else
							{
								double weightRowElimination          = ( 0.30);
								double weightTotalOccupiedCells      = (-0.00);
								double weightTotalShadowedHoles      = (-0.65);
								double weightPileHeightWeightedCells = (-0.10);
								double weightSumOfWellHeights        = (-0.20);

								int rowsEliminated = 0;
								rowsEliminated = tempBoard.CollapseAnyCompletedRows();
								// Single Ply (No next piece)
								// Averages around 1310 rows in 10 games, with a min of 445 and a max of 3710.
								trialMerit  = (weightRowElimination)*(double)(rowsEliminated);
								trialMerit += (weightTotalOccupiedCells)*(double)(tempBoard.TotalOccupiedCells());
								trialMerit += (weightTotalShadowedHoles)*(double)(tempBoard.TotalShadowedHoles());
								trialMerit += (weightPileHeightWeightedCells)*(double)(tempBoard.PileHeightWeightedCells());
								trialMerit += (weightSumOfWellHeights)*(double)(tempBoard.SumOfWellHeights());
							}



							// If this move is better than any move considered before,
							// or if this move is equally ranked but has a higher priority,
							// then update this to be our best move.
							if 
								( 
								(trialMerit > currentBestMerit) ||
								((trialMerit == currentBestMerit) && (trialPriority > currentBestPriority)) 
								)
							{
								currentBestPriority = trialPriority;
								currentBestMerit = trialMerit;
								currentBestTranslationDelta = trialTranslationDelta;
								currentBestRotationDelta = trialRotationDelta;
							}
						}
					}
				}
				
			}


			// Commit to this move
			bestTranslationDelta = currentBestTranslationDelta;
			bestRotationDelta    = currentBestRotationDelta;


			return( currentBestMerit );
		}







		double STStrategyColinFaheyTwoPiece2003::PrivateStrategyNextPiece
			( 
			STBoard &  board,
			STPiece &  piece,
			int    nextPieceKind,  // (-1)==None
			int &  bestRotationDelta,    // 0 or {0,1,2,3} 
			int &  bestTranslationDelta  // 0 or {...,-2,-1,0,1,2,...}
			)
		{
			// If board or piece is invalid, return.
			if (0 == board.IsValid()) 
			{
				return(0.0);
			}

			if (0 == piece.IsValid()) 
			{
				return(0.0);
			}


			int width  = 0;
			int height = 0;
			width  = board.GetWidth();
			height = board.GetHeight();


			int    currentBestTranslationDelta = 0;
			int    currentBestRotationDelta = 0;
			double  currentBestMerit = (-1.0e20);
			int    currentBestPriority = 0;

			int    trialTranslationDelta = 0;
			int    trialRotationDelta = 0;
			double  trialMerit = 0.0;
			int    trialPriority = 0;

			int    maxOrientations        = 0;
			int    moveAcceptable         = 0;
			int    count                  = 0;


			STBoard  tempBoard;
			STPiece  tempPiece;



			maxOrientations = STPiece::GetMaxOrientations( piece.GetKind() );



			for 
				( 
				trialRotationDelta = 0; 
				trialRotationDelta < maxOrientations;
				trialRotationDelta++ 
				)
			{
				
				// Make temporary copy of piece, and rotate the copy.
				
				tempPiece.CopyFromPiece( piece );

				for ( count = 0; count < trialRotationDelta; count++ )
				{
					tempPiece.Rotate();
				}
				


				
				// Determine the translation limits for this rotated piece.
				
				int moveIsPossible = 0;
				int minDeltaX      = 0;
				int maxDeltaX      = 0;
				board.DetermineAccessibleTranslationsForPieceOrientation
					(
					tempPiece,
					moveIsPossible, // OUT: 0==NONE POSSIBLE
					minDeltaX,    // Left limit
					maxDeltaX     // Right limit
					);
				


				
				// Consider all allowed translations for the current rotation.
				
				if (0 != moveIsPossible)
				{
					for 
						( 
						trialTranslationDelta = minDeltaX;
						trialTranslationDelta <= maxDeltaX;
						trialTranslationDelta++ 
						)
					{
						// Evaluate this move

						// Copy piece to temp and rotate and translate
						tempPiece.CopyFromPiece( piece );

						for ( count = 0; count < trialRotationDelta; count++ )
						{
							tempPiece.Rotate();
						}

						tempPiece.Translate( trialTranslationDelta, 0 );

						moveAcceptable = board.IsGoalAcceptable( tempPiece );

						if (0 != moveAcceptable)
						{
							// Since the piece can be (not necessarily GET) at the goal
							// horizontal translation and orientation, it's worth trying
							// out a drop and evaluating the move.
							tempBoard.CopyFromBoard( board );


							tempBoard.FullDropAndAddPieceToBoard( tempPiece );


							trialPriority = 0;



							// Okay, now do second move with "Next Piece"
							int   nextPieceBestRotation    = 0;  // Dummy variable
							int   nextPieceBestTranslation = 0;  // Dummy variable

							STPiece  nextPiece;
							nextPiece.SetKind( nextPieceKind );
							nextPiece.SetX( tempBoard.GetPieceSpawnX()     );
							nextPiece.SetY( tempBoard.GetPieceSpawnY() - 1 );
							nextPiece.SetOrientation( 1 );

							trialMerit = 
								STStrategyColinFaheyTwoPiece2003::PrivateStrategy
								( 
								1, // not just a single ply; We are calling from a parent ply.
								tempBoard,
								nextPiece,
								nextPieceBestRotation, 
								nextPieceBestTranslation
								);



							// If this move is better than any move considered before,
							// or if this move is equally ranked but has a higher priority,
							// then update this to be our best move.
							if 
								( 
								(trialMerit > currentBestMerit) ||
								((trialMerit == currentBestMerit) && (trialPriority > currentBestPriority)) 
								)
							{
								currentBestPriority = trialPriority;
								currentBestMerit = trialMerit;
								currentBestTranslationDelta = trialTranslationDelta;
								currentBestRotationDelta = trialRotationDelta;
							}
						}
					}
				}
				
			}


			// Commit to this move
			bestTranslationDelta = currentBestTranslationDelta;
			bestRotationDelta    = currentBestRotationDelta;

			return( currentBestMerit );
		}




	}
}


