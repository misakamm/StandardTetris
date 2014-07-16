// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1



#include "CPF.StandardTetris.STFile.h"



#include <windows.h>  // MessageBox()
#include <stdio.h>    // FILE, fopen(), fprintf(), fclose()
#include <math.h>     // atof() support
#include <stdlib.h>   // atof() support; stroul() : Converts string to ui32

// Disable unimportant "symbol too long" error for debug builds of STL string
#pragma warning( disable : 4786 )

#include <string>
#include <vector>
using namespace std;



namespace CPF
{
	namespace StandardTetris
	{



		STFileWriter::STFileWriter( )
		{
			m_p_FILE = ((FILE *)(0));
		}




		STFileWriter::~STFileWriter( )
		{
			(*this).Close();
		}




		int   STFileWriter::Open( char * lpcstrFilePathAndName )
		{
			// Close any currently open file.
			(*this).Close();

			// If we don't have a filename, fail.
			if (((char *)(0)) == lpcstrFilePathAndName)
			{
				char lpcstrTextMessage[ 2048 ];
				sprintf( lpcstrTextMessage, "ERROR: Filename is NULL." );                     
				MessageBoxA( NULL, lpcstrTextMessage, 
					"STFileWriter::Open()", MB_OK );
				return(-1);
			}

			m_p_FILE = fopen( lpcstrFilePathAndName, "w" );

			if (((FILE *)(0)) == m_p_FILE)
			{
				char lpcstrTextMessage[ 2048 ];
				sprintf( lpcstrTextMessage, "ERROR: Failed to open '%s' for writing.",
					lpcstrFilePathAndName );
				MessageBoxA( NULL, lpcstrTextMessage, 
					"STFileWriter::Open()", MB_OK );
				return(-1);    
			}

			return(0);
		}






		void  STFileWriter::Close( )
		{
			if (((FILE *)(0)) != m_p_FILE)
			{
				fclose( m_p_FILE );
			}
			m_p_FILE = ((FILE *)(0));
		}






		void  STFileWriter::WriteText( char * lpcstrText )
		{
			if (((char *)(0)) == lpcstrText)
			{
				// String is NULL.
				return;
			}
			if (((char)(0)) == lpcstrText[0])
			{
				// String is empty.
				return;
			}
			if (((FILE *)(0)) == m_p_FILE)
			{
				// File is not open.
				return;
			}

			fprintf( m_p_FILE, "%s", lpcstrText );
		}






		void  STFileWriter::WriteFormattedText( const char *format, ... )
		{
			if (((char *)(0)) == format)
			{
				// String is NULL.
				return;
			}
			if (((char)(0)) == format[0])
			{
				// String is empty.
				return;
			}
			if (((FILE *)(0)) == m_p_FILE)
			{
				// File is not open.
				return;
			}

			// Format the string
			va_list       ap;          // Argument pointer
			unsigned char s[ 8192 ];   // Output string              
			va_start( ap, format );
			vsprintf( (char *)s, format, ap );
			va_end  ( ap );

			if (((char)(0)) == s[0]) return;

			fprintf( m_p_FILE, "%s", s );
		}











		STFileReader::STFileReader( )
		{
		}







		void  STFileReader::Clear( )
		{
			mVectorString.clear();
		}







		STFileReader::~STFileReader( )
		{
			(*this).Clear();
		}







		int   STFileReader::ReadAndTokenizeFile( char * lpcstrFilePathAndName )
		{
			// First, clear any existing token list.
			(*this).Clear();

			// If we don't have a filename, fail.
			if (((char *)(0)) == lpcstrFilePathAndName)
			{
				char lpcstrTextMessage[ 2048 ];
				sprintf( lpcstrTextMessage, "ERROR: Filename is NULL." );                     
				MessageBoxA( NULL, lpcstrTextMessage, 
					"STFileReader::ReadAndTokenizeFile()", MB_OK );
				return(-1);
			}

			FILE * m_p_FILE = ((FILE *)(0));

			m_p_FILE = fopen( lpcstrFilePathAndName, "r" );

			if (((FILE *)(0)) == m_p_FILE)
			{
				char lpcstrTextMessage[ 2048 ];
				sprintf( lpcstrTextMessage, "ERROR: Failed to open '%s' for reading.",
					lpcstrFilePathAndName );
				MessageBoxA( NULL, lpcstrTextMessage, 
					"STFileReader::ReadAndTokenizeFile()", MB_OK );
				return(-1);    
			}


			// Read in file, putting contiguous non-whitespace in to
			// strings, and add to vector.
			string   string_Text = "";
			char     char_Value  = ((char)(0));
			int      collectingCharacters = 0;

			while( ! feof( m_p_FILE ) )
			{
				char_Value = fgetc( m_p_FILE );

				if ( (' '  == char_Value) ||
					('\t' == char_Value) ||
					('\r' == char_Value) ||
					('\n' == char_Value) ||
					('\b' == char_Value) )
				{
					// Whitespace encountered
					if (collectingCharacters)
					{
						mVectorString.push_back( string_Text );
						string_Text = ""; // Clobber string
						collectingCharacters = 0;
					}
					else
					{
						// Still whitespace...
					}
				}
				else
				{
					// Non-whitespace
					if (collectingCharacters)
					{
						// Continue accumulating
						string_Text += char_Value;
					}
					else
					{
						// Start accumulating
						string_Text = ""; // Clear string
						string_Text += char_Value; // Append first character
						collectingCharacters = 1;
					}
				}
			}

			// If we are still in the character collection state,
			// add the last string to the vector.
			if (collectingCharacters)
			{
				mVectorString.push_back( string_Text );
				string_Text = ""; // Clobber string
				collectingCharacters = 0;
			}

			fclose( m_p_FILE );
			m_p_FILE = ((FILE *)(0));

			return(0);
		}







		int   STFileReader::GetTotalTokens( )
		{
			int totalTokens = 0;

			totalTokens = (int)(mVectorString.size());

			return( totalTokens );
		}







		int   STFileReader::GetTokenByIndex( int index, char * lpcstrText )
		{
			int totalTokens = 0;
			totalTokens = (int)(mVectorString.size());

			if (((char *)(0)) == lpcstrText)
			{
				// String pointer is NULL.
				return(0);
			}

			// Clear result
			lpcstrText[0] = ((char)(0));

			if ((index < 0) || (index >= totalTokens))
			{
				// Index is out of range.  Give up.
				return(0);
			}

			// Okay, fetch string from vector of tokens.
			sprintf( lpcstrText, "%s", (char *)(mVectorString[ index ].c_str()) );
			return(1);
		}







		int   STFileReader::GetTokenAsIntegerByIndex( int index )
		{
			int totalTokens = 0;
			totalTokens = (int)(mVectorString.size());

			if ((index < 0) || (index >= totalTokens))
			{
				// Index is out of range.  Give up.
				return(0);
			}

			char * pcstrText = (char *)(0);
			pcstrText = (char *)(mVectorString[ index ].c_str());

			if (((char *)(0)) == pcstrText   )  return(0);
			if (((char  )(0)) == pcstrText[0])  return(0);

			int value = 0;
			value = (int)(atoi( (const char *)pcstrText ));

			return( value );
		}







		unsigned int  STFileReader::GetTokenAsUnsignedIntegerByIndex
			(
			int index
			)
		{
			int totalTokens = 0;
			totalTokens = (int)(mVectorString.size());

			if ((index < 0) || (index >= totalTokens))
			{
				// Index is out of range.  Give up.
				return(0);
			}

			char * pcstrText = (char *)(0);
			pcstrText = (char *)(mVectorString[ index ].c_str());

			if (((char *)(0)) == pcstrText   )  return(0);
			if (((char  )(0)) == pcstrText[0])  return(0);

			char * pcstrStopStringPtr = ((char *)(0));
			unsigned int uvalue = 0;
			uvalue = (unsigned int)(strtoul( pcstrText, &pcstrStopStringPtr, 10 ));

			return( uvalue );
		}







		float  STFileReader::GetTokenAsFloatByIndex
			(
			int index
			)
		{
			int totalTokens = 0;
			totalTokens = (int)(mVectorString.size());

			if ((index < 0) || (index >= totalTokens))
			{
				// Index is out of range.  Give up.
				return(0.0f);
			}

			char * pcstrText = (char *)(0);
			pcstrText = (char *)(mVectorString[ index ].c_str());

			if (((char *)(0)) == pcstrText   )  return(0.0f);
			if (((char  )(0)) == pcstrText[0])  return(0.0f);

			float value = (0.0f);
			value = (float)(atof(pcstrText));

			return( value );
		}







		int   STFileReader::FindTokenAtOrAfterIndex
			(
			int startIndex,
			char * lpcstrText
			)
		{
			if (((char *)(0)) == lpcstrText)
				return(-1);

			if (((char)(0)) == lpcstrText[0])
				return(-1);

			if (startIndex < 0)
				return(-1);

			int totalTokens = 0;
			totalTokens = (int)(mVectorString.size());

			if (startIndex >= totalTokens)
				return(-1);

			int indexToken  = 0;
			for ( 
				indexToken  = startIndex;
				indexToken < totalTokens;
			indexToken++
				)
			{
				if (0 == strcmp( (const char *)(lpcstrText), 
					(const char *)(mVectorString[indexToken].c_str()) ))
				{
					return( indexToken );
				}
			}

			return(-1); // Not found
		}







		// (-1)==Not found, otherwise index
		int   STFileReader::FindToken( char * lpcstrText )
		{
			return( (*this).FindTokenAtOrAfterIndex( 0, lpcstrText ) );
		}








		void  STFileReader::FindTextAndIntegerAtOrAfterIndex
			(
			int startIndex,
			char *pcstrText,
			int & ref_value
			)
		{
			ref_value = 0;

			int indexToken = 0;
			indexToken = (*this).FindTokenAtOrAfterIndex
				( startIndex, pcstrText );
			if (indexToken >= 0)
			{
				ref_value =
					(*this).GetTokenAsIntegerByIndex( indexToken + 1 );
			}
		}







		void  STFileReader::FindTextAndUnsignedIntegerAtOrAfterIndex
			(
			int startIndex,
			char *pcstrText,
			unsigned int & ref_uvalue
			)
		{
			ref_uvalue = 0;

			int indexToken = 0;
			indexToken = (*this).FindTokenAtOrAfterIndex
				( startIndex, pcstrText );
			if (indexToken >= 0)
			{
				ref_uvalue =
					(*this).GetTokenAsUnsignedIntegerByIndex
					( indexToken + 1 );
			}
		}







		void  STFileReader::FindTextAndFloatAtOrAfterIndex
			(
			int startIndex,
			char *pcstrText,
			float & ref_value
			)
		{
			ref_value = 0.0f;

			int indexToken = 0;
			indexToken = (*this).FindTokenAtOrAfterIndex
				( startIndex, pcstrText );
			if (indexToken >= 0)
			{
				ref_value =
					(*this).GetTokenAsFloatByIndex( indexToken + 1 );
			}
		}











		void  STFileReader::FindTextAndInteger
			(
			char * pcstrText,
			int &  ref_value
			)
		{
			(*this).FindTextAndIntegerAtOrAfterIndex
				( 0, pcstrText, ref_value );
		}












		void  STFileReader::FindTextAndUnsignedInteger
			(
			char * pcstrText,
			unsigned int &  ref_uvalue
			)
		{
			(*this).FindTextAndUnsignedIntegerAtOrAfterIndex
				( 0, pcstrText, ref_uvalue );
		}








		void  STFileReader::FindTextAndFloat
			(
			char * pcstrText,
			float &  ref_value
			)
		{
			(*this).FindTextAndFloatAtOrAfterIndex
				( 0, pcstrText, ref_value );
		}






	}
}
