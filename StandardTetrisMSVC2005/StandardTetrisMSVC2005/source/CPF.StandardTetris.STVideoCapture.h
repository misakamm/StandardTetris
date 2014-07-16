// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STVideoCaptureHeader
#define STVideoCaptureHeader



#include <windows.h>    // HWND, LRESULT, CALLBACK, ...
#include <vfw.h>        // LPVIDEOHDR, capCreateCaptureWindow(), ...



namespace CPF
{
	namespace StandardTetris
	{


		// Base class for other classes which wish to capture data.
		// The only purpose of this class is to provide a known method.
		class STVideoCaptureClient
		{
		public: virtual void VideoCaptureClientCallback
					(				
					unsigned char * bgrData,
					int imageWidth,
					int imageHeight
					)
				{
				}
		};







		class STVideoCapture
		{

		public: enum STVideoCaptureFormat
        {
            BGR320x240 = 0
        };




		private: enum { MAX_VIDEO_CAPTURE_CHANNELS = 16 };


		private: static STVideoCapture * 
			mListSTVideoCapture[ MAX_VIDEO_CAPTURE_CHANNELS ];


		private: static LRESULT CALLBACK  
				CommonCaptureCallback
				( 
				HWND hwnd, 
				LPVIDEOHDR video_hdr 
				);




		private: int mCapDriverConnectStatus; // capDriverConnect()
		private: HWND mHWNDCapture;
		private: STVideoCaptureFormat mSTVideoCaptureFormat;
		private: int mCapturePeriodMilliseconds;
		private: STVideoCaptureClient * mpSTVideoCaptureClient;
		private: unsigned char * mBGRData;





		public: int GetCaptureWidth();

		public: int GetCaptureHeight();


		private: void Clear( );



		public: STVideoCapture();

		public: ~STVideoCapture();



		private: LRESULT CALLBACK  
				InstanceCaptureCallback
				( 
				HWND hwnd, 
				LPVIDEOHDR video_hdr 
				);


		public: int               
				Initialize
				( 
				HWND hwndParentWindow, // This must be non-null
				STVideoCaptureFormat videoCaptureFormat,
				int capturePeriodMilliseconds,
				STVideoCaptureClient * pSTVideoCaptureClient
				);


		public: HWND GetHWND( );


		public: void Terminate();



		};



	}
}



#endif






