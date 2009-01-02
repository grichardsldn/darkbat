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


int readInt( char *bp, char **bpp )
{
	signed short val = *(signed short*)(bp);
	bp +=2;

	*bpp = bp;

	return (int) val;
}

	Model model;

void recv_pkt( char *bp )
{
	int magic;

	if( readInt( bp, &bp) != DKB_1_MAGIC )
	{
		printf("Bad magic number\n");
		exit(10);
		return;
	}

	if( readInt( bp, &bp ) != DKB_CONNECTION_REF )
	{
		printf("Not connection ref\n");
		exit(2);
		return;
	}
	int connection_ref = readInt( bp, &bp );

	int type;
	int ref;
	int pos_x = 0;
	int pos_y = 0;
	int pos_z = 0;

	do
	{
		type = readInt( bp, &bp);
	
		switch(type )
		{
		case DKB_REF:
			ref = readInt( bp, &bp);
			// clear this shape
			model.Remove( connection_ref + ref );
		break;
	
		case DKB_POS:
			pos_x = readInt( bp, &bp);
			pos_y = readInt( bp, &bp);
			pos_z = readInt( bp, &bp);		
		break;

		case DKB_ELEMENT_LINE:
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
				ref +connection_ref,
				50 );
	
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

void *recv_thread( void *arg )
{
	UDPSocket sock;

	sock.Bind( 1234, 0 );

	char buf[1500];
	for( ;; )
	{
		sock.ReceiveFrom( &buf[0], 1500);

		recv_pkt( &buf[0]);	
	}
	
}


int main(int argc, char **argv)
{
	View view( &model);

	Point a;

	//model.AddPoint( 5.0, 0.0, 1.0, 0);
	//model.AddPoint( 6.0, 0.0, 1.0, 0);

	model.AddLine( 5.0, 0.0, 1.0, 6.0, 0.0, 1.0, 1, 200);

	model.AddPoint( 6.0, 0.0, 2.0, 0, 200);
	model.AddPoint( 7.0, 0.0, 2.0, 0, 200);
	model.AddPoint( 7.0, 1.0, 2.0, 0, 200);
	model.AddPoint( 5.0, 0.0, -0.5, 0, 200);
	model.AddPoint( 7.0, 1.0, 2.2, 0, 200);
	model.AddPoint( 7.0, 1.0, 2.4, 0, 200);
	model.AddPoint( 7.0, 1.0, 2.6, 0, 200);

	int oa = 0;
	int ob = 0;
	// add random dots on the floor
	for( int i = 0 ; i < 60 ; i++)
	{	
		int a = (random() % 100) - 50;		
		int b = (random() % 100) - 50;
		int c = (random() % 3);
		model.AddPoint( (float)a, -5.0 + c, (float)b, 1, 300);
		//model.AddLine( (float)a, -5.0, (float)b, (float)oa, -5.0, (float)ob, 1 );
		oa = a; 
		ob = b;
	}

	pthread_t thread;

	pthread_create( &thread, NULL, recv_thread, NULL );

	view.Run( 400,400, argc, argv);
}

	
	
