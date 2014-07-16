// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#include "CPF.StandardTetris.STGameDrawing.h"



#include "CPF.StandardTetris.STBoard.h"
#include "CPF.StandardTetris.STPiece.h"
#include "CPF.StandardTetris.STStrategyManager.h"
#include "CPF.StandardTetris.STOpenGLFont.h"
#include "CPF.StandardTetris.STGame.h"
#include "CPF.StandardTetris.STVideoProcessing.h"
#include "CPF.StandardTetris.STEngine.h"
#include "CPF.StandardTetris.STConsole.h"


#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>



// Disable unimportant "symbol too long" error for debug builds of STL string
#pragma warning( disable : 4786 )

#include <string>
#include <vector>
using namespace std;



namespace CPF
{
	namespace StandardTetris
	{


		int STGameDrawing::mColorMode = 
			(STGameDrawing::COLOR_MODE_DEFAULT);



		int STGameDrawing::mFrameNumber = 0;



		float STGameDrawing::m_PieceColorTableRGB[] =
		{
			(  0.0f / 255.0f), (  0.0f / 255.0f), (  0.0f / 255.0f), // 0 BLACK
			
			(255.0f / 255.0f), (  0.0f / 255.0f), (  0.0f / 255.0f), // 1 RED
			(255.0f / 255.0f), (160.0f / 255.0f), (  0.0f / 255.0f), // 2 ORANGE
			(255.0f / 255.0f), (255.0f / 255.0f), (  0.0f / 255.0f), // 3 YELLOW
			(  0.0f / 255.0f), (255.0f / 255.0f), (  0.0f / 255.0f), // 4 GREEN
			(  0.0f / 255.0f), (255.0f / 255.0f), (255.0f / 255.0f), // 5 CYAN
			(  0.0f / 255.0f), (  0.0f / 255.0f), (255.0f / 255.0f), // 6 BLUE
			(255.0f / 255.0f), (  0.0f / 255.0f), (255.0f / 255.0f), // 7 MAGENTA
			
			( 64.0f / 255.0f), ( 64.0f / 255.0f), ( 64.0f / 255.0f), // 8 DK. GRAY
		};



		void STGameDrawing::GetPieceColorRGB
			(
			int     piece_Kind, // 0, 1..7
			float & ref_red,
			float & ref_green,
			float & ref_blue,
      int monochromeMode
			)
		{
			// DEFAULT: BLACK
			ref_red    = 0.0f;
			ref_green  = 0.0f;
			ref_blue   = 0.0f;

			// EMPTY
			if (piece_Kind <= 0)
			{
				ref_red    = 0.0f;
				ref_green  = 0.0f;
				ref_blue   = 0.0f;
				return;
			}

			// JUNK COLOR
			if (piece_Kind >= 8)
			{
				ref_red    = 0.5f;
				ref_green  = 0.5f;
				ref_blue   = 0.5f;
				return;
			}

			// Black & White Color Scheme
			if (0 != monochromeMode)
			{
				ref_red    = 1.0f;
				ref_green  = 1.0f;
				ref_blue   = 1.0f;
				return;
			}

			// COLOR 
			if ((piece_Kind >= 1) && (piece_Kind <= 7))
			{
				int colorIndex = 0;
				colorIndex = (3 * piece_Kind);
				ref_red    = STGameDrawing::m_PieceColorTableRGB[ colorIndex     ];
				ref_green  = STGameDrawing::m_PieceColorTableRGB[ colorIndex + 1 ];
				ref_blue   = STGameDrawing::m_PieceColorTableRGB[ colorIndex + 2 ];
			}
		}



		void  STGameDrawing::DrawPieceWithCurrentColor
			(
			float xMin,
			float yMin,
			float f32_CellDX,
			float f32_CellDY,
			STPiece & piece
			)
		{
			if (0 == piece.IsValid())  return;

			int point       = 0;
			int totalPoints = 0;

			totalPoints = piece.GetTotalPoints();

			for ( point = 1; point <= totalPoints; point++ )
			{
				int cellX = 0;
				int cellY = 0;

				cellX = piece.GetAbsolutePointX( point );
				cellY = piece.GetAbsolutePointY( point );

				float f32_PieceXMin = 0.0f;
				float f32_PieceXMax = 0.0f;
				float f32_PieceYMin = 0.0f;
				float f32_PieceYMax = 0.0f;

				f32_PieceXMin = xMin + (f32_CellDX * (float)(cellX-1));
				f32_PieceXMax = xMin + (f32_CellDX * (float)(cellX  ));
				f32_PieceYMin = yMin + (f32_CellDY * (float)(cellY-1));
				f32_PieceYMax = yMin + (f32_CellDY * (float)(cellY  ));

				glBegin(GL_QUADS);
				glVertex2f(f32_PieceXMin,f32_PieceYMax);
				glVertex2f(f32_PieceXMin,f32_PieceYMin);
				glVertex2f(f32_PieceXMax,f32_PieceYMin);
				glVertex2f(f32_PieceXMax,f32_PieceYMax);
				glEnd();
			}
		}








		void STGameDrawing::DrawBoardAndNextPiece
			( 
			float f32_Left,
			float bottom,
			float right,
			float top,
			STBoard & board,
			STPiece & piece,
			int   shadowMode, // 0==DISABLE
      int   monochromeMode,
			int   showNextPiece,
			int   nextPieceKind,  // (-1) == No next piece
			STPiece & pieceBestMove  // IN: Best move
			)
		{
			float target_aspect_ratio = 0.0f;
			float full_width    = 0.0f;
			float full_height   = 0.0f;
			float target_width  = 0.0f;
			float target_height = 0.0f;
			float x1  = 0.0f;
			float y1  = 0.0f;
			float x2  = 0.0f;
			float y2  = 0.0f;
			float cell_dx       = 0.0f;
			float cell_dy       = 0.0f;
			float red       = 0.0f;
			float green     = 0.0f;
			float blue      = 0.0f;
			float xb1 = 0.0f;
			float yb1 = 0.0f;
			float xb2 = 0.0f;
			float yb2 = 0.0f;
			float xp1 = 0.0f;
			float yp1 = 0.0f;
			float xp2 = 0.0f;
			float yp2 = 0.0f;

			int   width     = 0;
			int   height    = 0;

			int phase = 0;


			STGameDrawing::mFrameNumber++;  
			phase = (STGameDrawing::mFrameNumber);



			width  = board.GetWidth();
			height = board.GetHeight();



			target_aspect_ratio = (1.0f/2.0f);
			if (height > 0)
				target_aspect_ratio = ((float)width/(float)height);
			full_width  = (right - f32_Left);
			full_height = (top - bottom);

			if (full_width > (target_aspect_ratio * full_height))
			{
				// Fit to height
				target_width  = (full_height * target_aspect_ratio);
				target_height = full_height;
				x1 = f32_Left + ((full_width - target_width)/2.0f);
				y1 = bottom;
				x2 = x1 + target_width;
				y2 = y1 + target_height;
			}
			else
			{
				// Fit to width
				target_width  = full_width;
				target_height = (full_width / target_aspect_ratio);
				x1 = f32_Left;
				y1 = bottom + ((full_height - target_height)/2.0f);
				x2 = x1 + target_width;
				y2 = y1 + target_height;
			}

			// Grid Cell Pixel Dimensions
			if (showNextPiece)
			{
				cell_dx = ((x2-x1)/(float)(width+2));
				cell_dy = ((y2-y1)/(float)(height+4));
			}
			else
			{
				cell_dx = ((x2-x1)/(float)(width));
				cell_dy = ((y2-y1)/(float)(height));
			}


			// Board Location within 1:2 Region
			if (0 != showNextPiece)
			{
				xb1 = x1 + ( 1.0f * cell_dx);
				xb2 = x1 + ((float)(width+1) * cell_dx);
				yb1 = y1 + ( 0.5f * cell_dy);
				yb2 = y1 + ((0.5f + (float)(height)) * cell_dy);
			}
			else
			{
				xb1 = x1 + ( 0.0f * cell_dx);
				xb2 = x1 + ((float)(width)  * cell_dx);
				yb1 = y1 + ( 0.0f * cell_dy);
				yb2 = y1 + ((float)(height) * cell_dy);
			}


			// Next Piece Location (only applicable if flag set)
			if (0 != showNextPiece)
			{
				xp1 = x1 + ( (float)(((width-4)/2)+1) * cell_dx);
				xp2 = x1 + ( (float)(((width-4)/2)+5) * cell_dx);
				yp1 = y1 + ( ((float)(height+1)+0.5f) * cell_dy);
				yp2 = y1 + ( ((float)(height+3)+0.5f) * cell_dy);
			}


			// Draw black region for full 1:2 Region
			// glColor3f( 0.0f, 0.0f, 0.0f );
			// glBegin(GL_QUADS);
			// glVertex2f(x1,y2);
			// glVertex2f(x1,y1);
			// glVertex2f(x2,y1);
			// glVertex2f(x2,y2);
			// glEnd();




			
			// Draw "Best Move" Piece 
			
			if (0 != pieceBestMove.IsValid())
			{      
				red   = 0.30f;
				green = 0.45f;
				blue  = 0.55f;

				glColor3f( red, green, blue );

				STGameDrawing::DrawPieceWithCurrentColor
					( xb1, yb1, cell_dx, cell_dy, pieceBestMove );
			}
			




			
			// Draw Piece Shadow  (Do this BEFORE drawing the piece!)
			
			if ((1 == shadowMode) || (2 == shadowMode))
			{
				if (0 != piece.IsValid())
				{
					// Form a "shadow" piece, which is simply a copy of the current
					// piece, but dropped on to the pile.
					STPiece  Shadow_Piece;
					Shadow_Piece.CopyFromPiece( piece );
					board.DropPieceButBoardUnchanged
						( 
						Shadow_Piece
						);

					red   = 0.25f;
					green = 0.25f;
					blue  = 0.30f;

					glColor3f( red, green, blue );

					STGameDrawing::DrawPieceWithCurrentColor
						( xb1, yb1, cell_dx, cell_dy, Shadow_Piece );
				}
			}
			







			
			// Draw Piece
			
			if (2 != shadowMode) // Shadow mode 2 is "NO PIECE"! (shadow only!)
			{
				if (0 != piece.IsValid())
				{
					// Get color for this piece
					STGameDrawing::GetPieceColorRGB
						( piece.GetKind(), red, green, blue, monochromeMode );

					glColor3f( red, green, blue );

					STGameDrawing::DrawPieceWithCurrentColor
						( xb1, yb1, cell_dx, cell_dy, piece );
				}
			}
			





			
			// Draw Board
			
			// Draw occupied cells
			int   x         = 0;
			int   y         = 0;

			for ( y = 1; y <= height; y++ )
			{
				y1 = yb1 + (cell_dy * (float)(y-1));
				for ( x = 1; x <= width; x++ )
				{
					x1 = xb1 + (cell_dx * (float)(x-1));

					int value = board.GetCell( x, y );

					if (value > 0)
					{
						STGameDrawing::GetPieceColorRGB
							(
							value, // 0, 1..7
							red,
							green,
							blue,
              monochromeMode
							);

						glColor3f( red, green, blue );

						glBegin(GL_QUADS);
						glVertex2f(x1,y1+cell_dy);
						glVertex2f(x1,y1);
						glVertex2f(x1+cell_dx,y1);
						glVertex2f(x1+cell_dx,y1+cell_dy);
						glEnd();
					}
				}
			}

			// Draw Grid
			int maxDimension = 0;
			maxDimension = width;
			if (height > maxDimension) 
				maxDimension = height;

			float f32_Intensity = 0.0f;
			f32_Intensity = 0.3f * (1.0f - ((float)(maxDimension)/100.0f));
			if (f32_Intensity < 0.0f) f32_Intensity = 0.0f;
			if (f32_Intensity > 1.0f) f32_Intensity = 1.0f;

			float f32_Alpha = 0.0f;
			f32_Alpha = 0.6f * (1.0f - ((float)(maxDimension)/200.0f));
			if (f32_Alpha < 0.0f) f32_Alpha = 0.0f;
			if (f32_Alpha > 1.0f) f32_Alpha = 1.0f;

			glColor4f( f32_Intensity, f32_Intensity, f32_Intensity, f32_Alpha );


			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBegin(GL_LINES);
			// Vertical Lines
			for ( x = 0; x <= width; x++ )
			{
				x1 = xb1 + (cell_dx * (float)(x));
				glVertex2f( x1, yb1 );
				glVertex2f( x1, yb2 );
			}
			// Horizontal Lines
			for ( y = 0; y <= height; y++ )
			{
				y1 = yb1 + (cell_dy * (float)(y));
				glVertex2f( xb1, y1 );
				glVertex2f( xb2, y1 );
			}
			glEnd();
			glDisable(GL_BLEND);



			// Border Lines
			glColor3f( 0.5f, 0.5f, 0.5f );
			glBegin(GL_LINES);

			x = 0;
			x1 = xb1 + (cell_dx * (float)(x));
			glVertex2f( x1, yb1 );
			glVertex2f( x1, yb2 );

			x = width;
			x1 = xb1 + (cell_dx * (float)(x));
			glVertex2f( x1, yb1 );
			glVertex2f( x1, yb2 );

			y = 0;
			y1 = yb1 + (cell_dy * (float)(y));
			glVertex2f( xb1, y1 );
			glVertex2f( xb2, y1 );

			y = height;
			y1 = yb1 + (cell_dy * (float)(y));
			glVertex2f( xb1, y1 );
			glVertex2f( xb2, y1 );

			glEnd();
			



			
			// Draw "Next Piece" if applicable
			
			if (0 != showNextPiece)
			{
				// Draw occupied cells of "Next Piece"
				for ( y = 1; y <= 2; y++ )
				{
					y1 = yp1 + (cell_dy * (float)(y-1));
					for ( x = 1; x <= 4; x++ )
					{
						x1 = xp1 + (cell_dx * (float)(x-1));

						int value = STPiece::IsOffsetCellOccupied
							(
							nextPieceKind,
							1, // Orientation "1" is default
							(x - 3), // Access using X offset (-3)
							(y - 2)  // Access using X offset (-2)
							);

						if (value > 0)
						{
							STGameDrawing::GetPieceColorRGB // Returns WHITE for unknown
								(
								(nextPieceKind), // 0, 1..7
								red,
								green,
								blue, 
                monochromeMode
								);

							glColor3f( red, green, blue );

							glBegin(GL_QUADS);
							glVertex2f(x1,y1+cell_dy);
							glVertex2f(x1,y1);
							glVertex2f(x1+cell_dx,y1);
							glVertex2f(x1+cell_dx,y1+cell_dy);
							glEnd();
						}
					}
				}


				// Draw Grid
				glColor3f( 0.0f, 0.0f, 0.0f );
				glBegin(GL_LINES);
				// Vertical Lines
				for ( x = 0; x <= 4; x++ )
				{
					x1 = xp1 + (cell_dx * (float)(x));
					glVertex2f( x1, yp1 );
					glVertex2f( x1, yp2 );
				}
				// Horizontal Lines
				for ( y = 0; y <= 2; y++ )
				{
					y1 = yp1 + (cell_dy * (float)(y));
					glVertex2f( xp1, y1 );
					glVertex2f( xp2, y1 );
				}
				glEnd();
			}
			

		}











		void STGameDrawing::DrawStatusPane
			(
			int xMin,
			int yMin,
			int xMax,
			int yMax,
			STGame & game,
			STConsole & console
			)
		{
			float x  =    0.0f;
			float y  =    0.0f;
			float dy = (-14.0);

			x = (float)(xMin);
			y = (float)(yMax);

			
			glEnable( GL_SCISSOR_TEST );
			glScissor( xMin, yMin, ((xMax-xMin)+1), ((yMax-yMin)+1) );
			glColor3f( 1.0f, 1.0f, 1.0f );
			

			
			// Draw Text
			
			STOpenGLFont::FontPrint(x,y,     "level    %7d", game.GetCurrentLevel() );
			STOpenGLFont::FontPrint(x,y+=dy, "rows     %7d", game.GetCompletedRows() );
			STOpenGLFont::FontPrint(x,y+=dy, "score    %7I64d", game.GetScore() );

			y+=dy;
			y+=dy;

			if (0 != game.GameIsFinished())
			{
				glColor3f( 1.0f, 1.0f, 1.0f );
				STOpenGLFont::FontPrint(x,y, "GAME OVER" );
				glColor3f( 1.0f, 1.0f, 1.0f );
			}
			y+=dy;

			if (0 != game.GameIsPaused())
			{
				glColor3f( 0.8f, 0.8f, 0.8f );
				STOpenGLFont::FontPrint(x,y, "PAUSED" );
				glColor3f( 1.0f, 1.0f, 1.0f );
			}
			y+=dy;

			glColor3f( 0.5f, 0.5f, 0.5f );
			STOpenGLFont::FontPrint(x,y+=dy, "quit           esc" );
			STOpenGLFont::FontPrint(x,y+=dy, "new game       enter" );
			STOpenGLFont::FontPrint(x,y+=dy, "rotate         up" );
			STOpenGLFont::FontPrint(x,y+=dy, "left           left" );
			STOpenGLFont::FontPrint(x,y+=dy, "right          right" );
			STOpenGLFont::FontPrint(x,y+=dy, "drop           down" );
			STOpenGLFont::FontPrint(x,y+=dy, "instructions   i" );

			y+=dy;


			if (0 != game.GameIsPaused())
			{
				STOpenGLFont::FontPrint(x,y+=dy, "pause          p [*]" );
			}
			else
			{
				STOpenGLFont::FontPrint(x,y+=dy, "pause          p [ ]" );
			}

			if (0 != game.GameIsShowNextPiece())
			{
				STOpenGLFont::FontPrint(x,y+=dy, "next           n [*]" );
			}
			else
			{
				STOpenGLFont::FontPrint(x,y+=dy, "next           n [ ]" );
			}

			if (0 != game.GameIsAI())
			{
				STOpenGLFont::FontPrint(x,y+=dy, "AI             a [*]" );
			}
			else
			{
				STOpenGLFont::FontPrint(x,y+=dy, "AI             a [ ]" );
			}

			if (0 != game.GameIsOutputToRS232())
			{
				STOpenGLFont::FontPrint(x,y+=dy, "rs232          t [*]" );
			}
			else
			{
				STOpenGLFont::FontPrint(x,y+=dy, "rs232          t [ ]" );
			}

			if (0 != game.GameIsSpawnFromVideoCapture())
			{
				STOpenGLFont::FontPrint(x,y+=dy, "video          v [*]" );
			}
			else
			{
				STOpenGLFont::FontPrint(x,y+=dy, "video          v [ ]" );
			}

			if (0 != game.GetCalibrationModeFlagValue())
			{
				STOpenGLFont::FontPrint(x,y+=dy, "calibrate      c [*]" );
			}
			else
			{
				STOpenGLFont::FontPrint(x,y+=dy, "calibrate      c [ ]" );
			}

			if (0 != game.GameIsAutoRestart())
			{
				STOpenGLFont::FontPrint(x,y+=dy, "auto reset     u [*]" );
			}
			else
			{
				STOpenGLFont::FontPrint(x,y+=dy, "auto reset     u [ ]" );
			}

			if (0 != game.GameIsAutoWriteFile())
			{
				STOpenGLFont::FontPrint(x,y+=dy, "auto file      f [*]" );
			}
			else
			{
				STOpenGLFont::FontPrint(x,y+=dy, "auto file      f [ ]" );
			}

			if (0 != game.GameIsAnimateMoves())
			{
				STOpenGLFont::FontPrint(x,y+=dy, "animate        x [*]" );
			}
			else
			{
				STOpenGLFont::FontPrint(x,y+=dy, "animate        x [ ]" );
			}
			

			if (0 != game.GameIsSpawnFromVideoCapture())
			{
				// Draw statistics pane in remaining space!
				STGameDrawing::DrawStatisticsPane
					(
					xMin,
					yMin,
					xMax,
					(int)(y+2*dy), // Squish so that statistics appears below status text!
					game,
					console
					);
			}


			
			glDisable( GL_SCISSOR_TEST );
			
		}











		void STGameDrawing::DrawConsolePane
			(
			int xMin,
			int yMin,
			int xMax,
			int yMax,
			STConsole & console
			)
		{
			float x  =    0.0f;
			float y  =    0.0f;
			float dy = (-14.0);

			x = (float)(xMin);
			y = (float)(yMax);

			
			glEnable( GL_SCISSOR_TEST );
			glScissor( xMin, yMin, ((xMax-xMin)+1), ((yMax-yMin)+1) );
			glColor3f( 1.0f, 1.0f, 1.0f );
			

			
			// Draw Text of Console
			
			int totalLines = 0;
			totalLines = console.GetTotalLines();

			int lineIndex = 0;
			for ( lineIndex = 0; lineIndex < totalLines; lineIndex++ )
			{
				STOpenGLFont::FontPrint(x,y+=dy, "%s", console.GetLineByIndex( lineIndex ) );
			}
			

			
			glDisable( GL_SCISSOR_TEST );
			
		}











		void STGameDrawing::DrawStatisticsPane
			(
			int xMin,
			int yMin,
			int xMax,
			int yMax,
			STGame & game,
            STConsole & console
			)
		{
			float x  =    0.0f;
			float y  =    0.0f;
			float dy = (-14.0);

			x = (float)(xMin);
			y = (float)(yMax);

			
			glEnable( GL_SCISSOR_TEST );
			glScissor( xMin, yMin, ((xMax-xMin)+1), ((yMax-yMin)+1) );
			glColor3f( 1.0f, 1.0f, 1.0f );
			

			
			glColor3f( 0.5f, 0.5f, 0.5f );
			y-=dy;
			//STOpenGLFont::FontPrint(x,y+=dy, "O         %7d", game.GetPieceHistogramBinValue( 1 ) );
			//STOpenGLFont::FontPrint(x,y+=dy, "I         %7d", game.GetPieceHistogramBinValue( 2 ) );
			//STOpenGLFont::FontPrint(x,y+=dy, "S         %7d", game.GetPieceHistogramBinValue( 3 ) );
			//STOpenGLFont::FontPrint(x,y+=dy, "Z         %7d", game.GetPieceHistogramBinValue( 4 ) );
			//STOpenGLFont::FontPrint(x,y+=dy, "L         %7d", game.GetPieceHistogramBinValue( 5 ) );
			//STOpenGLFont::FontPrint(x,y+=dy, "J         %7d", game.GetPieceHistogramBinValue( 6 ) );
			//STOpenGLFont::FontPrint(x,y+=dy, "T         %7d", game.GetPieceHistogramBinValue( 7 ) );
			//STOpenGLFont::FontPrint(x,y+=dy, "sum       %7d", game.GetPieceHistogramSum() );
			STOpenGLFont::FontPrint(x,y+=dy, "beta      %7.5f/%2d", game.GetPileHeightBeta(), game.GetPileHeightBetaSamples() );
			STOpenGLFont::FontPrint(x,y+=dy, "base    %9.5f%%", game.GetHistoricRowsAtBottom() * 100 );

			y+=dy;

			STOpenGLFont::FontPrint(x,y+=dy, "rows      %7d", game.GetCompletedRows() );
			STOpenGLFont::FontPrint(x,y+=dy, "time         %7.2f s", game.GetTotalGameTime() );

			int   currentRowCount  = 0;
			float f32_CurrentTime      = 0.0f;
			f32_CurrentTime     = game.GetTotalGameTime();
			currentRowCount = game.GetCompletedRows();

			float rowsPerSecondGame = 0.0f;
			if (f32_CurrentTime > 0.001f)
			{
				rowsPerSecondGame = (float)(currentRowCount) / f32_CurrentTime;
			}
			STOpenGLFont::FontPrint(x,y+=dy, "rows/s       %7.2f", rowsPerSecondGame );


			static int   previousRowCount = 0;
			static float f32_PreviousTime     = 0.0f;
			static int   deltaRowCount  = 0;
			static float f32_DeltaTime      = 0.0f;
			if (f32_CurrentTime < 1.0f)
			{
				previousRowCount = 0;
				f32_PreviousTime     = 0.0f;
				deltaRowCount    = 0;
				f32_DeltaTime        = 0.0f;
			}

			if (f32_CurrentTime >= (f32_PreviousTime + 1.0f))
			{
				deltaRowCount    = (currentRowCount - previousRowCount);
				f32_DeltaTime        = (f32_CurrentTime - f32_PreviousTime);
				previousRowCount = currentRowCount;
				f32_PreviousTime     = f32_CurrentTime;
			}

			float rowsPerSecondPreviousSecond = 0.0f;
			if (f32_DeltaTime > 0.001f)
			{
				rowsPerSecondPreviousSecond = (float)(deltaRowCount) / f32_DeltaTime;
			}
			STOpenGLFont::FontPrint(x,y+=dy, "rows/s (1s)  %7.2f", rowsPerSecondPreviousSecond );

			y+=dy;

			STOpenGLFont::FontPrint(x,y+=dy, "board          %d x %d", 
				game.GetBoardWidth(),
				game.GetBoardHeight() 
				);
			STOpenGLFont::FontPrint(x,y+=dy, "speed bias    %3d", game.GetGameSpeedAdjustment() );
			STOpenGLFont::FontPrint(x,y+=dy, "frame rate   %7.2f f/s", (float)game.GetReportedFrameRate() );

			y+=dy;

			string currentStrategyName = STStrategyManager::GetCurrentStrategyName();
			STOpenGLFont::FontPrint(x,y+=dy, "%s", (char *)(currentStrategyName.c_str()) );
			y+=dy;

			STOpenGLFont::FontPrint(x,y+=dy, "total games     %d", game.GetHistoricTotalGames() );
			STOpenGLFont::FontPrint(x,y+=dy, "average rows    %I64d", game.GetHistoricAverageRows() );
			STOpenGLFont::FontPrint(x,y+=dy, "high rows       %d", game.GetHistoricHighRows() );

			y+=dy;

			int n = 0;
			n = game.GetHistoricTotalGames();
			if (n > 0)
			{
				if (n > 10) 
				{ 
					n = 10; 
				}
				STOpenGLFont::FontPrint(x,y+=dy, "previous games" );
				int i = 0;
				for ( i = (n-1); i >= 0; i-- )
				{
					STOpenGLFont::FontPrint(x,y+=dy, "rows #%2d  %10d", (n-i), 
						game.GetHistoricRowsBinValue( i ) );
				}
			}
			

			
			glDisable( GL_SCISSOR_TEST );
			

			
			// Draw Console in remaining space
			
			STGameDrawing::DrawConsolePane
				(
				(int)(x),
				yMin,
				xMax,
				(int)(y),
				console
				);
			
		}











		void STGameDrawing::DrawInstructions_ControlsPageA
			(
			int xMin,
			int yMin,
			int xMax,
			int yMax
			)
		{
			float x  =    0.0f;
			float y  =    0.0f;
			float dy = (-14.0);

			x = (float)(xMin);
			y = (float)(yMax);

			
			glEnable( GL_SCISSOR_TEST );
			glScissor( xMin, yMin, ((xMax-xMin)+1), ((yMax-yMin)+1) );
			glColor3f( 1.0f, 1.0f, 1.0f );
			

			STOpenGLFont::FontPrint(x,y+=dy,"---------------------------------------------------------------------------");
			STOpenGLFont::FontPrint(x,y+=dy,"                           Controls                                        ");
			STOpenGLFont::FontPrint(x,y+=dy,"---------------------------------------------------------------------------");
			STOpenGLFont::FontPrint(x,y+=dy,"rotate          up                      ");
			STOpenGLFont::FontPrint(x,y+=dy,"left            left                    ");
			STOpenGLFont::FontPrint(x,y+=dy,"right           right                   ");
			STOpenGLFont::FontPrint(x,y+=dy,"drop            down, space             ");
			STOpenGLFont::FontPrint(x,y+=dy," ");
			STOpenGLFont::FontPrint(x,y+=dy,"pause           p            [toggle]   ");
			STOpenGLFont::FontPrint(x,y+=dy," ");
			STOpenGLFont::FontPrint(x,y+=dy,"new game        enter                   New random piece sequence");
			STOpenGLFont::FontPrint(x,y+=dy,"restart game    shift-r                 Start game with same piece sequence");
			STOpenGLFont::FontPrint(x,y+=dy,"total reset     shift-enter             Conditions similar to app startup");
			STOpenGLFont::FontPrint(x,y+=dy,"                                                                           ");
			STOpenGLFont::FontPrint(x,y+=dy,"write file      shift-w                 Saves game to c:\\tetris_state_*.txt");
			STOpenGLFont::FontPrint(x,y+=dy,"load  file      shift-L                 Load a previously-saved game");
			STOpenGLFont::FontPrint(x,y+=dy," ");
			STOpenGLFont::FontPrint(x,y+=dy,"instructions    i                                                          ");
			STOpenGLFont::FontPrint(x,y+=dy,"shadow mode     s            [toggle]   Draws shadow where piece will land");
			STOpenGLFont::FontPrint(x,y+=dy,"hint mode       shift-h      [toggle]   Show current AI best move");
			STOpenGLFont::FontPrint(x,y+=dy,"color scheme    shift-k      [toggle]   Change color scheme");
			STOpenGLFont::FontPrint(x,y+=dy,"animate move    x            [toggle]   AI moves in slow-motion");
			STOpenGLFont::FontPrint(x,y+=dy,"junk row        shift-j                 Add random junk row under pile");
			STOpenGLFont::FontPrint(x,y+=dy,"S/Z pieces      z            [toggle]   Pieces alternate between S and Z");
			STOpenGLFont::FontPrint(x,y+=dy,"---------------------------------------------------------------------------");
			STOpenGLFont::FontPrint(x,y+=dy," ");
			STOpenGLFont::FontPrint(x,y+=dy," ");
			STOpenGLFont::FontPrint(x,y+=dy," ");
			STOpenGLFont::FontPrint(x,y+=dy,"                     [ Hit any key to exit menu ]            Down/Page-Down");

			glDisable( GL_SCISSOR_TEST );
		}











		void STGameDrawing::DrawInstructions_ControlsPageB
			(
			int xMin,
			int yMin,
			int xMax,
			int yMax
			)
		{
			float x  =    0.0f;
			float y  =    0.0f;
			float dy = (-14.0);

			x = (float)(xMin);
			y = (float)(yMax);

			
			glEnable( GL_SCISSOR_TEST );
			glScissor( xMin, yMin, ((xMax-xMin)+1), ((yMax-yMin)+1) );
			glColor3f( 1.0f, 1.0f, 1.0f );
			



			STOpenGLFont::FontPrint(x,y+=dy,"---------------------------------------------------------------------------");
			STOpenGLFont::FontPrint(x,y+=dy,"                           Controls (continued)                            ");
			STOpenGLFont::FontPrint(x,y+=dy,"---------------------------------------------------------------------------");
			STOpenGLFont::FontPrint(x,y+=dy,"AI              a            [toggle]   Current AI will play");
			STOpenGLFont::FontPrint(x,y+=dy,"switch AI       shift-a      [cycle ]   Switch to next AI type");
			STOpenGLFont::FontPrint(x,y+=dy,"auto-restart    u            [toggle]   New game after current game ends");
			STOpenGLFont::FontPrint(x,y+=dy,"auto-write      f            [toggle]   Game written to file upon ending");
			STOpenGLFont::FontPrint(x,y+=dy," ");
			STOpenGLFont::FontPrint(x,y+=dy,"speed up        +            [slider]   More  game iterations per render   ");
			STOpenGLFont::FontPrint(x,y+=dy,"slow down       -            [slider]   Fewer game iterations per render   ");
			STOpenGLFont::FontPrint(x,y+=dy," ");
			STOpenGLFont::FontPrint(x,y+=dy,"larger board    page-up      [slider]   Make board larger  (1 2 W H ratio)");
			STOpenGLFont::FontPrint(x,y+=dy,"smaller board   page-down    [slider]   Make board smaller (1 2 W H ratio)");
			STOpenGLFont::FontPrint(x,y+=dy,"more width      control-right[slider]   Increase Board Width");
			STOpenGLFont::FontPrint(x,y+=dy,"less width      control-left [slider]   Decrease Board Width");
			STOpenGLFont::FontPrint(x,y+=dy,"more height     control-up   [slider]   Increase Board Height");
			STOpenGLFont::FontPrint(x,y+=dy,"less height     control-down [slider]   Decrease Board Height");
			STOpenGLFont::FontPrint(x,y+=dy," ");
			STOpenGLFont::FontPrint(x,y+=dy,"video capture   v            [toggle]   Web camera supplies pieces");
			STOpenGLFont::FontPrint(x,y+=dy,"calibrate       c            [toggle]   1..7   Show piece to test camera");
			STOpenGLFont::FontPrint(x,y+=dy,"video b/w       b            [toggle]   Process video as grayscale");
			STOpenGLFont::FontPrint(x,y+=dy,"rs-232 output   t            [toggle]   Movements transmitted on COM1");
			STOpenGLFont::FontPrint(x,y+=dy," ");
			STOpenGLFont::FontPrint(x,y+=dy,"debug console   shift-q      [toggle]   Show read-only debugging console");
			STOpenGLFont::FontPrint(x,y+=dy,"---------------------------------------------------------------------------");
			STOpenGLFont::FontPrint(x,y+=dy," ");
			STOpenGLFont::FontPrint(x,y+=dy," ");
			STOpenGLFont::FontPrint(x,y+=dy," ");
			STOpenGLFont::FontPrint(x,y+=dy,"Up/Page-Up           [ Hit any key to exit menu ]            Down/Page-Down");

			glDisable( GL_SCISSOR_TEST );
		}











		void STGameDrawing::DrawInstructions_Credits
			(
			int xMin,
			int yMin,
			int xMax,
			int yMax
			)
		{
			float x  =    0.0f;
			float y  =    0.0f;
			float dy = (-14.0);

			x = (float)(xMin);
			y = (float)(yMax);

			
			glEnable( GL_SCISSOR_TEST );
			glScissor( xMin, yMin, ((xMax-xMin)+1), ((yMax-yMin)+1) );
			glColor3f( 1.0f, 1.0f, 1.0f );
			

			STOpenGLFont::FontPrint(x,y+=dy,"---------------------------------------------------------------------------");
			STOpenGLFont::FontPrint(x,y+=dy,"                           Credits                                         ");
			STOpenGLFont::FontPrint(x,y+=dy,"---------------------------------------------------------------------------");
			STOpenGLFont::FontPrint(x,y+=dy,"                                                                           ");
			STOpenGLFont::FontPrint(x,y+=dy,"All design and programming by Colin P. Fahey.                              ");
			STOpenGLFont::FontPrint(x,y+=dy,"                                                                           ");
			STOpenGLFont::FontPrint(x,y+=dy,"      Official web site:   http://colinfahey.com                           ");
			STOpenGLFont::FontPrint(x,y+=dy,"                                                                           ");
			STOpenGLFont::FontPrint(x,y+=dy,"                                                                           ");
			STOpenGLFont::FontPrint(x,y+=dy,"                                                                           ");
			STOpenGLFont::FontPrint(x,y+=dy," ");
			STOpenGLFont::FontPrint(x,y+=dy,"This application contains adaptations of the following algorithms from     ");
			STOpenGLFont::FontPrint(x,y+=dy,"other sources:                                                             ");
			STOpenGLFont::FontPrint(x,y+=dy,"                                                                           ");
			STOpenGLFont::FontPrint(x,y+=dy,"      Contributed Tetris AI:  Pierre Dellacherie (France)                  ");
			STOpenGLFont::FontPrint(x,y+=dy,"          (One-Piece; 2M+ rows; dellache@club-internet.fr )                ");
			STOpenGLFont::FontPrint(x,y+=dy,"                                                                           ");
			STOpenGLFont::FontPrint(x,y+=dy,"      ISAAC Pseudo-Random Number Generator:  Bob Jenkins                   ");
			STOpenGLFont::FontPrint(x,y+=dy,"          ( http://burtleburtle.net/bob/rand/isaac.html )                  ");
			STOpenGLFont::FontPrint(x,y+=dy,"                                                                           ");
			STOpenGLFont::FontPrint(x,y+=dy,"      Tetris AI:  Roger Espel Llima (France?)                              ");
			STOpenGLFont::FontPrint(x,y+=dy,"          (One-Piece; 40K rows; Part of xtris application )                ");
			STOpenGLFont::FontPrint(x,y+=dy,"                                                                           ");
			STOpenGLFont::FontPrint(x,y+=dy," ");
			STOpenGLFont::FontPrint(x,y+=dy,"---------------------------------------------------------------------------");
			STOpenGLFont::FontPrint(x,y+=dy," ");
			STOpenGLFont::FontPrint(x,y+=dy," ");
			STOpenGLFont::FontPrint(x,y+=dy," ");
			STOpenGLFont::FontPrint(x,y+=dy,"Up/Page-Up           [ Hit any key to exit menu ]                          ");

			glDisable( GL_SCISSOR_TEST );
		}











		void STGameDrawing::DrawFileList
			(
			STGame & game,
			int xMin,
			int yMin,
			int xMax,
			int yMax
			)
		{
			STGameState & gameState = game.GetGameState();

			STFileList & fileList = STEngine::GetFileList();

			// Screen was tuned to 640 x 480 size, so center the rendering of
			// instruction screens if the window dimensions are bigger.
			int width  = (xMax - xMin);
			int height = (yMax - yMin);
			if (width > 640)
			{
				xMin = ((width - 640)/2);
				xMax = xMin + 640;
			}
			if (height > 480)
			{
				yMin = ((height - 480)/2);
				yMax = yMin + 480;
			}


			float x  =    0.0f;
			float y  =    0.0f;
			float dy = (-14.0);

			x = (float)(xMin);
			y = (float)(yMax);

			
			glEnable( GL_SCISSOR_TEST );
			glScissor( xMin, yMin, ((xMax-xMin)+1), ((yMax-yMin)+1) );
			glColor3f( 1.0f, 1.0f, 1.0f );
			

			string  string_DirectoryPath;
			fileList.GetDirectoryPath( string_DirectoryPath );

			STOpenGLFont::FontPrint(x,y,    "-----------------------------------------------------------------------");
			STOpenGLFont::FontPrint(x,y+=dy,"file list" );
			STOpenGLFont::FontPrint(x,y+=dy,"%s", (char *)(string_DirectoryPath.c_str()) );
			STOpenGLFont::FontPrint(x,y+=dy,"-----------------------------------------------------------------------");

			STOpenGLFont::FontPrint(x,y+=dy," ");

			int totalFiles = 0;
			totalFiles = fileList.GetTotalItems();


			if (0 == totalFiles)
			{
				STOpenGLFont::FontPrint(x,y+=dy,"   [no tetris_state_*.txt files were found.]");
				STOpenGLFont::FontPrint(x,y+=dy," ");
			}
			else
			{
				int itemsPerPage = 20;

				gameState.mRelativeItem %= itemsPerPage; // Modulo items per page
				gameState.mRelativeItem += itemsPerPage; // Fix negative case
				gameState.mRelativeItem %= itemsPerPage; // Modulo items per page

				if (gameState.mFirstItem > (totalFiles - 1))
				{
					gameState.mFirstItem = (totalFiles - 1);
				}

				if (gameState.mFirstItem < 0)  
				{
					gameState.mFirstItem = 0;
				}

				if ((gameState.mRelativeItem + gameState.mFirstItem) > (totalFiles-1))
				{
					gameState.mRelativeItem--;
				}

				int pageRelativeItemIndex = 0;

				for 
					( 
					pageRelativeItemIndex = 0; 
					pageRelativeItemIndex < itemsPerPage;
					pageRelativeItemIndex++ 
						)
				{
					int indexList = 0;

					indexList = (pageRelativeItemIndex + gameState.mFirstItem);

					if ((indexList >= 0) && (indexList < totalFiles))
					{
						string string_FileName;
						fileList.GetItemNameByIndex( indexList, string_FileName );
						if (pageRelativeItemIndex == gameState.mRelativeItem)
						{
							if (0 != gameState.mLoadFlag)
							{
								string string_FilePathAndName;
								fileList.GetItemFullPathAndNameByIndex
									( indexList, string_FilePathAndName );
								gameState.mShowFileList      = 0;
								gameState.mLoadFlag      = 0;
								if ((int)(string_FilePathAndName.length()) > 0)
								{
									//game.LoadGameStateFromFile
									//	( (char *)(string_FilePathAndName.c_str()) );            
								}
							}
							glColor3f( 1.0f, 1.0f, 0.0f );
						}
						else
						{
							glColor3f( 1.0f, 1.0f, 1.0f );
						}

						STOpenGLFont::FontPrint(x,y+=dy,"  [%3d] \"%s\"", (indexList+1), (char *)(string_FileName.c_str()) );
						glColor3f( 1.0f, 1.0f, 1.0f );
					}
					else
						STOpenGLFont::FontPrint(x,y+=dy," " );
				} 
			}

			STOpenGLFont::FontPrint(x,y+=dy,"-----------------------------------------------------------------------");
			STOpenGLFont::FontPrint(x,y+=dy,"   next file:   down                  previous file:   up     ");
			STOpenGLFont::FontPrint(x,y+=dy,"   next page:   page-down             previous page:   page-up");
			STOpenGLFont::FontPrint(x,y+=dy,"   load     :   enter");
			STOpenGLFont::FontPrint(x,y+=dy,"   exit menu:   any other key");

			glDisable( GL_SCISSOR_TEST );
		}











		void STGameDrawing::DrawInstructions
			(
			int xMin,
			int yMin,
			int xMax,
			int yMax,
			STGame & game
			)
		{
            STGameState & gameState = game.GetGameState( );


			// Screen was tuned to 640 x 480 size, so center the rendering of
			// instruction screens if the window dimensions are bigger.
			int width  = (xMax - xMin);
			int height = (yMax - yMin);
			if (width > 640)
			{
				xMin = ((width - 640)/2);
				xMax = xMin + 640;
			}
			if (height > 480)
			{
				yMin = ((height - 480)/2);
				yMax = yMin + 480;
			}


			if (gameState.mShowInstructionPage < 1)
			{ 
				gameState.mShowInstructionPage = 1;
			}
			if (gameState.mShowInstructionPage > 3) 
			{
				gameState.mShowInstructionPage = 3;
			}

			switch( gameState.mShowInstructionPage )
			{
			case 1: 
				{
					STGameDrawing::DrawInstructions_ControlsPageA
						( xMin, yMin, xMax, yMax );
				}
				break;
			case 2: 
				{
					STGameDrawing::DrawInstructions_ControlsPageB
						( xMin, yMin, xMax, yMax );
				}
				break;
			case 3: 
				{
					STGameDrawing::DrawInstructions_Credits
						( xMin, yMin, xMax, yMax );
				}
				break;
			}
		}








		void STGameDrawing::HandleVideoCaptureGUI
			(
			STGame & game,
			STVideoProcessing & videoProcessing,
			float videoSheetX,
			float videoSheetY,
			float videoSheetWidth,
			float videoSheetHeight
			)
		{
			STGameState & gameState = game.GetGameState();


			if (0 == game.GameIsSpawnFromVideoCapture()) return;


			// Get Cursor Position

			POINT pt;
			RECT rect;
			GetCursorPos( & pt );
			ScreenToClient( STEngine::GetHWND(), &pt );
			GetClientRect( STEngine::GetHWND(), &rect );
			// Flip vertical
			pt.y = ((rect.bottom - rect.top) - pt.y);
			STEngine::SetClientWidth( (rect.right  - rect.left) + 1 );
			STEngine::SetClientHeight( (rect.bottom - rect.top)  + 1 );



			glBindTexture( GL_TEXTURE_2D, videoProcessing.mTextureOpenGLHandleBGR256x256 );


			float x1 = 0.0f;
			float y1 = 0.0f;
			float x2 = 0.0f;
			float y2 = 0.0f;

			x1 = videoSheetX;
			y1 = videoSheetY;     
			x2 = x1 + (videoSheetWidth  - 1.0f);
			y2 = y1 + (videoSheetHeight - 1.0f);

			float u1 = 0.0f;
			float v1 = 0.0f;
			float u2 = 0.0f;
			float v2 = 0.0f;

			u1 = 0.0f;
			v1 = 0.0f;
			u2 = 0.5f;
			v2 = 1.0f;

			glEnable( GL_SCISSOR_TEST );
			glScissor( (int)(x1), (int)(y1), (int)((x2-x1)+1), (int)((y2-y1)+1) );

			glEnable( GL_TEXTURE_2D );
			glColor3f( 1.0f, 1.0f, 1.0f );

			glBegin(GL_QUADS);
			glTexCoord2f(u1,v2);
			glVertex2f(x1,y2);

			glTexCoord2f(u1,v1);
			glVertex2f(x1,y1);

			glTexCoord2f(u2,v1);
			glVertex2f(x2,y1);

			glTexCoord2f(u2,v2);
			glVertex2f(x2,y2);
			glEnd();

			glDisable( GL_TEXTURE_2D );
			glDisable( GL_SCISSOR_TEST );



			// Only listen to the mouse in training/calibration mode
			if (0 != game.GetCalibrationModeFlagValue())
			{
				if (0 != GetAsyncKeyState( VK_LBUTTON ))
				{
					// Left button pressed
					if (0 == gameState.mSelectionState)
					{
						gameState.mSelectionState = 1;
						gameState.mSelectionX1 = pt.x;
						gameState.mSelectionY1 = pt.y;
						gameState.mSelectionX2 = pt.x;
						gameState.mSelectionY2 = pt.y;
					}
					else
					{
						gameState.mSelectionX2 = pt.x;
						gameState.mSelectionY2 = pt.y;
					}
				}
				else
				{
					// Left button released
					if (0 == gameState.mSelectionState)
					{
						// Nothing to do...
					}
					else
					{
						gameState.mSelectionState = 0;
					}
				}

				glEnable( GL_SCISSOR_TEST );
				glScissor( 0, 0, STEngine::GetClientWidth(), STEngine::GetClientHeight() );

				glColor3f( 1.0f, 0.0f, 0.0f );
				glBegin(GL_LINES);

				glVertex2f( (float)pt.x - 8.0f, (float)pt.y );
				glVertex2f( (float)pt.x + 8.0f, (float)pt.y );

				glVertex2f( (float)pt.x, (float)pt.y - 8.0f );
				glVertex2f( (float)pt.x, (float)pt.y + 8.0f );
				glEnd();
			}



			int xTexelMin = 0;
			int yTexelMin = 0;
			int xTexelMax = 0;
			int yTexelMax = 0;


			int xScreenMin = 0;
			int yScreenMin = 0;
			int xScreenMax = 0;
			int yScreenMax = 0;



			if (0 != ((GetAsyncKeyState(VK_SHIFT)) & 0x8000))
			{
				if (0 != ((GetAsyncKeyState(VK_LEFT))  & 0x8000)) 
				{
					gameState.mSelectionX2--;
				}
				if (0 != ((GetAsyncKeyState(VK_RIGHT)) & 0x8000)) 
				{
					gameState.mSelectionX2++;
				}
				if (0 != ((GetAsyncKeyState(VK_DOWN))  & 0x8000)) 
				{
					gameState.mSelectionY2--;
				}
				if (0 != ((GetAsyncKeyState(VK_UP))    & 0x8000)) 
				{
					gameState.mSelectionY2++;
				}
			}
			else
			{
				if (0 != ((GetAsyncKeyState(VK_LEFT))  & 0x8000)) 
				{
					gameState.mSelectionX1--;
				}
				if (0 != ((GetAsyncKeyState(VK_RIGHT)) & 0x8000)) 
				{
					gameState.mSelectionX1++;
				}
				if (0 != ((GetAsyncKeyState(VK_DOWN))  & 0x8000))
				{
					gameState.mSelectionY1--;
				}
				if (0 != ((GetAsyncKeyState(VK_UP))    & 0x8000)) 
				{
					gameState.mSelectionY1++;
				}
			}


			xScreenMin = gameState.mSelectionX1;
			yScreenMin = gameState.mSelectionY1;
			xScreenMax = gameState.mSelectionX2;
			yScreenMax = gameState.mSelectionY2;



			xTexelMin = (int)(256.0f * (((float)xScreenMin - videoSheetX) / videoSheetHeight));
			yTexelMin = (int)(256.0f * (((float)yScreenMin - videoSheetY) / videoSheetHeight));
			xTexelMax = (int)(256.0f * (((float)xScreenMax - videoSheetX) / videoSheetHeight));
			yTexelMax = (int)(256.0f * (((float)yScreenMax - videoSheetY) / videoSheetHeight));

			int disregard = 0;

			if (xTexelMin <   0) 
			{ 
				disregard = 1; 
				xTexelMin = 0; 
			}
			if (yTexelMin <   0) 
			{ 
				disregard = 1; 
				yTexelMin = 0; 
			}
			if (xTexelMax <   0) 
			{ 
				disregard = 1; 
				xTexelMax = 0; 
			}
			if (yTexelMax <   0) 
			{ 
				disregard = 1; 
				yTexelMax = 0; 
			}

			if (xTexelMin > 255) 
			{ 
				disregard = 1; 
				xTexelMin = 255; 
			}
			if (yTexelMin > 255) 
			{ 
				disregard = 1; 
				yTexelMin = 255; 
			}
			if (xTexelMax > 255) 
			{ 
				disregard = 1; 
				xTexelMax = 255; 
			}
			if (yTexelMax > 255) 
			{ 
				disregard = 1; 
				yTexelMax = 255; 
			}

			if (xTexelMin > xTexelMax)
			{
				int swap  = xTexelMin;
				xTexelMin = xTexelMax;
				xTexelMax = swap;
			}

			if (yTexelMin > yTexelMax)
			{
				int swap  = yTexelMin;
				yTexelMin = yTexelMax;
				yTexelMax = swap;
			}


			// Only set region if in training mode!
			if ((0 != game.GetCalibrationModeFlagValue()) && (0 == disregard))
			{
				videoProcessing.SetRegion( xTexelMin, yTexelMin, xTexelMax, yTexelMax );
			}


			videoProcessing.GetRegion( xTexelMin, yTexelMin, xTexelMax, yTexelMax );

			xScreenMin = (int)(videoSheetX + (videoSheetHeight * (float)xTexelMin / 256.0f));
			yScreenMin = (int)(videoSheetY + (videoSheetHeight * (float)yTexelMin / 256.0f));
			xScreenMax = (int)(videoSheetX + (videoSheetHeight * (float)xTexelMax / 256.0f));
			yScreenMax = (int)(videoSheetY + (videoSheetHeight * (float)yTexelMax / 256.0f));


			x1 = videoSheetX;
			y1 = videoSheetY;     
			x2 = x1 + (videoSheetWidth  - 1.0f);
			y2 = y1 + (videoSheetHeight - 1.0f);


			int currentClassification  = videoProcessing.GetRegionClassification();

			if (0 == currentClassification)
			{
				// If the previous classification was a PIECE, and the current classification
				// is something different, then submit the piece (which must have fallen
				// by a row by now).
				if ((gameState.mPreviousClassification >= 1) && (gameState.mPreviousClassification <= 7))
				{
					game.SpawnSpecifiedPiece( gameState.mPreviousClassification );
				}      
			}

			gameState.mPreviousClassification = currentClassification;


			float red   = 0.0f;
			float green = 0.0f;
			float blue  = 0.0f;

			STGameDrawing::GetPieceColorRGB // Returns WHITE for unknown
				(
				currentClassification, // 0..6
				red,
				green,
				blue, 
        gameState.mMonochromeColorMode
				);

			glColor3f( red, green, blue );

			glBegin(GL_LINES);

			glVertex2f( (float)xScreenMin, (float)yScreenMin );
			glVertex2f( (float)xScreenMin, (float)yScreenMax );

			glVertex2f( (float)xScreenMax, (float)yScreenMin );
			glVertex2f( (float)xScreenMax, (float)yScreenMax );

			glVertex2f( (float)xScreenMin, (float)yScreenMin );
			glVertex2f( (float)xScreenMax, (float)yScreenMin );

			glVertex2f( (float)xScreenMin, (float)yScreenMax );
			glVertex2f( (float)xScreenMax, (float)yScreenMax );

			// Horizontal divider
			glVertex2f( (float)xScreenMin, (float)((yScreenMin +  yScreenMax)/2) );
			glVertex2f( (float)xScreenMax, (float)((yScreenMin +  yScreenMax)/2) );

			// Vertical dividers
			glVertex2f( (float)(xScreenMin + ((xScreenMax-xScreenMin)/4)), (float)yScreenMin );
			glVertex2f( (float)(xScreenMin + ((xScreenMax-xScreenMin)/4)), (float)yScreenMax );

			glVertex2f( (float)(xScreenMin + 2 * ((xScreenMax-xScreenMin)/4)), (float)yScreenMin );
			glVertex2f( (float)(xScreenMin + 2 * ((xScreenMax-xScreenMin)/4)), (float)yScreenMax );

			glVertex2f( (float)(xScreenMin + 3 * ((xScreenMax-xScreenMin)/4)), (float)yScreenMin );
			glVertex2f( (float)(xScreenMin + 3 * ((xScreenMax-xScreenMin)/4)), (float)yScreenMax );

			glEnd();

			glDisable( GL_SCISSOR_TEST );
		}











		void STGameDrawing::DrawGameBoard
			(
			float xMin,
			float yMin,
			float xMax,
			float yMax,
			STGame & game
			)
		{
			if (0 != game.GetCalibrationModeFlagValue()) 
			{
				return;
			}


			glEnable( GL_SCISSOR_TEST );
			glScissor
				(
				(int)(xMin)-4,
				(int)(yMin)-4,
				(int)((xMax-xMin)+1)+8,
				(int)((yMax-yMin)+1)+8
				);



			STBoard   copyOfCurrentBoard;
			game.GetCopyOfCurrentBoard( copyOfCurrentBoard );
			STPiece   copyOfCurrentPiece;
			game.GetCopyOfCurrentPiece( copyOfCurrentPiece );

			STPiece  nextPiece;
			game.GetCopyOfNextPiece( nextPiece );
			int nextPieceKind = 0;
			nextPieceKind = nextPiece.GetKind();

			STPiece  bestMovePiece;
			game.GetCopyOfBestPiece( bestMovePiece );


			STGameDrawing::DrawBoardAndNextPiece
				( 
				xMin, // float f32_Left,
				yMin, // float bottom,
				xMax, // float right,
				yMax, // float top,
				copyOfCurrentBoard,
				copyOfCurrentPiece,
				game.GetGameShadowMode(), // Shadow Mode
        game.GameIsMonochromeMode(),
				game.GameIsShowNextPiece(), // show next piece
				nextPieceKind,
				bestMovePiece
				);

			glDisable( GL_SCISSOR_TEST );
		}












		void  STGameDrawing::DrawTrainingBoard
			(
			float xMin,
			float yMin,
			float xMax,
			float yMax,
			STGame & game
			)
		{
			STGameState & gameState = game.GetGameState();

			if (0 == game.GetCalibrationModeFlagValue()) 
			{
				return;
			}

			glEnable( GL_SCISSOR_TEST );
			glScissor
				(
				(int)(xMin)-4,
				(int)(yMin)-4,
				(int)((xMax-xMin)+1)+8,
				(int)((yMax-yMin)+1)+8
				);

			// Get a copy of the current board, just to get a board with the 
			// proper dimensions -- and then clear the copy.
			STBoard  tempBoard;
			game.GetCopyOfCurrentBoard( tempBoard );
			tempBoard.ClearCells();

			STPiece  trainingPiece;
			trainingPiece.SetOrientation( 1 );
			trainingPiece.SetKind( gameState.mCalibrationModeShapeCode );
			trainingPiece.SetX( tempBoard.GetPieceSpawnX() );
			trainingPiece.SetY( tempBoard.GetPieceSpawnY() );

			tempBoard.AddPieceToBoard
				(
				trainingPiece
				);

			STPiece  dummyPiece;

			STGameDrawing::DrawBoardAndNextPiece
				( 
				xMin, // float f32_Left,
				yMin, // float bottom,
				xMax, // float right,
				yMax, // float top,
				tempBoard,
				dummyPiece, // No shadow, so piece is not necessary
				0, // No shadow
        game.GameIsMonochromeMode(),
				game.GameIsShowNextPiece(), // show next piece
				trainingPiece.GetKind(), // For training purpose, make the next piece the same as the piece        
				dummyPiece
				);

			glDisable( GL_SCISSOR_TEST );
		}







		void STGameDrawing::DrawScreen
			(
			STVideoProcessing & videoProcessing,
            STGame & game,
            STConsole & console
			)
		{ 
			STGameState & gameState = game.GetGameState();


			gameState.mRenderFrameNumber++;


			// Master Frame has desired aspect ratio within the client area
			float masterFrameAspectRatio  = (3.0f/2.0f);
			float masterFrameWidth        = 0.0f;
			float masterFrameHeight       = 0.0f;
			float masterFrameX            = 0.0f;
			float masterFrameY            = 0.0f;

			// Master Sheet is a scaled down version of the frame (just to
			// have some border), centered within the frame area.
			float masterSheetToFrameScale = 0.95f;
			float masterSheetWidth        = 0.0f;
			float masterSheetHeight       = 0.0f;
			float masterSheetX            = 0.0f;
			float masterSheetY            = 0.0f;

			// Stats Frame is the first horizontal 1/3rd of the sheet.
			float statsFrameWidth         = 0.0f;
			float statsFrameHeight        = 0.0f;
			float statsFrameX             = 0.0f;
			float statsFrameY             = 0.0f;

			// Stats Sheet is a scaled down version of the Stats Frame
			// (just to have some border), centered within the Stats Frame area.
			float statsSheetToFrameScale  = 0.95f;
			float statsSheetWidth         = 0.0f;
			float statsSheetHeight        = 0.0f;
			float statsSheetX             = 0.0f;
			float statsSheetY             = 0.0f;

			// Board Frame is the second horizontal 1/3rd of the sheet.
			float boardFrameWidth         = 0.0f;
			float boardFrameHeight        = 0.0f;
			float boardFrameX             = 0.0f;
			float boardFrameY             = 0.0f;

			// Board Sheet is a scaled down version of the Board Frame
			// (just to have some border), centered within the Board Frame area.
			float boardSheetToFrameScale  = 0.95f;
			float boardSheetWidth         = 0.0f;
			float boardSheetHeight        = 0.0f;
			float boardSheetX             = 0.0f;
			float boardSheetY             = 0.0f;

			// Video Frame is the third horizontal 1/3rd of the sheet.
			float videoFrameWidth         = 0.0f;
			float videoFrameHeight        = 0.0f;
			float videoFrameX             = 0.0f;
			float videoFrameY             = 0.0f;

			// Video Sheet is a scaled down version of the Video Frame
			// (just to have some border), centered within the Video Frame area.
			float videoSheetToFrameScale  = 0.95f;
			float videoSheetWidth         = 0.0f;
			float videoSheetHeight        = 0.0f;
			float videoSheetX             = 0.0f;
			float videoSheetY             = 0.0f;

			// ==================================



			RECT rect;
			GetClientRect( STEngine::GetHWND(), &rect );
			STEngine::SetClientHeight( (rect.bottom - rect.top)  + 1 );
			STEngine::SetClientWidth( (rect.right  - rect.left) + 1 );



			// You MUST do this every frame, because sometimes, after
			// a WM_SIZE, the client rectangle doesn't seem correct
			// by the time we get here.
			glViewport( 0, 0, STEngine::GetClientWidth(), STEngine::GetClientHeight() );

			glClearColor( 0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			glOrtho
				(
				0.0f, (float)(STEngine::GetClientWidth()),
				0.0f, (float)(STEngine::GetClientHeight()),
				-1.0f, 1.0f
				);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();



			// Determine location and extent of Master Frame
			masterFrameAspectRatio  = (3.0f/2.0f);
			masterFrameWidth        = 0.0f;
			masterFrameHeight       = 0.0f;
			masterFrameX            = 0.0f;
			masterFrameY            = 0.0f;

			if ((float)STEngine::GetClientWidth() >= (masterFrameAspectRatio * (float)STEngine::GetClientHeight()))
			{
				// Fit to height
				masterFrameWidth  = ((float)STEngine::GetClientHeight() * masterFrameAspectRatio);
				masterFrameHeight = (float)STEngine::GetClientHeight();
				masterFrameX      = 0.0f + (((float)STEngine::GetClientWidth() - masterFrameWidth)/2.0f);
				masterFrameY      = 0.0f;
			}
			else
			{
				// Fit to width
				masterFrameWidth  = (float)STEngine::GetClientWidth();
				masterFrameHeight = ((float)STEngine::GetClientWidth() / masterFrameAspectRatio);
				masterFrameX      = 0.0f;
				masterFrameY      = 0.0f + (((float)STEngine::GetClientHeight() - masterFrameHeight)/2.0f);
			}


			// Determine the location and extent of the Master Sheet.
			// Master Sheet is a scaled down version of the frame (just to
			// have some border), centered within the frame area.
			masterSheetToFrameScale = 0.95f;
			masterSheetWidth        = (masterSheetToFrameScale * masterFrameWidth);
			masterSheetHeight       = (masterSheetToFrameScale * masterFrameHeight);
			masterSheetX            = masterFrameX + ((masterFrameWidth  - masterSheetWidth )/2.0f);
			masterSheetY            = masterFrameY + ((masterFrameHeight - masterSheetHeight)/2.0f);


			// Determine the location and extent of the Stats Frame.
			// Stats Frame is the first horizontal 1/3rd of the sheet.
			// NOTE: the Master Sheet has a 3:2 aspect ratio, so this
			// frame will have a 1:2 aspect ratio.
			statsFrameWidth         = (masterSheetWidth / 3.0f);
			statsFrameHeight        = masterSheetHeight;
			statsFrameX             = masterSheetX;
			statsFrameY             = masterSheetY;


			// Stats Sheet is a scaled down version of the Stats Frame
			// (just to have some border), centered within the Stats Frame area.
			statsSheetToFrameScale  = 0.95f;
			statsSheetWidth         = (statsSheetToFrameScale * statsFrameWidth);
			statsSheetHeight        = (statsSheetToFrameScale * statsFrameHeight);
			statsSheetX             = statsFrameX + ((statsFrameWidth  - statsSheetWidth )/2.0f);
			statsSheetY             = statsFrameY + ((statsFrameHeight - statsSheetHeight)/2.0f);

			// Board Frame is the second horizontal 1/3rd of the sheet.
			boardFrameWidth         = (masterSheetWidth / 3.0f);
			boardFrameHeight        = masterSheetHeight;
			boardFrameX             = masterSheetX + (masterSheetWidth / 3.0f);
			boardFrameY             = masterSheetY;

			// Board Sheet is a scaled down version of the Board Frame
			// (just to have some border), centered within the Board Frame area.
			boardSheetToFrameScale  = 0.95f;
			boardSheetWidth         = (boardSheetToFrameScale * boardFrameWidth);
			boardSheetHeight        = (boardSheetToFrameScale * boardFrameHeight);
			boardSheetX             = boardFrameX + ((boardFrameWidth  - boardSheetWidth )/2.0f);
			boardSheetY             = boardFrameY + ((boardFrameHeight - boardSheetHeight)/2.0f);

			// Video Frame is the third horizontal 1/3rd of the sheet.
			videoFrameWidth         = (masterSheetWidth / 3.0f);
			videoFrameHeight        = masterSheetHeight;
			videoFrameX             = masterSheetX + (2.0f * masterSheetWidth / 3.0f);;
			videoFrameY             = masterSheetY;

			// Video Sheet is a scaled down version of the Video Frame
			// (just to have some border), centered within the Video Frame area.
			videoSheetToFrameScale  = 0.95f;
			videoSheetWidth         = (videoSheetToFrameScale * videoFrameWidth);
			videoSheetHeight        = (videoSheetToFrameScale * videoFrameHeight);
			videoSheetX             = videoFrameX + ((videoFrameWidth  - videoSheetWidth )/2.0f);
			videoSheetY             = videoFrameY + ((videoFrameHeight - videoSheetHeight)/2.0f);


			float x1 = 0.0f;
			float y1 = 0.0f;
			float x2 = 0.0f;
			float y2 = 0.0f;

			if (0 != gameState.mShowInstructionPage)
			{
				x1 = 16;
				y1 = 16;
				x2 = x1 + (STEngine::GetClientWidth()  - 32.0f);
				y2 = y1 + (STEngine::GetClientHeight() - 32.0f);

				STGameDrawing::DrawInstructions( (int)x1, (int)y1, (int)x2, (int)y2, game );
			}
			else if ( gameState.mShowFileList != 0 )
			{
				x1 = 16;
				y1 = 16;
				x2 = x1 + (STEngine::GetClientWidth()  - 32.0f);
				y2 = y1 + (STEngine::GetClientHeight() - 32.0f);

				STGameDrawing::DrawFileList( game, (int)x1, (int)y1, (int)x2, (int)y2 );
			}
			else
			{
				if (0 == gameState.mShowConsole)
				{
					// VIDEO CAPTURE GUI
					if (0 != game.GameIsSpawnFromVideoCapture())
					{
						STGameDrawing::HandleVideoCaptureGUI
							(
							game,
							videoProcessing,
							videoSheetX,
							videoSheetY,
							videoSheetWidth,
							videoSheetHeight
							);
					}
					else
					{
						x1 = videoSheetX;
						y1 = videoSheetY;
						x2 = x1 + (videoSheetWidth - 1.0f);
						y2 = y1 + (videoSheetHeight - 1.0f);

						// Draw stats in entire remaining client area
						if ((STEngine::GetClientWidth() - x1) > 200) 
						{
							x1 = (float)(STEngine::GetClientWidth() - 200);
						}
						STGameDrawing::DrawStatisticsPane
							(
							(int)x1, // x1
							(int)(0), // y1
							(int)(STEngine::GetClientWidth()), // x2
							(int)(y2), // y2
							game,
							console
							);

					}



					x1 = boardSheetX;
					y1 = boardSheetY;
					x2 = x1 + (boardSheetWidth - 1.0f);
					y2 = y1 + (boardSheetHeight - 1.0f);
					if (STEngine::GetClientWidth() > 640)
					{
						x1 = (float)216;
						x2 = (float)(STEngine::GetClientWidth() - 216);
					}
					if (STEngine::GetClientHeight() > 480)
					{
						y1 = (float)64;
						y2 = (float)(STEngine::GetClientHeight() - 64);
					}


					// DRAW GAME BOARD
					if (0 == game.GetCalibrationModeFlagValue())
					{
						STGameDrawing::DrawGameBoard( x1, y1, x2, y2, game );
					}

					// DRAW TRAINING-MODE BOARD
					if (0 != game.GetCalibrationModeFlagValue())
					{
						STGameDrawing::DrawTrainingBoard( x1, y1, x2, y2, game );
					}

					x1 = statsSheetX;
					y1 = statsSheetY;
					x2 = x1 + (statsSheetWidth - 1.0f);
					y2 = y1 + (statsSheetHeight - 1.0f);

					x1 = 16;
					y1 = 0;
					STGameDrawing::DrawStatusPane( (int)x1, (int)y1, (int)x2, (int)y2, game, console );
				}
				else
				{
					// SHOW CONSOLE
					// Draw Board in Stats Sheet area
					x1 = statsSheetX;
					y1 = statsSheetY;
					x2 = x1 + (statsSheetWidth - 1.0f);
					y2 = y1 + (statsSheetHeight - 1.0f);

					// DRAW GAME BOARD
					if (0 == game.GetCalibrationModeFlagValue())
					{
						STGameDrawing::DrawGameBoard( x1, y1, x2, y2, game );
					}

					// DRAW TRAINING-MODE BOARD
					if (0 != game.GetCalibrationModeFlagValue())
					{
						STGameDrawing::DrawTrainingBoard( x1, y1, x2, y2, game );
					}

					// Draw console in remainder of screen
					x1 = boardSheetX;
					y1 = 0.0f;
					x2 = (float)(STEngine::GetClientWidth()) - 1.0f;
					y2 = y1 + (videoSheetHeight - 1.0f);

					STGameDrawing::DrawConsolePane( (int)x1, (int)y1, (int)x2, (int)y2, console );
				}
			}


			SwapBuffers( STEngine::GetHDC( ) );
		} 








	}
}