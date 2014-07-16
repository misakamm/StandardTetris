// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#include "CPF.StandardTetris.STPiece.h"



#include <memory.h>  // memset(), memcpy()



namespace CPF
{
	namespace StandardTetris
	{



		void  STPiece::Clear( )
		{
			// Acceptable use of memset().
			memset( (void *)(this), (int)(0), sizeof(STPiece) );
		}







		STPiece::STPiece( )
		{
			// Set all bytes to zero.
			(*this).Clear();
		}







		void  STPiece::CopyFromPiece( STPiece & piece )
		{
			memcpy
				(
				(void *      )(this),          // Destination
				(const void *)(&(piece)),  // Source
				sizeof(STPiece)    // Total Bytes
				);
		}







		int  STPiece::GetKind(  )
		{
			// 1,2,3,4,5,6,7 (O,I,S,Z,L,J,T); 0==None
			// If mKind is corrupted, fix it. 
			if ((mKind < 0) || (mKind > 7))
			{
				mKind = 0;
			}
			return( mKind );
		}







		void STPiece::SetKind
			(
			int kind   // 1,2,3,4,5,6,7 (O,I,S,Z,L,J,T); 0==None
			)
		{
			mKind = kind;

			// If mKind is corrupted, fix it. 
			if ((mKind < 0) || (mKind > 7))
			{
				mKind = 0;
			}
		}







		int  STPiece::GetX(  )
		{
			return( mX );
		}







		int  STPiece::GetY(  )
		{
			return( mY );
		}







		void STPiece::SetX( int x )
		{
			mX = x;
		}







		void STPiece::SetY( int y )
		{
			mY = y;
		}







		int  STPiece::GetOrientation(  )
		{
			// 1,2,3,4 (0,1/4,2/4,3/4-"turn"); 0==None
			// If mOrientation is corrupted, fix it. 
			if ((mOrientation < 0) || (mOrientation > 4))
			{
				mOrientation = 0;
			}
			return( mOrientation );
		}







		void STPiece::SetOrientation
			(
			int orientation // Orientation=1,2,3,4 (0,1/4,2/4,3/4-"turn"); 0==None
			)
		{
			mOrientation = orientation;

			// If mOrientation is corrupted, fix it. 
			if ((mOrientation < 0) || (mOrientation > 4))
			{
				mOrientation = 0;
			}
		}







		int STPiece::GetAbsolutePointX
			(
			int point // Point=1,2,3,4; 0==NONE
			)
		{
			if (point < 1) return(0);
			if (point > 4) return(0);
			return( mX + STPiece::GetCellOffsetX(mKind, mOrientation, point) );
		}







		int STPiece::GetAbsolutePointY
			(
			int point // Point=1,2,3,4; 0==NONE
			)
		{
			if (point < 1) return(0);
			if (point > 4) return(0);
			return( mY + STPiece::GetCellOffsetY(mKind, mOrientation, point) );
		}







		int  STPiece::GetAbsoluteXMin(  )
		{
			// Only do this if piece is currently valid.
			if (0 == (*this).IsValid()) return(0);

			int totalPoints = 0;
			totalPoints = STPiece::GetTotalPoints();
			if (totalPoints < 1) return(0);  // No points!

			// Get absolute X coordinate of first point.
			int x    = 0;
			x = (*this).GetAbsolutePointX( 1 );

			// Set minimum to first X value, as a hypothesis.
			int xMin = 0;
			xMin = x;

			// Loop through remaining points to find minimum absolute X.
			int point = 0;
			for ( point = 2; point <= totalPoints; point++ )
			{
				x = (*this).GetAbsolutePointX( point );
				if (x < xMin)  xMin = x;
			}

			return( xMin );
		}







		int  STPiece::GetAbsoluteXMax(  )
		{
			// Only do this if piece is currently valid.
			if (0 == (*this).IsValid()) return(0);

			int totalPoints = 0;
			totalPoints = STPiece::GetTotalPoints();
			if (totalPoints < 1) return(0);  // No points!

			// Get absolute X coordinate of first point.
			int x    = 0;
			x = (*this).GetAbsolutePointX( 1 );

			// Set maximum to first X value, as a hypothesis.
			int xMax = 0;
			xMax = x;

			// Loop through remaining points to find maximum absolute X.
			int point = 0;
			for ( point = 2; point <= totalPoints; point++ )
			{
				x = (*this).GetAbsolutePointX( point );
				if (x > xMax)  xMax = x;
			}

			return( xMax );
		}







		int  STPiece::GetAbsoluteYMin(  )
		{
			// Only do this if piece is currently valid.
			if (0 == (*this).IsValid()) return(0);

			int totalPoints = 0;
			totalPoints = STPiece::GetTotalPoints();
			if (totalPoints < 1) return(0);  // No points!

			// Get absolute Y coordinate of first point.
			int y    = 0;
			y = (*this).GetAbsolutePointY( 1 );

			// Set minimum to first Y value, as a hypothesis.
			int yMin = 0;
			yMin = y;

			// Loop through remaining points to find minimum absolute Y.
			int point = 0;
			for ( point = 2; point <= totalPoints; point++ )
			{
				y = (*this).GetAbsolutePointY( point );
				if (y < yMin)  yMin = y;
			}

			return( yMin );
		}







		int  STPiece::GetAbsoluteYMax(  )
		{
			// Only do this if piece is currently valid.
			if (0 == (*this).IsValid()) return(0);

			int totalPoints = 0;
			totalPoints = STPiece::GetTotalPoints();
			if (totalPoints < 1) return(0);  // No points!

			// Get absolute Y coordinate of first point.
			int y    = 0;
			y = (*this).GetAbsolutePointY( 1 );

			// Set maximum to first Y value, as a hypothesis.
			int yMax = 0;
			yMax = y;

			// Loop through remaining points to find maximum absolute Y.
			int point = 0;
			for ( point = 2; point <= totalPoints; point++ )
			{
				y = (*this).GetAbsolutePointY( point );
				if (y > yMax)  yMax = y;
			}

			return( yMax );
		}







		int   STPiece::IsValid(  )
		{
			if ((mKind        < 1) || (mKind        > 7)) return(0);
			if ((mOrientation < 1) || (mOrientation > 4)) return(0);
			return(1);
		}







		void  STPiece::Rotate(  )
		{
			// Only rotate if currently valid.
			if (0 == (*this).IsValid()) return;

			// First, retrieve maximum non-redundant orientation values
			int maxOrientations = 0;
			maxOrientations = STPiece::GetMaxOrientations( mKind );
			if (0 == maxOrientations) return;

			// Modulo the maximum number of non-redundant orientations, we increment.
			mOrientation = ((mOrientation % maxOrientations) + 1);
			// NOTE: This formula is essentially (((i++)-1)%N)+1 = (i%N)+1.
		}







		// Essentially multiple Rotate commands
		void  STPiece::RotateByCount( int count )
		{
			int step  = 0;
			int total = 0;

			total = count;  // Copy number (can be negative, zero, or positive)
			total %= 4; // Modulo 4 (-3,-2,-1,0,1,2,3)
			total += 4; // Add    4 ( 1, 2, 3,4,5,6,7)
			total %= 4; // Modulo 4 ( 1, 2, 3,0,1,2,3)

			for ( step = 0; step < total; step++ )
				(*this).Rotate();
		}







		const int STPiece::mPiecesXY_224 [ 224 ] =
		{
			// Index = 32*(kind-1) + 8*(orientation-1) + 2*(point-1) + (coordinate-1)
			//
			// Where:
			//   kind        = 1, 2, 3, 4, 5, 6, 7  (O,I,S,Z,L,J,T)
			//   orientation = 1, 2, 3, 4           (0,1/4,2/4,3/4-"turn")
			//   point       = 1, 2, 3, 4           (1,2,3,4)
			//   coordinate  = 1, 2                 (X,Y)

			// Piece 1 ("O": Box)
			-1, -1,   0, -1,   0,  0,  -1,  0,  // Orientation 0
			-1, -1,   0, -1,   0,  0,  -1,  0,  // Orientation 1 (redundant)
			-1, -1,   0, -1,   0,  0,  -1,  0,  // Orientation 2 (redundant)
			-1, -1,   0, -1,   0,  0,  -1,  0,  // Orientation 3 (redundant)

			// Piece 2 ("I": Line)
			-2,  0,  -1,  0,   0,  0,   1,  0,  // Orientation 0
			 0, -2,   0, -1,   0,  0,   0,  1,  // Orientation 1
			-2,  0,  -1,  0,   0,  0,   1,  0,  // Orientation 2 (redundant)
			 0, -2,   0, -1,   0,  0,   0,  1,  // Orientation 3 (redundant)

			// Piece 3 ("S": Step Up)
			-1, -1,   0, -1,   0,  0,   1,  0,  // Orientation 0
			 1, -1,   0,  0,   1,  0,   0,  1,  // Orientation 1
			-1, -1,   0, -1,   0,  0,   1,  0,  // Orientation 0 (redundant)
			 1, -1,   0,  0,   1,  0,   0,  1,  // Orientation 1 (redundant)

			// Piece 4 ("Z": Step Down)
			0, -1,   1, -1,  -1,  0,   0,  0,  // Orientation 0
			0, -1,   0,  0,   1,  0,   1,  1,  // Orientation 1
			0, -1,   1, -1,  -1,  0,   0,  0,  // Orientation 2 (redundant)
			0, -1,   0,  0,   1,  0,   1,  1,  // Orientation 3 (redundant)

			// Piece 5 ("L")
			-1, -1,  -1,  0,   0,  0,   1,  0,  // Orientation 0
			 0, -1,   1, -1,   0,  0,   0,  1,  // Orientation 1
			-1,  0,   0,  0,   1,  0,   1,  1,  // Orientation 2
			 0, -1,   0,  0,  -1,  1,   0,  1,  // Orientation 3

			// Piece 6 ("J")
		 	 1, -1,  -1,  0,   0,  0,   1,  0,  // Orientation 0
			 0, -1,   0,  0,   0,  1,   1,  1,  // Orientation 1
			-1,  0,   0,  0,   1,  0,  -1,  1,  // Orientation 2
			-1, -1,   0, -1,   0,  0,   0,  1,  // Orientation 3

			// Piece 7 ("T")
			 0, -1,  -1,  0,   0,  0,   1,  0,  // Orientation 0
			 0, -1,   0,  0,   1,  0,   0,  1,  // Orientation 1
			-1,  0,   0,  0,   1,  0,   0,  1,  // Orientation 2
			 0, -1,  -1,  0,   0,  0,   0,  1,  // Orientation 3
		};







		int STPiece::GetMaxOrientations
			(
			int kind  // 1==O 2==I 3==S 4==Z 5==L 6==J 7==T;  0==NONE
			)
		{
			if (1 == kind) return( 1 );
			if (2 == kind) return( 2 );
			if (3 == kind) return( 2 );
			if (4 == kind) return( 2 );
			if (5 == kind) return( 4 );
			if (6 == kind) return( 4 );
			if (7 == kind) return( 4 );
			return( 0 );
		}







		int STPiece::GetCellOffsetX
			(
			int kind,          // Kind        = 1,2,3,4,5,6,7;  0==NONE
			int orientation,   // Orientation = 1,2,3,4;        0==NONE
			int point          // Point       = 1,2,3,4;        0==NONE
			)
		{
			if (kind        <=  0  ) return(0);
			if (kind        >   7  ) return(0);
			if (orientation <=  0  ) return(0);
			if (orientation >   4  ) return(0);
			if (point       <=  0  ) return(0);
			if (point       >   4  ) return(0);

			int index = 0;
			index = 32*(kind-1) + 8*(orientation-1) + 2*(point-1);
			if (index       <   0  ) return(0);
			if (index       >= 224 ) return(0);

			return( STPiece::mPiecesXY_224[ index ] );  
		}







		int STPiece::GetCellOffsetY
			(
			int kind,          // Kind        = 1,2,3,4,5,6,7;  0==NONE
			int orientation,   // Orientation = 1,2,3,4;        0==NONE
			int point          // Point       = 1,2,3,4;        0==NONE
			)
		{
			if (kind        <=  0  ) return(0);
			if (kind        >   7  ) return(0);
			if (orientation <=  0  ) return(0);
			if (orientation >   4  ) return(0);
			if (point       <=  0  ) return(0);
			if (point       >   4  ) return(0);

			int index = 0;
			index = 32*(kind-1) + 8*(orientation-1) + 2*(point-1) + 1;
			if (index       <   0  ) return(0);
			if (index       >= 224 ) return(0);

			return( STPiece::mPiecesXY_224[ index ] );  
		}












		int STPiece::IsOffsetCellOccupied
			(
			int kind,
			int orientation,
			int x,
			int y
			)
		{
			// This function will indicates if a cell relative to the piece origin
			// is occupied by this piece.
			if (kind        <=  0  ) return(0);
			if (kind        >   7  ) return(0);
			if (orientation <=  0  ) return(0);
			if (orientation >   4  ) return(0);

			// Loop through all points and test against supplied point.
			int totalPoints = 0;
			totalPoints = STPiece::GetTotalPoints();
			int point = 0;
			for ( point = 1; point <= totalPoints; point++ )
			{
				if ( (x == STPiece::GetCellOffsetX( kind, orientation, point )) &&
					(y == STPiece::GetCellOffsetY( kind, orientation, point )) )
				{
					return(1);
				}
			}
			return(0);
		}





	}
}
