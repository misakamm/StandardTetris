// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#ifndef STPieceSequenceHeader
#define STPieceSequenceHeader



#include "CPF.StandardTetris.STRandom.h"
#include "CPF.StandardTetris.STPiece.h"



// Disable unimportant "symbol too long" error for debug builds of STL string
#pragma warning( disable : 4786 )

#include <string>
#include <vector>
using namespace std;




namespace CPF
{
	namespace StandardTetris
	{



		class STPieceSequence
		{
		public: enum STPieceSelectionSource
				{
					STPieceSelectionSourceRandom = 0,
					STPieceSelectionSourceAlternatingSAndZ = 1,
					STPieceSelectionSourceQueue = 2
            // Queue can accept piece shapes from video capture, network, etc.
            // Queue can accept piece shapes from a file.
            // Queue can accept piece shapes selected by an adversarial
            //   "worst-piece-possible" AI (like "birdtris") to
            //   maximize the challenge to the player/AI.
				};


		private: int mPieceSelectionSource;

		private: int mCachedSelectedPieceShapeCurrent;
		private: int mCachedSelectedPieceShapeNext;


        // state information for the different piece-sequence sources

		private: __int64 mCachedRandomSeedUsedMostRecentlyToInitializeRandomGenerator;
		private: STRandom mSTRandom;
		private: int mAlternatingSZState;
		private: vector<int> mQueue;








		public: STPieceSequence ( );



		private: void PrivateAdvanceRandom ( );

		private: void PrivateAdvanceAlternatingSZ ( );

		private: int PrivateQueueIsEmpty ( );

		private: int PrivateQueuePeekItem ( );

		private: int PrivateQueueGetItem ( );

		private: void PrivateAdvanceQueue ( );






        // CLIENT OPERATIONS


        // STPiece.STPieceShape.None (0) means piece information unavailable
		public: int  ClientPeekSelectedPieceCurrent( );

		// STPiece.STPieceShape.None (0) means piece information unavailable
		public: int ClientPeekSelectedPieceNext( );



        // The following method is called by the client (i.e., game engine) to
        // request an update of the current and next pieces.  It is possible 
        // that the next piece and possibly the current piece become UNAVAILABLE.
        // It is the client's responsibility to check for pieces to become 
        // available in the future.
        // Sources of piece sequences that are external to this application, such as
        // video capture and network packets, are asynchronous and essentially 
        // non-deterministic (i.e., algorithm and parameters are essentially unknown).
        // The client must wait for new pieces to become available.
        // In any case, the following method should be called whenever the client
        // has consumed the current piece.

		public: void ClientRequestSelectionUpdate ( );


        // The following method selects the source to use for piece spawning.
		public: void ClientSelectPieceSelectionSource
          (
          int pieceSelectionSource
          );


		public: int ClientCheckPieceSelectionSource ( );




        // Call the following before each new game, using seed value that is
        // as random as possible.  Calling this method with the same seed value
        // should start the deterministic generators in the same initial state.

		public: void ClientRequestSelectionGeneratorReset
          (
            __int64 seedValue
          );






        // SERVER OPERATIONS

        // These methods are only used when the source of the piece sequence is
        // external to this application, such as video capture or network packets.
        // Such external sources will be regarded as remote piece servers, and this
        // class will be a proxy.
        // The following methods are used to accept piece sequence data from the
        // external sources.  Any received data will be cached, so that the client
        // can consume the data asynchronously at its own rate.

		public: void ServerQueueReset ( );

		public: void ServerQueueSubmitPiece ( int shape );




        // DIRECT METHODS

        // Such methods query/modify properties of this object directly and
        // without processing of any kind; suitable for writing state to 
        // persisting storage, or reading state from storage.


		public: int DirectGetPieceSelectionSource ( ) ;

		public: void DirectSetPieceSelectionSource ( int pieceSelectionSource );



		public: int DirectGetCurrentPieceShape ( ) ;

		public: void DirectSetCurrentPieceShape ( int currentPieceShape );



		public: int DirectGetNextPieceShape ( );

		public: void DirectSetNextPieceShape ( int nextPieceShape );



		public: int DirectGetAlternateSZState ( ); 

		public: void DirectSetAlternateSZState ( int alternateSZState );



		public: int DirectGetTotalQueueElements ( ); 

		public: int DirectGetQueueElementByIndex ( int index );



		public: void DirectQueueClear ( );

		public: void DirectAddQueueElement ( int value );



		public: __int64 DirectGetSeedUsedMostRecentlyToInitializeRandomNumberGenerator ( );

		public: void DirectSetSeedUsedMostRecentlyToInitializeRandomNumberGenerator ( __int64 cachedRandomSeedUsedMostRecentlyToInitializeRandomGenerator );



		public: __int64 DirectGetCurrentRandomNumberGeneratorInternalStateValue ( );

		public: void DirectSetCurrentRandomNumberGeneratorInternalStateValue ( __int64 randomNumberGeneratorInternalState );


				 
		};




	}
}



#endif






