// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STGameDrawingHeader
#define STGameDrawingHeader



#include <windows.h>



#include "CPF.StandardTetris.STBoard.h"
#include "CPF.StandardTetris.STPiece.h"
#include "CPF.StandardTetris.STGame.h"
#include "CPF.StandardTetris.STVideoProcessing.h"
#include "CPF.StandardTetris.STConsole.h"



namespace CPF
{
	namespace StandardTetris
	{


		class STGameDrawing
		{

		private:  static float m_PieceColorTableRGB[];
		private:  static int   mColorMode;
		private:  static int   mFrameNumber;


		public:  enum
				 {
					 COLOR_MODE_DEFAULT       = 0,
					 COLOR_MODE_BLACKANDWHITE = 1
				 };

		public:  static  void SetColorMode( int colorMode )
				 { mColorMode = colorMode; }

		public:  static  int  GetColorMode( )
				 { return( mColorMode ); }

		private:
			static void  DrawPieceWithCurrentColor
				(
				float xMin,
				float yMin,
				float f32_CellDX,
				float f32_CellDY,
				STPiece & piece
				);

		public:
			static void GetPieceColorRGB
				(
				int     piece_Kind, // 0==NONE, 1..7==PIECE, 8==JUNK PIECE
				float & ref_red,
				float & ref_f32_Green,
				float & ref_blue,
        int monochromeMode
				);

		public:
			static void DrawBoardAndNextPiece
				( 
				float f32_Left,
				float bottom,
				float right,
				float top,
				STBoard & ref_STBoard,
				STPiece & piece,
				int   shadowMode, // 0==DISABLE
        int   monochromeMode,
				int   showNextPiece,
				int   nextPieceKind,  // (-1) == No next piece
				STPiece & pieceBestMove  // IN: Best move
				);





		public: static void DrawStatusPane
					(
					int xMin,
					int yMin,
					int xMax,
					int yMax,
					STGame & game,
					STConsole & console
					);


		public: static void DrawStatisticsPane
					(
					int xMin,
					int yMin,
					int xMax,
					int yMax,
					STGame & game,
					STConsole & console
					);


		public: static void DrawConsolePane
					(
					int xMin,
					int yMin,
					int xMax,
					int yMax,
					STConsole & console
					);


		public: static void DrawInstructions_ControlsPageA
					(
					int xMin,
					int yMin,
					int xMax,
					int yMax
					);


		public: static void DrawInstructions_ControlsPageB
					(
					int xMin,
					int yMin,
					int xMax,
					int yMax
					);


		public: static void DrawInstructions_Credits
					(
					int xMin,
					int yMin,
					int xMax,
					int yMax
					);


		public: static void DrawInstructions
					(
					int xMin,
					int yMin,
					int xMax,
					int yMax,
					STGame & game
					);


		public: static void DrawFileList
					(
					STGame & game,
					int xMin,
					int yMin,
					int xMax,
					int yMax
					);


		public: static void HandleVideoCaptureGUI
					(
					STGame & game,
					STVideoProcessing & videoProcessing,
					float videoSheetX,
					float videoSheetY,
					float videoSheetWidth,
					float videoSheetHeight
					);


		public: static void DrawGameBoard
					(
					float xMin,
					float yMin,
					float xMax,
					float yMax,
					STGame & game
					);


		public: static void DrawTrainingBoard
					(
					float xMin,
					float yMin,
					float xMax,
					float yMax,
					STGame & game
					);

		public: static void DrawScreen
					(
					STVideoProcessing & videoProcessing,
					STGame & game,
					STConsole & console
					);


		};


	}
}



#endif






