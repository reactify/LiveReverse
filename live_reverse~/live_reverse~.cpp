//
//  live_reverse~
//
//  Created by Ragnar Hrafnkelsson on 04/12/2016.
//  Copyright © 2016 Reactify. All rights reserved.
//

#include "ext.h"
#include "z_dsp.h"
#include "../../LiveReverse.hpp"

void *live_reverse_class;

typedef struct _live_reverse {
	t_pxobject	x_obj;
	LiveReverse<double>* external;
} t_live_reverse;

void *live_reverse_new( double val );
void live_reverse_free( t_live_reverse *x );
void live_reverse_perform64( t_live_reverse *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam );
void live_reverse_dsp64( t_live_reverse *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags );
void live_reverse_assist( t_live_reverse *x, void *b, long m, long a, char *s );

void ext_main(void *r)
{
	t_class *c;

	c = class_new("live_reverse~", (method)live_reverse_new, (method)live_reverse_free, (short)sizeof(t_live_reverse), 0L, A_DEFFLOAT, 0);

	class_addmethod( c, (method)live_reverse_dsp64, "dsp64", A_CANT, 0 );
	class_addmethod( c, (method)live_reverse_assist,"assist",A_CANT,0 );

	class_dspinit( c );	// must call this function for MSP object classes
	class_register( CLASS_BOX, c );
	live_reverse_class = c;
}

void live_reverse_assist( t_live_reverse *x, void *b, long m, long a, char *s )
{
	if ( m == ASSIST_INLET ) {
		switch ( a ) {
		case 0:
			strcpy(s,"Specify cpu usage % here");
			break;
		}
	}
}

void *live_reverse_new( double val )
{
	t_live_reverse *x = (t_live_reverse *)object_alloc((t_class*)live_reverse_class);
	
  // intin( x, 1 ); // TODO: Setter for buffer length
	
	dsp_setup( (t_pxobject *)x, 2 );
	outlet_new( x, "signal" );
	outlet_new( x, "signal" );
	
	x->external = new LiveReverse<double>( 2, 22050 );
	
	return x;
}

void live_reverse_free( t_live_reverse *x )
{
	delete x->external;
	dsp_free( (t_pxobject *)x );
}

void live_reverse_perform64( t_live_reverse *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam )
{
  assert( numins  >= 2 );
	assert( numouts >= 2 );
  
  long numBytes = sampleframes * sizeof(double);
  
  double **bTemp = (double **)alloca( numins * sizeof(double*) );
  for ( int i = 0; i < numins; ++i ) {
    bTemp[i] = (double *)alloca( numBytes );
    memcpy( bTemp[i], ins[i], numBytes );
  }
  
	x->external->process( bTemp, bTemp, sampleframes );
  
  for ( int i = 0; i < numouts; ++i ) {
    memcpy( outs[i], bTemp[i], numBytes );
  }
}

void live_reverse_dsp64( t_live_reverse *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags )
{
	object_method( dsp64, gensym("dsp_add64"), x, live_reverse_perform64, 0, NULL );
}
