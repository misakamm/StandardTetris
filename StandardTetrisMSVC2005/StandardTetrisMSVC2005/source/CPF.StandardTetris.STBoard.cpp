// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#include "CPF.StandardTetris.STBoard.h"


#include <algorithm>
#include <memory.h> // memset(), memcpy()
#include <stdlib.h> // rand()
#include <math.h> // log()



namespace CPF
{
	namespace StandardTetris
	{



		void  STBoard::DestroyBuffer( )
		{
			if (((int *)(0)) != m_pcells)
			{
				free( (void *)(m_pcells) );
			}

			memset( (void *)(this), (int)(0), sizeof(STBoard) );
		}



		void   STBoard::SetBoardDimensions( int width, int height )
		{
			(*this).DestroyBuffer();

			if (width  <= 0) 
			{
				return;
			}
			if (height <= 0) 
			{
				return;
			}

			int totalCells = 0;
			totalCells = (width * height);

			int * p_buffer = (int *)(0);
			p_buffer = (int *) malloc( totalCells * sizeof(int) );

			if (((int *)(0)) == p_buffer) 
			{
				return;
			}

			// Commit to member variables
			mWidth       = width;
			mHeight      = height;
			m_pcells      = p_buffer;
			mPieceSpawnX = (1 + (int)(width/2));
			mPieceSpawnY = height - 1;
		}



		STBoard::STBoard( )
		{
			memset( (void *)(this), (int)(0), sizeof(STBoard) );

			// Create board with default dimensions (10 x 20).
			(*this).SetBoardDimensions( 10, 20 );
		}



		STBoard::~STBoard( )
		{
			(*this).DestroyBuffer();
		}



		void   STBoard::ClearCells( )
		{
			if (0 == (*this).IsValid())  
			{
				return;
			}

			memset( (void *)(m_pcells), (int)(0), GetTotalBytes() );
		}



		int  STBoard::GetCellIndex( int x, int y )
		{
			// Return (-1) for all invalid cases.
			if (mWidth  <=  0) 
			{
				return(-1);
			}

			if (mHeight <=  0)
			{
				return(-1);
			}

			if (((int *)(0)) == m_pcells)
			{
				return(-1);
			}

			if (x < 1)         
			{
				return(-1);
			}

			if (x > mWidth)         
			{
				return(-1);
			}

			if (y < 1)         
			{
				return(-1);
			}

			if (y > mHeight)        
			{
				return(-1);
			}

			// All values okay
			return( (mWidth * (y-1)) + (x-1) );
		}



		int    STBoard::GetCell( int x, int y )
		{
			if (0 == (*this).IsValid())
			{
				return( 0 );
			}

			if (x        >  mWidth )
			{
				return( 0 );
			}
			if (y        >  mHeight)
			{
				return( 0 );
			}

			int index = 0;
			index = GetCellIndex( x, y );

			if (index <  0)
			{
				return( 0 ); // Outside the board is emptiness!
			}

			if (index >= GetTotalCells())
			{
				return( 0 ); // Outside the board is emptiness!
			}

			return( m_pcells[ index ] );
		}



		void   STBoard::SetCell( int x, int y, int value )
		{
			if (0 == (*this).IsValid())
			{  
				return;
			}

			if (x        >  mWidth )
			{
				return;
			}

			if (y        >  mHeight) 
			{
				return;
			}

			int index = 0;
			index = GetCellIndex( x, y );

			if (index <  0) 
			{
				return; // Outside the board is emptiness!
			}

			if (index >= GetTotalCells()) 
			{
				return; // Outside the board is emptiness!
			}

			m_pcells[ index ] = value;
		}



		void   STBoard::CopyFromBoard( STBoard & board )
		{
			// If source is invalid, destroy local buffer.
			if (0 == board.IsValid())
			{
				(*this).DestroyBuffer();
				return;
			}

			// If destination is invalid, or if the destination dimensions do not 
			// match the source dimensions, destroy the destination buffer and create a
			// new buffer with the proper dimensions.
			if 
				( 
				(0  ==  (*this).IsValid() ) ||
				(mWidth  != board.mWidth ) ||
				(mHeight != board.mHeight) 
				)
			{
				(*this).DestroyBuffer();
				(*this).SetBoardDimensions
					(
					board.mWidth, 
					board.mHeight
					);
			}

			// At this point the destination should be suitable for containing a copy of
			// the source.
			if (0 != (*this).IsValid())
			{
				memcpy
					( 
					(void *      )(m_pcells),  // Destination
					(const void *)(board.m_pcells),  // Source
					(int         )(GetTotalBytes())          // Total Bytes
					);
			}
		}



		// Collapse and return total completed rows
		int    STBoard::CollapseAnyCompletedRows( )
		{
			// Check board
			if (0 == (*this).IsValid())  
			{
				return( 0 );
			}


			int totalRowsCollapsed = 0;

            // The following algorithm is space-efficient, but not time-efficient; it's
            // designed to be reliable and easy to understand:
            //
            //   Scan each row of the current board, starting from the bottom of the pile.
            //   If any row is completely filled, then "eliminate" the row by collapsing
            //   all rows above the complete row down to fill the gap.  Note that we must
            //   check the same row again if we do a collapse.

			int y = 0;

			for ( y = 1; y < mHeight; y++ ) // bottom-to-top (except top row)
			{
				// Check if the row is full
				int x = 0;
				int rowIsFull = 1; // Hypothesis
				for ( x = 1; ((x <= mWidth) && (0 != rowIsFull)); x++ )
				{
					int cellValue = GetCell( x, y );
					if (0 == cellValue)  rowIsFull = 0;
				}


				// If this row is complete, update statistics, and collapse pile down
				if (0 != rowIsFull)
				{
					totalRowsCollapsed++; // We completed a row!

					int copySourceX = 0;
					int copySourceY = 0;

					for 
						( 
						copySourceY = (y + 1); 
					copySourceY <= mHeight; 
					copySourceY++ 
						)
					{
						for 
							( 
							copySourceX = 1; 
						copySourceX <= mWidth; 
						copySourceX++ 
							)
						{
							int cellValue = GetCell( copySourceX, copySourceY );
							SetCell( copySourceX, (copySourceY - 1),  cellValue );
						}
					}

					// Clear top row ("copy" from infinite emptiness above board)
					for 
						( 
						copySourceX = 1; 
					copySourceX <= mWidth; 
					copySourceX++ 
						)
					{
						SetCell( copySourceX, mHeight, 0 );
					}

					y--; // Force the same row to be checked again
				}

			}

			return( totalRowsCollapsed );
		}



		void   STBoard::RandomCells( )
		{
			if (0 == (*this).IsValid())  
			{
				return;
			}

			int n = 0;
			int i = 0;
			n = GetTotalCells();
			for ( i = 0; i < n; i++ )
			{
				m_pcells[ i ] = (rand() % 8); // {0==EMPTY, {1..7}==PIECE}
			}
		}



		void  STBoard::FillBoardWithTestPattern( int patternIndex )
		{
			if (0 == (*this).IsValid())  
			{
				return;
			}

			int x         = 0;
			int y         = 0;
			int value     = 0;
			int maxHeight = 0;


			int state = patternIndex;

			maxHeight = (1 + (patternIndex % mHeight));

			for ( y = 1; y <= maxHeight; y++ )
			{
				for ( x = 1; x <= mWidth; x++ )
				{
					value = 0;

					if ((patternIndex >> ((x - 1) % 8)) & 0x1)
					{ 
						value = 8;
					}

					if ((patternIndex >> ((y - 1) % 8)) & 0x1) 
					{
						value = (8 - value);
					}

					(*this).SetCell( x, y, value );
				}
			}

			for ( y = (maxHeight+1); y <= mHeight; y++ )
			{
				for ( x = 1; x <= mWidth; x++ )
				{
					(*this).SetCell( x, y, 0 );
				}
			}

		}



		// 1..mHeight
		void  STBoard::ClearRow( int y )
		{
			if (0 == (*this).IsValid())  
			{
				return;
			}

			int x = 0;
			for ( x = 1; x <= mWidth; x++ )
			{
				(*this).SetCell( x, y, 0 );
			}
		}



		void  STBoard::LiftPileByOneRowAndAddRandomJunk( )
		{
			// First, do top-down row copying to raise all rows by one row, with the
			// top row being lost.
			int cell = 0;
			int x = 0;
			int y = 0;
			for ( y = mHeight; y > 1; y-- )  // Top-down
			{
				// Copy row (y-1) to row y; i.e., copy upward.
				for ( x = 1; x <= mWidth; x++ )
				{
					cell = (*this).GetCell( x, (y-1)          );
					(*this).SetCell(  x,  y,  cell  );
				}
			}
			// Put random junk in bottom row (row 1).
			for ( x = 1; x <= mWidth; x++ )
			{
				cell = 0; // 0==EMPTY
				if (rand() % 3)  cell = 8; // 8==JUNK CELL
				(*this).SetCell( x,  1,  cell );
			}
		}



		// 0..(mWidth * mHeight)
		int    STBoard::TotalOccupiedCells ( )
		{
			if (0 == (*this).IsValid())  
			{
				return( 0 );
			}

			int totalOccupiedCells = 0;

			int n = 0;
			n = GetTotalCells();

			int i = 0;
			for ( i = 0; i < n; i++ )
			{
				if (0 != m_pcells[ i ])  
				{
					totalOccupiedCells++;
				}
			}

			return( totalOccupiedCells );
		}



		int    STBoard::TotalShadowedHoles( )
		{
			if (0 == (*this).IsValid())  
			{
				return( 0 );
			}

			int totalShadowedHoles = 0;

			// For each column we search top-down through the rows,
			// noting the first occupied cell, and counting any
			// unoccupied cells after that point...
			int x = 0;
			for ( x = 1; x <= mWidth; x++ )
			{
				int encounteredOccupiedCell = 0;
				int y = 0;
				for ( y = mHeight; y >= 1; y-- ) // Top-to-Bottom
				{
					int cellValue = 0;
					cellValue = GetCell( x, y );

					if (0 != cellValue)
					{
						encounteredOccupiedCell = 1;
					}
					else
					{
						// Cell is un-occupied...
						if (0 != encounteredOccupiedCell)
						{
							// ...and we already encountered an occupied cell above,
							// so this counts as a hole.
							totalShadowedHoles++;
						}
					}
				}
			}

			return( totalShadowedHoles );
		}

		// 0..(mHeight)
		int    STBoard::PileAvgHeight( )
		{
			if (0 == (*this).IsValid())  
			{
				return( 0 );
			}

			int x = 0;
			int y = 1024;
			int ym[128] = {0};

			// Top-down search for non-empty cell...
			for ( x = 1; x <= mWidth; x++ )
			{
				ym[x] = GetColumnHeight(x);
			}
			for ( x = 1; x <= mWidth; x++ )
			{
				if ( ym[x] < ym[x-1] && ym[x] < ym[x+1] ) ym[x] = std::min(ym[x-1], ym[x+1]) - 1;
			}
			for ( x = 1; x <= mWidth; x++ )
			{
				if ( y > ym[x] ) y = ym[x];
			}

			return y; // entire board is empty
		}

		double    STBoard::PileHeightBeta( LongLong HeightHistogram[], int* Samples )
		{
			double
				sum_xy = 0,
				sum_x  = 0,
				sum_y  = 0,
				sum_x2 = 0;
			int n = 0;
			for ( int x = 2; HeightHistogram[x] >= 100; ++x )
			{
				sum_xy += x * log((double)HeightHistogram[x]);
				sum_x += x;
				sum_y += log((double)HeightHistogram[x]);
				sum_x2 += x * x;
				++n;
			}
			*Samples = 0;
			if ( n < 2 ) return 0;
			*Samples = n;
			return exp( -(n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x));
		}


		// 0..(mHeight)
		int    STBoard::PileMaxHeight( )
		{
			if (0 == (*this).IsValid())  
			{
				return( 0 );
			}

			int x = 0;
			int y = 0;

			// Top-down search for non-empty cell...
			for ( y = mHeight; y >= 1; y-- )
			{
				for ( x = 1; x <= mWidth; x++ )
				{
					int cellValue = 0;
					cellValue = (*this).GetCell( x, y );
					if (0 != cellValue)  
					{
						return( y );
					}
				}
			}

			return( 0 ); // entire board is empty
		}



		int    STBoard::PileHeightWeightedCells( )
		{
			if (0 == (*this).IsValid())  
			{
				return( 0 );
			}

			int x = 0;
			int y = 0;

			int totalWeightedCells = 0;

			for ( y = 1; y <= mHeight; y++ )
			{
				for ( x = 1; x <= mWidth; x++ )
				{
					int cellValue = 0;
					cellValue = (*this).GetCell( x, y );
					if (0 != cellValue)  
					{
						totalWeightedCells += y;
					}
				}
			}

			return( totalWeightedCells );
		}



		// 0..(mHeight)
		int    STBoard::GetColumnHeight( int x )
		{
			if (0 == (*this).IsValid())  
			{
				return( 0 );
			}

			if (x <= 0) 
			{
				return( 0 );
			}

			if (x > mWidth)
			{
				return( 0 );
			}

			// Top-Down Search for first occupied cell
			int y = 0;
			for ( y = mHeight; y >= 1; y-- ) // top-down search
			{
				int cellValue = (*this).GetCell( x, y );
				if (0 != cellValue)
				{
					return( y );
				}
			}
			return( 0 );
		}



		int    STBoard::SumOfWellHeights( )  
		{
			if (0 == (*this).IsValid())
			{
				return( 0 );
			}

			int  sumOfWellHeights    = 0;
			int  columnHeight        = 0;
			int  columnHeightToLeft  = 0;
			int  columnHeightToRight = 0;

			// Determine height of LEFT  well
			columnHeight        = (*this).GetColumnHeight( 1 );
			columnHeightToRight = (*this).GetColumnHeight( 2 );
			if (columnHeightToRight > columnHeight)
			{
				sumOfWellHeights += (columnHeightToRight - columnHeight);
			}

			// Determine height of RIGHT well
			columnHeightToLeft = (*this).GetColumnHeight( mWidth - 1 );
			columnHeight       = (*this).GetColumnHeight( mWidth     );
			if (columnHeightToLeft > columnHeight)
			{
				sumOfWellHeights += (columnHeightToLeft - columnHeight);
			}


			// Now do all interior columns, 1..8, which have TWO
			// adjacent lines...
			int x = 0;
			for ( x = 2; x <= (mWidth - 1); x++ )
			{
				columnHeightToLeft  = (*this).GetColumnHeight( x - 1 );
				columnHeight        = (*this).GetColumnHeight( x     );
				columnHeightToRight = (*this).GetColumnHeight( x + 1 );

				if (columnHeightToLeft > columnHeightToRight)
				{
					columnHeightToLeft = columnHeightToRight;
				}
				else
				{
					columnHeightToRight = columnHeightToLeft;
				}

				if (columnHeightToLeft > columnHeight)
				{
					sumOfWellHeights += (columnHeightToLeft - columnHeight);
				}
			}

			return( sumOfWellHeights );
		}



		// 0..mHeight
		int  STBoard::GetTotalCompletedRows( )
		{
			if (0 == (*this).IsValid())  
			{
				return( 0 );
			}

			int totalCompletedRows = 0;

			int x = 0;
			int y = 0;

			// Check each row
			for ( y = 1; y <= mHeight; y++ )
			{
				// Check if this row is full.
				int rowIsFull = 1; // Hypothesis
				for ( x = 1; ( (x <= mWidth) && (0 != rowIsFull) ); x++ )
				{
					int cellValue = 0;
					cellValue = GetCell( x, y );
					if (0 == cellValue)  
					{
						rowIsFull = 0;
					}
				}

				if (0 != rowIsFull) 
				{
					totalCompletedRows++;
				}
			}

			return( totalCompletedRows );
		}



		// 0..mWidth
		int  STBoard::GetTransitionCountForRow( int y )
		{
			if (0 == (*this).IsValid())  
			{
				return( 0 );
			}

			int transitionCount = 0;

			int x     = 0;
			int flagA = 0;
			int flagB = 0;

			// Check cell and neighbor to right...
			for ( x = 1; x < mWidth; x++ )
			{
				flagA = GetCell( x , y );
				flagB = GetCell( (x + 1), y );

				// If a transition from occupied to unoccupied, or
				// from unoccupied to occupied, then it's a transition.
				if 
					( 
					((0 != flagA) && (0 == flagB)) ||
					((0 == flagA) && (0 != flagB)) 
					)
				{
					transitionCount++;
				}
			}

			// Check transition between left-exterior and column 1.
			// (NOTE: Exterior is implicitly "occupied".)
			flagA = GetCell( 1, y );
			if (0 == flagA) 
			{
				transitionCount++;
			}

			// Check transition between column 'mWidth' and right-exterior.
			// (NOTE: Exterior is implicitly "occupied".)
			flagA = GetCell( mWidth, y );
			if (0 == flagA) 
			{
				transitionCount++;
			}

			return( transitionCount );
		}



		// 1..(mHeight+1)
		int  STBoard::GetTransitionCountForColumn( int x )
		{
			if (0 == (*this).IsValid())  
			{
				return( 0 );
			}

			int transitionCount = 0;

			int y     = 0;
			int flagA = 0;
			int flagB = 0;

			// Check cell and neighbor above...
			for ( y = 1; y < mHeight; y++ )
			{
				flagA = GetCell( x, y     );
				flagB = GetCell( x, y + 1 );
				// If a transition from occupied to unoccupied, or
				// from unoccupied to occupied, it's a transition.
				if 
					( 
					((0 != flagA) && (0 == flagB)) ||
					((0 == flagA) && (0 != flagB)) 
					)
				{
					transitionCount++;
				}
			}

			// Check transition between bottom-exterior and row Y=1.
			// (NOTE: Bottom exterior is implicitly "occupied".)
			flagA = GetCell( x, 1 );
			if (0 == flagA) 
			{
				transitionCount++;
			}

			// Check transition between column 'mHeight' and above-exterior.
			// (NOTE: Sky above is implicitly UN-"occupied".)
			flagA = GetCell( x, mHeight );
			if (0 != flagA) 
			{
				transitionCount++;
			}

			return( transitionCount );
		}



		// 0..(mHeight-1)
		int  STBoard::GetBuriedHolesForColumn( int x )
		{
			if (0 == (*this).IsValid())  
			{
				return( 0 );
			}

			int totalHoles = 0;
			int cell   = 0;
			int enable = 0;
			int y      = 0;

			for ( y = mHeight; y >= 1; y-- )
			{
				cell = GetCell( x, y );

				if (0 != cell)
				{
					enable = 1;
				}
				else
				{
					if (0 != enable)
					{
						totalHoles++;
					}
				}
			}

			return( totalHoles );
		}



		// 0..topY
		int  STBoard::GetBlanksDownBeforeBlockedForColumn
			(
			int x,
			int topY 
			)
		{
			if (0 == (*this).IsValid())  
			{
				return( 0 );
			}

			int totalBlanksBeforeBlocked = 0;
			int cell = 0;
			int y = 0;

			for ( y = topY; y >= 1; y-- )
			{
				cell = GetCell( x, y );

				if (0 != cell)
				{
					return( totalBlanksBeforeBlocked );
				}
				else
				{
					totalBlanksBeforeBlocked++;
				}
			}

			return( totalBlanksBeforeBlocked );
		}





		// 0..O(mHeight*mHeight)
		int  STBoard::GetAllWellsForColumn( int x )
		{
			if (0 == (*this).IsValid())
			{
				return( 0 );
			}

			int wellValue  = 0;
			int y          = 0;
			int cellLeft   = 0;
			int cellRight  = 0;
			int blanksDown = 0;

			for ( y = mHeight; y >= 1; y-- )
			{
				if ((x-1) >= 1)
				{
					cellLeft  = GetCell( (x - 1), y );
				}
				else
				{
					cellLeft = 1;
				}

				if ((x + 1) <= mWidth)
				{
					cellRight = GetCell( (x + 1), y );
				}
				else
				{
					cellRight = 1;
				}

				if ((0 != cellLeft) && (0 != cellRight))
				{
					blanksDown = (*this).GetBlanksDownBeforeBlockedForColumn( x, y );
					wellValue += blanksDown;
				}
			}

			return( wellValue );
		}




		int  STBoard::IsGoalAcceptable
			(
			STPiece &  piece
			)
		{
			if (0 == (*this).IsValid()) 
			{
				return( 0 );
			}

			// If piece kind or orientation is invalid, goal is not acceptable.
			if (0 == piece.IsValid()) 
			{
				return( 0 );
			}

			// Fast check: If piece origin lies outside board, goal is not acceptable.
			if (piece.GetX() < 1) 
			{
				return( 0 );
			}

			if (piece.GetY() < 1) 
			{
				return( 0 );
			}

			if (piece.GetX() > (*this).GetWidth ()) 
			{
				return( 0 );
			}

			if (piece.GetY() > (*this).GetHeight()) 
			{
				return( 0 );
			}

			// Consider the absolute position of all points of the piece.
			// If any of the points lie outside the board, goal is not acceptable.
			// If any of the points coincide with an occupied cell of the board,
			//  the goal is not acceptable.
			int point       = 0;
			int totalPoints = 0;
			int absoluteX   = 0;
			int absoluteY   = 0;
			totalPoints = piece.GetTotalPoints();
			for ( point = 1; point <= totalPoints; point++ )
			{
				absoluteX = piece.GetAbsolutePointX( point );
				absoluteY = piece.GetAbsolutePointY( point );
				// Absolute point must be in the board area
				if (absoluteX < 1) 
				{
					return( 0 );
				}

				if (absoluteX > (*this).GetWidth ())
				{
					return( 0 );
				}

				if (absoluteY < 1)
				{
					return( 0 );
				}

				if (absoluteY > (*this).GetHeight()) 
				{
					return( 0 );
				}

				// Absolute point cannot overlap an occupied cell of the board.
				if (0 != (*this).GetCell( absoluteX, absoluteY ))
				{
					return( 0 );
				}
			}

			// If we made it to this point, the goal is acceptable.
			return( 1 );
		}





		void STBoard::DetermineAccessibleTranslationsForPieceOrientation
			(
			STPiece &  piece,
			int & ref_movePossible, // OUT: 0==NONE POSSIBLE
			int & ref_minDeltaX,    // Left limit
			int & ref_maxDeltaX     // Right limit
			)
		{
			// Clear Results.
			ref_movePossible = 0;
			ref_minDeltaX    = 0;
			ref_maxDeltaX    = 0;


			if (0 == (*this).IsValid()) 
			{
				return;
			}

			if (0 == piece.IsValid()) 
			{
				return;
			}


			int width  = 0;
			int height = 0;
			width  = (*this).GetWidth();
			height = (*this).GetHeight();


			STPiece  temp_Piece;
			int moveAcceptable = 0;
			int trialTranslationDelta = 0;


			// Check if we can move at all.
			moveAcceptable = (*this).IsGoalAcceptable( piece );
			if (0 != moveAcceptable)
			{
				ref_movePossible = 1;
			}
			else
			{
				return;
			}



			// Scan from center to left to find left limit.
			int stillAcceptable = 1;
			for 
				( 
				trialTranslationDelta = 0;
			( (trialTranslationDelta >= (-(width))) && (stillAcceptable) );
			trialTranslationDelta-- 
				)
			{
				// Copy piece to temp and translate
				temp_Piece.CopyFromPiece( piece );
				temp_Piece.Translate( trialTranslationDelta, 0 );

				moveAcceptable =
					(*this).IsGoalAcceptable( temp_Piece );
				if (0 != moveAcceptable)
				{
					ref_minDeltaX = trialTranslationDelta;
				}
				else
				{
					stillAcceptable = 0;
				}
			}


			// Scan from center to right to find right limit.
			stillAcceptable = 1;
			for 
				( 
				trialTranslationDelta = 0;
			( (trialTranslationDelta <= width) && (stillAcceptable) );
			trialTranslationDelta++ 
				)
			{
				// Copy piece to temp and translate
				temp_Piece.CopyFromPiece( piece );
				temp_Piece.Translate( trialTranslationDelta, 0 );

				moveAcceptable =
					(*this).IsGoalAcceptable( temp_Piece );
				if (0 != moveAcceptable)
				{
					ref_maxDeltaX = trialTranslationDelta;
				}
				else
				{
					stillAcceptable = 0;
				}
			}
		}





		void  STBoard::AddPieceToBoard
			( 
			STPiece &  piece
			)
		{
			if (0 == (*this).IsValid()) 
			{
				return;
			}

			// If piece kind or orientation is invalid, adding is not acceptable.
			if (0 == piece.IsValid())
			{
				return;
			}

			// Fast check: If piece origin lies outside board, adding is not acceptable.
			if (piece.GetX() < 1) 
			{
				return;
			}

			if (piece.GetY() < 1) 
			{
				return;
			}

			if (piece.GetX() > (*this).GetWidth ()) 
			{
				return;
			}

			if (piece.GetY() > (*this).GetHeight()) 
			{
				return;
			}

			// Consider the absolute position of all points of the piece, and set the
			// corresponding cells on the board.
			int point       = 0;
			int totalPoints = 0;
			int absoluteX   = 0;
			int absoluteY   = 0;
			totalPoints = piece.GetTotalPoints();
			for ( point = 1; point <= totalPoints; point++ )
			{
				absoluteX = piece.GetAbsolutePointX( point );
				absoluteY = piece.GetAbsolutePointY( point );
				// Set absolute position on the board.
				// (NOTE: Board will silently discard points with invalid (x,y).)
				(*this).SetCell( absoluteX, absoluteY, piece.GetKind() );
			}
		}





		void STBoard::DropPieceButBoardUnchanged
			( 
			STPiece &  piece
			)
		{
			if (0 == (*this).IsValid())
			{
				return;
			}

			if (0 == piece.IsValid())
			{
				return;
			}


			// Special case: Cannot place piece at starting location.
			int goalAcceptable = 0;

			goalAcceptable = 
				(*this).IsGoalAcceptable( piece );

			if (0 == goalAcceptable)
			{
				// No dropping at all!
				return;
			}


			// Try successively larger drop distances, up to the point of failure.
			// The last successful drop distance becomes our drop distance.
			int boardHeight      = 0;
			int lastSuccessfulDropDistance = 0;
			int firstFailureEncountered    = 0;
			int trialDropDistance          = 0;

			boardHeight = (*this).GetHeight();

			STPiece  temp_Piece = piece;

			for 
				( 
				trialDropDistance = 0;
				( (0 == firstFailureEncountered) && (trialDropDistance <= boardHeight) );
				trialDropDistance++ 
				)
			{
				// Set temporary piece to new trial Y
				temp_Piece.SetY( piece.GetY() - trialDropDistance );

				goalAcceptable = (*this).IsGoalAcceptable( temp_Piece );

				if (0 == goalAcceptable)
				{
					// We failed to drop this far.  Stop drop search.
					firstFailureEncountered = 1;
				}
				else
				{
					lastSuccessfulDropDistance = trialDropDistance;
				}
			}
			// Simply update the piece Y value.
			piece.SetY( piece.GetY() - lastSuccessfulDropDistance );
		}





		// WARNING: Will unconditionally add, even if putting the piece down at the
		// starting location (let alone dropping) is impossible!
		void STBoard::FullDropAndAddPieceToBoard
			( 
			STPiece &  piece
			)
		{
			if (0 == (*this).IsValid())
			{
				return;
			}

			if (0 == piece.IsValid())
			{
				return;
			}


			// Drop piece as far as it will go.  A drop of zero distance is possible.
			(*this).DropPieceButBoardUnchanged( piece );

			// Simply add the piece to the board.
			(*this).AddPieceToBoard( piece );
		}





		// The following counts the number of cells (0..4) of a piece that would
		// be eliminated by dropping the piece.
		int STBoard::CountPieceCellsEliminated
			( 
			STPiece &  piece
			)
		{
			if (0 == (*this).IsValid())
			{
				return( 0 );
			}

			if (0 == piece.IsValid())
			{
				return( 0 );
			}


			// Copy piece and board so that this measurement is not destructive.
			STBoard   boardCopy;
			STPiece   PieceCopy;
			boardCopy.CopyFromBoard( (*this) );
			PieceCopy.CopyFromPiece( piece );

			// Drop piece on to the board copy
			boardCopy.FullDropAndAddPieceToBoard( PieceCopy );

			// Scan rows.  For each completed row, check all absolute Y values for the
			// piece.  If any absolute Y of the piece matches the completed row Y,
			// increment the total eliminated cells.
			int pieceCellsEliminated = 0;

			int y  = 0;
			int x  = 0;
			int rowCompleted = 0;
			int totalPoints  = 0;
			int point        = 0;
			int pointY       = 0;
			int width        = 0;
			int height       = 0;
			int cell         = 0;

			width  = boardCopy.GetWidth();
			height = boardCopy.GetHeight();

			totalPoints = piece.GetTotalPoints();

			for ( y = 1; y <= height; y++ )
			{

				rowCompleted = 1; // Hypothesis

				for ( x = 1; ((x <= width) && (0 != rowCompleted)); x++ )
				{
					cell = boardCopy.GetCell( x, y );
					if (0 == cell) 
					{
						rowCompleted = 0;     
					}
				}

				if (0 != rowCompleted)
				{
					// Find any matching absolute Y values in piece.
					for ( point = 1; point <= totalPoints; point++ )
					{
						pointY = piece.GetAbsolutePointY( point );
						if (pointY == y)  
						{
							pieceCellsEliminated++;  // Moohahahaaa!
						}
					}
				}
			}

			return( pieceCellsEliminated );
		}




	}
}
