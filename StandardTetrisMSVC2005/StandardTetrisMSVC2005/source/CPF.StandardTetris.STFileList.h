// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STFileListHeader
#define STFileListHeader



// Disable unimportant "symbol too long" error for debug builds of STL string
#pragma warning( disable : 4786 )
#include <string>
#include <vector>
using namespace std;



namespace CPF
{
	namespace StandardTetris
	{


		class STFileItem
		{
		public:
			string   m_string_FileName;
			string   m_string_FilePathAndName;
			int      mFileSizeInBytes;

		public:
			static bool name_compare
				(
				const STFileItem & a,
				const STFileItem & b
				)
			{
				// "<" makes the newest file appear LAST in the list
				return( a.m_string_FileName < b.m_string_FileName ? 1 : 0 );
			}
		};



		class STFileList
		{
		private:
			string  m_string_Path;
			vector< STFileItem >  mVectorSTFileItem;
			void PrivateClearEntries( );
			void PrivateSortEntries( );

		public:
			void GetDirectoryPath( string & ref_string_Path )
			{ ref_string_Path = m_string_Path; }
			void ScanDirectory( string & ref_string_Path );  // "C:", not "C:\"
			int  GetTotalItems( );
			void GetItemNameByIndex( int index, string & ref_string_Name );
			void GetItemFullPathAndNameByIndex( int index, string & ref_string_Name );
		};



	}
}



#endif






