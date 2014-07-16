// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STOpenGLFontHeader
#define STOpenGLFontHeader



#include <windows.h>     // Necessary for OpenGL #include's
#include <gl\gl.h>       // OpenGL
#include <gl\glu.h>      // GLU library



// The following 128 x 128, 1-bit-per-pixel bitmap is
// Courier New 10pt font, where each character is in a
// 8x16 pixel cell.  The character grid is 16 x 8.
// The first 96 chars are ASCII chars 32..127.
// The last  32 chars are ASCII chars 192..223.
#define FONT_WIDTH  (8.0f)
#define FONT_HEIGHT (16.0f)




namespace CPF
{
	namespace StandardTetris
	{



		class STOpenGLFont
		{

		public: static unsigned char  Courier10FontBitmap[2048];

		public: static GLuint  gFontTextureName;

		public: static void   FontMakeTexture();

		public: static void   FontPrint( float x, float y, const char *format, ... );

		};


	}
}




#endif






