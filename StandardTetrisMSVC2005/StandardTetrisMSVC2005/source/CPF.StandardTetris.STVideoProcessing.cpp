// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#include "CPF.StandardTetris.STVideoProcessing.h"


#include "CPF.StandardTetris.STVideoCapture.h"


#include <windows.h>
#include <GL/gl.h>

#include <stdio.h>
#include <memory.h>

// Disable unimportant "warning C4311: 'type cast' : pointer truncation from 'LPVOID' to 'LPARAM'" 
// warning for the VFW.H macros
#pragma warning( disable : 4311 )



namespace CPF
{
	namespace StandardTetris
	{



		void STVideoProcessing::Clear ( )
        {
            mSTVideoCapture = (STVideoCapture *)0;

            mTextureDataBGR256x256 = (unsigned char *)0;
            mTextureOpenGLHandleBGR256x256 = 0;

            mVideoLUT256 = (unsigned char *)0;

            mProcessXMin = 32;
            mProcessYMin = 160;
            mProcessXMax = 96;
            mProcessYMax = 224;
            mCurrentClassification = (-1);
        }






        STVideoProcessing::STVideoProcessing ( )
        {
            (*this).Clear( );
        }





     

        void STVideoProcessing::CopyDataFromCaptureDataToLocalBuffer
        (
            unsigned char * bgrData,
            int imageWidth,
            int imageHeight
        )
        {
            // The following is a hardcoded algorithm to do a very specific copy:
            // Copy ( X = 80, Y = 0, W = 160, H = 240 ) from ( W = 320, H = 240 ) source
            // to   ( X =  0, Y = 8, W = 160, H = 240 ) on   ( W = 256, H = 256 ) destination
            int sourceByteOffset = (80 * 3);
            int destinationByteOffset = (8 * (256 * 3));

            int copyLineCount = 0;
            for (copyLineCount = 0; copyLineCount < 240; copyLineCount++)
            {
                int byteCount = 0;
                for (byteCount = 0; byteCount < (160 * 3); byteCount++)
                {
                    (*this).mTextureDataBGR256x256[destinationByteOffset + byteCount] = 
                        bgrData[sourceByteOffset + byteCount];
                }
                destinationByteOffset += (256 * 3);
                sourceByteOffset += (320 * 3);
            }
        }









        //private
        //void
        //CopyGrayscale
        //(
        //    byte[] gray,
        //    byte[] color,
        //    int length
        //)
        //{
        //    int n = (length * length);
        //    int i = 0;
        //    int j = 0;

        //    // YUV:   grayscale = (0.299 * red) + (0.587 * green) + (0.114 * blue);
        //    for (i = 0, j = 0; j < n; i += 3, j++)
        //    {
        //        gray[j] = (byte)
        //            (
        //              (0.299f * (float)color[i])
        //            + (0.587f * (float)color[i + 1])
        //            + (0.114f * (float)color[i + 2])
        //            );
        //    }
        //}





        void STVideoProcessing::BinarizeRegion ( )
        {
            if ((unsigned char *)0 == (*this).mTextureDataBGR256x256)
            {
                return;
            }

            int ix = 0;
            int iy = 0;
            unsigned char r = (unsigned char)0;
            unsigned char g = (unsigned char)0;
            unsigned char b = (unsigned char)0;
            unsigned char merged = (unsigned char)0;
            unsigned char * dataBGR = mTextureDataBGR256x256;

            if ((unsigned char *)0 == (*this).mVideoLUT256)
            {
                (*this).mVideoLUT256 = (unsigned char *) malloc( 256 );
                int i = 0;
                for (i = 0; i < 255; i++)
                {
                    (*this).mVideoLUT256[i] = (unsigned char)0;
                    if (i >= 128) (*this).mVideoLUT256[i] = (unsigned char)255;
                }
            }

            if (mProcessXMin < 0) mProcessXMin = 0;
            if (mProcessXMin > 255) mProcessXMin = 255;
            if (mProcessYMin < 0) mProcessYMin = 0;
            if (mProcessYMin > 255) mProcessYMin = 255;

            if (mProcessXMax < 0) mProcessXMax = 0;
            if (mProcessXMax > 255) mProcessXMax = 255;
            if (mProcessYMax < 0) mProcessYMax = 0;
            if (mProcessYMax > 255) mProcessYMax = 255;

            int kx = 0;
            int ky = 0;
            for (iy = mProcessYMin; iy <= mProcessYMax; iy++)
            {
                ky = ((3 * 256) * iy);
                for (ix = mProcessXMin; ix <= mProcessXMax; ix++)
                {
                    kx = (3 * ix);
                    b = dataBGR[ky + kx];
                    g = dataBGR[ky + kx + 1];
                    r = dataBGR[ky + kx + 2];
					unsigned char ave = (unsigned char)(((int)r + (int)g + (int)b) >> 2);

                    merged = (*this).mVideoLUT256[ave];

                    dataBGR[ky + kx] = merged;
                    dataBGR[ky + kx + 1] = merged;
                    dataBGR[ky + kx + 2] = merged;
                }
            }

        }










        void STVideoProcessing::ClassifyRegion ( )
        {
            if ((unsigned char *)0 == (*this).mTextureDataBGR256x256)
            {
                mCurrentClassification = (-1);
                return;
            }

            (*this).BinarizeRegion( );

            // Go through the 4 x 2 = 8 cells in the region and
            // simply determine if the majority of pixels in each
            // cell is white.

            int regionWidth = ((mProcessXMax - mProcessXMin) + 1);
            int regionHeight = ((mProcessYMax - mProcessYMin) + 1);

            int cellWidth = (regionWidth / 4);
            int cellHeight = (regionHeight / 2);
            int cellPixels = (cellWidth * cellHeight);

            int whitePixels = 0;

            unsigned char pixel = (unsigned char)0;
            unsigned char * dataBGR = (*this).mTextureDataBGR256x256;

            int cellX = 0;
            int cellY = 0;
            int cellIndex = 0;
            int bits[8];

            for (cellY = 0; cellY < 2; cellY++)
            {
                for (cellX = 0; cellX < 4; cellX++)
                {
                    cellIndex = (4 * (1 - cellY)) + cellX;
                    bits[cellIndex] = 0;

                    // Count pixels in this cell
                    whitePixels = 0;

                    int ix = 0;
                    int iy = 0;
                    int kx = 0;
                    int ky = 0;
                    for (iy = 0; iy <= cellHeight; iy++)
                    {
                        ky = ((3 * 256) * (mProcessYMin + cellY * cellHeight + iy));
                        for (ix = 0; ix <= cellWidth; ix++)
                        {
                            kx = (3 * (mProcessXMin + cellX * cellWidth + ix));
                            pixel = dataBGR[ky + kx];  // Really the blue component, but we binarized already!
                            if (0 != pixel)
                            {
                                whitePixels++;
                            }
                        }
                    }

                    if (whitePixels > (cellPixels / 2))
                    {
                        bits[cellIndex] = 1;
                    }
                }
            }

            if
                (
                ((0 == bits[0])) && ((0 != bits[1])) && ((0 != bits[2])) && ((0 == bits[3])) &&
                ((0 == bits[4])) && ((0 != bits[5])) && ((0 != bits[6])) && ((0 == bits[7]))
                )
            {
                mCurrentClassification = 1; // "O"
            }

            else if
                (
                ((0 != bits[0])) && ((0 != bits[1])) && ((0 != bits[2])) && ((0 != bits[3])) &&
                ((0 == bits[4])) && ((0 == bits[5])) && ((0 == bits[6])) && ((0 == bits[7]))
                )
            {
                mCurrentClassification = 2; // "I"
            }

            else if
                (
                ((0 == bits[0])) && ((0 == bits[1])) && ((0 != bits[2])) && ((0 != bits[3])) &&
                ((0 == bits[4])) && ((0 != bits[5])) && ((0 != bits[6])) && ((0 == bits[7]))
                )
            {
                mCurrentClassification = 3; // "S"
            }

            else if
                (
                ((0 == bits[0])) && ((0 != bits[1])) && ((0 != bits[2])) && ((0 == bits[3])) &&
                ((0 == bits[4])) && ((0 == bits[5])) && ((0 != bits[6])) && ((0 != bits[7]))
                )
            {
                mCurrentClassification = 4; // "Z"
            }

            else if
                (
                ((0 == bits[0])) && ((0 != bits[1])) && ((0 != bits[2])) && ((0 != bits[3])) &&
                ((0 == bits[4])) && ((0 != bits[5])) && ((0 == bits[6])) && ((0 == bits[7]))
                )
            {
                mCurrentClassification = 5; // "L"
            }

            else if
                (
                ((0 == bits[0])) && ((0 != bits[1])) && ((0 != bits[2])) && ((0 != bits[3])) &&
                ((0 == bits[4])) && ((0 == bits[5])) && ((0 == bits[6])) && ((0 != bits[7]))
                )
            {
                mCurrentClassification = 6; // "J"
            }

            else if
                (
                ((0 == bits[0])) && ((0 != bits[1])) && ((0 != bits[2])) && ((0 != bits[3])) &&
                ((0 == bits[4])) && ((0 == bits[5])) && ((0 != bits[6])) && ((0 == bits[7]))
                )
            {
                mCurrentClassification = 7; // "T"
            }

            else
            {
                mCurrentClassification = 0; // 0 == NOTHING
            }

        }








        void STVideoProcessing::SetRegion
        (
            int xMin,
            int yMin,
            int xMax,
            int yMax
        )
        {
            mProcessXMin = xMin;
            mProcessYMin = yMin;
            mProcessXMax = xMax;
            mProcessYMax = yMax;
        }








        void STVideoProcessing::GetRegion
        (
            int & xMin,
            int & yMin,
            int & xMax,
            int & yMax
        )
        {
            xMin = mProcessXMin;
            yMin = mProcessYMin;
            xMax = mProcessXMax;
            yMax = mProcessYMax;
        }








        void STVideoProcessing::ClearRegionStatus ( )
        {
            (*this).mCurrentClassification = (-1);

            // Clear the image to make sure we don't accidentally 
            // attempt to classify the image again before a new frame
            // has been captured.  (NOTE: It would be worthwhile to
            // determine if this paranoia is too much.  At the very
            // least, an alternative mechanism, such as a flag, could
            // be used instead.)
            if ((unsigned char *)0 != (*this).mTextureDataBGR256x256)
            {
				memset( (void *)(*this).mTextureDataBGR256x256, 0, (256 * (256 * 3)) );
            }

        }








        int STVideoProcessing::GetRegionClassification ( )
        {
            return ((*this).mCurrentClassification);
        }






        void STVideoProcessing::VideoCaptureClientCallback
            (
            unsigned char * bgrData,
            int imageWidth,
            int imageHeight
            )
        {
            // Copy the captured data to a local buffer
            (*this).CopyDataFromCaptureDataToLocalBuffer( bgrData, imageWidth, imageHeight );

            // Process the image data
            (*this).ClassifyRegion( );

            // Update the associated texture
            glBindTexture( GL_TEXTURE_2D, (*this).mTextureOpenGLHandleBGR256x256 );

            glTexSubImage2D
            (
                GL_TEXTURE_2D,         // target
                0,                     // level
                0,                     // xoffset
                0,                     // yoffset
                256,                   // width
                256,                   // height
                GL_BGR_EXT,            // format
                GL_UNSIGNED_BYTE,      // type
                (*this).mTextureDataBGR256x256  // texel data
            );

        }







        int STVideoProcessing::Initialize
        (
            HWND hwndParentWindow
        )
        {
            (*this).Clear( );


            (*this).mTextureDataBGR256x256 = (unsigned char *) malloc( 256 * (256 * 3) );


            (*this).mSTVideoCapture = new STVideoCapture();

            int capturePeriodMilliseconds = 33; // 33 milliseconds --> 30 frames per second (max)

            // Attempt to initialize video capture
            int videoCaptureInitializationResult = 0;
            videoCaptureInitializationResult = 
                ((*this).mSTVideoCapture)->Initialize
                    ( 
                    hwndParentWindow,
					STVideoCapture::BGR320x240,
                    capturePeriodMilliseconds,
					(STVideoCaptureClient *)this
                    );

            if (0 == videoCaptureInitializationResult)
            {
                return(0);
            }




            // Create OpenGL texture object
            GLuint temp = 0;
            glGenTextures( 1, &temp );
            (*this).mTextureOpenGLHandleBGR256x256 = (int)temp;


            // Fill the OpenGL texture with initial data
            glBindTexture
            (
                GL_TEXTURE_2D,
                (*this).mTextureOpenGLHandleBGR256x256
            );

            glTexImage2D
            (
                GL_TEXTURE_2D,
                0,
                3,
                256,
                256,
                0,
                GL_BGR_EXT,
                GL_UNSIGNED_BYTE,
                (*this).mTextureDataBGR256x256
            );

            // Set texture mapping mode and filtering modes
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );


            // deselect the current texture
            glBindTexture( GL_TEXTURE_2D, 0 );         


            return(1);
        }










        void STVideoProcessing::Terminate ( )
        {
            if ((STVideoCapture *)0 != (*this).mSTVideoCapture)
            {
                ((*this).mSTVideoCapture)->Terminate( );
				delete (*this).mSTVideoCapture;
				(*this).mSTVideoCapture = (STVideoCapture *)0;
            }

            if (0 != mTextureOpenGLHandleBGR256x256)
            {
                // Delete OpenGL texture object
                GLuint temp = 0;
				temp = (GLuint)((*this).mTextureOpenGLHandleBGR256x256);
                glDeleteTextures( 1, &temp );
            }

            (*this).Clear( );
        }





	}
}
