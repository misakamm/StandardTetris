// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#include "CPF.StandardTetris.STRS232.h"



#include <windows.h>



namespace CPF
{
	namespace StandardTetris
	{


		HANDLE  STRS232::m_uhCom = ((HANDLE)(0));



		int   STRS232::InitializePort(  )
		{
			DCB    dcb;
			int    success;
			char   *pcstr_CommPort = "COM1";

			STRS232::m_uhCom = CreateFileA
				( 
				pcstr_CommPort,
				(GENERIC_READ | GENERIC_WRITE),
				0,    // comm devices must be opened w/exclusive-access
				NULL, // no security attributes
				OPEN_EXISTING, // comm devices must use OPEN_EXISTING
				0,    // not overlapped I/O
				NULL  // hTemplate must be NULL for comm devices
				);


			if (STRS232::m_uhCom == INVALID_HANDLE_VALUE) 
			{
				STRS232::m_uhCom = 0;
				MessageBoxA
					(
					NULL, 
					"CreateFile() failed to create COM1", 
					"ERROR: STRS232::InitializePort()",
					MB_OK
					);
				return (1);
			}


			//  BOOL SetupComm(
			//  HANDLE hFile,     // handle to communications device
			//  DWORD dwInQueue,  // size of input buffer
			//  DWORD dwOutQueue  // size of output buffer
			//);

			SetupComm( STRS232::m_uhCom, 1024, 1024 );


			// We will build on the current configuration, and skip setting the size
			// of the input and output buffers with SetupComm.

			success = GetCommState ( STRS232::m_uhCom, & dcb );

			if (!(success)) 
			{
				MessageBoxA
					(
					NULL, 
					"GetCommState() failed", 
					"ERROR: STRS232::InitializePort()",
					MB_OK
					);
				return (2);
			}

			// Fill in the DCB: baud=9,600 bps, 8 data bits, no parity, and 1 stop bit.

			dcb.BaudRate = CBR_9600;     // set the baud rate
			dcb.ByteSize = 8;            // data size, xmit, and rcv
			dcb.Parity   = NOPARITY;     // no parity bit
			dcb.StopBits = ONESTOPBIT;   // one stop bit

			success = SetCommState	(	STRS232::m_uhCom, & dcb	);


			if (!(success)) 
			{
				MessageBoxA
					(
					NULL, 
					"SetCommState() failed", 
					"ERROR: STRS232::InitializePort()",
					MB_OK
					);
				return (3);
			}


			//typedef struct _COMMTIMEOUTS {  
			//  DWORD ReadIntervalTimeout; 
			//  DWORD ReadTotalTimeoutMultiplier; 
			//  DWORD ReadTotalTimeoutConstant; 
			//  DWORD WriteTotalTimeoutMultiplier; 
			//  DWORD WriteTotalTimeoutConstant; 
			//} COMMTIMEOUTS,*LPCOMMTIMEOUTS; 

			COMMTIMEOUTS timeouts;
			timeouts.ReadIntervalTimeout         = 100;
			timeouts.ReadTotalTimeoutMultiplier  = 1;
			timeouts.ReadTotalTimeoutConstant    = 100;
			timeouts.WriteTotalTimeoutMultiplier = 1;
			timeouts.WriteTotalTimeoutConstant   = 100;

			//BOOL SetCommTimeouts(
			//  HANDLE hFile,                  // handle to comm device
			//  LPCOMMTIMEOUTS lpCommTimeouts  // time-out values
			//);

			success = SetCommTimeouts ( 	STRS232::m_uhCom, 	& timeouts 	);


			if (!(success)) 
			{
				MessageBoxA
					(
					NULL, 
					"SetCommTimeouts() failed", 
					"ERROR: STRS232::InitializePort()",
					MB_OK
					);
				return (4);
			}

			// Serial port successfully reconfigured.

			return(0);
		}









		int   STRS232::TerminatePort( )
		{
			if ( (INVALID_HANDLE_VALUE == STRS232::m_uhCom) ||
				(0                    == STRS232::m_uhCom) )
			{
				STRS232::m_uhCom = ((HANDLE)(0));
				return(0);
			}

			int success = CloseHandle( STRS232::m_uhCom );
			STRS232::m_uhCom = ((HANDLE)(0));

			if (!(success)) 
			{
				MessageBoxA
					(
					NULL, 
					"CloseHandle() failed", 
					"ERROR: STRS232::TerminatePort()",
					MB_OK
					);
				return (1);
			}

			return(0);
		}









		int STRS232::FakeStrlen( char * p_i8_Buffer )
		{
			if (((char *)(0)) == p_i8_Buffer)
				return( 0 );
			int i = 0;
			while( ((char)(0)) != p_i8_Buffer[i] ) i++;
			return( i );
		}







		void  STRS232::SetRelay( int relay, int on )
		{
			if ( (INVALID_HANDLE_VALUE == STRS232::m_uhCom) ||
				(0                    == STRS232::m_uhCom) )
			{
				return;
			}

			char message[32];

			if (0 != on)
			{
				message[0] = (byte)'s';
				message[1] = (byte)'k';
				message[2] = (byte)((int)'0' + relay);
				message[3] = (byte)'\r';
				message[4] = (byte)'\0';
			}
			else
			{
				message[0] = (byte)'r';
				message[1] = (byte)'k';
				message[2] = (byte)((int)'0' + relay);
				message[3] = (byte)'\r';
				message[4] = (byte)'\0';
			}

			unsigned long int   ubytesWritten = 0;
			int   success = 0;
			success = 
				WriteFile
				(
				STRS232::m_uhCom,
				message,
				4,
				(&(ubytesWritten)),
				0 
				);

			if (!(success))
			{
				MessageBoxA
					(
					NULL, 
					"WriteFile() failed", 
					"ERROR: STRS232::SetRelay()",
					MB_OK
					);
				return;
			}

			FlushFileBuffers( STRS232::m_uhCom );
		}








		void  STRS232::SetRelayBits( int bits )
		{
			if ( (INVALID_HANDLE_VALUE == STRS232::m_uhCom) ||
				(0                    == STRS232::m_uhCom) )
			{
				return;
			}

			char message[32];

			//sprintf( message, "MK%03d\r", bits );
			message[0] = (byte)'M';
			message[1] = (byte)'K';
			message[2] = (byte)((int)'0' + ((bits / 100) % 10));
			message[3] = (byte)((int)'0' + ((bits / 10) % 10));
			message[4] = (byte)((int)'0' + ((bits / 1) % 10));
			message[5] = (byte)'\r'; // CR
			message[6] = (byte)0;

			unsigned long int   ubytesWritten = 0;
			int   success = 0;


			success = 
				WriteFile
				(
				STRS232::m_uhCom,
				message,
				FakeStrlen( message ),
				(&(ubytesWritten)),
				0 
				);

			if (!(success))
			{
				MessageBoxA
					(
					NULL, 
					"WriteFile() failed", 
					"ERROR: STRS232::SetRelayBits()",
					MB_OK
					);
				return;
			}

			FlushFileBuffers( STRS232::m_uhCom );
		}








		void  STRS232::Test(  )
		{
			int i = 0;
			int n = 0;
			n = 256;
			for ( i = 0; i < n; i++ )
			{
				STRS232::SetRelayBits( (int)(i&0xff) );
				//SetRelay( (i%8), (int)((i/13)&0x1) );
				Sleep( 1 );
			}
			STRS232::SetRelayBits( 0 );
		}







		void  STRS232::MomentaryRelay_LEFT   (  ) // Relay 0
		{
			Sleep( STRS232::KEY_WAIT_MILLISECONDS );
			STRS232::SetRelayBits( 0x01 );
			Sleep( STRS232::KEY_PRESS_MILLISECONDS );
			STRS232::SetRelayBits( 0 );
		}

		void  STRS232::MomentaryRelay_RIGHT  (  ) // Relay 1
		{
			Sleep( STRS232::KEY_WAIT_MILLISECONDS );
			STRS232::SetRelayBits( 0x02 );
			Sleep( STRS232::KEY_PRESS_MILLISECONDS );
			STRS232::SetRelayBits( 0 );
		}

		void  STRS232::MomentaryRelay_ROTATE (  ) // Relay 2
		{
			Sleep( STRS232::KEY_WAIT_MILLISECONDS );
			STRS232::SetRelayBits( 0x04 );
			Sleep( STRS232::KEY_PRESS_MILLISECONDS );
			STRS232::SetRelayBits( 0 );
		}

		void  STRS232::MomentaryRelay_DROP   (  ) // Relay 3
		{
			Sleep( STRS232::KEY_WAIT_MILLISECONDS );
			STRS232::SetRelayBits( 0x08 );
			Sleep( STRS232::KEY_PRESS_MILLISECONDS );
			STRS232::SetRelayBits( 0 );
		}

		void  STRS232::MomentaryRelay_RESET  (  ) // Relay 4
		{
			Sleep( STRS232::KEY_WAIT_MILLISECONDS );
			STRS232::SetRelayBits( 0x10 );
			Sleep( STRS232::KEY_PRESS_MILLISECONDS );
			STRS232::SetRelayBits( 0 );
		}




	}
}


