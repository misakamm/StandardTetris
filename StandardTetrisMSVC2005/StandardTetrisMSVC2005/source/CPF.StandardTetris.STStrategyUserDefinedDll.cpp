// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#include "CPF.StandardTetris.STStrategyUserDefinedDll.h"



#include "CPF.StandardTetris.STBoard.h"
#include "CPF.StandardTetris.STPiece.h"



#include <stdlib.h> // rand()



// Disable unimportant "symbol too long" error for debug builds of STL string
#pragma warning( disable : 4786 )

#include <string>
#include <vector>
using namespace std;

#include "windows.h"
#include <shellapi.h>

namespace CPF
{
	namespace StandardTetris
	{

		STStrategyUserDefinedDll::STStrategyUserDefinedDll()
		{
			hInst = NULL;
		}

		bool STStrategyUserDefinedDll::LoadDll(std::string path)
		{
			if ( IsLoad() )
			{
				FreeLibrary((HMODULE)hInst);
				hInst = NULL;
			}

			hInst = LoadLibraryA(path.c_str());
			if ( IsLoad() )
			{
				mNameFunc = (NameFunc)GetProcAddress((HMODULE)hInst, "_Name@0");
				if ( mNameFunc == NULL )
				{
					mNameFunc = (NameFunc)GetProcAddress((HMODULE)hInst, "Name@0");
				}
				if ( mNameFunc == NULL )
				{
					mNameFunc = (NameFunc)GetProcAddress((HMODULE)hInst, "Name");
				}
				if ( mNameFunc == NULL )
				{
					mNameFunc = (NameFunc)GetProcAddress((HMODULE)hInst, "_AIName@0");
				}
				if ( mNameFunc == NULL )
				{
					mNameFunc = (NameFunc)GetProcAddress((HMODULE)hInst, "AIName@0");
				}
				if ( mNameFunc == NULL )
				{
					mNameFunc = (NameFunc)GetProcAddress((HMODULE)hInst, "AIName");
				}

				mStrategyFunc = (StrategyFunc)GetProcAddress((HMODULE)hInst, "_AI@40");
				if ( mStrategyFunc == NULL )
				{
					mStrategyFunc = (StrategyFunc)GetProcAddress((HMODULE)hInst, "AI@40");
				}
				if ( mStrategyFunc == NULL )
				{
					mStrategyFunc = (StrategyFunc)GetProcAddress((HMODULE)hInst, "AI");
				}
				if ( mStrategyFunc == NULL )
				{
					mStrategyFunc = (StrategyFunc)GetProcAddress((HMODULE)hInst, "_TetrisAI@40");
				}
				if ( mStrategyFunc == NULL )
				{
					mStrategyFunc = (StrategyFunc)GetProcAddress((HMODULE)hInst, "TetrisAI@40");
				}
				if ( mStrategyFunc == NULL )
				{
					mStrategyFunc = (StrategyFunc)GetProcAddress((HMODULE)hInst, "TetrisAI");
				}

				mStrategyPathFunc = (StrategyPathFunc)GetProcAddress((HMODULE)hInst, "_AIPath@36");
				if ( mStrategyPathFunc == NULL )
				{
					mStrategyPathFunc = (StrategyPathFunc)GetProcAddress((HMODULE)hInst, "AIPath@36");
				}
				if ( mStrategyPathFunc == NULL )
				{
					mStrategyPathFunc = (StrategyPathFunc)GetProcAddress((HMODULE)hInst, "AIPath");
				}
				if ( mStrategyPathFunc == NULL )
				{
					mStrategyPathFunc = (StrategyPathFunc)GetProcAddress((HMODULE)hInst, "_TetrisAIPath@36");
				}
				if ( mStrategyPathFunc == NULL )
				{
					mStrategyPathFunc = (StrategyPathFunc)GetProcAddress((HMODULE)hInst, "TetrisAIPath@36");
				}
				if ( mStrategyPathFunc == NULL )
				{
					mStrategyPathFunc = (StrategyPathFunc)GetProcAddress((HMODULE)hInst, "TetrisAIPath");
				}
				return true;
			}
			return false;
		}
		bool STStrategyUserDefinedDll::IsLoad()
		{
			if ( hInst ) return true;
			return false;
		}

		string STStrategyUserDefinedDll::GetStrategyName ( )
		{
			if ( /*::GetAsyncKeyState( 'R' ) ||*/ ! IsLoad() )
			{
				int nNumargs;
				LPWSTR *ppArgv = CommandLineToArgvW(GetCommandLineW(), &nNumargs);
				if (nNumargs > 1)
				{
					char buf[255] = "";
					BOOL conv;
					WideCharToMultiByte(CP_ACP, 0, ppArgv[1], lstrlen(ppArgv[1]), buf, 255, 0, &conv);
					LoadDll(buf);
				}
				if ( ! IsLoad() )
				{
					LoadDll("ai.dll");
				}

				LocalFree(ppArgv);
				//HeapFree(GetProcessHeap(), 0, ppArgv);
			}
			if ( IsLoad() )
			{
				if ( mNameFunc )
				{
					return mNameFunc();
				}
			}
			return "No dll loaded!";
		}




		// WARNING: Moves requiring rotation must wait until piece has fallen by
		// at least one row!
		// Perform all rotations, and then perform translations.  This
		// avoids the problem of getting the piece jammed on the sides
		// of the board where rotation is impossible. ***
		// Also, the following strategy does not take advantage of the
		// possibility of using free-fall and future movements to
		// slide under overhangs and fill them in.


		void STStrategyUserDefinedDll::GetBestMoveOncePerPiece
			(
			STBoard & board,
			STPiece & piece,
			int nextPieceFlag, // 0 == no next piece available or known
			int nextPieceShape, // 0 == no piece available or known
			int & bestRotationDelta, // 0 or {0,1,2,3}
			int & bestTranslationDelta // 0 or {...,-2,-1,0,1,2,...}
			)
		{
			bestRotationDelta    = 0;
			bestTranslationDelta = 0;


			// We are given the current board, and the current piece
			// configuration.  Our goal is to evaluate various possible
			// moves and return the best move we explored.


			STStrategyUserDefinedDll::PrivateStrategy
				(
				0, // not called from parent ply; Just this one ply.
				board,
				piece,
				nextPieceFlag,
				nextPieceShape,
				bestRotationDelta,     // 0 or {0,1,2,3}
				bestTranslationDelta   // 0 or {...,-2,-1,0,1,2,...}
				);

		}





		int STStrategyUserDefinedDll::PrivateStrategy
			( 
			int  flagCalledFromParentPly,  // True if called from a parent level
			STBoard &  board,
			STPiece &  piece,
			int nextPieceFlag, // 0 == no next piece available or known
			int nextPieceShape, // 0 == no piece available or known
			int &  bestRotationDelta,    // 0 or {0,1,2,3} 
			int &  bestTranslationDelta  // 0 or {...,-2,-1,0,1,2,...}
			)
		{
			// If board or piece is invalid, return.
			if (0 == board.IsValid()) return 0;
			if (0 == piece.IsValid()) return 0;
			if ( ! IsLoad() ) return 0;

			int width  = 0;
			int height = 0;
			width  = board.GetWidth();
			height = board.GetHeight();

			char* Board = (char*)malloc(width*height+1);
			char pieceMap[] = " OISZLJT";
			char curPiece = pieceMap[piece.GetKind()];
			int curX = piece.GetX();
			int curY = piece.GetY();
			int curR = piece.GetOrientation();
			char nextPiece = nextPieceFlag ? pieceMap[nextPieceShape] : ' ';
			int bestX = curX;
			int bestRotation = curR;

			for ( int y = 0; y < height; ++y )
			{
				for ( int x = 0; x < width; ++x )
				{
					if ( board.GetCell( x + 1, y + 1 ) )
						Board[ y * width + x ] = '1';
					else
						Board[ y * width + x ] = '0';
				}
			}
			Board[width*height] = 0;

			if ( mStrategyFunc )
			{
				mStrategyFunc(width, height,
					Board,
					curPiece,
					curX, curY, curR,
					nextPiece,
					&bestX, &bestRotation
					);
			}
			free(Board);

			bestTranslationDelta = bestX - piece.GetX();
			bestRotationDelta = (bestRotation - (piece.GetOrientation()) + 4) % 4;
			return 0;

		}
		void STStrategyUserDefinedDll::GetBestMovePath
			(
			STBoard & board,
			STPiece & piece,
			int nextPieceFlag, // false == no next piece available or known
			int nextPieceShape, // None == no piece available or known
			char path[]
		)
		{
			if ( mStrategyPathFunc )
			{
				// If board or piece is invalid, return.
				if (0 == board.IsValid()) return;
				if (0 == piece.IsValid()) return;
				if ( ! IsLoad() ) return;

				int width  = 0;
				int height = 0;
				width  = board.GetWidth();
				height = board.GetHeight();

				char* Board = (char*)malloc(width*height+1);
				char pieceMap[] = " OISZLJT";
				char curPiece = pieceMap[piece.GetKind()];
				int curX = piece.GetX();
				int curY = piece.GetY();
				int curR = piece.GetOrientation();
				char nextPiece = nextPieceFlag ? pieceMap[nextPieceShape] : ' ';

				for ( int y = 0; y < height; ++y )
				{
					for ( int x = 0; x < width; ++x )
					{
						if ( board.GetCell( x + 1, y + 1 ) )
							Board[ y * width + x ] = '1';
						else
							Board[ y * width + x ] = '0';
					}
				}
				Board[width*height] = 0;

				mStrategyPathFunc(width, height,
					Board,
					curPiece,
					curX, curY, curR,
					nextPiece,
					path
					);
				free(Board);
			}
			else
			{
				int bestRotationDelta = 0;
				int bestTranslationDelta = 0;
				GetBestMoveOncePerPiece(
					board,
					piece,
					nextPieceFlag,
					nextPieceShape,
					bestRotationDelta,
					bestTranslationDelta
					);
				char* p = path;
				for ( ; bestRotationDelta > 0; --bestRotationDelta )
				{
					*p++ = 'z';
				}
				for ( ; bestTranslationDelta < 0; ++bestTranslationDelta )
				{
					*p++ = 'l';
				}
				for ( ; bestTranslationDelta > 0; --bestTranslationDelta )
				{
					*p++ = 'r';
				}
				p[0] = 0;
			}
		}

	}
}



