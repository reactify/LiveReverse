//
//  main.cpp
//  LiveReverse
//
//  Created by Ragnar Hrafnkelsson on 01/12/2016.
//  Copyright © 2016 reactify. All rights reserved.
//

#include "LiveReverse.hpp"

int main(int argc, const char * argv[]) {
  
  int numChannels = 2;
  int windowSize = 512;
  int bufferSize = windowSize;
  
  LiveReverse r( numChannels, windowSize );
  
  float **bIn  = (float **)alloca( numChannels * sizeof(float*) );
  float **bOut = (float **)alloca( numChannels * sizeof(float*) );
  for ( int i = 0; i < numChannels; ++i ) {
    bIn[i] = bOut[i] = (float *)alloca( bufferSize * sizeof(float) );
  }
  
  // Fill buffer with incremented value
  for (int i = 0; i < bufferSize; ++i ) {
    bIn[0][i] = (float)i;
  }
  
  // Process and verify output
  r.process( bIn, bOut, bufferSize );
  for (int i = 0; i < bufferSize; ++i ) {
    printf( "Out: %i = %f\n", i, bOut[0][i] );
  }
  r.process( bIn, bOut, bufferSize );
  for (int i = 0; i < bufferSize; ++i ) {
    printf( "Out: %i = %f\n", i, bOut[0][i] );
  }
  
  return 0;
}
