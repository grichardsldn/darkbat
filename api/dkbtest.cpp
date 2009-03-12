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

class Vol : public dkbClickReceiver
{
	public:
	void ReceiveClick( int clickref, int key );
	dkbObj *dkb_obj;	
	dkbShape *shape;
	int vol;
	int ref;

	Vol( int def, int clickref);	
	private:
	void ReDraw();
};

Vol::Vol( int def, int a_ref )
{
	vol = def;
	ref = a_ref;
	dkb_obj = new dkbObj();
	shape = NULL;
	
	ReDraw();

	dkbBlock block;	
	dkbPos pos;
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	dkb_obj->project( block, pos );
}

void Vol::ReDraw()
{
	dkb_obj->removeShape( ref );
	if (shape != NULL )
	{
		delete shape;
	}

	int x = 1;
	int y = 3;
	int z = 1;
	shape = new dkbShape();		
	//shape->addLine( x, y,z, x , y, z + 2,0 );
	shape->addLine( x, y + 10, z, x , y + 10, z+2,0);
	shape->addLine( x, y + vol, z, x, y + vol, z+2,0 );
	shape->addClickTriangle(x, y, z,
				 x, y, z + 2,
				x, y - 1, x + 1,
				 0, this, ref );

	dkbPos pos;
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;

	dkbAngle angle;
	printf("Calling addShape from ReDraw\n");
	dkb_obj->addShape( shape, angle, pos, ref);
	printf("Called addShape from ReDraw\n");
}

void Vol::ReceiveClick( int clickref, int key )
{
	if( key == '1' )
	{
		if( vol > 0 ) vol --;
	}
	if( key =='2' )
	{
		if( vol < 10 ) vol ++;
	}
	printf("vol=%d\n", vol);
	ReDraw();
}

main()
{
	Vol vol(2, 300);
	sleep(30);
}

void main2()
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
	sleep(30);
}	
