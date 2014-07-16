// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#include "CPF.StandardTetris.STGameState.h"



#include "CPF.StandardTetris.STBoard.h"
#include "CPF.StandardTetris.STPiece.h"
#include "CPF.StandardTetris.STPieceSequence.h"



namespace CPF
{
	namespace StandardTetris
	{






		STGameState::STGameState( )
		{
            // Core Game State

            // Board and Piece
			mSTBoardCurrent.SetBoardDimensions( 10, 20 );
			mSTBoardCurrent.ClearCells();
			mSTPieceCurrent.Clear();

            // Core Game State Variables
			mGameOver = 0;
			mIterationCountdownSeconds = (0.05f);
			mCurrentPiecePointValue = 0; // starts at 24+(3*(level-1))
			mCompletedRows = 0;

            // Piece Sequence Generator
			mSTPieceSequence.ClientSelectPieceSelectionSource
				( 
				STPieceSequence::STPieceSelectionSourceRandom
				);

            // User Options

			mPaused                   = 0;
			mShowNextPiece            = 0;
			mAI                       = 0;
			mSpawnFromVideoCapture    = 0;
			mOutputToRS232            = 0;
			mAutoRestart              = 0;
			mAutoWriteFile            = 0;

            // Game Speed Adjustment
            // -2, -3, -4,... : Slow Mode (delay proportional to index)
            // -1 : Normal, Clipped at 0.20 sec/row
            //  0 : Normal
            // +1 : Fast Mode (still render bound)
            // +2 : Very Fast Mode (still render bound)
            // +3, +4, +5,... : Multiple moves per rendered frame
            mGameSpeedAdjustment = 0;

			mShadowMode = 0;
			mHintMode = 0;
			mMonochromeColorMode = 0;


            // Statistics for User Consideration Only

            // Updated at piece spawning or row completion

			int i = 0;
			int n = 0;

			n =   8;  
			for ( i = 0; i < n; i++ ) 
			{
				mPieceHistogram [ i ] = 0;
			}

			n = 200;  
			for ( i = 0; i < n; i++ ) 
			{
				mHeightHistogram[ i ] = 0;
			}


			mTotalElapsedTimeSeconds = (0.0f);
			mScore = 0;

            // Only updated when game ends
			mHistoricHighScore = 0;
			mHistoricHighRows = 0;
			mHistoricHighPieces = 0;
			mHistoricCumulativeRows = 0; // Used to get average: rows / games
			mHistoricTotalGames = 0;
			n =  20;  
			for ( i = 0; i < n; i++ )
			{
				mHistoricRows[ i ] = 0; // Past games [0]==most recent
			}


			// Cached Derived or Copied Values

            // Next Piece (Not always used or available; Only 'kind' aspect is relevant)
			mSTPieceNext.Clear();

            // Best Move (determined by relevant AI upon piece spawning)
			mSTPieceBestMove.Clear();

            // state of animation of a an AI-executed move
			mAnimateAIMovesEnable = 1;
			mAnimateAIMovesStartingY = 0;
			mAnimateAIMovesFinalSafeY = 0;
			mAnimateAITotalInitialCommands = 0;
			mAnimateAICommandsExecuted = 0;
			mAnimateAICommandsPerRow = (0.0f);
			mAnimateAIMovesPendingRotation = 0;
			mAnimateAIMovesPendingTranslation = 0;



            // RUN-TIME STATE INFORMATION (DO NOT STORE IN A FILE)

            // video capture related
            mPreviousClassification = (-1);

            mSelectionState = 0;
            mSelectionX1 = 0;
            mSelectionY1 = 0;
            mSelectionX2 = 0;
            mSelectionY2 = 0;

            mCalibrationModeFlag = 0; // false == OFF
            mCalibrationModeShapeCode = 0;  // 1..7 shape

            // file list
            mFirstItem = 0;
            mRelativeItem = 0;
            mShowFileList = 0;
            mLoadFlag = 0;

            // miscellaneous
            mRenderFrameNumber = 0;
            mShowInstructionPage = 0;
            mShowConsole = 0;
            mReportedFrameRate = 0.0f;
		}






	}
}