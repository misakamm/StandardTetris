// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STGameStateHeader
#define STGameStateHeader



#include "CPF.StandardTetris.STBoard.h"
#include "CPF.StandardTetris.STPiece.h"
#include "CPF.StandardTetris.STPieceSequence.h"


namespace CPF
{
	namespace StandardTetris
	{


		// The following class contains the complete state information for a game.
		// ALL FIELDS ARE PUBLIC.  This means anyone who gains possession of this
		// object can change whatever they wish.  The idea is that the game object
		// will have a PRIVATE game state object, but the code of the game object
		// can offer trusted external methods (such as file I/O methods) full access
		// to the game state object.



		class STGameState
		{

			// Core Game State

			// Board and Piece
		public: STBoard   mSTBoardCurrent;
		public: STPiece   mSTPieceCurrent;

				// Core Game State Variables
		public: int      mGameOver;
		public: float    mIterationCountdownSeconds;
		public: int      mCurrentPiecePointValue; // Starts at 24+(3*level)
		public: int      mCompletedRows;

				// Piece Sequence Generator
		public: STPieceSequence   mSTPieceSequence;




				// User Options

		public: int      mPaused;
		public: int      mShowNextPiece;
		public: int      mAI;
		public: int      mSpawnFromVideoCapture;
		public: int      mOutputToRS232;
		public: int      mAutoRestart;
		public: int      mAutoWriteFile;

				// Game Speed Adjustment
				// -2, -3, -4,... : Slow Mode (delay proportional to index)
				// -1 : Normal, Clipped at 0.20 sec/row
				//  0 : Normal
				// +1 : Fast Mode (still render bound)
				// +2 : Very Fast Mode (still render bound)
				// +3, +4, +5,... : Multiple moves per rendered frame
		public: int                   mGameSpeedAdjustment;

		public: int                   mShadowMode; // 0==OFF, 1==SHADOW, 2==SHADOW-ONLY
		public: int                   mHintMode;
		public: int                   mMonochromeColorMode;





				// Statistics for User Consideration Only

				// Updated at piece spawning or row completion
		public: int                   mPieceHistogram [   8 ]; // Count of each piece type
		public: LongLong              mHeightHistogram[ 202 ]; // Height after each landing
		public: double                mPileHeightBeta;
		public: int                   mPileHeightBetaSamples;
		public: float                 mTotalElapsedTimeSeconds;
		public: LongLong              mScore;

				// Only updated when game ends
		public: LongLong              mHistoricHighScore;
		public: int                   mHistoricHighRows;
		public: int                   mHistoricHighPieces;
		public: LongLong              mHistoricCumulativeRows;  // Used to get average: rows / games
		public: int                   mHistoricTotalGames;
		public: int                   mHistoricRows [ 20 ]; // Past games [0]==most recent




				// Cached Derived or Copied Values

				// Next Piece (Not always used or available; Only 'kind' aspect is relevant)
		public: STPiece   mSTPieceNext;
				// Best Move (determined by relevant AI upon piece spawning)
		public: STPiece   mSTPieceBestMove;



				// Move Animation Effect

		public: int        mAnimateAIMovesEnable;
		public: int        mAnimateAIMovesStartingY;
		public: int        mAnimateAIMovesFinalSafeY;
		public: int        mAnimateAITotalInitialCommands;
		public: int        mAnimateAICommandsExecuted;
		public: double     mAnimateAICommandsPerRow;
		public: int        mAnimateAIMovesPendingRotation;
		public: int        mAnimateAIMovesPendingTranslation;
		public: std::string mAnimateAIMoves;



				// RUN-TIME STATE INFORMATION (DO NOT STORE IN A FILE)

				// video capture related
		public: int mPreviousClassification;

		public: int mSelectionState;
		public: int mSelectionX1;
		public: int mSelectionY1;
		public: int mSelectionX2;
		public: int mSelectionY2;

		public: int mCalibrationModeFlag; // false == OFF
		public: int mCalibrationModeShapeCode;  // 1..7 shape

				// file list
		public: int mFirstItem;
		public: int mRelativeItem;
		public: int mShowFileList;
		public: int mLoadFlag;

				// miscellaneous
		public: int mRenderFrameNumber;
		public: int mShowInstructionPage;
		public: int mShowConsole;
		public: double mReportedFrameRate;


		public: STGameState( );
		};



	}
}




#endif






