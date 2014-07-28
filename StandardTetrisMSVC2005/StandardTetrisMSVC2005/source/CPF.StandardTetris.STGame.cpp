// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#include "CPF.StandardTetris.STGame.h"



#include "CPF.StandardTetris.STGameState.h"
#include "CPF.StandardTetris.STBoard.h"
#include "CPF.StandardTetris.STPiece.h"
#include "CPF.StandardTetris.STStrategyManager.h"
#include "CPF.StandardTetris.STTime.h"
#include "CPF.StandardTetris.STRS232.h"
#include "CPF.StandardTetris.STFile.h"
#include "CPF.StandardTetris.STRandom.h"
#include "CPF.StandardTetris.STPieceSequence.h"
#include "CPF.StandardTetris.STGameFile.h"




namespace CPF
{
	namespace StandardTetris
	{



		STGame::STGame( )
		{
		}


		STGame::~STGame( )
		{
		}


		void STGame::GetCopyOfCurrentBoard( STBoard & board )
		{
			board.CopyFromBoard( mGameState.mSTBoardCurrent );
		}


		void STGame::GetCopyOfCurrentPiece( STPiece & piece )
		{
			piece.CopyFromPiece( mGameState.mSTPieceCurrent );
		}


		void   STGame::InputEvent_Left   ( )
		{
			if (mGameState.mOutputToRS232) STRS232::MomentaryRelay_LEFT();
			(*this).PrivateTranslatePiece( -1 );
		}



		void   STGame::InputEvent_Right  ( )
		{
			if (mGameState.mOutputToRS232) STRS232::MomentaryRelay_RIGHT();
			(*this).PrivateTranslatePiece( 1 );
		}



		void   STGame::InputEvent_Down  ( )
		{
			(*this).PrivateFreeFallPiece( );
		}



		void   STGame::InputEvent_ZeroLeft   ( )
		{
			while ( (*this).PrivateTranslatePiece( -1 ) );
		}



		void   STGame::InputEvent_ZeroRight  ( )
		{
			while ( (*this).PrivateTranslatePiece( 1 ) );
		}



		void   STGame::InputEvent_ZeroDown  ( )
		{
			while ( (*this).PrivateVTranslatePiece( ) );
		}



		void   STGame::InputEvent_Rotate ( )
		{
			if (mGameState.mOutputToRS232) STRS232::MomentaryRelay_ROTATE();
			(*this).PrivateRotatePiece();
		}



		void   STGame::InputEvent_CounterRotate ( )
		{
			(*this).PrivateCounterRotatePiece();
		}



		void   STGame::InputEvent_Drop   ( )
		{
			if (mGameState.mOutputToRS232) STRS232::MomentaryRelay_DROP();
			(*this).PrivateDropPiece();
		}


		void   STGame::InputEvent_Reset  ( )
		{
			if (mGameState.mOutputToRS232) STRS232::MomentaryRelay_RESET();
			(*this).PrivateGameReset();
		}



		void   STGame::InputEvent_Pause  ( )
		{
			mGameState.mPaused = 1;
		}



		void   STGame::InputEvent_Resume ( )
		{
			mGameState.mPaused = 0;
		}



		void   STGame::InputEvent_ShowNextPieceOn  ( )
		{
			mGameState.mShowNextPiece = 1;
		}



		void   STGame::InputEvent_ShowNextPieceOff ( )
		{
			mGameState.mShowNextPiece = 0;
		}


		void   STGame::InputEvent_AIStart ( )
		{
			mGameState.mAI = 1;
			(*this).ClearAllStatistics();
			(*this).InputEvent_Reset(); // Starting AI should start clean
		}



		void   STGame::InputEvent_AIStop  ( )
		{
			mGameState.mAI = 0;
			(*this).ClearAllStatistics();
			(*this).InputEvent_Reset(); // Stopping AI should stop clean
		}



		void   STGame::InputEvent_VideoStart ( )
		{
			mGameState.mShadowMode = 0; // HACK: Stop shadow mode
			mGameState.mGameSpeedAdjustment = 0;
			mGameState.mSpawnFromVideoCapture = 1;

            // Switch piece sequence generator to queue mode.
            mGameState.mSTPieceSequence.ServerQueueReset();
            mGameState.mSTPieceSequence.ClientSelectPieceSelectionSource
            ( 
				STPieceSequence::STPieceSelectionSourceQueue
            );

			// Clobber current piece, board, etc.
			(*this).PrivateGameReset();
			(*this).ClearAllStatistics();
		}



		void   STGame::InputEvent_VideoStop  ( )
		{
			mGameState.mSpawnFromVideoCapture = 0;

            // Switch piece sequence generator to "random" mode.
            mGameState.mSTPieceSequence.ServerQueueReset();
            mGameState.mSTPieceSequence.ClientSelectPieceSelectionSource
            (
			STPieceSequence::STPieceSelectionSourceRandom
            );

			// Clobber current piece, board, etc.
			(*this).PrivateGameReset();
			(*this).ClearAllStatistics();
		}



		void   STGame::InputEvent_RS232Start ( )
		{
			mGameState.mOutputToRS232 = 1;
		}



		void   STGame::InputEvent_RS232Stop  ( )
		{
			mGameState.mOutputToRS232 = 0;
		}



		void   STGame::InputEvent_AutoRestartOn ( )
		{
			mGameState.mAutoRestart = 1;
		}



		void   STGame::InputEvent_AutoRestartOff  ( )
		{
			mGameState.mAutoRestart = 0;
		}



		void   STGame::InputEvent_GameSpeedIncrease ( )
		{
			if ( mGameState.mGameSpeedAdjustment >= 8 ) mGameState.mGameSpeedAdjustment *= 2;
			else mGameState.mGameSpeedAdjustment++;
		}



		void   STGame::InputEvent_GameSpeedDecrease ( )
		{
			if ( mGameState.mGameSpeedAdjustment > 8 ) mGameState.mGameSpeedAdjustment /= 2;
			else mGameState.mGameSpeedAdjustment--;
		}



		void   STGame::InputEvent_GameBoardIncrease ( )
		{
			int width  = 0;
			int height = 0;
			int length = 0;

			width  = mGameState.mSTBoardCurrent.GetWidth();
			height = mGameState.mSTBoardCurrent.GetHeight();

			length = width;
			if (length > (height/2))  length = (height/2);

			length++;

			if (length <   4)  length =   4;
			if (length > 200)  length = 200;

			width  = length;
			height = (2 * length);

			mGameState.mSTBoardCurrent.SetBoardDimensions( width, height );
			mGameState.mSTBoardCurrent.ClearCells();

			(*this).InputEvent_Reset();
			(*this).ClearAllStatistics();
		}



		void   STGame::InputEvent_GameBoardDecrease ( )
		{
			int width  = 0;
			int height = 0;
			int length = 0;

			width  = mGameState.mSTBoardCurrent.GetWidth();
			height = mGameState.mSTBoardCurrent.GetHeight();

			length = width;
			if (length > (height/2))  length = (height/2);

			length--;

			if (length <   4)  length =   4;
			if (length > 200)  length = 200;

			width  = length;
			height = (2 * length);

			mGameState.mSTBoardCurrent.SetBoardDimensions( width, height );
			mGameState.mSTBoardCurrent.ClearCells();

			(*this).InputEvent_Reset();
			(*this).ClearAllStatistics();
		}



		void   STGame::InputEvent_GameBoardIncreaseWidth ( )
		{
			int width  = 0;
			int height = 0;
			width  = mGameState.mSTBoardCurrent.GetWidth();
			height = mGameState.mSTBoardCurrent.GetHeight();
			width++;
			if (width <   4)  width =   4;
			if (width > 200)  width = 200;
			mGameState.mSTBoardCurrent.SetBoardDimensions( width, height );
			mGameState.mSTBoardCurrent.ClearCells();
			(*this).InputEvent_Reset();
			(*this).ClearAllStatistics();
		}



		void   STGame::InputEvent_GameBoardDecreaseWidth ( )
		{
			int width  = 0;
			int height = 0;
			width  = mGameState.mSTBoardCurrent.GetWidth();
			height = mGameState.mSTBoardCurrent.GetHeight();
			width--;
			if (width <   4)  width =   4;
			if (width > 200)  width = 200;
			mGameState.mSTBoardCurrent.SetBoardDimensions( width, height );
			mGameState.mSTBoardCurrent.ClearCells();
			(*this).InputEvent_Reset();
			(*this).ClearAllStatistics();
		}



		void   STGame::InputEvent_GameBoardIncreaseHeight ( )
		{
			int width  = 0;
			int height = 0;
			width  = mGameState.mSTBoardCurrent.GetWidth();
			height = mGameState.mSTBoardCurrent.GetHeight();
			height++;
			if (height <   4)  height =   4;
			if (height > 200)  height = 200;
			mGameState.mSTBoardCurrent.SetBoardDimensions( width, height );
			mGameState.mSTBoardCurrent.ClearCells();
			(*this).InputEvent_Reset();
			(*this).ClearAllStatistics();
		}



		void   STGame::InputEvent_GameBoardDecreaseHeight ( )
		{
			int width  = 0;
			int height = 0;
			width  = mGameState.mSTBoardCurrent.GetWidth();
			height = mGameState.mSTBoardCurrent.GetHeight();
			height--;
			if (height <   4)  height =   4;
			if (height > 200)  height = 200;
			mGameState.mSTBoardCurrent.SetBoardDimensions( width, height );
			mGameState.mSTBoardCurrent.ClearCells();
			(*this).InputEvent_Reset();
			(*this).ClearAllStatistics();
		}



		void   STGame::InputEvent_SZPieceModeStart ( )
		{
            mGameState.mSTPieceSequence.ClientSelectPieceSelectionSource
				( STPieceSequence::STPieceSelectionSourceAlternatingSAndZ );
			(*this).InputEvent_Reset();
			(*this).ClearAllStatistics();
		}



		void   STGame::InputEvent_SZPieceModeStop  ( )
		{
			if (0 != mGameState.mSpawnFromVideoCapture)
			{
                // Switch piece sequence source to queue
                mGameState.mSTPieceSequence.ServerQueueReset();
                mGameState.mSTPieceSequence.ClientSelectPieceSelectionSource
					( STPieceSequence::STPieceSelectionSourceQueue );
			}
			else
			{
                // Set piece sequence source to random
                mGameState.mSTPieceSequence.ClientSelectPieceSelectionSource
					( STPieceSequence::STPieceSelectionSourceRandom );
			}
		}



		void   STGame::InputEvent_GameStateWriteToFile( )
		{
			STGameFile::SaveGameState( (*this).mGameState );
		}



		void STGame::InputEvent_ToggleAutoWriteFile( )
		{
			if (0 != mGameState.mAutoWriteFile)
				mGameState.mAutoWriteFile = 0;
			else
				mGameState.mAutoWriteFile = 1;
		}



		void   STGame::LoadGameStateFromFile( char * pcstr_FilePathAndName )
		{
			// Do a hard-reset
			(*this).InputEvent_HardReset();

			// Pause the game
			if (0 == ((*this).GameIsPaused()))
				(*this).InputEvent_Pause();

			// Load Game State from specified file.
			STGameFile::LoadGameState( mGameState, pcstr_FilePathAndName );
		}



		void   STGame::InputEvent_ShadowModeCycle ( )
		{
			mGameState.mShadowMode++;
			if (mGameState.mShadowMode > 2) mGameState.mShadowMode = 0; // Wrapping
			if (mGameState.mShadowMode < 0) mGameState.mShadowMode = 0; // Insanity
		}



		void  STGame::InputEvent_SoftReset( )
		{
            // The goal is to start the game over with the same deterministic initial
            // conditions.  Thus, even when a game is over we can play the exact same
            // game again.
            // The key is to restore the initial conditions of the piece sequence 
            // generator.
            int pieceSequenceSource = 
				STPieceSequence::STPieceSelectionSourceRandom;
            __int64 randomSeedMostRecentlyUsedToInitializeRandomNumberGenerator = (__int64) 0;


            // Cache the random seed and the next piece
            pieceSequenceSource =
                mGameState.mSTPieceSequence.DirectGetPieceSelectionSource();

            randomSeedMostRecentlyUsedToInitializeRandomNumberGenerator = 
                mGameState.mSTPieceSequence.DirectGetSeedUsedMostRecentlyToInitializeRandomNumberGenerator( );


			// Do a regular reset...
			(*this).InputEvent_Reset();
			(*this).ClearAllStatistics();


            // Restore the random seed
            mGameState.mSTPieceSequence.ClientSelectPieceSelectionSource
                ( pieceSequenceSource );

            mGameState.mSTPieceSequence.ClientRequestSelectionGeneratorReset
                ( randomSeedMostRecentlyUsedToInitializeRandomNumberGenerator );
		}





		void   STGame::InputEvent_HardReset( )
		{
			// Do a regular reset...
			(*this).InputEvent_Reset();

			// ...and clear other flags and statistics.
			mGameState.mSTBoardCurrent.SetBoardDimensions( 10, 20 );
			mGameState.mSTBoardCurrent.ClearCells();
			mGameState.mSTPieceCurrent.Clear();

			mGameState.mGameOver              = 0;
			mGameState.mCurrentPiecePointValue    = 0;
			mGameState.mCompletedRows             = 0;

            // Set piece sequence source to RANDOM
            mGameState.mSTPieceSequence.ClientSelectPieceSelectionSource
				( STPieceSequence::STPieceSelectionSourceRandom );
            SeedPieceSequenceGeneratorWithCurrentTime( );

			mGameState.mPaused                = 0;
			mGameState.mShowNextPiece         = 0;
			mGameState.mAI                    = 0;
			mGameState.mSpawnFromVideoCapture = 0;
			mGameState.mOutputToRS232         = 0;
			mGameState.mAutoRestart           = 0;
			mGameState.mAutoWriteFile         = 0;
			mGameState.mGameSpeedAdjustment       = 0;
			mGameState.mShadowMode                = 0;
			mGameState.mHintMode              = 0;
			mGameState.mAnimateAIMovesEnable      = 0;

			(*this).ClearAllStatistics();
		}



		void   STGame::InputEvent_AddRowOfJunk( )
		{
			mGameState.mSTBoardCurrent.LiftPileByOneRowAndAddRandomJunk();
		}




		void   STGame::InputEvent_HintModeStart ( )
		{
			mGameState.mHintMode = 1;

			(*this).PrivateUpdateBestMovePiece( );
		}



		void   STGame::InputEvent_HintModeStop  ( )
		{
			mGameState.mHintMode = 0;

			// Clear the current "best move" piece
			mGameState.mSTPieceBestMove.Clear( );
		}



		void  STGame::InputEvent_SelectNextAI( )
		{
			STStrategyManager::SelectNextStrategy( );

			// If AI is active, clear the historic row histogram, etc...
			if (0 != mGameState.mAI)
			{
				(*this).ClearAllStatistics( );
				(*this).InputEvent_Reset( ); // Switching AI should switch clean
			}
		}



		void   STGame::InputEvent_ToggleMoveAnimation( )
		{
			if (0 == mGameState.mAnimateAIMovesEnable)
			{
				mGameState.mAnimateAIMovesEnable = 1;
			}
			else
			{
				// Set final safe Y to some absurd level so that any remaining
				// moves are forced to occur.
				mGameState.mAnimateAIMovesFinalSafeY = 
					(1 + mGameState.mSTBoardCurrent.GetHeight());
				// Do any remaining pending moves...
				(*this).PrivateAIAnimationProcessing();
				mGameState.mAnimateAIMovesEnable = 0;
			}
			// In either case, clobber the current state.
			mGameState.mAnimateAIMovesStartingY          = 0;
			mGameState.mAnimateAIMovesFinalSafeY         = 0;
			mGameState.mAnimateAITotalInitialCommands    = 0;
			mGameState.mAnimateAICommandsExecuted        = 0;
			mGameState.mAnimateAICommandsPerRow          = (0.0f);
			mGameState.mAnimateAIMovesPendingRotation    = 0;
			mGameState.mAnimateAIMovesPendingTranslation = 0;
		}



		void   STGame::ConditionalAdvanceGameTimeByDelta( float f32_DeltaSeconds )
		{
			// All this function does is advance the time, conditionally...
			if (0 != mGameState.mPaused)
				return;

			if (0 != mGameState.mGameOver)
				return;

			// Advance total elapsed time
			mGameState.mTotalElapsedTimeSeconds   += f32_DeltaSeconds;

			// Decrease remaining countdown
			mGameState.mIterationCountdownSeconds -= f32_DeltaSeconds;
		}



		void   STGame::ConditionalAdvanceGameIteration( int forceMove )
		{
			if (0 != mGameState.mPaused)
				return;

			if (0 != mGameState.mAI)
			{
				(*this).PrivateAIProcessing();
			}

			if (0 != mGameState.mAutoRestart)
			{
				(*this).PrivateAutoRestartProcessing();
			}

			if (0 != mGameState.mGameOver)
				return;


			if ((mGameState.mIterationCountdownSeconds <= 0.0f) || (0 != forceMove))
			{
				// If we do not currently have a piece, attempt to spawn a piece...
				if (0 == mGameState.mSTPieceCurrent.IsValid())
				{
					// Attempt to spawn a piece.
					(*this).PrivateSpawnPiece();

					// If the spawn occurred, reset the iteration countdown, otherwise
					// the countdown remains expired, forcing us to try spawning repeatedly
					// until we succeed.
					if (0 != mGameState.mSTPieceCurrent.IsValid())
					{
						mGameState.mIterationCountdownSeconds = PrivateGetCountdownInitialValue();
					}
				}
				else
				{
					// We have a piece.  Attempt a free-fall iteration.
					// Unconditionally reset the countdown.
					if ( mGameState.mAnimateAIMoves.empty() )
					(*this).PrivateFreeFallPiece();
					mGameState.mIterationCountdownSeconds = PrivateGetCountdownInitialValue();
				}
			}
		}




		void  STGame::GetCopyOfNextPiece( STPiece & ref_NextPiece )
		{
			ref_NextPiece.CopyFromPiece( mGameState.mSTPieceNext );
		}




		void STGame::GetCopyOfBestPiece( STPiece & piece )
		{
			piece.CopyFromPiece( mGameState.mSTPieceBestMove );
		}




		void   STGame::SpawnSpecifiedPiece( int pieceType )
		{
			// Add piece to FIFO part of mGameState.m_StandardTetrisPieceSequence...
			mGameState.mSTPieceSequence.ServerQueueSubmitPiece( pieceType );
		}




		void   STGame::PrivateGameReset( )
		{
			int i = 0;
			int n = 0;

			// Game Reset does NOT clobber all parts of the game object!
			// For example, the following are not cleared:
			//   (1) Paused flag;
			//   (2) Historical statistics;
			// However, the following are cleared, for example:
			//   (1) Current board;
			//   (2) Current piece;
			//   (3) Rows Completed;
			//   (4) Score;
			//   (5) Game-Over flag;
			// And the following values are initialized:
			//   (1) Iteration countdown time;


			mGameState.mSTBoardCurrent.ClearCells();
			mGameState.mSTPieceCurrent.Clear();


			mGameState.mGameOver              = 0;
			mGameState.mIterationCountdownSeconds = (0.05f);
			mGameState.mCurrentPiecePointValue    = 0; // Starts at 24+(3*level)
			mGameState.mCompletedRows             = 0;

            // Piece sequence generator
            // Reset queue (even if we aren't using it now)
            mGameState.mSTPieceSequence.ServerQueueReset();
            SeedPieceSequenceGeneratorWithCurrentTime( );

			// UNCHANGED: mGameState.mPaused;
			// UNCHANGED: mGameState.mShowNextPiece;
			// UNCHANGED: mGameState.mAI;
			// UNCHANGED: mGameState.mSpawnFromVideoCapture;
			// UNCHANGED: mGameState.mOutputToRS232;
			// UNCHANGED: mGameState.mAutoRestart;
			// UNCHANGED: mGameState.mGameSpeedAdjustment;

			mGameState.mTotalElapsedTimeSeconds = (0.0f);
			n =   8;  
			for ( i = 0; i < n; i++ )
			{ 
				mGameState.mPieceHistogram [ i ] = 0;
			}

			n = 200;  
			for ( i = 0; i <= n; i++ ) 
			{
				mGameState.mHeightHistogram[ i ] = 0;
			}

			mGameState.mScore = 0;

			// Only updated when game ends
			// UNCHANGED: mGameState.mHistoricHighScore;
			// UNCHANGED: mGameState.mHistoricHighRows;
			// UNCHANGED: mGameState.mHistoricHighPieces;
			// UNCHANGED: mGameState.mHistoricCumulativeRows;
			// UNCHANGED: mGameState.mHistoricTotalGames;
			STTime::SetReferenceTimeToNow();

			// Move Animation
			// UNCHANGED: mGameState.mAnimateAIMovesEnable;
			mGameState.mAnimateAIMovesStartingY          = 0;
			mGameState.mAnimateAIMovesFinalSafeY         = 0;
			mGameState.mAnimateAITotalInitialCommands    = 0;
			mGameState.mAnimateAICommandsExecuted        = 0;
			mGameState.mAnimateAICommandsPerRow          = (0.0f);
			mGameState.mAnimateAIMovesPendingRotation    = 0;
			mGameState.mAnimateAIMovesPendingTranslation = 0;
		}




		float  STGame::PrivateGetCountdownInitialValue ( )
		{
			// -2, -3, -4,... : Slow Mode (delay proportional to index)
			// -1 : Normal, Clipped at 0.20 sec/row
			//  0 : Normal
			// +1 : Fast Mode (still render bound)
			// +2 : Very Fast Mode (still render bound)
			// +3, +4, +5,... : Multiple moves per rendered frame

			if (0 == mGameState.mGameSpeedAdjustment)
			{
				// Normal Tetris Speed Rule
				return( (5.0f /*- (float)GetCurrentLevel()*/) / 20.0f );
			}

			if ((-1) == mGameState.mGameSpeedAdjustment)
			{
				// Normal Tetris Speed Rule, but clamped to 0.20
				float f32_Delay = ( (20.0f /*- (float)GetCurrentLevel()*/) / 20.0f );
				if (f32_Delay < 0.20f) f32_Delay = 0.20f;
				return( f32_Delay );
			}

			if (mGameState.mGameSpeedAdjustment <= (-2))
			{
				// Slowness is proportional to speed adjustment
				float f32_Delay = 10.0f;//((0.5f) * (-(mGameState.mGameSpeedAdjustment)));
				return( f32_Delay );
			}

			if ((1) == mGameState.mGameSpeedAdjustment)
			{
				return( 0.05f ); // Level 9 fastness
			}

			if ((1) == mGameState.mGameSpeedAdjustment)
			{
				return( 0.0f ); // Render speed bound
			}

			if (mGameState.mGameSpeedAdjustment >= (2))
			{
				// Render speed bound...
				//   ...but main function also does multiple calls for even more speed
				return( 0.0f ); 
			}

			// We should never get here
			return( 0.5f ); 
		}





		// Might cause "game over" flag
		void   STGame::PrivateSpawnPiece( )
		{
			// Regardless of how we got here, clear the current piece.
			mGameState.mSTPieceCurrent.Clear();
			mGameState.mSTPieceNext.Clear();
			mGameState.mCurrentPiecePointValue  = 0;
			// Clear the current "best move" piece
			mGameState.mSTPieceBestMove.Clear();


			// Don't spawn if the game is over!
			if (mGameState.mGameOver)
			{
				return;
			}


			// The ONLY source of the next piece is the piece sequence object.
			// It is possible that the "next piece" and even the current piece
			// are currently unavailable.  If the information is not available,
			// we should leave the current piece cleared, which will force the
			// spawning code to continue attempting to spawn.
			int currentKind = 0;
			int nextKind    = 0;
			currentKind = mGameState.mSTPieceSequence.ClientPeekSelectedPieceCurrent();
			nextKind    = mGameState.mSTPieceSequence.ClientPeekSelectedPieceNext();
			mGameState.mSTPieceSequence.ClientRequestSelectionUpdate();


			if ((nextKind >= 1) && (nextKind <= 7))
			{
				// Set Next Piece (NOTE: Might not be available)
				mGameState.mSTPieceNext.SetKind( nextKind );
				mGameState.mSTPieceNext.SetX( 0 ); // NOTE: Not used
				mGameState.mSTPieceNext.SetY( 0 ); // NOTE: Not used
				mGameState.mSTPieceNext.SetOrientation( 1 ); // NOTE: Not used
			}

			if ((currentKind < 1) || (currentKind > 7))
			{
				// No current piece means we should leave and allow this function to
				// be called in the future.
				return;
			}

			// Set current piece
			mGameState.mSTPieceCurrent.SetKind( currentKind );
			mGameState.mSTPieceCurrent.SetX( mGameState.mSTBoardCurrent.GetPieceSpawnX() );
			mGameState.mSTPieceCurrent.SetY( mGameState.mSTBoardCurrent.GetPieceSpawnY() );
			mGameState.mSTPieceCurrent.SetOrientation( 1 );

			// Set the point value for this piece
			mGameState.mCurrentPiecePointValue = 
				24 + (3 * (*this).GetCurrentLevel());

			mGameState.mPieceHistogram[ currentKind ]++; // Update histogram


			// If goal (spawn location) is not acceptable, game is over.
			int okayToSpawn = 0;

			okayToSpawn = mGameState.mSTBoardCurrent.IsGoalAcceptable
				(
				mGameState.mSTPieceCurrent
				);

			if (0 == okayToSpawn)
			{
				// ********************** GAME OVER! **************************
				mGameState.mGameOver = 1;
				// Update historical statistics
				if (mGameState.mScore         > mGameState.mHistoricHighScore)
					mGameState.mHistoricHighScore = mGameState.mScore;
				if (mGameState.mCompletedRows > mGameState.mHistoricHighRows)
					mGameState.mHistoricHighRows  = mGameState.mCompletedRows;
				int totalPieces = 0;
				totalPieces = (*this).GetPieceHistogramSum();
				if (totalPieces > mGameState.mHistoricHighPieces)
					mGameState.mHistoricHighPieces = totalPieces;
				mGameState.mHistoricCumulativeRows += mGameState.mCompletedRows; // Used for average: rows/games
				mGameState.mHistoricTotalGames++;
				int i = 0;
				int n = 20;
				for ( i = (n-1); i >= 1; i-- )
				{
					mGameState.mHistoricRows[i] = mGameState.mHistoricRows[i-1];
				}
				mGameState.mHistoricRows[0] = mGameState.mCompletedRows;
			}
			else
			{
				// Cache the current best move
				(*this).PrivateUpdateBestMovePiece( );
			}

		}




		// Succeeds if allowed
		void   STGame::PrivateRotatePiece( )
		{
			// If current board or piece is invalid, leave.
			if (0 == mGameState.mSTBoardCurrent.IsValid()) return;
			if (0 == mGameState.mSTPieceCurrent.IsValid()) return;

			// Copy the current piece to a temporary piece, rotate it, and determine if
			// it is entirely on the board and does not overlap any occupied cells.
			STPiece  temp_Piece;
			temp_Piece.CopyFromPiece( mGameState.mSTPieceCurrent );
			temp_Piece.Rotate();

			int okayToRotate = 0;
			okayToRotate = 
				mGameState.mSTBoardCurrent.IsGoalAcceptable
				(
				temp_Piece
				);

			if (0 != okayToRotate)
			{
				// Rotation acceptable; Rotate actual piece.
				mGameState.mSTPieceCurrent.Rotate();
			}
		}




		// Succeeds if allowed
		void   STGame::PrivateCounterRotatePiece( )
		{
			// If current board or piece is invalid, leave.
			if (0 == mGameState.mSTBoardCurrent.IsValid()) return;
			if (0 == mGameState.mSTPieceCurrent.IsValid()) return;

			// Copy the current piece to a temporary piece, rotate it, and determine if
			// it is entirely on the board and does not overlap any occupied cells.
			STPiece  temp_Piece;
			temp_Piece.CopyFromPiece( mGameState.mSTPieceCurrent );
			temp_Piece.RotateByCount(3);

			int okayToRotate = 0;
			okayToRotate = 
				mGameState.mSTBoardCurrent.IsGoalAcceptable
				(
				temp_Piece
				);

			if (0 != okayToRotate)
			{
				// Rotation acceptable; Rotate actual piece.
				mGameState.mSTPieceCurrent.RotateByCount(3);
			}
		}




		// Succeeds if allowed
		int    STGame::PrivateTranslatePiece( int horizontalDirectionSign )
		{
			// If current board or piece is invalid, leave.
			if (0 == mGameState.mSTBoardCurrent.IsValid()) return 0;
			if (0 == mGameState.mSTPieceCurrent.IsValid()) return 0;

			// Copy the current piece to a temporary piece, translate it, and determine
			// if it is entirely on the board and does not overlap any occupied cells.
			STPiece  temp_Piece;
			temp_Piece.CopyFromPiece( mGameState.mSTPieceCurrent );
			if (horizontalDirectionSign < 0)
				temp_Piece.Translate( -1, 0 );
			else
				temp_Piece.Translate(  1, 0 );

			int okayToTranslate = 0;
			okayToTranslate = 
				mGameState.mSTBoardCurrent.IsGoalAcceptable
				(
				temp_Piece
				);

			if (0 != okayToTranslate)
			{
				// Translation acceptable; Translate actual piece.
				if (horizontalDirectionSign < 0)
					mGameState.mSTPieceCurrent.Translate( -1, 0 );
				else
					mGameState.mSTPieceCurrent.Translate(  1, 0 );
			}
			return okayToTranslate;
		}
		// Succeeds if allowed
		int    STGame::PrivateVTranslatePiece( )
		{
			// If current board or piece is invalid, leave.
			if (0 == mGameState.mSTBoardCurrent.IsValid()) return 0;
			if (0 == mGameState.mSTPieceCurrent.IsValid()) return 0;

			// Copy the current piece to a temporary piece, translate it, and determine
			// if it is entirely on the board and does not overlap any occupied cells.
			STPiece  temp_Piece;
			temp_Piece.CopyFromPiece( mGameState.mSTPieceCurrent );
			temp_Piece.Translate( 0, -1 );

			int okayToTranslate = 0;
			okayToTranslate = 
				mGameState.mSTBoardCurrent.IsGoalAcceptable
				(
				temp_Piece
				);

			if (0 != okayToTranslate)
			{
				mGameState.mSTPieceCurrent.Translate( 0, -1 );
			}
			return okayToTranslate;
		}




		// Decrements point value, possible landing
		void   STGame::PrivateFreeFallPiece( )
		{
			// If current board or piece is invalid, leave.
			if (0 == mGameState.mSTBoardCurrent.IsValid()) return;
			if (0 == mGameState.mSTPieceCurrent.IsValid()) return;

			// Copy the current piece to a temporary piece, translate it, and determine
			// if it is entirely on the board and does not overlap any occupied cells.
			STPiece  temp_Piece;
			temp_Piece.CopyFromPiece( mGameState.mSTPieceCurrent );
			temp_Piece.Translate(  0, -1 );

			int okayToFall = 0;
			okayToFall = 
				mGameState.mSTBoardCurrent.IsGoalAcceptable
				(
				temp_Piece
				);

			if (0 != okayToFall)
			{
				// Falling acceptable; Translate actual piece.
				mGameState.mSTPieceCurrent.Translate( 0, -1 );
				mGameState.mCurrentPiecePointValue--;
			}
			else
			{
				// Translation not acceptable; piece lands!
				(*this).PrivateTransferPieceToPile();
			}
		}




		// Translates piece as far as it will fall and lands it
		void   STGame::PrivateDropPiece( )
		{
			// If current board or piece is invalid, leave.
			if (0 == mGameState.mSTBoardCurrent.IsValid()) return;
			if (0 == mGameState.mSTPieceCurrent.IsValid()) return;

			mGameState.mScore += mGameState.mCurrentPiecePointValue;
			mGameState.mCurrentPiecePointValue = 0;

			// Try increasingly larger dropping distances until failure.
			// Then transfer the piece to the pile...
			mGameState.mSTBoardCurrent.FullDropAndAddPieceToBoard
				( 
				mGameState.mSTPieceCurrent
				);

			// Collapse any completed rows
			(*this).PrivateCollapseAnyCompletedRows();

			// Clobber current piece
			mGameState.mSTPieceCurrent.Clear();

			// Clobber cached best move
			mGameState.mSTPieceBestMove.Clear();

			// Increment pile height bin
			int resultingPileHeight = 0;
			resultingPileHeight = mGameState.mSTBoardCurrent.PileAvgHeight();
			if ((resultingPileHeight >= 0) && (resultingPileHeight < 200))
			{
				mGameState.mHeightHistogram[ resultingPileHeight ]++;
				mGameState.mHeightHistogram[200]++;
			}
			mGameState.mPileHeightBeta = mGameState.mSTBoardCurrent.PileHeightBeta( mGameState.mHeightHistogram, &mGameState.mPileHeightBetaSamples );
		}




		// Actually puts cells in to board, updates score, clears piece
		void   STGame::PrivateTransferPieceToPile( )
		{
			// If current board or piece is invalid, leave.
			if (0 == mGameState.mSTBoardCurrent.IsValid()) return;
			if (0 == mGameState.mSTPieceCurrent.IsValid()) return;

			if (mGameState.mCurrentPiecePointValue > 0)
				mGameState.mScore += mGameState.mCurrentPiecePointValue;
			mGameState.mCurrentPiecePointValue = 0;


			// Transfer piece to current board
			mGameState.mSTBoardCurrent.AddPieceToBoard
				( 
				mGameState.mSTPieceCurrent
				);

			// Collapse any completed rows
			(*this).PrivateCollapseAnyCompletedRows();

			// Clobber current piece
			mGameState.mSTPieceCurrent.Clear();

			// Clobber cached best move
			mGameState.mSTPieceBestMove.Clear();

			// Increment pile height bin
			int resultingPileHeight = 0;
			resultingPileHeight = mGameState.mSTBoardCurrent.PileAvgHeight();
			if ((resultingPileHeight >= 0) && (resultingPileHeight < 200))
			{
				mGameState.mHeightHistogram[ resultingPileHeight ]++;
				mGameState.mHeightHistogram[200]++;
			}
			mGameState.mPileHeightBeta = mGameState.mSTBoardCurrent.PileHeightBeta( mGameState.mHeightHistogram, &mGameState.mPileHeightBetaSamples );
		}



		void   STGame::PrivateAIProcessing( )
		{
			// Leave if board is invalid.
			if (0 == mGameState.mSTBoardCurrent.IsValid()) return;
			// Leave if no current piece
			if (0 == mGameState.mSTPieceCurrent.IsValid()) return;


			// AI move animation
			if (0 != ((*this).PrivateAIAnimationProcessing()))
				return;


			// HACK HACK HACK HACK HACK HACK HACK HACK HACK HACK HACK HACK 
			// For the moment we do the sub-optimal but reliable thing of
			// waiting for the piece to fall to the first row before giving
			// the AI the chance to decide on a best move.  The border of
			// the board will not block rotations.  (It is still possible
			// for the pile to block rotations.)
			if ( mGameState.mSTPieceCurrent.GetY() != 
				(mGameState.mSTBoardCurrent.GetHeight()-1) )
			{
				// Unless the piece is exactly one row below the top of the board,
				// just give up on doing the AI.
				return;
			}
			// HACK HACK HACK HACK HACK HACK HACK HACK HACK HACK HACK HACK 



			// We don't care how the piece came to be on the board
			// (spawned internally or spawned by external source).
			// If there is a piece, we immediately find the best move
			// and execute it (including a drop).
			int bestRotationDelta    = 0;
			int bestTranslationDelta = 0;

			// COPY THE BOARD AND PIECE BEFORE CALLING AI!
			// We don't want AI messing up board or piece under innocent or
			// cheating circumstances.
			STBoard  CopyOfCurrentBoard;
			STPiece  CopyOfCurrentPiece;

			CopyOfCurrentBoard.CopyFromBoard( mGameState.mSTBoardCurrent );
			CopyOfCurrentPiece.CopyFromPiece( mGameState.mSTPieceCurrent );

			//STStrategyManager::GetBestMoveOncePerPiece
			//	(
			//	CopyOfCurrentBoard,
			//	CopyOfCurrentPiece,
			//	mGameState.mShowNextPiece,  // Showing "Next Piece" ?
			//	mGameState.mSTPieceNext.GetKind(),     // Next piece kind
			//	bestRotationDelta,
			//	bestTranslationDelta
			//	);
			char path[2048] = "";
			STStrategyManager::GetBestMovePath
				(
				CopyOfCurrentBoard,
				CopyOfCurrentPiece,
				mGameState.mShowNextPiece,  // Showing "Next Piece" ?
				mGameState.mSTPieceNext.GetKind(),     // Next piece kind
				path
				);


			if ( 0 == mGameState.mAnimateAIMovesEnable)
			{
				for ( int i = 0; i < 1024 && path[i]; ++i )
				{
					bool err = false;
					switch ( path[i] )
					{
					case 'l':
						(*this).InputEvent_Left();
						break;
					case 'r':
						(*this).InputEvent_Right();
						break;
					case 'd':
						(*this).InputEvent_Down();
						break;
					case 'L':
						(*this).InputEvent_ZeroLeft();
						break;
					case 'R':
						(*this).InputEvent_ZeroRight();
						break;
					case 'D':
						(*this).InputEvent_ZeroDown();
						break;
					case 'z':
						(*this).InputEvent_Rotate();
						break;
					case 'c':
						(*this).InputEvent_CounterRotate();
						break;
					case 'V':
						//(*this).InputEvent_Drop();
						break;
					default:
						err = true;
						break;
					}
					if ( err ) break;
				}
				(*this).InputEvent_Drop();

				//// ROTATE
				//int rotateCount = 0;
				//for ( rotateCount = 0; rotateCount < bestRotationDelta; rotateCount++ )
				//{
				//	(*this).InputEvent_Rotate();
				//}

				//// TRANSLATE
				//int translateCount = 0;
				//if (bestTranslationDelta < 0)
				//{
				//	for ( translateCount = 0; translateCount > bestTranslationDelta; translateCount-- )
				//	{
				//		(*this).InputEvent_Left();
				//	}
				//}
				//if (bestTranslationDelta > 0)
				//{
				//	for ( translateCount = 0; translateCount < bestTranslationDelta; translateCount++ )
				//	{
				//		(*this).InputEvent_Right();
				//	}
				//}

				//// DROP
				//(*this).InputEvent_Drop();
			}
			else
			{
				mGameState.mAnimateAIMoves = path;
				mGameState.mAnimateAIMoves += "V";
				// Set up move to be executed in the future.
				mGameState.mAnimateAIMovesPendingRotation    = bestRotationDelta;
				mGameState.mAnimateAIMovesPendingTranslation = bestTranslationDelta;

				mGameState.mAnimateAITotalInitialCommands = bestRotationDelta;
				if (bestTranslationDelta > 0)
					mGameState.mAnimateAITotalInitialCommands += bestTranslationDelta;
				else
					mGameState.mAnimateAITotalInitialCommands -= bestTranslationDelta;

				mGameState.mAnimateAICommandsExecuted = 0;

				mGameState.mAnimateAIMovesStartingY = mGameState.mSTPieceCurrent.GetY();

				mGameState.mAnimateAIMovesFinalSafeY = 
					(mGameState.mSTBoardCurrent.PileMaxHeight() + 4); // +3 + paranoia!

				int fallRows = 0;
				fallRows = (mGameState.mAnimateAIMovesStartingY -
					mGameState.mAnimateAIMovesFinalSafeY);

				// Worst-case scenario: Execute all moves now!
				mGameState.mAnimateAICommandsPerRow = 
					(float)(1 + mGameState.mAnimateAITotalInitialCommands);

				// If we can fall free for some rows, then compute the number of
				// commands per row we must execute, with a slight over-estimate.
				if (fallRows > 0)
				{
					mGameState.mAnimateAICommandsPerRow = 
						(float)(1 + mGameState.mAnimateAITotalInitialCommands)
						/ (float)(fallRows);
				}

				// Attempt processing immediately!
				// REASON: If the game is very fast, there may only be one cycle
				// before the piece falls by one row, which could terminate the
				// game.  Thus, we have to be able to act immediately, as if the
				// animation effect were turned off.  Otherwise, the performance
				// of the AI would depend on whether or not the animation effect
				// was on, which is bad.
				(*this).PrivateAIAnimationProcessing();
			}
		}



		int   STGame::PrivateAIAnimationProcessing( )
		{
			if ( mGameState.mAnimateAIMoves.empty() )
			{
				return 0;
			}
			if ( !mGameState.mAnimateAIMoves.empty() && mGameState.mIterationCountdownSeconds <= 0 )
			{
				bool err = false;
				for ( ;; )
				{
					int curmove = mGameState.mAnimateAIMoves[0];
					switch ( curmove )
					{
					case 'l':
						(*this).InputEvent_Left();
						break;
					case 'r':
						(*this).InputEvent_Right();
						break;
					case 'd':
						(*this).InputEvent_Down();
						break;
					case 'L':
						(*this).InputEvent_ZeroLeft();
						break;
					case 'R':
						(*this).InputEvent_ZeroRight();
						break;
					case 'D':
						(*this).InputEvent_ZeroDown();
						break;
					case 'z':
						(*this).InputEvent_Rotate();
						break;
					case 'c':
						(*this).InputEvent_CounterRotate();
						break;
					case 'V':
						(*this).InputEvent_Drop();
						break;
					default:
						err = true;
						mGameState.mAnimateAIMoves = " ";
						break;
					}
					mGameState.mAnimateAIMoves = std::string(mGameState.mAnimateAIMoves.c_str()+1);
					if ( mGameState.mAnimateAIMoves.empty() || !(mGameState.mAnimateAIMoves[0] == curmove && curmove == 'd') )
						break;
				}
				mGameState.mAnimateAICommandsExecuted++;
				mGameState.mIterationCountdownSeconds = PrivateGetCountdownInitialValue();
			}
			return 1;

			if (0 == mGameState.mAnimateAIMovesEnable)  return(0);

			if ((0 == mGameState.mAnimateAIMovesPendingRotation) &&
				(0 == mGameState.mAnimateAIMovesPendingTranslation))
			{
				return(0);
			}


			int currentY = 0;
			currentY = mGameState.mSTPieceCurrent.GetY();


			// If we reached the final Y, force the execution of all remaining
			// commands.
			if (currentY <= mGameState.mAnimateAIMovesFinalSafeY)
			{
				// Execute all moves now!
				while (mGameState.mAnimateAIMovesPendingRotation > 0)
				{
					(*this).InputEvent_Rotate();
					mGameState.mAnimateAIMovesPendingRotation--;
					mGameState.mAnimateAICommandsExecuted++;
				}

				while (mGameState.mAnimateAIMovesPendingTranslation != 0)
				{
					if (mGameState.mAnimateAIMovesPendingTranslation < 0)
					{
						(*this).InputEvent_Left();
						mGameState.mAnimateAIMovesPendingTranslation++;
						mGameState.mAnimateAICommandsExecuted++;
					}
					else if (mGameState.mAnimateAIMovesPendingTranslation > 0)
					{
						(*this).InputEvent_Right();
						mGameState.mAnimateAIMovesPendingTranslation--;
						mGameState.mAnimateAICommandsExecuted++;
					}
				}
				return(1);
			}

			// Compute the theoretical number of commands that we should execute
			// up to this point in time.
			int droppedRows = 0;
			droppedRows = (mGameState.mAnimateAIMovesStartingY - currentY);

			int goalExecutedCommands = 0;
			goalExecutedCommands = 
				(int)(mGameState.mAnimateAICommandsPerRow * (float)droppedRows);

			// Execute Rotations until either rotations are exhausted, or
			// the total number of executed commands reaches the desired level.
			while ( ((mGameState.mAnimateAICommandsExecuted) < (goalExecutedCommands)) &&
				(mGameState.mAnimateAIMovesPendingRotation > 0) )
			{
				(*this).InputEvent_Rotate();
				mGameState.mAnimateAIMovesPendingRotation--;
				mGameState.mAnimateAICommandsExecuted++;
			}

			while ( ((mGameState.mAnimateAICommandsExecuted) < (goalExecutedCommands)) &&
				(mGameState.mAnimateAIMovesPendingTranslation != 0) )
			{
				if (mGameState.mAnimateAIMovesPendingTranslation < 0)
				{
					(*this).InputEvent_Left();
					mGameState.mAnimateAIMovesPendingTranslation++;
					mGameState.mAnimateAICommandsExecuted++;
				}
				else if (mGameState.mAnimateAIMovesPendingTranslation > 0)
				{
					(*this).InputEvent_Right();
					mGameState.mAnimateAIMovesPendingTranslation--;
					mGameState.mAnimateAICommandsExecuted++;
				}
			}

			return(1);
		}



		void   STGame::PrivateAutoRestartProcessing( )
		{
			if (0 == mGameState.mAutoRestart)
			{
				return;
			}

			if (0 == mGameState.mGameOver)
			{
				return;
			}

			// Write file if auto-write file is enabled.
			if (mGameState.mAutoWriteFile)
			{
				(*this).InputEvent_GameStateWriteToFile();
			}

			// Reset the game.
			(*this).InputEvent_Reset();
		}



		// Affects total rows
		void   STGame::PrivateCollapseAnyCompletedRows( )
		{
			int totalRowsCollapsed = 0;

			totalRowsCollapsed = 
				mGameState.mSTBoardCurrent.CollapseAnyCompletedRows();

			if (totalRowsCollapsed > 0)
			{
				// We completed one or more rows!
				mGameState.mCompletedRows += totalRowsCollapsed;
			}
		}



		void  STGame::PrivateUpdateBestMovePiece( )
		{
			// Clear the current "best move" piece
			mGameState.mSTPieceBestMove.Clear();

			if (0 == mGameState.mHintMode) return;


			// Set some basic initial conditions
			mGameState.mSTPieceBestMove.SetKind( mGameState.mSTPieceCurrent.GetKind() );
			mGameState.mSTPieceBestMove.SetX( mGameState.mSTBoardCurrent.GetPieceSpawnX() );
			mGameState.mSTPieceBestMove.SetY( mGameState.mSTBoardCurrent.GetPieceSpawnY() );
			mGameState.mSTPieceBestMove.SetOrientation( 1 );

			int bestRotationDelta    = 0;
			int bestTranslationDelta = 0;

			STStrategyManager::GetBestMoveOncePerPiece
				(
				mGameState.mSTBoardCurrent,
				mGameState.mSTPieceBestMove,
				(*this).GameIsShowNextPiece(),
				mGameState.mSTPieceNext.GetKind(),
				bestRotationDelta,
				bestTranslationDelta
				);

			// Apply movement to piece
			mGameState.mSTPieceBestMove.RotateByCount( bestRotationDelta );
			mGameState.mSTPieceBestMove.Translate    ( bestTranslationDelta, 0 );
			// Drop piece
			mGameState.mSTBoardCurrent.DropPieceButBoardUnchanged
				( 
				mGameState.mSTPieceBestMove
				);
		}









        void STGame::GetGeneratorSeed
        (
            __int64 & randomSeedMostRecentlyUsedToInitializeRandomNumberGenerator
        )
        {
            randomSeedMostRecentlyUsedToInitializeRandomNumberGenerator =
                mGameState.mSTPieceSequence.DirectGetSeedUsedMostRecentlyToInitializeRandomNumberGenerator( );
        }












        void STGame::SetGeneratorSeed
        (
            __int64 seedValue
        )
        {
            mGameState.mSTPieceSequence.ClientRequestSelectionGeneratorReset( seedValue );
        }












        void  STGame::SeedPieceSequenceGeneratorWithCurrentTime( )
        {
            // Seed the sequence generator with the time
            __int64 seedValue = 0;

            // Simply use the raw counter value (64-bit value) as the seed.
			unsigned int high = 0;
			unsigned int low = 0;
			STTime::GetCounterCurrentHighLow( high, low );

			seedValue |= high;
			seedValue <<= 32;
			seedValue |= low;

            // Seed the generator
            SetGeneratorSeed( seedValue );
        }











		void STGame::ClearAllStatistics( )
		{
			ClearHistoricRowsHistogram();

			int i = 0;
			int n = 0;
			n =   8; for ( i = 0; i < n; i++ ) mGameState.mPieceHistogram [i] = 0;
			n = 200; for ( i = 0; i < n; i++ ) mGameState.mHeightHistogram[i] = 0;
			n =  20; for ( i = 0; i < n; i++ ) mGameState.mHistoricRows[i]    = 0;
			mGameState.mTotalElapsedTimeSeconds = (0.0f);
			mGameState.mHistoricHighScore      = 0;
			mGameState.mHistoricHighRows       = 0;
			mGameState.mHistoricHighPieces     = 0;
			mGameState.mHistoricCumulativeRows = 0;
			mGameState.mHistoricTotalGames     = 0;
		}






	}
}