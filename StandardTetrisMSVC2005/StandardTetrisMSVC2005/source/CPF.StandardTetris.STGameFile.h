// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STGameFileHeader
#define STGameFileHeader



#include "CPF.StandardTetris.STBoard.h"
#include "CPF.StandardTetris.STPiece.h"
#include "CPF.StandardTetris.STPieceSequence.h"
#include "CPF.StandardTetris.STGameState.h"



namespace CPF
{
	namespace StandardTetris
	{



		class  STGameFile
		{

		public:  static int  SaveGameState( STGameState & ref_State );

		public:  static int  LoadGameState
					 (
					 STGameState &  ref_State,
					 char *  pcstr_FilePathAndName
					 );
		};



	}
}



#endif






