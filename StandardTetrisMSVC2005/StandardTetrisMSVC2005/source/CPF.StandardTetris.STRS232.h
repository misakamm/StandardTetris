// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STRS232Header
#define STRS232Header


#include <windows.h>



namespace CPF
{
	namespace StandardTetris
	{


		class STRS232
		{

		private:  enum { KEY_WAIT_MILLISECONDS  = 30 };
		private:  enum { KEY_PRESS_MILLISECONDS = 30 };

		private:  static HANDLE  m_uhCom;


		public:  static int   InitializePort( );
		public:  static int   TerminatePort( );
		public:  static void  SetRelay( int relay, int on );
		public:  static void  SetRelayBits( int bits );
		public:  static void  Test( );


				 // Tetris controls
		public:  static void  MomentaryRelay_LEFT   ( ); // Relay 0
		public:  static void  MomentaryRelay_RIGHT  ( ); // Relay 1
		public:  static void  MomentaryRelay_ROTATE ( ); // Relay 2
		public:  static void  MomentaryRelay_DROP   ( ); // Relay 3
		public:  static void  MomentaryRelay_RESET  ( ); // Relay 4


		private:  static int FakeStrlen( char * p_i8_Buffer );

		};



	}
}




#endif






