#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <pthread.h>
#include "dkb.h"
#include "UDPSocket.h"
#include "../common/dkb_const.h"

#define ELEMENT_NONE (100)
#define ELEMENT_LINE (1)
#define ELEMENT_POINT ( 2)
#define ELEMENT_FLATRECT (3)

char *writeInt( char *buf, int val)
{
	signed short sval = (signed short) val;

	*((signed short*)buf) = sval;	

	return buf + 2;
}
	
char *dkbPos::write( char *buf )
{
	buf = writeInt( buf, x );
	buf = writeInt( buf, y );
	buf = writeInt( buf, z );
}


class dkbShapeEntry
{
	public:
	dkbShape *shape;
	bool allocated;
	dkbPos position;
	int ref;

	dkbShapeEntry();	
};

class dkbElement
{
	public:
	int x1; 
	int y1;
	int z1;
	int x2;
	int y2;
	int z2;
	int col;
	int type;

	dkbElement* next;

	dkbElement();
	char *write( char *buf);
};


char *dkbElement::write( char *buf )
{
	switch( type )
	{
		case ELEMENT_LINE:
		{
			buf = writeInt( buf, DKB_ELEMENT_LINE );

			// x1, y1, z1, x2, y2, z2, col 
			buf = writeInt( buf, x1 );
			buf = writeInt( buf, y1 );
			buf = writeInt( buf, z1 );
			buf = writeInt( buf, x2 );
			buf = writeInt( buf, y2 );
			buf = writeInt( buf, z2 );
			buf = writeInt( buf, col );
		} 
	}
}

dkbElement::dkbElement()
{
	type = ELEMENT_NONE;
	next = NULL;
}

void dkbShape::addElement( dkbElement *element )
{
	element->next = head;	
	head = element;
}

void dkbShape::addLine( int an_x1, int a_y1, int a_z1,
			int an_x2, int a_y2, int a_z2,
			int a_col)
{
	dkbElement *e = new dkbElement();
	e->type = ELEMENT_LINE;
	e->x1 = an_x1;	
	e->y1 = a_y1;
	e->z1 = a_z1;
	e->x2 = an_x2;
	e->y2 = a_y2;
	e->z2 = a_z2;
	e->col = a_col;
	
	addElement( e );
}

void dkbShape::addPoint( int ax_x1, int a_y1, int a_1, int a_col)
{
}

dkbShapeEntry::dkbShapeEntry()
{
	allocated = false;
}

dkbObj::dkbObj()
{
	ref = random() & 0x7fff;

	for (int i = 0 ; i < 10 ; i++)
	{
		shapes[i] = new dkbShapeEntry();
	}
}

void dkbObj::addShape( dkbShape *shape, dkbAngle angle, dkbPos trans,
	int ref)
{
	int found = -1;
	for (int i = 0 ; i < 10 ; i++)
	{
		if( shapes[i]->allocated == false)	
		{
			found = i;
			break;
		}
	}
	if( found == -1 ) 
	{
		printf("dkbObj: too many shapes\n");
		return;
	}

	shapes[found]->shape = shape;
	shapes[found]->position = trans;
	shapes[found]->ref = ref;
	shapes[found]->allocated = true;
}

bool dkbObj::connect( dkbBlock block )
{
	return false;
}

void dkbObj::removeShape( int ref )
{
}

void dkbObj::project( dkbBlock block, dkbPos pos )
{
	socket = new UDPSocket();
	socket->SetTarget(0x7f000001, 1234);

	pthread_create( &run_thread, NULL, start, this);

}

void dkbObj::moveRel( dkbPos offset )
{
}	

dkbShape::dkbShape()
{
	head = NULL;
}	

void *dkbObj::start( void * a_this)
{
	dkbObj *obj = (dkbObj*) a_this;
	obj->Start();

	return NULL;
}

void dkbObj::Xmit()
{
	char buffer[1500];

	char *bp = &buffer[0];

	// write the magic number and the reference number
	bp = writeInt( bp, DKB_1_MAGIC );

	bp = writeInt( bp, DKB_CONNECTION_REF );
	bp = writeInt( bp, ref );

	printf("project: shapes are:\n");
	for( int i = 0 ; i <10 ; i++)
	{
		if (shapes[i]->allocated)
		{
			// write the ref for this shape
			bp = writeInt( bp, DKB_REF );
			bp = writeInt( bp, shapes[i]->ref );

			// write the position for this shape
			bp = writeInt( bp, DKB_POS );
			bp = shapes[i]->position.write( bp );
		
			dkbElement *cur = shapes[i]->shape->head;
			while ( cur != NULL )
			{		
				bp = cur->write(bp );

				cur = cur->next;
			}
			
		}
	}
	bp = writeInt( bp, DKB_END );

	socket->Send( &buffer[0], (int)(bp - &buffer[0]) );
}

void dkbObj::Start()
{
	while ( 1)
	{
		Xmit();
		sleep(1);
	}
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

	dkbObj obj;
	dkbPos pos;
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	dkbAngle angle;
	obj.addShape( &cube ,angle, pos,  0 );	
	pos.x = 0;
	pos.y = 5;
	pos.z = 0;
	obj.addShape( &cube ,angle, pos,  1 );	

	dkbBlock block;	
	obj.project( block, pos );
	
	sleep(10);
}	
