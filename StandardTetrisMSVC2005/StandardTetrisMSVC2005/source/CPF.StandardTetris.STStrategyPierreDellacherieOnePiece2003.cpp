// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



//#define DEBUGGING_PRINT_STATEMENTS



#include "CPF.StandardTetris.STStrategyPierreDellacherieOnePiece2003.h"



#include "CPF.StandardTetris.STBoard.h"
#include "CPF.StandardTetris.STPiece.h"
#include "CPF.StandardTetris.STStrategy.h"
#include "CPF.StandardTetris.STEngine.h"
#include "CPF.StandardTetris.STConsole.h"



#include <stdlib.h> // abs()

// Disable unimportant "symbol too long" error for debug builds of STL string
#pragma warning( disable : 4786 )

#include <string>
#include <vector>
using namespace std;




namespace CPF
{
	namespace StandardTetris
	{


		string STStrategyPierreDellacherieOnePiece2003::GetStrategyName ( )
		{
			return ((string)"Pierre Dellacherie (one-piece, 2003)");
		}






		// WARNING: Moves requiring rotation must wait until piece has fallen by
		// at least one row!
		// Perform all rotations, and then perform translations.  This
		// avoids the problem of getting the piece jammed on the sides
		// of the board where rotation is impossible. ***
		// Also, the following strategy does not take advantage of the
		// possibility of using free-fall and future movements to
		// slide under overhangs and fill them in.

		void STStrategyPierreDellacherieOnePiece2003::GetBestMoveOncePerPiece
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

			STStrategyPierreDellacherieOnePiece2003::PrivateStrategy
				(
				0, // 0 == not called from parent ply; Just this one ply.
				board,
				piece,
				bestRotationDelta,     // 0 or {0,1,2,3}
				bestTranslationDelta   // 0 or {...,-2,-1,0,1,2,...}
				);
		}





		double STStrategyPierreDellacherieOnePiece2003::PrivateStrategy
			( 
			int flagCalledFromParentPly,  // True if called from a parent level
			STBoard & board,
			STPiece & piece,
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




			int currentBestTranslationDelta  = 0;
			int currentBestRotationDelta = 0;
			double currentBestMerit = (-1.0e+20); // Really bad!
			int currentBestPriority = 0;

			int trialTranslationDelta = 0;
			int trialRotationDelta = 0;
			double trialMerit = 0.0;
			int trialPriority = 0;

			int moveAcceptable = 0;
			int count = 0;

			STBoard  tempBoard;
			STPiece  tempPiece;



			int maxOrientations = 0;
			maxOrientations =
				STPiece::GetMaxOrientations( piece.GetKind() );


#ifdef DEBUGGING_PRINT_STATEMENTS
			STEngine::GetConsole().AddLine( " " );
			STEngine::GetConsole().AddLine( "STStrategyPierreDellacherieOnePiece2003" );
#endif


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
					minDeltaX,    // left limit
					maxDeltaX     // right limit
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

						moveAcceptable =
							board.IsGoalAcceptable
							(
								tempPiece
							);

						if (0 != moveAcceptable)
						{
							// Since the piece can be (not necessarily GET) at the goal
							// horizontal translation and orientation, it's worth trying
							// out a drop and evaluating the move.
							tempBoard.CopyFromBoard( board );


							tempBoard.FullDropAndAddPieceToBoard
								( 
								tempPiece
								);


							// Pierre Dellacherie (France) Board & Piece Evaluation Function
							STStrategyPierreDellacherieOnePiece2003::PrivateStrategyEvaluate
								( 
								tempBoard,
								tempPiece,
								trialMerit,
								trialPriority
								);

#ifdef DEBUGGING_PRINT_STATEMENTS
							STEngine::GetConsole().AddToLastLine
								(
								"M:%5.2f R:%1d dX:%2d P:%3d",
								trialMerit,
								trialRotationDelta,
								trialTranslationDelta,
								trialPriority
								);
#endif

							// If this move is better than any move considered before,
							// or if this move is equally ranked but has a higher priority,
							// then update this to be our best move.
							if 
								( 
								(trialMerit > currentBestMerit) ||
								((trialMerit == currentBestMerit) && (trialPriority > currentBestPriority)) 
								)
							{
								currentBestMerit            = trialMerit;
								currentBestPriority         = trialPriority;
								currentBestTranslationDelta = trialTranslationDelta;
								currentBestRotationDelta    = trialRotationDelta;
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




		// The following evaluation function was adapted from Pascal code submitted by:
		// Pierre Dellacherie (France).  (E-mail : dellache@club-internet.fr)
		//
		// This amazing one-piece algorithm completes an average of roughly 600 000 
		// rows, and often attains 2 000 000 or 2 500 000 rows.  However, the algorithm
		// sometimes completes as few as 15 000 rows.  I am fairly certain that this
		// is NOT due to statistically abnormal patterns in the falling piece sequence.
		//
		// Pierre Dellacherie corresponded with me via e-mail to help me with the 
		// conversion of his Pascal code to C++.
		//
		// WARNING:
		//     If there is a single board and piece combination with the highest
		//     'rating' value, it is the best combination.  However, among
		//     board and piece combinations with EQUAL 'rating' values,
		//     the highest 'ref_priority' value wins.
		//
		//     So, the complete rating is: { rating, priority }.
		//


		void  STStrategyPierreDellacherieOnePiece2003::PrivateStrategyEvaluate
			(
			STBoard & board,
			STPiece & piece,
			double & rating,
			int & priority
			)
		{
			rating = 0.0;
			priority = 0;

			if (0 == board.IsValid()) 
			{
				return;
			}

			if (0 == piece.IsValid()) 
			{
				return;
			}



			int boardWidth = 0;
			int boardHeight = 0;
			boardWidth = board.GetWidth();
			boardHeight = board.GetHeight();




			int   erodedPieceCellsMetric = 0;
			int   boardRowTransitions    = 0;
			int   boardColTransitions    = 0;
			int   boardBuriedHoles       = 0;
			int   boardWells             = 0;

			int   pileHeight             = 0;
			int   x                      = 0;
			int   y                      = 0;






			// Landing Height (vertical midpoint)

			double landingHeight = (0.0);
			landingHeight = 
				((0.5) * ((double)piece.GetAbsoluteYMin() 
				+ (double)piece.GetAbsoluteYMax()));






			int completedRows = 0;
			completedRows = board.GetTotalCompletedRows();

			if (completedRows > 0)
			{
				// Count piece cells eroded by completed rows before doing collapse on pile.
				int pieceCellsEliminated = 0;
				pieceCellsEliminated = board.CountPieceCellsEliminated( piece );

				// Now it's okay to collapse completed rows
				board.CollapseAnyCompletedRows();

				// Weight eroded cells by completed rows
				erodedPieceCellsMetric = (completedRows * pieceCellsEliminated);
			}





			// Note that this evaluation of pile height is AFTER collapsing
			// any completed rows.
			pileHeight  = board.PileMaxHeight();
			// Each empty row (above pile height) has two (2) "transitions"
			// (We could call board.GetTransitionCountForRow( y ) for
			// these unoccupied rows, but this is an optimization.)
			boardRowTransitions = 2 * (boardHeight - pileHeight);
			// Only go up to the pile height, and later we'll account for the
			// remaining rows transitions (2 per empty row).
			for ( y = 1; y <= pileHeight; y++ )
			{
				boardRowTransitions += (board.GetTransitionCountForRow( y ));
			}






			for ( x = 1; x <= boardWidth; x++ )
			{
				boardColTransitions += (board.GetTransitionCountForColumn( x ));
				boardBuriedHoles    += (board.GetBuriedHolesForColumn( x ));
				boardWells          += (board.GetAllWellsForColumn( x ));
			}







			// Final Rating

			rating   =  ( 0.0);
			rating  += ((-1.0) * (landingHeight));
			rating  += (( 1.0) * ((double)(erodedPieceCellsMetric)));
			rating  += ((-1.0) * ((double)(boardRowTransitions)));
			rating  += ((-1.0) * ((double)(boardColTransitions)));
			rating  += ((-4.0) * ((double)(boardBuriedHoles)));
			rating  += ((-1.0) * ((double)(boardWells)));

			// EXPLANATION:
			//   [1] Punish landing height
			//   [2] Reward eroded piece cells
			//   [3] Punish row    transitions
			//   [4] Punish column transitions
			//   [5] Punish buried holes (cellars)
			//   [6] Punish wells



#ifdef DEBUGGING_PRINT_STATEMENTS
				STEngine::GetConsole().AddLine
				( 
				"D:%5.2f R:%1d RC:%3d CC:%3d H:%3d W:%3d ",
				21.0f-landingHeight,     // #1
				erodedPieceCellsMetric,  // #2
				-boardRowTransitions,    // #3
				-boardColTransitions,    // #4
				-4*boardBuriedHoles,     // #5
				-boardWells              // #6
				);
#endif



			// PRIORITY:  
			//   Priority is further differentiation between possible moves.
			//   We further rate moves accoding to the following:
			//            * Reward deviation from center of board
			//            * Reward pieces to the left of center of the board
			//            * Punish rotation
			//   Priority is less important than the rating, but among equal
			//   ratings we select the option with the greatest priority.
			//   In principle we could simply factor priority in to the rating,
			//   as long as the priority was less significant than the smallest
			//   variations in rating, but for large board widths (>100), the
			//   risk of loss of precision in the lowest bits of the rating
			//   is too much to tolerate.  So, this priority is stored in a
			//   separate variable.

			priority = 0;
			
			priority += 100 * abs( piece.GetX() - board.GetPieceSpawnX() );

			if (piece.GetX() < board.GetPieceSpawnX()) 
			{
				priority += 10;
			}
			priority -= (piece.GetOrientation() - 1);

		}




	}
}