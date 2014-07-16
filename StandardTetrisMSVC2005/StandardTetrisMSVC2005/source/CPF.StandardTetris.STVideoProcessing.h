// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STVideoProcessingHeader
#define STVideoProcessingHeader



#include "CPF.StandardTetris.STVideoCapture.h"


#include <windows.h>



namespace CPF
{
	namespace StandardTetris
	{



		class STVideoProcessing : public STVideoCaptureClient
		{


		private: STVideoCapture * mSTVideoCapture;

		private: unsigned char * mTextureDataBGR256x256;
		public: int mTextureOpenGLHandleBGR256x256;

		private: unsigned char * mVideoLUT256;

		private: int mProcessXMin;
		private: int mProcessYMin;
		private: int mProcessXMax;
		private: int mProcessYMax;
		private: int mCurrentClassification;





		private: void Clear ( );


		public: STVideoProcessing ( );


		private: void CopyDataFromCaptureDataToLocalBuffer
					 (
					 unsigned char * bgrData,
					 int imageWidth,
					 int imageHeight
					 );


		private: void BinarizeRegion ( );

		private: void ClassifyRegion ( );

		public:	void
				SetRegion
				(
				int xMin,
				int yMin,
				int xMax,
				int yMax
				);


		public:	void
				GetRegion
				(
				int & xMin,
				int & yMin,
				int & xMax,
				int & yMax
				);


		public: void ClearRegionStatus ( );



		public: int GetRegionClassification ( );



		public: virtual void VideoCaptureClientCallback
					(
					unsigned char * bgrData,
					int imageWidth,
					int imageHeight
					);


		public: int
				Initialize
				(
				HWND hwndParentWindow
				);



		public: void Terminate ( );



		};


	}
}




#endif

