// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.


#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1


#include "CPF.StandardTetris.STUserInterface.h"


#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>

#include "CPF.StandardTetris.STEngine.h"
#include "CPF.StandardTetris.STGame.h"
#include "CPF.StandardTetris.STGameState.h"
#include "CPF.StandardTetris.STConsole.h"
#include "CPF.StandardTetris.STStrategyManager.h"
#include "CPF.StandardTetris.STRS232.h"
#include "CPF.StandardTetris.STVideoProcessing.h"
#include "CPF.StandardTetris.STFileList.h"




namespace CPF
{
	namespace StandardTetris
	{








		void STUserInterface::HandleKeyPress
			( 
			unsigned int uwParam, 
			unsigned int ulParam 
			)
		{

			STGame & game = STEngine::GetGame();

			STGameState & gameState = game.GetGameState();

			STVideoProcessing & videoProcessing = STEngine::GetVideoProcessing();



			// Check the SHIFT key.
			int shiftKeyPressed = 0;
			if (0 != ((GetAsyncKeyState( VK_SHIFT )) & 0x8000)) 
			{
				shiftKeyPressed = 1;
			}

			// Check the CONTROL key.
			int controlKeyPressed = 0;
			if (0 != ((GetAsyncKeyState( VK_CONTROL )) & 0x8000))
			{
				controlKeyPressed = 1;
			}


			// Priority of key press handling:
			//   (1) Instructions;    ESCAPE exits instructions;
			//   (2) File Menu;       ESCAPE exits file menu;
			//   (3) Calibrate;       ESCAPE cancels calibrate mode;
			//   (4) Video Capture;   ESCAPE quits application;
			//   (5) Normal;          ESCAPE quits application;


			// INSTRUCTIONS

			if (0 != game.InstructionGetState())
			{
				switch( uwParam )
				{
				case VK_DOWN:
				case VK_NEXT: // Page-Down
				case VK_RIGHT:
					{
						// Next page
						game.InstructionsNextPage();
					}
					break; 

				case VK_UP:
				case VK_PRIOR: // Page-Up
				case VK_LEFT:
					{
						// Previous page
						game.InstructionsPreviousPage();
					}
					break; 

				default:
					{
						// User hit a key, but it wasn't relevant, so exit menu.
						game.InstructionsHide();
						// NOTE: Don't resume!
						////StandardTetrisApplication::Game().InputEvent_Resume();
					}
					break;
				}
				return;
			}
			else if (uwParam == 'I')
			{
				game.InstructionsShow();
				game.InputEvent_Pause();
				return;
			}




			// FILE LIST

			if (0 != gameState.mShowFileList)
			{
				switch( uwParam )
				{
				case VK_NEXT: // Page-Down
					{
						// Next page
						gameState.mFirstItem += 20;
					}
					break; 

				case VK_PRIOR: // Page-Up
					{
						// Previous page
						gameState.mFirstItem -= 20;
					}
					break; 

				case VK_DOWN:
					{
						// Next Item
						gameState.mRelativeItem++;
						if (gameState.mRelativeItem > 19) 
						{
							gameState.mFirstItem++;
							gameState.mRelativeItem = 19;
						}
					}
					break; 

				case VK_UP:
					{
						// Previous Item
						gameState.mRelativeItem--;
						if (gameState.mRelativeItem < 0) 
						{
							gameState.mFirstItem--;
							gameState.mRelativeItem = 0;
						}
					}
					break; 

				case VK_RETURN:
					{
						// Load item
						gameState.mLoadFlag      = 1;
					}
					break; 

				default:
					{
						// User hit a key, but it wasn't relevant, so exit menu.
						gameState.mShowFileList = 0;
						// NOTE: Don't resume.
						////STEngine::Game().InputEvent_Resume();
					}
					break;
				}
				return;
			}
			else if ((uwParam == 'L') && (0 != shiftKeyPressed))
			{
				// SHIFT-L will read a text file in to the game state.
				game.InputEvent_Pause();
				gameState.mShowFileList  = 1;
				gameState.mFirstItem     = 0;
				gameState.mRelativeItem  = 0;
				gameState.mLoadFlag      = 0;
				STEngine::GetFileList().ScanDirectory( (string)(STEngine::GetApplicationPathOnlyANSI()) );
				return;    
			}




			// Calibrate Mode
			// (NOTE: See how normal mode enters calibrate mode by pressing 'C'.)

			if (0 != game.GetCalibrationModeFlagValue())
			{
				if ((VK_ESCAPE == uwParam) || ('C' == uwParam))
				{
					game.SetCalibrationModeFlagValue(0);
					return;
				}

				if (uwParam == 'V')
				{
					if (0 == game.GameIsSpawnFromVideoCapture())
					{
						// Set up sane conditions
						game.InputEvent_Reset();
						game.InputEvent_ShowNextPieceOff();
						game.InputEvent_AutoRestartOff();
						// Initialize Video Capture
						videoProcessing.Initialize( STEngine::GetHWND() );
						videoProcessing.ClearRegionStatus();
						game.InputEvent_VideoStart();
						videoProcessing.ClearRegionStatus();
					}
					else
					{
						videoProcessing.Terminate( );
						videoProcessing.ClearRegionStatus();
						game.InputEvent_VideoStop();
						videoProcessing.ClearRegionStatus();
					}
					return;
				}

				switch( uwParam )
				{  
					// Training Mode Piece Selection
				case '0': game.SetCalibrationModeShapeCode(0); break;
				case '1': game.SetCalibrationModeShapeCode(1); break;
				case '2': game.SetCalibrationModeShapeCode(2); break;
				case '3': game.SetCalibrationModeShapeCode(3); break;
				case '4': game.SetCalibrationModeShapeCode(4); break;
				case '5': game.SetCalibrationModeShapeCode(5); break;
				case '6': game.SetCalibrationModeShapeCode(6); break;
				case '7': game.SetCalibrationModeShapeCode(7); break;
				case '8': game.SetCalibrationModeShapeCode(0); break;
				case '9': game.SetCalibrationModeShapeCode(0); break;
				}
				return;
			}




			// Video Capture
			// The following is not mutually-exclusive with normal game play.

			if (0 != game.GameIsSpawnFromVideoCapture())
			{
				if (uwParam == VK_RETURN)
				{
					videoProcessing.ClearRegionStatus();
					game.ClearPreviousClassification();
					game.InputEvent_Reset();
					Sleep(200);
					videoProcessing.ClearRegionStatus();
					game.ClearPreviousClassification();
					game.InputEvent_Reset();
					Sleep(200);
					videoProcessing.ClearRegionStatus();
					game.ClearPreviousClassification();
					game.InputEvent_Reset();
					Sleep(200);
					videoProcessing.ClearRegionStatus();
					game.ClearPreviousClassification();
					game.InputEvent_Reset();
					Sleep(200);
				}

				if (uwParam == 'V')
				{
					if (0 == game.GameIsSpawnFromVideoCapture())
					{
						// Set up sane conditions
						game.InputEvent_Reset();
						game.InputEvent_ShowNextPieceOff();
						game.InputEvent_AutoRestartOff();
						// Initialize Video Capture
						videoProcessing.Initialize( STEngine::GetHWND() );
						videoProcessing.ClearRegionStatus();
						game.InputEvent_VideoStart();
						videoProcessing.ClearRegionStatus();
					}
					else
					{
						videoProcessing.Terminate( );
						videoProcessing.ClearRegionStatus();
						game.InputEvent_VideoStop();
						videoProcessing.ClearRegionStatus();
					}
					return;
				}

			}




			// Console Mode

			if (0 != gameState.mShowConsole)
			{
				if (uwParam == VK_DELETE)
				{
					STEngine::GetConsole().ClearAllLines();
				} 
				else
				{
					// Any key other than delete or P (pause) exits console mode.
					if (uwParam != 'P')
					{
						gameState.mShowConsole = 0;
					}
				}
			}
			else
			{
				if ((uwParam == 'Q') && (0 != shiftKeyPressed))
				{
					// SHIFT-Q : Console
					gameState.mShowConsole = 1;
				}        
			}




			// Normal Game Play

			// QUIT KEY:  ESCAPE
			if (uwParam == VK_ESCAPE)
			{
				DestroyWindow( STEngine::GetHWND() );
				PostQuitMessage( 0 );
				return;
			}

			// Enter Calibrate Mode
			if (uwParam == 'C')
			{
				game.SetCalibrationModeFlagValue(1);
				game.SetCalibrationModeShapeCode(1);    
			}

			// Enable Video Capture
			if (uwParam == 'V')
			{
				if (0 == game.GameIsSpawnFromVideoCapture())
				{
					// Set up sane conditions
					game.InputEvent_Reset();
					game.InputEvent_ShowNextPieceOff();
					game.InputEvent_AutoRestartOff();
					// Initialize Video Capture
					videoProcessing.Initialize( STEngine::GetHWND() );
					videoProcessing.ClearRegionStatus();
					game.InputEvent_VideoStart();
					videoProcessing.ClearRegionStatus();
				}
			}

			// Reset Game
			if (uwParam == VK_RETURN)
			{
				if (0 != shiftKeyPressed)
				{
					game.InputEvent_HardReset();
				}
				else
				{
					game.InputEvent_Reset();
				}
			}

			if (uwParam == 'P')
			{
				if (0 != game.GameIsPaused())
				{
					game.InputEvent_Resume();
				}
				else
				{
					game.InputEvent_Pause();
				}
			}

			if (uwParam == 'A')
			{
				if (0 != shiftKeyPressed)
				{
					STStrategyManager::SelectNextStrategy();
				}
				else
				{
					if (0 != game.GameIsAI())
					{
						game.InputEvent_AIStop();
					}
					else
					{
						game.InputEvent_AIStart();
					}
				}
			}

			if (uwParam == 'T')
			{
				if (game.GameIsOutputToRS232())
				{
					game.InputEvent_RS232Stop();
					STRS232::TerminatePort();
				}
				else
				{
					STRS232::InitializePort();
					game.InputEvent_RS232Start();
				}
			}

			if ((uwParam == VK_SUBTRACT) || (uwParam == 0xbd))
			{
				if (0 == shiftKeyPressed)
				{
					game.InputEvent_GameSpeedDecrease();
				}
			}
			if ((uwParam == VK_ADD) || (uwParam == 0xbb))
			{
				if (0 == shiftKeyPressed)
				{
					game.InputEvent_GameSpeedIncrease();
				}
			}

			if ((uwParam == 'W') && (0 != shiftKeyPressed))
			{
				// SHIFT-W will write out a text file (c:\tetris_state.txt)
				game.InputEvent_GameStateWriteToFile();
			}

			if (uwParam == VK_NEXT) // Page-Down
			{
				// Page-Down: Decrease Board Size
				game.InputEvent_GameBoardDecrease();
			}
			if (uwParam == VK_PRIOR) // Page-Up
			{
				// Page-Up:  Increase Board Size
				game.InputEvent_GameBoardIncrease();
			}


			if (0 != controlKeyPressed)
			{
				if (uwParam == VK_UP)
				{
					game.InputEvent_GameBoardIncreaseHeight();
				}
				if (uwParam == VK_LEFT)
				{
					game.InputEvent_GameBoardDecreaseWidth();
				}
				if (uwParam == VK_RIGHT)
				{
					game.InputEvent_GameBoardIncreaseWidth();
				}
				if (uwParam == VK_DOWN)
				{
					game.InputEvent_GameBoardDecreaseHeight();
				}
			}



			// COLOR SCHEME
			if ((uwParam == 'K')&&(0 != shiftKeyPressed))
			{
        if (0 == gameState.mMonochromeColorMode)
        {
            gameState.mMonochromeColorMode = 1;
        }
        else
        {
            gameState.mMonochromeColorMode = 0;
        }
			}



			// Non Video-Capture Options

			if (0 == game.GameIsSpawnFromVideoCapture())
			{
				// Only respond to user piece-control input if AI is not active.
				if (0 == game.GameIsAI())
				{
					if (0 == controlKeyPressed)
					{
						if (uwParam == VK_UP)
						{
							game.InputEvent_Rotate();
						}
						if (uwParam == VK_LEFT)
						{
							game.InputEvent_Left();
						}
						if (uwParam == VK_RIGHT)
						{
							game.InputEvent_Right();
						}
						if (uwParam == VK_DOWN)
						{
							game.InputEvent_Down();
						}
						if (uwParam == VK_SPACE)
						{
							game.InputEvent_Drop();
						}
					}
				}

				if (uwParam == 'Z')
				{
					if ((STPieceSequence::STPieceSelectionSourceAlternatingSAndZ) == 
						game.GetPieceSelectionSource())
					{
						// Since we're in S/Z mode, stop.
						game.InputEvent_SZPieceModeStop();
					}
					else
					{
						// Start S/Z mode.
						game.InputEvent_SZPieceModeStart();
					}
				}

				if (uwParam == 'S')
				{
					// S will cycle the shadow mode.
					game.InputEvent_ShadowModeCycle();
				}

				if ((uwParam == 'J') && (0 != shiftKeyPressed))
				{
					// SHIFT-J : Add line of random junk to bottom of the pile.
					game.InputEvent_AddRowOfJunk();
				}

				if ((uwParam == 'H') && (0 != shiftKeyPressed))
				{
					// SHIFT-H : Hint Mode
					if (0 != game.GameIsHintMode())
					{
						game.InputEvent_HintModeStop();
					}
					else
					{
						game.InputEvent_HintModeStart();
					}
				}

				if (uwParam == 'N')
				{
					if (game.GameIsShowNextPiece())
					{
						game.InputEvent_ShowNextPieceOff();
					}
					else
					{
						game.InputEvent_ShowNextPieceOn();
					}
				}

				if (uwParam == 'X')
				{
					game.InputEvent_ToggleMoveAnimation();
				}

				if (uwParam == 'U')
				{
					if (0 != game.GameIsAutoRestart())
					{
						game.InputEvent_AutoRestartOff();
					}
					else
					{
						game.InputEvent_AutoRestartOn();
					}
				}
				if (uwParam == 'F')
				{
					//if (STEngine::Game().GameIsAutoWriteFile())
					game.InputEvent_ToggleAutoWriteFile();
				}


				if ((uwParam == 'R') && (0 != shiftKeyPressed))
				{
					// SHIFT-R : Soft reset (game goes back to same random seed)
					game.InputEvent_SoftReset();
				}
			}

		}


	}
}
