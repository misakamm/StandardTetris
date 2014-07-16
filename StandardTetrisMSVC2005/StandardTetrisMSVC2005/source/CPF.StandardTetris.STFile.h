// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STFileHeader
#define STFileHeader



#include <stdio.h>  // FILE, fopen(), fprintf(), fclose()

// Disable unimportant "symbol too long" error for debug builds of STL string
#pragma warning( disable : 4786 )

#include <string>
#include <vector>
using namespace std;




namespace CPF
{
	namespace StandardTetris
	{


		class STFileWriter
		{
		private:  FILE *   m_p_FILE;


		public:  STFileWriter( );
		public:  ~STFileWriter( );
		public:  int   Open( char * lpcstrFilePathAndName );
		public:  void  Close( );
		public:  void  WriteText( char * lpcstrText );
		public:  void  WriteFormattedText( const char *format, ... );
		};



		class STFileReader
		{
		private:  vector< string >  mVectorString;


		public:  STFileReader();
		public:  ~STFileReader();

		public:  void         Clear( );
		public:  int          ReadAndTokenizeFile( char * lpcstrFilePathAndName );
		public:  int          GetTotalTokens( );
		public:  int          GetTokenByIndex( int index, char * lpcstrText );

		public:  int          GetTokenAsIntegerByIndex( int index );
		public:  unsigned int GetTokenAsUnsignedIntegerByIndex( int index );
		public:  float        GetTokenAsFloatByIndex( int index );

		public:  int          FindTokenAtOrAfterIndex( int startIndex, char * lpcstrText );
		public:  int          FindToken( char * lpcstrText ); // (-1)==Not found, otherwise index

		public:  void  FindTextAndIntegerAtOrAfterIndex( int startIndex, char *pcstrText, int & ref_value );
		public:  void  FindTextAndUnsignedIntegerAtOrAfterIndex( int startIndex, char *pcstrText, unsigned int & ref_uvalue );
		public:  void  FindTextAndFloatAtOrAfterIndex
					 ( 
					 int startIndex,
					 char *pcstrText,
					 float & ref_value
					 );

		public:  void  FindTextAndInteger        ( char *pcstrText, int          & ref_value  );
		public:  void  FindTextAndUnsignedInteger( char *pcstrText, unsigned int & ref_uvalue );
		public:  void  FindTextAndFloat          ( char *pcstrText, float        & ref_value  );
		};




	}
}


#endif






