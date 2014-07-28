// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STGameHeader
#define STGameHeader



#include "CPF.StandardTetris.STBoard.h"
#include "CPF.StandardTetris.STPiece.h"
#include "CPF.StandardTetris.STPieceSequence.h"
#include "CPF.StandardTetris.STGameState.h"



namespace CPF
{
	namespace StandardTetris
	{



		class STGame
		{

		private: STGameState   mGameState;

		public: STGameState & GetGameState ( )
        {
            return (mGameState);
        }



				 // Constructor and Destructor

		public: STGame();
		public: ~STGame();




				// Core Game State

				// Board and Piece
				// NOTE: Clients of the StandardTetrisGame cannot access the current board
				//       and piece directly.  Clients can request copies.
		public: void GetCopyOfCurrentBoard( STBoard & board );
		public: void GetCopyOfCurrentPiece( STPiece & piece );

				// Core Game State Variables
		public: int   GameIsFinished( )
				{ return( mGameState.mGameOver ); }
		public: float GetIterationCountdownSeconds( )
				{ return( mGameState.mIterationCountdownSeconds ); }
		public: int   GetCurrentPiecePointValue( )
				{ return( mGameState.mCurrentPiecePointValue ); }
		public: int   GetCompletedRows( )
				{ return( mGameState.mCompletedRows ); }
		public: int   GetCurrentLevel( )
				{
					int currentLevel = ((mGameState.mCompletedRows - 1) / 10);
					if (currentLevel < 0)  currentLevel = 0;
					if (currentLevel > 9)  currentLevel = 9;
					return( currentLevel );    
				}




				// User Options
				// (NOTE: The user can only change the game state via input events.)

		public: int GameIsPaused               ( )
				{ return( mGameState.mPaused                ); }
		public: int GameIsShowNextPiece        ( )
				{ return( mGameState.mShowNextPiece         ); }
		public: int GameIsAI                   ( )
				{ return( mGameState.mAI                    ); }
		public: int GameIsSpawnFromVideoCapture( )
				{ return( mGameState.mSpawnFromVideoCapture ); }
		public: int GameIsOutputToRS232        ( )
				{ return( mGameState.mOutputToRS232         ); }
		public: int GameIsAutoRestart          ( )
				{ return( mGameState.mAutoRestart           ); }
		public: int GetGameSpeedAdjustment( )
				{ return( mGameState.mGameSpeedAdjustment       ); }
		public: int GetGameShadowMode( )
				{ return( mGameState.mShadowMode ); }
		public: int GameIsHintMode( )
				{ return( mGameState.mHintMode ); }
    public: int GameIsMonochromeMode ( )
        { return ( mGameState.mMonochromeColorMode ); }
		public: int GameIsAutoWriteFile( )
				{ return( mGameState.mAutoWriteFile ); }
		public: int GameIsAnimateMoves( )
				{ return( mGameState.mAnimateAIMovesEnable ); }




				// Input Events
				// Clients of the game object can call these event functions based on user
				// input (key pressing), AI decisions, or other sources.

				// Core Game Controls
		public: void   InputEvent_Left   ( );
		public: void   InputEvent_Right  ( );
		public: void   InputEvent_Down  ( );
		public: void   InputEvent_ZeroLeft   ( );
		public: void   InputEvent_ZeroRight  ( );
		public: void   InputEvent_ZeroDown  ( );
		public: void   InputEvent_Rotate ( );
		public: void   InputEvent_CounterRotate ( );
		public: void   InputEvent_Drop   ( );
				// Basic Game Management
		public: void   InputEvent_Reset  ( );
		public: void   InputEvent_Pause  ( );
		public: void   InputEvent_Resume ( );
				// User Options
		public: void   InputEvent_ShowNextPieceOn  ( );
		public: void   InputEvent_ShowNextPieceOff ( );
		public: void   InputEvent_AIStart ( );
		public: void   InputEvent_AIStop  ( );
		public: void   InputEvent_VideoStart ( );
		public: void   InputEvent_VideoStop  ( );
		public: void   InputEvent_RS232Start ( );
		public: void   InputEvent_RS232Stop  ( );
		public: void   InputEvent_AutoRestartOn ( );
		public: void   InputEvent_AutoRestartOff  ( );
		public: void   InputEvent_GameSpeedIncrease ( );
		public: void   InputEvent_GameSpeedDecrease ( );
		public: void   InputEvent_GameStateWriteToFile ( );

		public: void   InputEvent_GameBoardIncrease ( );
		public: void   InputEvent_GameBoardDecrease ( );

		public: void   InputEvent_GameBoardIncreaseWidth ( );
		public: void   InputEvent_GameBoardDecreaseWidth ( );
		public: void   InputEvent_GameBoardIncreaseHeight ( );
		public: void   InputEvent_GameBoardDecreaseHeight ( );

		public: void   InputEvent_SZPieceModeStart ( );
		public: void   InputEvent_SZPieceModeStop  ( );
		public: void   InputEvent_ShadowModeCycle( );
		public: void   InputEvent_HardReset( );
		public: void   InputEvent_AddRowOfJunk( );
		public: void   InputEvent_HintModeStart ( );
		public: void   InputEvent_HintModeStop  ( );
		public: void   InputEvent_SoftReset( );
		public: void   InputEvent_SelectNextAI( );
		public: void   InputEvent_ToggleAutoWriteFile( );
		public: void   InputEvent_ToggleMoveAnimation( );







				// Access to Statistics

		public: LongLong GetScore( ) 
				{ 
					return( mGameState.mScore ); 
				}

		public: float  GetTotalGameTime( )
				{ 
					return( mGameState.mTotalElapsedTimeSeconds ); 
				}

		public: int    GetPieceHistogramBinValue( int pieceKind )
				{
					// 1==O,2==I,3==S,4==Z,5==L,6==J,7==T;   0==NONE
					if ((pieceKind >= 1) && (pieceKind <= 7))
					{
						return( mGameState.mPieceHistogram[ pieceKind ] );
					}
					return(0);
				}

		public: int    GetPieceHistogramSum( )
				{
					return
						( 
						mGameState.mPieceHistogram[ 1 ]  // O
					+ mGameState.mPieceHistogram[ 2 ]  // I
					+ mGameState.mPieceHistogram[ 3 ]  // S
					+ mGameState.mPieceHistogram[ 4 ]  // Z
					+ mGameState.mPieceHistogram[ 5 ]  // L
					+ mGameState.mPieceHistogram[ 6 ]  // J
					+ mGameState.mPieceHistogram[ 7 ]  // T
					);
				}

		public: double GetPileHeightBeta( )
				{
					return mGameState.mPileHeightBeta;
				}

		public: int GetPileHeightBetaSamples( )
				{
					return mGameState.mPileHeightBetaSamples;
				}

		public: int GetHistoricTotalGames ( )
				{ 
					return( mGameState.mHistoricTotalGames ); 
				}

		public: LongLong GetHistoricAverageRows( )
				{
					if (mGameState.mHistoricTotalGames <= 0) return(0);
					return( mGameState.mHistoricCumulativeRows / mGameState.mHistoricTotalGames );
				}

		public: LongLong GetHistoricHighScore ( ) { return( mGameState.mHistoricHighScore  ); }
		public: int   GetHistoricHighRows  ( ) { return( mGameState.mHistoricHighRows   ); }
		public: double   GetHistoricRowsAtBottom  ( )
				{
					if ( mGameState.mHeightHistogram[200] <= 0 ) return 0;
					return (double)( mGameState.mHeightHistogram[0] + mGameState.mHeightHistogram[1] + mGameState.mHeightHistogram[2] ) / mGameState.mHeightHistogram[200] ;
				}
		public: int   GetHistoricHighPieces( ) { return( mGameState.mHistoricHighPieces ); }
		public: LongLong   GetHistoricCumulativeRows( ) { return( mGameState.mHistoricCumulativeRows ); }


		public: LongLong GetHeightHistogramBinValue( int height )
				{
					if (height <    0) return(0);
					if (height >= 200) return(0);
					return( mGameState.mHeightHistogram[ height ] );
				}

		public: int   GetHistoricRowsBinValue( int index )
				{
					if (index <   0) return(0);
					if (index >= 20) return(0);
					return( mGameState.mHistoricRows[ index ] );
				}

		public: void ClearHistoricRowsHistogram( )
				{
					int i = 0;
					int n = 0;
					n = 20;
					for ( i = 0; i < n; i++ )
					{
						mGameState.mHistoricRows[i] = 0;
					}
				}

		public: void ClearAllStatistics( );











		public: void ClearPreviousClassification()
				{
					(*this).mGameState.mPreviousClassification = 0;
				}


		public: int GetCalibrationModeFlagValue()
				{
					return ((*this).mGameState.mCalibrationModeFlag);
				}


		public: void SetCalibrationModeFlagValue( int flag )
				{
					(*this).mGameState.mCalibrationModeFlag = flag;
				}


		public: void SetCalibrationModeShapeCode ( int value )
				{
					(*this).mGameState.mCalibrationModeShapeCode = value;
				}





				// Instruction Control

		public: int  InstructionGetState()
				{
					return( (*this).mGameState.mShowInstructionPage ); 
				}


		public: void InstructionsShow()
				{
					(*this).mGameState.mShowInstructionPage = 1;
				}


		public: void InstructionsHide()
				{
					(*this).mGameState.mShowInstructionPage = 0;
				}


		public: void InstructionsNextPage()
				{
					(*this).mGameState.mShowInstructionPage++; 
				}


		public: void InstructionsPreviousPage()
				{
					if ((*this).mGameState.mShowInstructionPage > 1)
					{
						(*this).mGameState.mShowInstructionPage--;
					}
				}


		public: void ConsoleShow()
				{
					(*this).mGameState.mShowConsole = 1;
				}


		public: void ConsoleHide()
				{
					(*this).mGameState.mShowConsole = 0;
				}


		public: void SetReportedFrameRate
					(
					double reportedFrameRate
					)
				{
					(*this).mGameState.mReportedFrameRate = reportedFrameRate;
				}


		public: double GetReportedFrameRate()
				{
					return( (*this).mGameState.mReportedFrameRate ); 
				}







				// Call ConditionalAdvanceGameTimeByDelta() each time the time interval
				// is updated (typically once per rendering frame).
				// Call ConditionalAdvanceGameIteration() directly after 
				// ConditionalAdvanceGameTimeByDelta(), at least once, or possibly 
				// multiple times per frame, to potentially force multiple game iterations
				// that do not depend on real time.

		public: void   ConditionalAdvanceGameTimeByDelta( float f32_DeltaSeconds ); // Once per frame
		public: void   ConditionalAdvanceGameIteration  ( int forceMove ); // Once or more per frame



				// The following function is only useful for rendering purposes, since this
				// board is likely to include a piece that is still falling.

		public: void GetCopyOfNextPiece( STPiece & ref_NextPiece );



				// The following is used by external data sources to add pieces to the 
				// piece sequence FIFO (if enabled).

		public: void   SpawnSpecifiedPiece( int pieceType );




				// Piece Sequence

		public: void GetGeneratorSeed
        (
            __int64 & randomSeedMostRecentlyUsedToInitializeRandomNumberGenerator
        );

        public: void SetGeneratorSeed( __int64 seedValue );

        public: void SeedPieceSequenceGeneratorWithCurrentTime( );


		public: STPieceSequence & GetRefSequenceObject( )
				{ 
					return( mGameState.mSTPieceSequence ); 
				}

		public: int GetGeneratorType( ) 
				{ 
					return( mGameState.mSTPieceSequence.ClientCheckPieceSelectionSource() ); 
				}

		public: void SetGeneratorType( int generatorType )
				{ 
					mGameState.mSTPieceSequence.ClientSelectPieceSelectionSource( generatorType ); 
				}

		public: int GetPieceSelectionSource( )
				{
					return( mGameState.mSTPieceSequence.ClientCheckPieceSelectionSource() );
				}



				// Miscellaneous

		public: int GetBoardWidth( )
				{ 
					return( mGameState.mSTBoardCurrent.GetWidth() ); 
				}

		public: int GetBoardHeight( )
				{ 
					return( mGameState.mSTBoardCurrent.GetHeight() ); 
				}

		public: void  GetCopyOfBestPiece( STPiece & piece );

		public: void  LoadGameStateFromFile( char * pcstr_FilePathAndName );



				// The following methods are intended for use by the game object for 
				// internal processing.


		private: void   PrivateGameReset( ); 
		private: float  PrivateGetCountdownInitialValue ( );

		private: void   PrivateSpawnPiece( ); // Might cause "game over" flag
		private: void   PrivateRotatePiece( ); // Succeeds if allowed
		private: void   PrivateCounterRotatePiece( ); // Succeeds if allowed
		private: int    PrivateTranslatePiece( int horizontalDirectionSign ); // Succeeds if allowed
		private: int    PrivateVTranslatePiece( ); // Succeeds if allowed
		private: void   PrivateFreeFallPiece( ); // Decrements point value, possible landing
		private: void   PrivateDropPiece( );  // Translates piece as far as it will fall and lands it
		private: void   PrivateTransferPieceToPile( ); // Actually puts cells in to board, updates score, clears piece

		private: void   PrivateAIProcessing( );
		private: int    PrivateAIAnimationProcessing( );
		private: void   PrivateAutoRestartProcessing( );
		private: void   PrivateCollapseAnyCompletedRows( ); // Affects total rows
		private: void   PrivateUpdateBestMovePiece( );

		};


	}
}




#endif






