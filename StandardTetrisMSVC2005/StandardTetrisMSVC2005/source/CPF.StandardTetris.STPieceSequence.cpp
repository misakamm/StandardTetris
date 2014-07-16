// All contents of this file written by Colin Fahey ( http://colinfahey.com )
// 2007 June 4 ; Visit web site to check for any updates to this file.



#include "CPF.StandardTetris.STPieceSequence.h"



#include "CPF.StandardTetris.STRandom.h"
#include "CPF.StandardTetris.STPiece.h"



namespace CPF
{
	namespace StandardTetris
	{



		STPieceSequence::STPieceSequence ( )
		{
			(*this).mCachedRandomSeedUsedMostRecentlyToInitializeRandomGenerator = (__int64)0;
			(*this).mSTRandom.SetState((__int64)0);
			(*this).mAlternatingSZState = 0;
			(*this).mQueue.clear();
		}




		void STPieceSequence::PrivateAdvanceRandom ( )
		{
			int pieceShapeIndexCurrent = 0;
			if ( mCachedSelectedPieceShapeNext == STPiece::None )
				mCachedSelectedPieceShapeNext = (*this).mSTRandom.GetIntegerInRangeUsingCurrentState( 1, 7 );
			pieceShapeIndexCurrent = mCachedSelectedPieceShapeNext;
				//(*this).mSTRandom.GetIntegerInRangeUsingCurrentState( 1, 7 );

			(*this).mSTRandom.Advance( );

			int pieceShapeIndexNext = 0;
			pieceShapeIndexNext =
				(*this).mSTRandom.GetIntegerInRangeUsingCurrentState( 1, 7 );

			// Set current and next piece shapes
			(*this).mCachedSelectedPieceShapeCurrent = pieceShapeIndexCurrent;

			(*this).mCachedSelectedPieceShapeNext = pieceShapeIndexNext;
		}



		void STPieceSequence::PrivateAdvanceAlternatingSZ ( )
		{
			if (0 == (*this).mAlternatingSZState)
			{
				(*this).mAlternatingSZState = 1;
				(*this).mCachedSelectedPieceShapeCurrent = STPiece::S;
				(*this).mCachedSelectedPieceShapeNext = STPiece::Z;
			}
			else
			{
				(*this).mAlternatingSZState = 0;
				(*this).mCachedSelectedPieceShapeCurrent = STPiece::Z;
				(*this).mCachedSelectedPieceShapeNext = STPiece::S;
			}
		}






		int STPieceSequence::PrivateQueueIsEmpty ( )
		{
			if ((int)((*this).mQueue.size()) <= 0)
			{
				return (1);
			}
			return (0);
		}

		int STPieceSequence::PrivateQueuePeekItem ( )
		{
			if ((int)((*this).mQueue.size()) <= 0)
			{
				return (0);
			}
			return ((*this).mQueue[0]);
		}

		int STPieceSequence::PrivateQueueGetItem ( )
		{
			if ((int)((*this).mQueue.size()) <= 0)
			{
				return (0);
			}
			int value = ((*this).mQueue[0]);
			(*this).mQueue.erase( (*this).mQueue.begin() );
			return (value);
		}

		void STPieceSequence::PrivateAdvanceQueue ( )
		{
			(*this).mCachedSelectedPieceShapeCurrent = STPiece::None;
			(*this).mCachedSelectedPieceShapeNext = STPiece::None;

			if ((int)((*this).mQueue.size()) <= 0)
			{
				return;
			}

			// We definitely have one shape in the queue
			int pieceShapeIndexCurrent = 0;
			pieceShapeIndexCurrent = (*this).PrivateQueuePeekItem( );

			// Remove the shape from the queue
			(*this).mQueue.erase( (*this).mQueue.begin() );

			// There might be another shape in the queue.  We'll peek.
			int pieceShapeIndexNext = 0;
			pieceShapeIndexNext = (*this).PrivateQueuePeekItem( );


			// Set current and next piece shapes
			(*this).mCachedSelectedPieceShapeCurrent = pieceShapeIndexCurrent;

			(*this).mCachedSelectedPieceShapeNext = pieceShapeIndexNext;
		}






		// CLIENT OPERATIONS


		// STPiece.STPieceShape.None means piece information unavailable
		int STPieceSequence::ClientPeekSelectedPieceCurrent( )
		{
			return( (*this).mCachedSelectedPieceShapeCurrent );
		}


		// STPiece.STPieceShape.None means piece information unavailable
		int STPieceSequence::ClientPeekSelectedPieceNext( )
		{
			return( (*this).mCachedSelectedPieceShapeNext );
		}



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

		void STPieceSequence::ClientRequestSelectionUpdate ( )
		{
			//(*this).mCachedSelectedPieceShapeCurrent = STPiece::None;
			//(*this).mCachedSelectedPieceShapeNext = STPiece::None;

			switch((*this).mPieceSelectionSource)
			{
			case STPieceSelectionSourceRandom:
				{
					(*this).PrivateAdvanceRandom( );
				}
				break;

			case STPieceSelectionSourceAlternatingSAndZ:
				{
					(*this).PrivateAdvanceAlternatingSZ( );
				}
				break;

			case STPieceSelectionSourceQueue:
				{
					(*this).PrivateAdvanceQueue( );
				}
				break;

			default:
				{
				}
				break;
			}
		}








		// The following method selects the source to use for piece spawning.
		void STPieceSequence::ClientSelectPieceSelectionSource
			(
			int pieceSelectionSource
			)
		{
			(*this).mCachedSelectedPieceShapeCurrent = STPiece::None;
			(*this).mCachedSelectedPieceShapeNext = STPiece::None;

			switch (pieceSelectionSource)
			{
			case STPieceSelectionSourceRandom:
				{
					(*this).mPieceSelectionSource = pieceSelectionSource;
				}
				break;

			case STPieceSelectionSourceAlternatingSAndZ:
				{
					(*this).mPieceSelectionSource = pieceSelectionSource;
				}
				break;

			case STPieceSelectionSourceQueue:
				{
					(*this).mPieceSelectionSource = pieceSelectionSource;
				}
				break;

			default:
				{
					(*this).mPieceSelectionSource = STPieceSelectionSourceRandom;
				}
				break;
			}
		}



		int STPieceSequence::ClientCheckPieceSelectionSource ( )
		{
			return ((*this).mPieceSelectionSource);
		}




		// Call the following before each new game, using seed value that is
		// as random as possible.  Calling this method with the same seed value
		// should start the deterministic generators in the same initial state.

		void STPieceSequence::ClientRequestSelectionGeneratorReset
			(
			__int64 seedValue
			)
		{
			// Cache seed value
			(*this).mCachedRandomSeedUsedMostRecentlyToInitializeRandomGenerator = seedValue;
			(*this).mSTRandom.SetState(seedValue);

			// Clear cached selections
			(*this).mCachedSelectedPieceShapeCurrent = STPiece::None;
			(*this).mCachedSelectedPieceShapeNext = STPiece::None;

			// FIFO : Simply clear the FIFO.
			(*this).mQueue.clear( );

			// S/Z alternating sequence : set phase
			(*this).mAlternatingSZState = 0;
			if (0 != (seedValue % 2))
			{
				(*this).mAlternatingSZState = 1;
			}
		}






		// SERVER OPERATIONS

		// These methods are only used when the source of the piece sequence is
		// external to this application, such as video capture or network packets.
		// Such external sources will be regarded as remote piece servers, and this
		// class will be a proxy.
		// The following methods are used to accept piece sequence data from the
		// external sources.  Any received data will be cached, so that the client
		// can consume the data asynchronously at its own rate.

		void STPieceSequence::ServerQueueReset ( )
		{
			(*this).mQueue.clear( );
		}

		void STPieceSequence::ServerQueueSubmitPiece ( int shape )
		{
			int pieceIndex = 0;
			pieceIndex = shape;
			(*this).mQueue.push_back( pieceIndex );
		}




		// DIRECT METHODS

		// Such methods query/modify properties of this object directly and
		// without processing of any kind; suitable for writing state to 
		// persisting storage, or reading state from storage.




		int STPieceSequence::DirectGetPieceSelectionSource ( ) 
		{ 
			return ((*this).mPieceSelectionSource); 
		}

		void STPieceSequence::DirectSetPieceSelectionSource ( int pieceSelectionSource )
		{
			(*this).mPieceSelectionSource = pieceSelectionSource;
		}



		int STPieceSequence::DirectGetCurrentPieceShape ( ) 
		{ 
			return ((*this).mCachedSelectedPieceShapeCurrent); 
		}

		void STPieceSequence::DirectSetCurrentPieceShape ( int currentPieceShape )
		{
			(*this).mCachedSelectedPieceShapeCurrent = currentPieceShape;
		}



		int STPieceSequence::DirectGetNextPieceShape ( ) 
		{ 
			return ((*this).mCachedSelectedPieceShapeNext); 
		}

		void STPieceSequence::DirectSetNextPieceShape ( int nextPieceShape )
		{
			(*this).mCachedSelectedPieceShapeNext = nextPieceShape;
		}



		int STPieceSequence::DirectGetAlternateSZState ( ) 
		{ 
			return ((*this).mAlternatingSZState); 
		}

		void STPieceSequence::DirectSetAlternateSZState ( int alternateSZState )
		{
			(*this).mAlternatingSZState = alternateSZState;
		}




		int STPieceSequence::DirectGetTotalQueueElements ( ) 
		{ 
			return ((int)(*this).mQueue.size()); 
		}

		int STPieceSequence::DirectGetQueueElementByIndex ( int index )
		{
			if ((index >= 0) && (index < ((int)((*this).mQueue.size()))))
			{
				return ((*this).mQueue[index]);
			}
			return (0);
		}


		void STPieceSequence::DirectQueueClear ( )
		{
			(*this).mQueue.clear( );
		}

		void STPieceSequence::DirectAddQueueElement ( int value )
		{
			(*this).mQueue.push_back( value );
		}





		__int64 STPieceSequence::DirectGetSeedUsedMostRecentlyToInitializeRandomNumberGenerator ( )
		{
			return ((*this).mCachedRandomSeedUsedMostRecentlyToInitializeRandomGenerator);
		}

		void STPieceSequence::DirectSetSeedUsedMostRecentlyToInitializeRandomNumberGenerator ( __int64 cachedRandomSeedUsedMostRecentlyToInitializeRandomGenerator )
		{
			(*this).mCachedRandomSeedUsedMostRecentlyToInitializeRandomGenerator = cachedRandomSeedUsedMostRecentlyToInitializeRandomGenerator;
		}



		__int64 STPieceSequence::DirectGetCurrentRandomNumberGeneratorInternalStateValue ( )
		{
			return ((*this).mSTRandom.GetState( ));
		}

		void STPieceSequence::DirectSetCurrentRandomNumberGeneratorInternalStateValue ( __int64 randomNumberGeneratorInternalState )
		{
			(*this).mSTRandom.SetState( randomNumberGeneratorInternalState );
		}




	}
}


