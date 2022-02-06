#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <pthread.h>

#include "line.h"
#include "window.h"
#include "model.h"
#include "view.h"
#include "UDPSocket.h"
#include "../common/dkb_const.h"
#include "source.h"

int readInt( char *bp, char **bpp )
{
	signed short val = *(signed short*)(bp);
	bp +=2;

	*bpp = bp;

	return (int) val;
}

	Model model;
	View *viewptr;

void recv_pkt( char *bp, int from_id, unsigned int address, unsigned short port)
{
	int magic;
	
	int header = readInt( bp, &bp );
	
	if( header != DKB_1_MAGIC )
	{
		if( header == DKB_VIEWPOINT_1_MAGIC )
		{
			printf("DKB VIEW\n");
			int x = readInt( bp, &bp );
			int y = readInt( bp, &bp );
			printf("GDR: x=%d\n", x );		
			viewptr->SetScreenOffset( x, y );
			return;
		}
		else
		{	
			printf("Bad magic number\n");
			exit(10);
			return;
		}
	}

	if( readInt( bp, &bp ) != DKB_CONNECTION_REF )
	{
		printf("Not connection ref\n");
		exit(2);
		return;
	}
	int connection_ref = readInt( bp, &bp );

	Source::Record( port, address, connection_ref );

	int type;
	int ref;
	int pos_x = 0;
	int pos_y = 0;
	int pos_z = 0;
	int object_pos_x = 0;
	int object_pos_y = 0;
	int object_pos_z = 0;

	do
	{
		type = readInt( bp, &bp);
printf("type=%i\n", type);	
		switch(type )
		{
		case DKB_REF:
			ref = readInt( bp, &bp);
			// clear this shape
			model.Remove( connection_ref + ref );
		break;
	
		case DKB_POS:
			object_pos_x = readInt( bp, &bp);
			object_pos_y = readInt( bp, &bp);
			object_pos_z = readInt( bp, &bp);		
		break;

		case DKB_RELPOS:
			pos_x = readInt( bp, &bp) + object_pos_x;
			pos_y = readInt( bp, &bp) + object_pos_y;
			pos_z = readInt( bp, &bp) + object_pos_z;		
		break;

		case DKB_ELEMENT_CLICKTRI:
		{
			int x1, y1, z1;
			int x2, y2, z2;
			int x3, y3, z3;
			int col; // not written into model
			int clickref; // ignore at the moment
		
			x1 = readInt( bp, &bp);	
			y1 = readInt( bp, &bp);	
			z1 = readInt( bp, &bp);	
			x2 = readInt( bp, &bp);	
			y2 = readInt( bp, &bp);	
			z2 = readInt( bp, &bp);	
			x3 = readInt( bp, &bp);	
			y3 = readInt( bp, &bp);	
			z3 = readInt( bp, &bp);	
			col = readInt( bp, &bp );
			clickref= readInt( bp, &bp);

			printf("GDR: AddTri %d %d %d %d %d %d %d %d %d\n",
				x1,y1,z1,x2,y2,z2,x3,y3,z3);	
			model.AddClickTri( (float)(x1 + pos_x),
				(float)( y1 + pos_y),
				(float)( z1 + pos_z),
				(float)( x2 + pos_x),
				(float)( y2 + pos_y),
				(float)( z2 + pos_z),
				(float)( x3 + pos_x),
				(float)( y3 + pos_y),
				(float)( z3 + pos_z),
				ref + connection_ref, connection_ref, from_id,
				50,
				clickref );
		}	
		break;

		case DKB_ELEMENT_LINE:
		{
			int x1, y1, z1;
			int x2, y2, z2;
			int col; // not written into model
		
			x1 = readInt( bp, &bp);	
			y1 = readInt( bp, &bp);	
			z1 = readInt( bp, &bp);	
			x2 = readInt( bp, &bp);	
			y2 = readInt( bp, &bp);	
			z2 = readInt( bp, &bp);	
			col = readInt( bp, &bp );

			printf("GDR: AddLine %d %d %d %d %d %d\n",
				x1,y1,z1,x2,y2,z2);	
			model.AddLine( (float)(x1 + pos_x),
				(float)( y1 + pos_y),
				(float)( z1 + pos_z),
				(float)( x2 + pos_x),
				(float)( y2 + pos_y),
				(float)( z2 + pos_z),
				ref +connection_ref, connection_ref,
				100 );
		}
		break;

		case DKB_END:
		{
			printf("End of packet\n");
			break;
		}	
		default:
			printf("unknown type\n");
			exit(4);
		}

			
	} while (type != DKB_END );
}

UDPSocket sock;

void *recv_thread( void *arg )
{
	sock.Bind( 1234, 0 );

	char buf[1500];
	for( ;; )
	{
		unsigned int address;
		unsigned short port;
		sock.ReceiveFrom( &buf[0], 1500, &address, &port);

		recv_pkt( &buf[0], 0, address, port);	
	}
	
}
void arrow(float x, float y, float z, Model *model, float lifespan ) {
	model->AddLine( 8 + x,y,z, x,y,z, 1,0,lifespan);
	model->AddLine( x,y,z, x+2,y,z - 1, 1,0,lifespan);
	model->AddLine( x,y,z, x+2,y,z + 1, 1,0,lifespan);
}
	

int main(int argc, char **argv)
{
	View view( &model);
	viewptr = &view;

	Point a;

	// add some objects
	for( float x = 0; x < 5 ; x++) {
		arrow(  55 - (x * 55), -8, 0, &model, 300 + x * 2);
	}	
 
	int oa = 0;
	int ob = 0;
	// add random dots on the floor
	for( int i = 0 ; i < 160 ; i++)
	{	
		int a = (random() % 400) - 200;		
		int b = (random() % 400) - 200;
		int c = (random() % 3);
		model.AddPoint( (float)a, -5.0 + c, (float)b, 1,0, 30000);
		// model.AddLine( (float)a, -5.0, (float)b, (float)oa, -5.0, (float)ob, 1 );
		oa = a; 
		ob = b;
	}

	pthread_t thread;

	pthread_create( &thread, NULL, recv_thread, NULL );

	view.Run( 400,400, argc, argv);
}

	
	
