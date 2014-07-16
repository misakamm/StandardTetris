// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1


#include "CPF.StandardTetris.STGameFile.h"



#include "CPF.StandardTetris.STDateTime.h"
#include "CPF.StandardTetris.STEngine.h"
#include "CPF.StandardTetris.STGameState.h"
#include "CPF.StandardTetris.STGame.h"
#include "CPF.StandardTetris.STBoard.h"
#include "CPF.StandardTetris.STPiece.h"
#include "CPF.StandardTetris.STFile.h"
#include "CPF.StandardTetris.STRandom.h"
#include "CPF.StandardTetris.STPieceSequence.h"
#include "CPF.StandardTetris.STStrategyManager.h"
#include "CPF.StandardTetris.STGameDrawing.h"



// Disable unimportant "symbol too long" error for debug builds of STL string
#pragma warning( disable : 4786 )

#include <string>
#include <vector>
using namespace std;




namespace CPF
{
	namespace StandardTetris
	{





		int  STGameFile::SaveGameState
			( 
			STGameState & gameState
			)
		{
			STFileWriter  file;
			int result = 0;

			int total = 0;
			int index = 0;

			
			// File name is "tetris_state_YYYYMMDD_HHMMSSMMM.txt"
			
			STDateTime  DateTime;
			DateTime.GetCurrentDateAndTime();
			string string_DateTime;
			DateTime.GetStringYYYYMMDD_HHMMSSMMM( string_DateTime );
			string string_FileName;
			string_FileName  = "";
			string_FileName += (string)("tetris_state_");
			string_FileName += string_DateTime;
			string_FileName += (string)(".txt");
			

			
			// Path is the same as the path to the application *.EXE file

			string string_Path;
			string_Path  = "";
			string_Path += (string)(STEngine::GetApplicationPathOnlyANSI());
			

			
			// File Path and Name
			
			string string_FilePathAndName;
			string_FilePathAndName  = "";
			string_FilePathAndName += string_Path;
			string_FilePathAndName += "\\";
			string_FilePathAndName += string_FileName;
			

			result = file.Open( (char *)(string_FilePathAndName.c_str()) );

			if (0 != result)  return(-1);


			// Simply print out game state information to the text file.
			// The order of fields doesn't matter.  Fields can be added
			// or removed.  It is the consumer's responsibility to 
			// acquire any fields and compensate for "missing" fields.
			// NOTE: Not all of these fields are supposed to be restored
			// on loading!  Many are DERIVED values, or are session-dependent,
			// or should be cleared upon loading to give the user the 
			// chance to alter settings.  For example, don't restore the
			// pause/unpaused state, or the video capture state.


			file.WriteFormattedText( "fileName                 \"%s\"\n", (char *)(string_FileName.c_str()) );

			DateTime.GetStringYYYY_MMM_DD_HH_MM_SS_MMM( string_DateTime );
			file.WriteFormattedText( "localDateAndTime         \"%s\"\n", (char *)(string_DateTime.c_str()) );
						
			file.WriteFormattedText( "standardTetrisVersion    \"%s\"\n", STEngine::GetApplicationFriendlyNameANSI() );
			

			file.WriteFormattedText( "\n" );

			
			
			file.WriteFormattedText( "boardWidth  %d\n", gameState.mSTBoardCurrent.GetWidth()  );
			file.WriteFormattedText( "boardHeight %d\n", gameState.mSTBoardCurrent.GetHeight() );
			
			file.WriteFormattedText( "boardCurrent\n" );

            int width = 0;
            int height = 0;
			width  = gameState.mSTBoardCurrent.GetWidth();
			height = gameState.mSTBoardCurrent.GetHeight();

			int x = 0;
            int y = 0;
			for ( y = height; y >= 1; y-- ) // top-down
			{
				for ( x = 1; x <= width; x++ )
				{
					int cell = 0;
					cell = gameState.mSTBoardCurrent.GetCell( x, y );
					file.WriteFormattedText( " %d", cell );
				}
				file.WriteFormattedText( "\n" );
			}
			file.WriteFormattedText( "\n" );
			



			
			file.WriteFormattedText( "pieceCurrent\n" );
			file.WriteFormattedText( "shape %d\n", gameState.mSTPieceCurrent.GetKind() );
			file.WriteFormattedText( "x %d\n", gameState.mSTPieceCurrent.GetX() );
			file.WriteFormattedText( "y %d\n", gameState.mSTPieceCurrent.GetY() );
			file.WriteFormattedText( "orientation %d\n", gameState.mSTPieceCurrent.GetOrientation() );
			file.WriteFormattedText( "\n" );





			file.WriteFormattedText( "gameOver %d\n", gameState.mGameOver );
			
			file.WriteFormattedText( "iterationCountdownSeconds %f\n", gameState.mIterationCountdownSeconds );
			file.WriteFormattedText( "currentPiecePointValue %d\n", gameState.mCurrentPiecePointValue   );
			file.WriteFormattedText( "completedRows %d\n", gameState.mCompletedRows             );
			file.WriteFormattedText( "\n" );
			
			
			



            file.WriteFormattedText( "pieceSelectionSource %d\n", (int)gameState.mSTPieceSequence.DirectGetPieceSelectionSource( ) );
            file.WriteFormattedText( "pieceSelectionShapeCurrent %d\n", (int)gameState.mSTPieceSequence.DirectGetCurrentPieceShape( ) );
            file.WriteFormattedText( "pieceSelectionShapeNext %d\n",(int)gameState.mSTPieceSequence.DirectGetNextPieceShape( ) );


			char tempString[256];


			memset( (void *)(tempString), 0, 256 );
			_i64toa( gameState.mSTPieceSequence.DirectGetSeedUsedMostRecentlyToInitializeRandomNumberGenerator( ), tempString, 10 );
            file.WriteFormattedText( "cachedRandomSeedUsedMostRecentlyToInitializeRandomGenerator %s\n", tempString );


			memset( (void *)(tempString), 0, 256 );
			_i64toa( gameState.mSTPieceSequence.DirectGetCurrentRandomNumberGeneratorInternalStateValue( ), tempString, 10 );
			file.WriteFormattedText( "currentInternalRandomNumberGeneratorStateForPieceSequence %s\n", tempString );



            file.WriteFormattedText( "alternatingSZState %d\n", gameState.mSTPieceSequence.DirectGetAlternateSZState( )  );

            file.WriteFormattedText( "pieceSequenceTotalQueueElements %d\n", gameState.mSTPieceSequence.DirectGetTotalQueueElements( ) );

            file.WriteFormattedText( "pieceSequenceQueueElements \n" );

            int totalQueueElements = 0;
            totalQueueElements = gameState.mSTPieceSequence.DirectGetTotalQueueElements( );

            int indexQueueElement = 0;
            for (indexQueueElement = 0; indexQueueElement < totalQueueElements; indexQueueElement++)
            {
                int queueElementValue = 0;
                queueElementValue = gameState.mSTPieceSequence.DirectGetQueueElementByIndex( indexQueueElement );
                file.WriteFormattedText( " %d", queueElementValue );
                if (31 == (indexQueueElement % 32))
                {
					file.WriteFormattedText( "\n" );
                }
            }
			file.WriteFormattedText( "\n" );
			file.WriteFormattedText( "\n" );



			
			// User Options
						
			file.WriteFormattedText( "paused %d\n", gameState.mPaused );

			file.WriteFormattedText( "showNextPiece %d\n", gameState.mShowNextPiece );

			file.WriteFormattedText( "aiActive %d\n", gameState.mAI );

			file.WriteFormattedText( "spawnFromVideoCapture %d\n", gameState.mSpawnFromVideoCapture );

			file.WriteFormattedText( "outputToRS232 %d\n", gameState.mOutputToRS232 );

			file.WriteFormattedText( "autoRestart %d\n", gameState.mAutoRestart );

			file.WriteFormattedText( "autoWriteFile %d\n", gameState.mAutoWriteFile );

			file.WriteFormattedText( "gameSpeedAdjustment %d\n", gameState.mGameSpeedAdjustment );

			file.WriteFormattedText( "shadowMode %d\n", gameState.mShadowMode );

			file.WriteFormattedText( "hintMode %d\n", gameState.mHintMode );

			file.WriteFormattedText( "monochromeColorMode %d\n", STGameDrawing::GetColorMode() );


            // Currently-selected AI
            // (be sure to eliminate the double-quotes around the name when reading back in)
			string currentStrategyName = STStrategyManager::GetCurrentStrategyName();
			file.WriteFormattedText( "currentStrategyName \"%s\"\n", (char *)(currentStrategyName.c_str()) );
			


			
			// Statistics for User Consideration Only
						
			file.WriteFormattedText( "\n" );


			total = sizeof( gameState.mPieceHistogram ) / sizeof( gameState.mPieceHistogram[0] );
			file.WriteFormattedText( "pieceHistogramLength %d\n", total  );
			file.WriteFormattedText( "pieceHistogram\n" );
			for ( index = 0; index < total; index++ )
			{
				file.WriteFormattedText( "%d\n", gameState.mPieceHistogram[ index ] );
			}
			file.WriteFormattedText( "\n" );
			file.WriteFormattedText( "\n" );






            // Height histogram 
            // (find highest index with a non-zero count, and only show indices
            // up to that index on their own lines, with all subsequent zero 
            // values condensed with 32 such values on a single line)

			total = sizeof(gameState.mHeightHistogram) / sizeof(gameState.mHeightHistogram[0]);

			file.WriteFormattedText( "heightHistogramLength %d\n", total );

			file.WriteFormattedText( "heightHistogram\n" );

			int maxY = (-1);
			for ( y = 0; y < total; y++ )
			{
				if (gameState.mHeightHistogram[ y ] > 0)
				{
					maxY = y;
				}
			}

			for ( index = 0; index <= maxY; index++ )
			{
				file.WriteFormattedText( "%I64d\n", gameState.mHeightHistogram[ index ] );
			}

			int k = 0;
			for ( index = (maxY+1); index < total; index++ )
			{
				file.WriteFormattedText( "%I64d ", gameState.mHeightHistogram[ index ] );
				k++;
				if (0 == (k % 32))
				{
					file.WriteFormattedText( "\n" );
				}
			}

			file.WriteFormattedText( "\n" );
			file.WriteFormattedText( "\n" );




			file.WriteFormattedText( "totalElapsedTimeSeconds  %8.2f\n", gameState.mTotalElapsedTimeSeconds );


			file.WriteFormattedText( "score %I64d\n", gameState.mScore );

			file.WriteFormattedText( "historicHighScore %d\n", gameState.mHistoricHighScore );
			file.WriteFormattedText( "historicHighRows %d\n", gameState.mHistoricHighRows );
			file.WriteFormattedText( "historicHighPieces %d\n", gameState.mHistoricHighPieces );
			file.WriteFormattedText( "historicCumulativeRows %I64d\n", gameState.mHistoricCumulativeRows );
			file.WriteFormattedText( "historicTotalGames %d\n", gameState.mHistoricTotalGames );

			file.WriteFormattedText( "\n" );
			



			total = sizeof(gameState.mHistoricRows) / sizeof(gameState.mHistoricRows[0]);
			file.WriteFormattedText( "historicRowsLength %d\n", total );
			file.WriteFormattedText( "historicRows\n" );
			for ( index = 0; index < total; index++ )
			{
				file.WriteFormattedText( "%d\n", gameState.mHistoricRows[ index ] );
			}
			file.WriteFormattedText( "\n" );
			file.WriteFormattedText( "\n" );




			// Copied or derived values
			
			file.WriteFormattedText( "pieceNext\n" );
			file.WriteFormattedText( "shape %d\n", gameState.mSTPieceNext.GetKind() );
			file.WriteFormattedText( "x %d\n", gameState.mSTPieceNext.GetX() );
			file.WriteFormattedText( "y %d\n", gameState.mSTPieceNext.GetY() );
			file.WriteFormattedText( "orientation %d\n", gameState.mSTPieceNext.GetOrientation() );
			file.WriteFormattedText( "\n" );
			

			file.WriteFormattedText( "pieceBestMove\n" );
			file.WriteFormattedText( "shape %d\n", gameState.mSTPieceBestMove.GetKind() );
			file.WriteFormattedText( "x %d\n", gameState.mSTPieceBestMove.GetX() );
			file.WriteFormattedText( "y %d\n", gameState.mSTPieceBestMove.GetY() );
			file.WriteFormattedText( "orientation %d\n", gameState.mSTPieceBestMove.GetOrientation() );
			file.WriteFormattedText( "\n" );



            // state of animation of a an AI-executed move

			file.WriteFormattedText( "animateAIMovesEnable %d\n", gameState.mAnimateAIMovesEnable );
			file.WriteFormattedText( "animateAIMovesStartingY %d\n", gameState.mAnimateAIMovesStartingY );
			file.WriteFormattedText( "animateAIMovesFinalSafeY %d\n", gameState.mAnimateAIMovesFinalSafeY );
			file.WriteFormattedText( "animateAITotalInitialCommands %d\n", gameState.mAnimateAITotalInitialCommands );
			file.WriteFormattedText( "animateAICommandsExecuted %d\n", gameState.mAnimateAICommandsExecuted );
			file.WriteFormattedText( "animateAICommandsPerRow %8.3f\n", gameState.mAnimateAICommandsPerRow );
			file.WriteFormattedText( "animateAIMovesPendingRotation %d\n", gameState.mAnimateAIMovesPendingRotation );
			file.WriteFormattedText( "animateAIMovesPendingTranslation %d\n", gameState.mAnimateAIMovesPendingTranslation );
			file.WriteFormattedText( "\n" );
			



            // Derived values

            // Scaled height histogram

			file.WriteFormattedText( "\n" );
			file.WriteFormattedText( "\n" );
			
			total = sizeof(gameState.mHeightHistogram) / sizeof(gameState.mHeightHistogram[0]);
			LongLong sum = 0;
			for ( y = 0; y < total; y++ )
			{
				sum += gameState.mHeightHistogram[ y ];
			}

			if (sum > 0)
			{
				file.WriteFormattedText( "scaledHistogramOfPileHeights \n" );

				float fraction = 0.0f;

				for ( y = 0; y <= maxY; y++ )
				{
					fraction = (float)(gameState.mHeightHistogram[ y ]) / (float)(sum);
					file.WriteFormattedText( "%f\n", fraction );
				}
				file.WriteFormattedText( "\n" );
			}
			


			file.WriteFormattedText( "\n" );
			file.WriteFormattedText( "\n" );

						


			file.Close();
			return(0);
		}












		int   STGameFile::LoadGameState
			(
			STGameState &   gameState,
			char *  pcstr_FilePathAndName
			)
		{
			STFileReader  file;
			
			int result = 0;

			result = file.ReadAndTokenizeFile( pcstr_FilePathAndName );

			if (0 != result)  
			{
				return(0);
			}


			int totalTokens = 0;
			totalTokens = file.GetTotalTokens();

			int indexToken = 0;

			int offset = 0;

			int intValue = 0;

			__int64 int64Value = (__int64)0;

            char textTemp[256];



			
			
			gameState.mSTBoardCurrent.DestroyBuffer();




			// BOARD
			
			int boardWidth     = 0;
			file.FindTextAndInteger( "boardWidth",  boardWidth  );

			int boardHeight    = 0;
			file.FindTextAndInteger( "boardHeight", boardHeight );


			int x = 0;
			int y = 0;
			if ((boardWidth > 0) && (boardHeight > 0))
			{
				gameState.mSTBoardCurrent.SetBoardDimensions( boardWidth, boardHeight );

				indexToken = file.FindToken( "boardCurrent" );
				if (indexToken >= 0)
				{
					for ( y = boardHeight; y >= 1; y-- ) // top-down
					{
						for ( x = 1; x <= boardWidth; x++ )
						{
                            // string at (index + 1) corresponds to
                            //   ( x = 0, y = boardHeight )
							offset = 
								(1 + ((boardWidth * (boardHeight - y)) + (x - 1)));

							intValue = 
								file.GetTokenAsIntegerByIndex( indexToken + offset );

							gameState.mSTBoardCurrent.SetCell( x, y, intValue );
						}
					}
				}
			}

			



            int shape = 0;
            int orientation = 0;

			// CURRENT PIECE
			
			gameState.mSTPieceCurrent.Clear();

			indexToken = file.FindToken( "pieceCurrent" );

			if (indexToken >= 0)
			{
				file.FindTextAndIntegerAtOrAfterIndex
					( indexToken, "shape", shape );

				file.FindTextAndIntegerAtOrAfterIndex
					( indexToken, "x", x );

				file.FindTextAndIntegerAtOrAfterIndex
					( indexToken, "y", y );

				file.FindTextAndIntegerAtOrAfterIndex
					( indexToken, "orientation", orientation );

				gameState.mSTPieceCurrent.SetKind( shape );
				gameState.mSTPieceCurrent.SetX( x );
				gameState.mSTPieceCurrent.SetY( y );
				gameState.mSTPieceCurrent.SetOrientation( orientation );
			}
			



            // BASIC GAME STATE
			
			file.FindTextAndInteger( "gameOver",  gameState.mGameOver  );

			gameState.mIterationCountdownSeconds = (0.05f); // Disregard file

			file.FindTextAndInteger( "currentPiecePointValue", gameState.mCurrentPiecePointValue  );

			file.FindTextAndInteger( "completedRows",  gameState.mCompletedRows  );
			



            // PIECE SELECTION STATE


            int pieceSelectionSource = 0;
            file.FindTextAndInteger( "pieceSelectionSource", pieceSelectionSource );
            gameState.mSTPieceSequence.DirectSetPieceSelectionSource( pieceSelectionSource );

            int pieceSelectionShapeCurrent = 0;
            file.FindTextAndInteger( "pieceSelectionShapeCurrent", pieceSelectionShapeCurrent );
            gameState.mSTPieceSequence.DirectSetCurrentPieceShape( pieceSelectionShapeCurrent );

            int pieceSelectionShapeNext = 0;
            file.FindTextAndInteger( "pieceSelectionShapeNext", pieceSelectionShapeNext );
            gameState.mSTPieceSequence.DirectSetNextPieceShape( pieceSelectionShapeNext );




            __int64 cachedRandomSeedUsedMostRecentlyToInitializeRandomGenerator = (__int64)0;
            indexToken = file.FindToken( "cachedRandomSeedUsedMostRecentlyToInitializeRandomGenerator" );
			if (indexToken >= 0)
			{
					memset( (void *)textTemp, 0, sizeof(textTemp) );
                    file.GetTokenByIndex( (indexToken + 1), textTemp );
					int64Value = _atoi64( textTemp );
					cachedRandomSeedUsedMostRecentlyToInitializeRandomGenerator = int64Value;
			}
            gameState.mSTPieceSequence.DirectSetSeedUsedMostRecentlyToInitializeRandomNumberGenerator( cachedRandomSeedUsedMostRecentlyToInitializeRandomGenerator );





            __int64 currentInternalRandomNumberGeneratorStateForPieceSequence = (__int64)0;
            indexToken = file.FindToken( "currentInternalRandomNumberGeneratorStateForPieceSequence" );
			if (indexToken >= 0)
			{
					memset( (void *)textTemp, 0, sizeof(textTemp) );
                    file.GetTokenByIndex( (indexToken + 1), textTemp );
					int64Value = _atoi64( textTemp );
					currentInternalRandomNumberGeneratorStateForPieceSequence = int64Value;
			}
            gameState.mSTPieceSequence.DirectSetCurrentRandomNumberGeneratorInternalStateValue( currentInternalRandomNumberGeneratorStateForPieceSequence );





            int alternatingSZState = 0;
            file.FindTextAndInteger( "alternatingSZState", alternatingSZState );
            gameState.mSTPieceSequence.DirectSetAlternateSZState( alternatingSZState );

            gameState.mSTPieceSequence.DirectQueueClear( );

            int pieceSequenceTotalQueueElements = 0;
            file.FindTextAndInteger( "pieceSequenceTotalQueueElements", pieceSequenceTotalQueueElements );

            indexToken = file.FindToken( "pieceSequenceQueueElements" );
            if ((pieceSequenceTotalQueueElements > 0) && (indexToken >= 0))
            {
                for (offset = 0; offset < pieceSequenceTotalQueueElements; offset++)
                {
                    intValue = file.GetTokenAsIntegerByIndex( (indexToken + 1) + offset );
                    gameState.mSTPieceSequence.DirectAddQueueElement( intValue );
                }
            }




            // USER OPTIONS

            
            // DO NOT RESTORE PAUSED STATE
            gameState.mPaused = 1; // FORCE TO BE PAUSED UPON LOAD

			file.FindTextAndInteger( "showNextPiece",    gameState.mShowNextPiece  );

			file.FindTextAndInteger( "aiActive",               gameState.mAI  );

			file.FindTextAndInteger( "spawnFromVideoCapture",  gameState.mSpawnFromVideoCapture );

			file.FindTextAndInteger( "outputToRS232",  gameState.mOutputToRS232 );

			file.FindTextAndInteger( "autoRestart",  gameState.mAutoRestart );

			file.FindTextAndInteger( "autoWriteFile",  gameState.mAutoWriteFile );

			file.FindTextAndInteger( "gameSpeedAdjustment",  gameState.mGameSpeedAdjustment  );

			file.FindTextAndInteger( "shadowMode",           gameState.mShadowMode  );

			file.FindTextAndInteger( "hintMode",         gameState.mHintMode  );

			file.FindTextAndInteger( "monochromeColorMode",  gameState.mMonochromeColorMode  );





            // Currently-selected AI
            // (be sure to eliminate the double-quotes around the name when reading back in)
			string currentStrategyName = "";

			int totalStrings = file.GetTotalTokens();
			int indexString = file.FindToken( "currentStrategyName" );
            if (indexString >= 0)
            {
                int doubleQuoteCount = 0;
				int offset = 0;
                for 
                    (
                    offset = 1; 
                    ((offset < 64) && (doubleQuoteCount < 2) && ((indexString + offset) < totalStrings)); 
                    offset++
                    )
                {
					memset( (void *)textTemp, 0, sizeof(textTemp) );
                    file.GetTokenByIndex( (indexString + offset), textTemp );
                    if (offset > 1)
                    {
                        currentStrategyName += " ";
                    }
					string stringTemp = textTemp;
                    if ((string::npos) != stringTemp.find( (string)"\"" ))
                    {
                        doubleQuoteCount++;
						stringTemp.erase( stringTemp.find( (string)"\"" ), 1 );
                    }
                    currentStrategyName += stringTemp;
                }
            }

			STStrategyManager::SetCurrentStrategyByName( currentStrategyName );
			







            // PIECE HISTOGRAM

			int total = 0;
			total = sizeof(gameState.mPieceHistogram) / sizeof(gameState.mPieceHistogram[0]);

            for ( offset = 0; offset < total; offset++ )
            {
                gameState.mPieceHistogram[offset] = 0;
            }

            int pieceHistogramLength = 0;
            file.FindTextAndInteger( "pieceHistogramLength", pieceHistogramLength );

            if (pieceHistogramLength >= total)
            {
                indexToken = file.FindToken( "pieceHistogram" );
                if (indexToken >= 0)
                {
                    for (offset = 0; offset < total; offset++)
                    {
                        intValue = file.GetTokenAsIntegerByIndex( (indexToken + 1) + offset );

                        gameState.mPieceHistogram[offset] = intValue;
                    }
                }
            }




			



            // HEIGHT HISTOGRAM

			total = sizeof(gameState.mHeightHistogram) / sizeof(gameState.mHeightHistogram[0]);

            for (offset = 0; offset < total; offset++)
            {
                gameState.mHeightHistogram[offset] = 0;
            }

            int heightHistogramLength = 0;
            file.FindTextAndInteger( "heightHistogramLength", heightHistogramLength );

            if (heightHistogramLength >= total)
            {
                indexToken = file.FindToken( "heightHistogram" );
                if (indexToken >= 0)
                {
                    for (offset = 0; offset < total; offset++)
                    {
                        intValue = file.GetTokenAsIntegerByIndex( (indexToken + 1) + offset );

                        gameState.mHeightHistogram[offset] = intValue;
                    }
                }
            }





            // OTHER STATISTICS
			
			file.FindTextAndFloat( "totalElapsedTimeSeconds", gameState.mTotalElapsedTimeSeconds  );

			//file.FindTextAndInteger( "score",                   gameState.mScore );

			//file.FindTextAndInteger( "historicHighScore",       gameState.mHistoricHighScore  );

			file.FindTextAndInteger( "historicHighRows",        gameState.mHistoricHighRows  );

			file.FindTextAndInteger( "historicHighPieces",      gameState.mHistoricHighPieces  );

			//file.FindTextAndInteger( "historicCumulativeRows",  gameState.mHistoricCumulativeRows  );

			file.FindTextAndInteger( "historicTotalGames",      gameState.mHistoricTotalGames  );  
			


			



            // HISTORIC ROWS LIST

			total = sizeof(gameState.mHistoricRows) / sizeof(gameState.mHistoricRows[0]);

			for (offset = 0; offset < total; offset++)
            {
                gameState.mHistoricRows[offset] = 0;
            }

            int historicRowsLength = 0;
            file.FindTextAndInteger( "historicRowsLength", historicRowsLength );

            if (historicRowsLength >= total)
            {
                indexToken = file.FindToken( "historicRows" );
                if (indexToken >= 0)
                {
                    for (offset = 0; offset < total; offset++)
                    {
                        intValue = file.GetTokenAsIntegerByIndex( (indexToken + 1) + offset );

                        gameState.mHistoricRows[offset] = intValue;
                    }
                }
            }




			
			// NEXT PIECE
			
			gameState.mSTPieceNext.Clear();

			indexToken = file.FindToken( "pieceNext" );

			if (indexToken >= 0)
			{
				file.FindTextAndIntegerAtOrAfterIndex
					( indexToken, "shape", shape );

				file.FindTextAndIntegerAtOrAfterIndex
					( indexToken, "x", x );

				file.FindTextAndIntegerAtOrAfterIndex
					( indexToken, "y", y );

				file.FindTextAndIntegerAtOrAfterIndex
					( indexToken, "orientation", orientation );

				gameState.mSTPieceNext.SetKind( shape );
				gameState.mSTPieceNext.SetX( x );
				gameState.mSTPieceNext.SetY( y );
				gameState.mSTPieceNext.SetOrientation( orientation );
			}
			




			// BEST-MOVE PIECE

			gameState.mSTPieceBestMove.Clear();

			indexToken = file.FindToken( "pieceBestMove" );

			if (indexToken >= 0)
			{
				file.FindTextAndIntegerAtOrAfterIndex
					( indexToken, "shape", shape );

				file.FindTextAndIntegerAtOrAfterIndex
					( indexToken, "x", x );

				file.FindTextAndIntegerAtOrAfterIndex
					( indexToken, "y", y );

				file.FindTextAndIntegerAtOrAfterIndex
					( indexToken, "orientation", orientation );

				gameState.mSTPieceBestMove.SetKind( shape );
				gameState.mSTPieceBestMove.SetX( x );
				gameState.mSTPieceBestMove.SetY( y );
				gameState.mSTPieceBestMove.SetOrientation( orientation );
			}

			




			// move animation state
			
			file.FindTextAndInteger( "animateAIMovesEnable",             gameState.mAnimateAIMovesEnable );

			file.FindTextAndInteger( "animateAIMovesStartingY",          gameState.mAnimateAIMovesStartingY );

			file.FindTextAndInteger( "animateAIMovesFinalSafeY",         gameState.mAnimateAIMovesFinalSafeY );

			file.FindTextAndInteger( "animateAITotalInitialCommands",    gameState.mAnimateAITotalInitialCommands );

			file.FindTextAndInteger( "animateAICommandsExecuted",        gameState.mAnimateAICommandsExecuted );

			float floatValue = 0.0f;
			file.FindTextAndFloat  ( "animateAICommandsPerRow",          floatValue );
			gameState.mAnimateAICommandsPerRow = (double) floatValue;

			file.FindTextAndInteger( "animateAIMovesPendingRotation",    gameState.mAnimateAIMovesPendingRotation );

			file.FindTextAndInteger( "animateAIMovesPendingTranslation", gameState.mAnimateAIMovesPendingTranslation );
			


			return( 1 );
		}






	}
}

