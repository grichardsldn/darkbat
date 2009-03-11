#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "dkb.h"

class Receiver : public dkbClickReceiver
{
	public:
	void ReceiveClick( int clickref, int key );
	char name[1024];
	Receiver( char *aname);
};

Receiver::Receiver( char *aname)
{
	strcpy( name, aname);
}

void Receiver::ReceiveClick( int clickref, int key )
{
	printf("Receiver %s got clickref %d, key %d\n",
		name, clickref, key );
}



main()
{
	dkbShape cube;
	// lower face 
	cube.addLine( 0,0,0, 1,0,0, 0);
	cube.addLine( 1,0,0, 1,0,1, 0);
	cube.addLine( 1,0,1, 0,0,1, 0);
	cube.addLine( 0,0,1, 0,0,0, 0);

	// upper face
	cube.addLine( 0,1,0, 1,1,0, 0);
	cube.addLine( 1,1,0, 1,1,1, 0);
	cube.addLine( 1,1,1, 0,1,1, 0);
	cube.addLine( 0,1,1, 0,1,0, 0);

	// connect the two
	cube.addLine( 0,0,0, 0,1,0, 0 );
	cube.addLine( 1,0,0, 1,1,0, 0 );
	cube.addLine( 1,0,1, 1,1,1, 0 );
	cube.addLine( 0,0,1, 0,1,1, 0 );


	Receiver *rxa = new Receiver("A");
	Receiver *rxb = new Receiver("B");

	dkbShape tri;
	tri.addClickTriangle( 0,0,0, 1,1,1, 3,1,2, 0, rxa,100 );
	dkbShape tri2;
	tri2.addClickTriangle( 3,0,0, 1,1,1, 3,1,2, 0, rxb ,101 );

	dkbObj obj;
	dkbPos pos;
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	dkbAngle angle;
	obj.addShape( &cube ,angle, pos,  0 );	
	dkbBlock block;	
	obj.project( block, pos );

	usleep(300000);
	pos.x = 0;
	pos.y = 5;
	pos.z = 0;
	obj.addShape( &cube ,angle, pos,  1 );	
	usleep(300000);
	obj.addShape( &tri ,angle, pos,  2 );	
	obj.addShape( &tri2 ,angle, pos,  3 );	


	sleep(8);
	//obj.removeShape( 2 );	
	sleep(10);
}	
