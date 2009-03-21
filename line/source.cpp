#include <stdio.h>
#include "source.h"
#include <assert.h>

Source Source::sources[NUM_SOURCES];

Source::Source()
{
	ref = -1;
}


Source *Source::findRef( int a_ref )
{
	printf("GDR: findRef: #%d\n", a_ref );
	for ( int i = 0 ; i < NUM_SOURCES ; i++)
	{
		if( sources[i].ref == a_ref )
		{
			return &sources[i];
		}
	}
	return NULL;
}
void Source::Tell()
{
	printf("Source: NUM_SOURCES=%d\n", NUM_SOURCES);
	
	for( int i = 0 ; i < NUM_SOURCES ; i++)
	{
		if( sources[i].ref != -1 )
		{
			printf("  ref=#%d, address=&%08x port=&%04x\n",
				sources[i].ref, sources[i].address,
				sources[i].port );
		}
	}
}

Source *Source::Record( unsigned short port, unsigned int address, int ref )
{
	printf("GDR: Record: port=#%d, address=&%08x, ref=#%d\n", 
		port, address, ref );
	int num_free = 0;
	int last_free = 0;
	for( int i = 0 ; i <NUM_SOURCES ; i++)
	{
		if( sources[i].ref == -1 )
		{
			last_free = i;
			num_free ++;
		}
		else if(( sources[i].port == port) && (sources[i].address == address ))
		{
			// found it
			return &sources[i];
		}
	}
	assert( num_free > 0 );
	// if got here, it is a new entry.
	if( num_free > 0)
	{
		sources[last_free].ref = ref;	
		sources[last_free].port = port;	
		sources[last_free].address = address;	
	
		return &sources[last_free];
	}	

	return NULL;	
}

#if 0
main()
{
	Source::Tell();
	Source::Record( 1, 100, 300 );
	Source::Tell();
	Source *s = Source::findRef(300);
	assert( s != NULL );
	printf("address of ref 300 is %d\n", s->address );
	s = Source::findRef(301);
	assert( s == NULL);
	Source::Record( 1, 101, 300 );
	s = Source::findRef(300);
	assert( s != NULL );
	printf("address of ref 300 is %d\n", s->address );
	Source::Record( 1, 102, 301 );
	s = Source::findRef(301);
	printf("address of ref 301 is %d\n", s->address );
}
#endif
