// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STBoardHeader
#define STBoardHeader


#include "CPF.StandardTetris.STPiece.h"




namespace CPF
{
	namespace StandardTetris
	{


		// LOGICAL CHARACTERISTICS:		
		//
		//   * The horizontal axis is called "X" and points to the right,
		//     and has an origin such that the left column is called X = 1,
		//     and the right column is called X = Width.
		//
		//   * The vertical axis is called "Y" and points upward, and has
		//     an origin such that the bottom row is called Y = 1, and the
		//     top row is called Y = Height.
		//
		//    * Each piece spawns with its origin coincident with the cell
		//         (   X = (1 + (int)(Width/2)),    Y = Height   ).
		//
		//    * The standard width is 10, and the standard height is 20.
		//
		// IMPLEMENTATION:
		//
		// The board is stored in a one-dimensional array of integers, arranged
		// such that cells are accessed according to the following index:
		//
		//    Cell Index   =   (Width * (Y - 1))    +    (X - 1);
		//
		// The array has (Width * Height) integers total.  The first integer
		// corresponds to the cell (1,1), and the last integer corresponds to
		// the cell (Width, Height).



		class STBoard
		{
		private: int     mWidth;        // Width  is 10 for standard Tetris play
		private: int     mHeight;       // Height is 20 for standard Tetris play
		private: int *   m_pcells;       // Allocated upon construction and size changes
		private: int     mPieceSpawnX;  // For a board width  of 10, Spawn X =  6
		private: int     mPieceSpawnY;  // For a board height of 20, Spawn Y = 20



				 // Get Properties
		public: int  IsValid( )
				{
					if (mWidth  <= 0           ) return( 0 );
					if (mHeight <= 0           ) return( 0 );
					if (((int *)(0)) == m_pcells) return( 0 );
					return(1);
				}
		public: int  GetWidth      ( ) { return( mWidth       ); }
		public: int  GetHeight     ( ) { return( mHeight      ); }
		public: int  GetPieceSpawnX( ) { return( mPieceSpawnX ); }
		public: int  GetPieceSpawnY( ) { return( mPieceSpawnY ); }
		public: int  GetTotalCells ( ) { return( mWidth * mHeight ); }
		public: int  GetTotalBytes ( )
				{ return( mWidth * mHeight * sizeof(int) ); }

		public: int  GetCellIndex( int x, int y ); // (-1) if invalid.


				// Fundamental Operations
		public: STBoard();
		public: ~STBoard();
		public: void   DestroyBuffer( );
		public: void   SetBoardDimensions( int width, int height );
		public: void   ClearCells( );
		public: int    GetCell( int x, int y );
		public: void   SetCell( int x, int y, int value );
		public: void   CopyFromBoard( STBoard & board );

				// Advanced Operations
		public: int    CollapseAnyCompletedRows( ); // Collapse and return total completed rows
		public: void   RandomCells( );
		public: void   ClearRow( int y ); // 1..mHeight
		public: void   LiftPileByOneRowAndAddRandomJunk( );
		public: void   FillBoardWithTestPattern( int patternIndex );

				// Board Statistics
		public: int    TotalOccupiedCells ( );      // 0..(mWidth * mHeight)
		public: int    TotalShadowedHoles ( );     
		public: int    PileAvgHeight      ( );      // 0..mHeight
		public: int    PileMaxHeight      ( );      // 0..mHeight
		public: double PileHeightBeta( LongLong HeightHistogram[], int* Samples );
		public: int    PileHeightWeightedCells( ); 
		public: int    GetColumnHeight    ( int x ); // 0..mHeight
		public: int    SumOfWellHeights   ( );     

		public: int    GetTotalCompletedRows( ); // 0..mHeight
		public: int    GetTransitionCountForRow( int y ); // 0..mWidth
		public: int    GetTransitionCountForColumn( int x ); // 1..(mHeight+1)
		public: int    GetBuriedHolesForColumn( int x ); // 0..(mHeight-1)
		public: int    GetBlanksDownBeforeBlockedForColumn( int x, int topY ); // 0..topY
		public: int    GetAllWellsForColumn( int x ); // 0..O(mHeight*mHeight)


		public: int IsGoalAcceptable
					(
					STPiece &  piece
					);

		public: void DetermineAccessibleTranslationsForPieceOrientation
					(
					STPiece &  piece,
					int &                    ref_movePossible, // OUT: 0==NONE POSSIBLE
					int &                    ref_minDeltaX,    // Left limit
					int &                    ref_maxDeltaX     // Right limit
					);

		public:  void AddPieceToBoard
					 ( 
					 STPiece &  piece
					 );

		public:  void DropPieceButBoardUnchanged
					 ( 
					 STPiece &  piece
					 );

				 // The following counts the number of cells (0..4) of a piece that would
				 // be eliminated by dropping the piece.
		public:  int CountPieceCellsEliminated
					 ( 
					 STPiece &  piece
					 );

				 // WARNING: Will unconditionally add, even if putting the piece down at the
				 // starting location (let alone dropping) is impossible!
		public:  void FullDropAndAddPieceToBoard
					 ( 
					 STPiece &  piece
					 );

		};



	}
}



#endif





