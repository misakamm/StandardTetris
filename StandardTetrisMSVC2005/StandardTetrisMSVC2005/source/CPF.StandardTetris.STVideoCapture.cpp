// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#include "CPF.StandardTetris.STVideoCapture.h"


#include <windows.h>
#include <GL/gl.h>

#include <stdio.h>

// Disable unimportant "warning C4311: 'type cast' : pointer truncation from 'LPVOID' to 'LPARAM'" 
// warning for the VFW.H macros
#pragma warning( disable : 4311 )



namespace CPF
{
	namespace StandardTetris
	{



		STVideoCapture * 
			STVideoCapture::mListSTVideoCapture
			[ STVideoCapture::MAX_VIDEO_CAPTURE_CHANNELS ] = { 0 };





		LRESULT CALLBACK  
			STVideoCapture::CommonCaptureCallback
			( 
			HWND        hwnd, 
			LPVIDEOHDR  video_hdr 
			)
		{
			int i = 0;
			int n = (STVideoCapture::MAX_VIDEO_CAPTURE_CHANNELS);

			for ( i = 0; i < n; i++ )
			{
				if (0 != STVideoCapture::mListSTVideoCapture[i])
				{
					if ((STVideoCapture::mListSTVideoCapture[i]->GetHWND()) == hwnd)
					{
						return( STVideoCapture::mListSTVideoCapture[i]->InstanceCaptureCallback( hwnd, video_hdr ) );
					}
				}
			}
			return( 0 );
		}







        int STVideoCapture::GetCaptureWidth ( )
        {
            int width = 0;
            switch ((*this).mSTVideoCaptureFormat)
            {
			case BGR320x240: width = 320; break;
            }
            return (width);
        }


        int STVideoCapture::GetCaptureHeight ( )
        {
            int height = 0;
            switch ((*this).mSTVideoCaptureFormat)
            {
			case BGR320x240: height = 240; break;
            }
            return (height);
        }



        void STVideoCapture::Clear ( )
        {
            mCapDriverConnectStatus = 0; // capDriverConnect()
            mHWNDCapture = ((HWND)(0));
			mSTVideoCaptureFormat = BGR320x240;
            mCapturePeriodMilliseconds = 67; // 67 milliseconds --> 15 frames per second maximum
            //// mClientCaptureCallback = null;
            mBGRData = ((unsigned char *)(0));
        }





		STVideoCapture::STVideoCapture()
		{
			(*this).Clear( );


			int i = 0;
			int n = (STVideoCapture::MAX_VIDEO_CAPTURE_CHANNELS);

			for ( i = 0; i < n; i++ )
			{
				if (0 == STVideoCapture::mListSTVideoCapture[i])
				{
					STVideoCapture::mListSTVideoCapture[i] = (this);
					i = n;
				}
			}
		}



		STVideoCapture::~STVideoCapture()
		{
			int i = 0;
			int n = (STVideoCapture::MAX_VIDEO_CAPTURE_CHANNELS);

			for ( i = 0; i < n; i++ )
			{
				if ((this) == STVideoCapture::mListSTVideoCapture[i])
				{
					STVideoCapture::mListSTVideoCapture[i] = (STVideoCapture *)(0);
				}
			}
		}







		LRESULT CALLBACK  
			STVideoCapture::InstanceCaptureCallback
			( 
			HWND        hwnd, 
			LPVIDEOHDR  videoHdr 
			)
		{ 

			if ( (STVideoCaptureClient *)0 == (*this).mpSTVideoCaptureClient )
			{
				return( TRUE );
			}

            // Copy image data to local buffer before calling client
            int captureWidth = 0;
            int captureHeight = 0;
            captureWidth = (*this).GetCaptureWidth( );
            captureHeight = (*this).GetCaptureHeight( );


            int sourceStrideBytes = 0;
            int sourceCopyStartByteOffset = 0;
            int destinationStrideBytes = 0;
            int destinationCopyStartByteOffset = 0;
            int bytesToCopyPerLine = 0;
            
            sourceCopyStartByteOffset = 0;
            sourceStrideBytes = (captureWidth * 3);
            destinationCopyStartByteOffset = 0;
            destinationStrideBytes = (captureWidth * 3);
            bytesToCopyPerLine = (captureWidth * 3);



			unsigned char * pSourceData = (unsigned char *)(videoHdr->lpData);

            int lineIndex = 0;
            for (lineIndex = 0; lineIndex < captureHeight; lineIndex++)
            {
				unsigned char * pSourceLineStart = 
					(unsigned char *)(&(pSourceData[sourceCopyStartByteOffset]));

				unsigned char * pDestinationLineStart = 
					(unsigned char *)(&((*this).mBGRData[destinationCopyStartByteOffset]));

				memcpy( (void *)pDestinationLineStart, (const void *)pSourceLineStart, bytesToCopyPerLine );

                sourceCopyStartByteOffset += sourceStrideBytes;
                destinationCopyStartByteOffset += destinationStrideBytes;
            }


            // Call the client
            ((*this).mpSTVideoCaptureClient)->VideoCaptureClientCallback
                ( 
                (*this).mBGRData, 
                (*this).GetCaptureWidth(), 
                (*this).GetCaptureHeight() 
                );

            return (TRUE);
		} 






		int   
			STVideoCapture::Initialize
			( 
            HWND hwndParentWindow, // This must be non-null
            STVideoCaptureFormat videoCaptureFormat,
            int capturePeriodMilliseconds,
			STVideoCaptureClient * pSTVideoCaptureClient
			)
		{
			(*this).Clear();




            // Validate capture format
            int captureWidth = 0;
            int captureHeight = 0;
            switch (videoCaptureFormat)
            {
			case BGR320x240: { captureWidth = 320; captureHeight = 240; } break;
            }

            if ((0 == captureWidth) || (0 == captureHeight))
            {
                printf( "Invalid capture format." );
                (*this).Clear();
                return (FALSE);
            }
            (*this).mSTVideoCaptureFormat = videoCaptureFormat;




            // We create a capture window only because we are required to create
            // such a window to use the Video for Windows (VFW32) API.  We make
            // this window tiny to keep it out of the way of our application.
            // For our purposes, this window is simply a necessary interface to
            // VFW functionality.
            (*this).mHWNDCapture =
                capCreateCaptureWindowA
                (
                    "STVideoCapture Window",
                    (WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS), // style
                    0, // x
                    0, // y
                    4, // width
                    4, // height
                    hwndParentWindow, // Parent window : This must be non-null
                    0 // nID
                );



			if (((HWND)(0)) == (*this).mHWNDCapture)
			{ 
                printf( "Failed to create video capture window." );
                DestroyWindow( (*this).mHWNDCapture );
                (*this).Clear();
                return (FALSE);
			}




			(*this).mCapDriverConnectStatus = capDriverConnect( (*this).mHWNDCapture, 0 );

            if (0 == (*this).mCapDriverConnectStatus)
            {
                printf( "Failed to connect to video driver." );
                DestroyWindow( (*this).mHWNDCapture );
                (*this).Clear();
                return (FALSE);
            }



			CAPDRIVERCAPS  capdrivercaps;
			memset( (&(capdrivercaps)),   0,  sizeof(capdrivercaps) );
			capDriverGetCaps( (*this).mHWNDCapture, &(capdrivercaps), sizeof(capdrivercaps) );




			//capDlgVideoFormat( mHWNDCapture );
			//capDlgVideoSource( mHWNDCapture );  // Source; Brightness, Contrast, Saturation, Exposure



            // Allocate a local buffer to store capture data
            int bytesPerPixel = 3;
            switch (videoCaptureFormat)
            {
			case BGR320x240: { bytesPerPixel = 3; } break;
            }
            int totalImageBytes = 0;
            totalImageBytes = (captureHeight * (captureWidth * bytesPerPixel));
            (*this).mBGRData = (unsigned char *) malloc( totalImageBytes );
			memset( (*this).mBGRData,   0,  totalImageBytes );



			// Set the video stream callback function
			(*this).mpSTVideoCaptureClient = pSTVideoCaptureClient;
			capSetCallbackOnFrame( (*this).mHWNDCapture, STVideoCapture::CommonCaptureCallback );


			// Set the preview rate in milliseconds
            (*this).mCapturePeriodMilliseconds = capturePeriodMilliseconds;
            capPreviewRate( (*this).mHWNDCapture, capturePeriodMilliseconds );


			// Disable preview mode
			capPreview( (*this).mHWNDCapture, FALSE );


			// Setup the data we want returned to us
			BITMAPINFO  capbitmapinfo;
			memset( &(capbitmapinfo), 0, sizeof(BITMAPINFO) );
			capbitmapinfo.bmiHeader.biSize           = sizeof(BITMAPINFOHEADER);
			capbitmapinfo.bmiHeader.biWidth          = captureWidth;
			capbitmapinfo.bmiHeader.biHeight         = captureHeight;
			capbitmapinfo.bmiHeader.biPlanes         =  1;
			capbitmapinfo.bmiHeader.biBitCount       = 24;
			capbitmapinfo.bmiHeader.biCompression    = BI_RGB;
			capbitmapinfo.bmiHeader.biSizeImage      = totalImageBytes;
			capbitmapinfo.bmiHeader.biXPelsPerMeter  = 100;
			capbitmapinfo.bmiHeader.biYPelsPerMeter  = 100;

			BOOL setVideoFormatResult = FALSE;
			setVideoFormatResult = capSetVideoFormat( (*this).mHWNDCapture, &(capbitmapinfo), sizeof(BITMAPINFO) );

            if (FALSE == setVideoFormatResult)
            {
                printf( "Failed to set the desired video capture format." );
                capSetCallbackOnFrame( (*this).mHWNDCapture, NULL ); // disable the callback function
                DestroyWindow( (*this).mHWNDCapture );
                (*this).Clear( );
                return (FALSE);
            }


			return( TRUE );
		} 







        HWND STVideoCapture::GetHWND ( )
        {
            return ((*this).mHWNDCapture);
        }







		void  STVideoCapture::Terminate()
		{
			int i = 0;
			int n = (STVideoCapture::MAX_VIDEO_CAPTURE_CHANNELS);

			for ( i = 0; i < n; i++ )
			{
				if ((this) == STVideoCapture::mListSTVideoCapture[i])
				{
					STVideoCapture::mListSTVideoCapture[i] = (STVideoCapture *)(0);
				}
			}


			if (0 != (*this).mCapDriverConnectStatus)
			{ 
				capSetCallbackOnFrame( (*this).mHWNDCapture, NULL );
			}

			if (((HWND)0) != (*this).mHWNDCapture)
			{
				DestroyWindow( (*this).mHWNDCapture );
			}

			if (((unsigned char *)(0)) != (*this).mBGRData)
			{ 
				free( (*this).mBGRData );
			} 

			(*this).Clear( );
		}





	}
}
