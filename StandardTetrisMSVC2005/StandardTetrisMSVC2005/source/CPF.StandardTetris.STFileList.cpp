// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1



#include "CPF.StandardTetris.STFileList.h"



#include <windows.h>  // HANDLE, FindFirstFile(), ...
#include <string.h>   // strcmp()
#include <memory.h>   // memset()

// Disable unimportant "symbol too long" error for debug builds of STL string
#pragma warning( disable : 4786 )
#include <string>
#include <vector>
#include <algorithm>  // sort(start1, end1, bool_compare) ;
using namespace std;



namespace CPF
{
	namespace StandardTetris
	{


		void STFileList::PrivateClearEntries( )
		{
			mVectorSTFileItem.clear();
		}



		void STFileList::PrivateSortEntries( )
		{
			int totalItems = 0;
			totalItems = (int)(mVectorSTFileItem.size());

			if (totalItems <= 0)
			{
				return;
			}

			sort  // sort() is an <algorithm> function
				( 
				mVectorSTFileItem.begin(),
				mVectorSTFileItem.end(),
				STFileItem::name_compare
				);
		}



		// NOTE: Path should be "C:", not "C:\"

		void STFileList::ScanDirectory( string & ref_string_Path )
		{
			// Clobber existing cached entries
			(*this).PrivateClearEntries();

			m_string_Path = ref_string_Path;

			// Find all files in the specified directory and add to the cache of file
			// items.
			char              pcstr_SearchName[ 1024 ];
			HANDLE            HANDLE_FindFile            = ((HANDLE)(0));
			BOOL              bool_Result                = ((BOOL)(0));
			WIN32_FIND_DATAA   WIN32_FIND_DATA_Structure;


			memset
				(
				(void *)(&(WIN32_FIND_DATA_Structure)),
				(int)(0),
				sizeof(WIN32_FIND_DATA_Structure)
				);

			//// Tack "*" on to the path to form a search name...
			//sprintf( pcstr_SearchName, "%s\\*", (char *)(ref_string_Path.c_str()) );

			// Tack "standard_tetris_*.txt" on to the path to form a search name...
			sprintf
				( 
				pcstr_SearchName, 
				"%s\\tetris_state_*.txt", 
				(char *)(ref_string_Path.c_str()) 
				);

			HANDLE_FindFile = 
				FindFirstFileA
				(
				pcstr_SearchName,
				(&(WIN32_FIND_DATA_Structure))
				);

			if (INVALID_HANDLE_VALUE == HANDLE_FindFile)
			{
				// Failed to open directory.
				return;
			}


			do
			{
				// If it's not ".." or "." then it's worth considering...
				if 
					( 
					(strcmp( "..", WIN32_FIND_DATA_Structure.cFileName )!=0) &&
					(strcmp(  ".", WIN32_FIND_DATA_Structure.cFileName )!=0) 
					)
				{
					if (0 == (FILE_ATTRIBUTE_DIRECTORY & (WIN32_FIND_DATA_Structure.dwFileAttributes)))
					{
						// This item is a file.
						STFileItem  FileItem;

						// I'd be really surprised if the tetris state ever takes more than 4 GB!
						FileItem.mFileSizeInBytes = (int)(WIN32_FIND_DATA_Structure.nFileSizeLow);

						// File Name
						FileItem.m_string_FileName += (string)(WIN32_FIND_DATA_Structure.cFileName);

						// File Path and Name
						FileItem.m_string_FilePathAndName  = ref_string_Path;
						FileItem.m_string_FilePathAndName += (string)("\\");
						FileItem.m_string_FilePathAndName += (string)(WIN32_FIND_DATA_Structure.cFileName);

						// Add item to our cache
						mVectorSTFileItem.push_back( FileItem );
					}
				}

				// Clobber entire find data structure
				memset
					(
					(void *)(&(WIN32_FIND_DATA_Structure)),
					(int)(0),
					sizeof(WIN32_FIND_DATA_Structure)
					);

				bool_Result = 
					FindNextFileA
					( 
					HANDLE_FindFile, 
					(&(WIN32_FIND_DATA_Structure)) 
					);
				// 0==bool_Result   ==>  GetLastError() --> ERROR_NO_MORE_FILES
			}
			while( bool_Result );

			(*this).PrivateSortEntries();
		}



		int  STFileList::GetTotalItems( )
		{
			return( (int)(mVectorSTFileItem.size()) );
		}



		void STFileList::GetItemNameByIndex
			(
			int index,
			string & ref_string_Name
			)
		{
			ref_string_Name = "";
			if (index < 0) return;
			int totalItems = 0;
			totalItems = (int)(mVectorSTFileItem.size());
			if (index >= totalItems)  return;

			ref_string_Name = mVectorSTFileItem[ index ].m_string_FileName;
		}



		void STFileList::GetItemFullPathAndNameByIndex
			(
			int      index,
			string & ref_string_Name
			)
		{
			ref_string_Name = "";
			if (index < 0) return;
			int totalItems = 0;
			totalItems = (int)(mVectorSTFileItem.size());
			if (index >= totalItems)  return;

			ref_string_Name =
				mVectorSTFileItem[ index ].m_string_FilePathAndName;
		}


	}
}
