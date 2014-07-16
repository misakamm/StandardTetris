// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STPieceHeader
#define STPieceHeader

typedef __int64 LongLong;


namespace CPF
{
	namespace StandardTetris
	{



		class STPiece
		{

		public: enum STPieceShape
				{
					None = 0,
					O = 1,
					I = 2,
					S = 3,
					Z = 4,
					L = 5,
					J = 6,
					T = 7
				};


				// Properties
		private:  int    mKind;        // 1,2,3,4,5,6,7 (O,I,S,Z,L,J,T); 0==None
		private:  int    mX;           // X Origin; must be on the board
		private:  int    mY;           // Y Origin; must be on the board
		private:  int    mOrientation; // 1,2,3,4 (0,1/4,2/4,3/4-"turn"); 0==None


		public:  STPiece( );

		public:  void  Clear( );

		public:  void  CopyFromPiece( STPiece & piece );

		public:  int   GetKind( );
		public:  void  SetKind( int kind );  // Kind=1,2,3,4,5,6,7

		public:  int   GetX( );
		public:  int   GetY( );
		public:  void  SetX( int x );
		public:  void  SetY( int y );

		public:  int   GetOrientation( );
		public:  void  SetOrientation( int orientation );  // Orientation=1,2,3,4

		public:  static int GetTotalPoints( ) { return(4); }
		public:  int   GetAbsolutePointX( int point );  // Point=1,2,3,4
		public:  int   GetAbsolutePointY( int point );  // Point=1,2,3,4

		public:  int   GetAbsoluteXMin( );
		public:  int   GetAbsoluteXMax( );
		public:  int   GetAbsoluteYMin( );
		public:  int   GetAbsoluteYMax( );

		public:  int   IsValid( ); // False if kind or orientation is invalid

		public:  void  Rotate( ); // Advance orientation by one step, modulo max orientations
		public:  void  RotateByCount( int count ); // Essentially multiple Rotate commands

		public:  void  Translate( int deltaX, int deltaY )
				 {
					 mX += deltaX;
					 mY += deltaY;
				 }

				 // Static methods and data that do not depend on specific piece.
		public:  static int GetMaxOrientations( int kind );
		public:  static int GetCellOffsetX( int kind, int orientation, int point );
		public:  static int GetCellOffsetY( int kind, int orientation, int point );
		public:  static int IsOffsetCellOccupied
					 (
					 int kind,
					 int orientation,
					 int x,
					 int y
					 );

		private: static const int mPiecesXY_224 [ 224 ]; 

		};


	}
}


#endif






