// SOME content of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.
// Most of the code here is an ADAPTATION of an MD5 implementation in C 
// created by RSA Data Security, Inc in 1991.  Please see comments below for
// more information quoted from the original C code.  
// The C++ adaptation here merges the original MD5 code and corresponding 
// testing code.



#ifndef STMD5Header
#define STMD5Header



namespace CPF
{
	namespace StandardTetris
	{



		class STMD5
		{


			// MD5.H - header file for MD5C.C
			//
			// Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
			// rights reserved.
			//
			// License to copy and use this software is granted provided that it
			// is identified as the "RSA Data Security, Inc. MD5 Message-Digest
			// Algorithm" in all material mentioning or referencing this software
			// or this function.
			//
			// License is also granted to make and use derivative works provided
			// that such works are identified as "derived from the RSA Data
			// Security, Inc. MD5 Message-Digest Algorithm" in all material
			// mentioning or referencing the derived work.
			//
			// RSA Data Security, Inc. makes no representations concerning either
			// the merchantability of this software or the suitability of this
			// software for any particular purpose. It is provided "as is"
			// without express or implied warranty of any kind.
			//
			// These notices must be retained in any copies of any part of this
			// documentation and/or software.


			// MD5 context.
		public: typedef struct 
				{
				public: unsigned int  state[4];        // state (ABCD)
				public: unsigned int  count[2];        // number of bits, modulo 2^64 (lsb first)
				public: unsigned char buffer[64];      // input buffer
				} 
				MD5_CTX;


		public: static void MD5Init
					( 
					MD5_CTX * context // context
					);

		public: static void MD5Update 
					( 
					MD5_CTX * context, // context
					unsigned char * input, // input block
					unsigned int inputLen // length of input block 
					);

		public: static void MD5Final
					( 
					unsigned char * digest, // message digest (16 bytes)
					MD5_CTX * context // context
					);




		public: static void MD5_memcpy 
					(
					unsigned char * output,
					unsigned char * input,
					unsigned int len
					);

		public: static void MD5_memset 
					(
					unsigned char * output,
					int value,
					unsigned int len
					);

		public: static void MD5Transform 
					(
					unsigned int  state[4],
					unsigned char block[64]
				);



				// Encodes input (UINT4) into output (unsigned char). Assumes len is
				//  a multiple of 4.

		public: static void Encode 
					(
					unsigned char *output,
					unsigned int  *input,
					unsigned int len
					);

				// Decodes input (unsigned char) into output (UINT4). Assumes len is
				//  a multiple of 4.

		public: static void Decode 
					(
					unsigned int  *output,
					unsigned char *input,
					unsigned int len
					);

		public: static void MD5String ( char * text );

		public: static void MD5Print (unsigned char digest[16]);

		public: static void MD5TimeTrial ();

		public: static void MD5TestSuite ();

		public: static void MD5File (char *filename);

		public: static void MD5Filter ();

		};


	}
}



#endif






