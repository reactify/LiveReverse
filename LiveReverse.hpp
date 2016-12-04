//
//  LiveReverse.hpp
//  LiveReverse
//
//  Created by Ragnar Hrafnkelsson on 01/12/2016.
//  Copyright © 2016 Reactify. All rights reserved.
//

#pragma once

#include <stdlib.h>
#include <vector>

template <typename T>
class LiveReverse {
public:
  
  //!
  LiveReverse( int channels, int windowSize ) : channels( channels ) {
    bufferSize = windowSize * 2; // We use two windows in one buffer
    bInternal = (T **)malloc( channels * sizeof(T*) );
    for ( int i = 0; i < channels; ++i ) {
      bInternal[i] = (T *)calloc( bufferSize, sizeof(T) );
    }
    writePos = windowSize; // Start writing one window ahead
  }
  
  //!
  ~LiveReverse() {
    for ( int i = 0; i < channels; ++i ) {
      free( bInternal[i] );
    }
    free( bInternal );
  }
  
  //!
  inline void process( T **const bIn, T **const bOut, int size ) {
    for ( int j = 0; j < size; ++j ) {
      // Read position is one window behind write position
      // and we read from back to front ( reverse )
      
      int readPos = ( bufferSize - writePos ) - 1;
      
      // Fill buffers
      for ( int i = 0; i < channels; ++i ) {
        bInternal[i][writePos] = bIn[i][j];
        bOut[i][j] = bInternal[i][ readPos ];
      }
      
      // Advance write position
      writePos = ( writePos + 1 ) % bufferSize;
    }
  }
  
private:
  int channels;
  int writePos;
  int bufferSize;
  T** bInternal;
};
