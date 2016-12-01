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

class LiveReverse {
public:
  
  //!
  LiveReverse( int channels, int windowSize ) : channels( channels ), windowSize( windowSize ) {
    bufferSize = windowSize * 2; // We use two windows in one buffer
    bInternal = (float **)malloc( channels * sizeof(float*) );
    for ( int i = 0; i < channels; ++i ) {
      bInternal[i] = (float *)calloc( bufferSize, sizeof(float) );
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
  void process( float **const bIn, float **const bOut, int size ) {
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
  int windowSize;
  int bufferSize;
  float** bInternal;
};
