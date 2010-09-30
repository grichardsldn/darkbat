#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <linux/videodev.h>
#include <linux/videodev2.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "UDPSocket.h"


#define VIDEO_SIZE_X ( 351 )
#define VIDEO_SIZE_Y (200 )

#define NUM_CHANGEPOINTS ( 5000 )

#define CHANGE_THRESHOLD (5)
#define NUM_CHANGES_THRESHOLD ( 2 )

class ChangePoint
{
	public:
	int posx;
	int posy;
	int last;
	int threshold;
	unsigned char *buffer;
	ChangePoint( int x, int y, int threshold, unsigned char *buffer );
	int Changed();

	private:
	int ReadRed();
};

int ChangePoint::ReadRed()
{
	int offset = posx + (posy * VIDEO_SIZE_X );
	offset += 145;	
	return buffer[ (offset * 3 ) ];
}

int ChangePoint::Changed()
{
	int changed = 0;
	int val = ReadRed();
	int diff = val - last;
	//if( abs( diff ) > threshold )
	//{
//		changed = 1;
//	}
	if( val > 50 ) changed=1;
	last = val;
	return changed;
}

ChangePoint::ChangePoint( int x, int y, int a_threshold, unsigned char *a_buffer )
{
	posx = x;
	posy = y;
	threshold = a_threshold;
	buffer = a_buffer;
	last = ReadRed();
}

void SendView( UDPSocket*sock, int x, int y )
{
	signed short int buf[3];
	buf[0] = 65; // DKB_VIEWPOINT_1_MAGIC
	buf[1] = x;
	buf[2] = y;
	printf("GDR: SendView: x=%d\n", x );
	sock->Send( (char *)&buf[0], 6 );
}

main()
{
  int dev;

	UDPSocket *sock = new UDPSocket();
	sock->SetTarget( 0x7f000001, 1234 );
	dev = open("/dev/video0", O_RDONLY );

	assert( dev != -1);
	printf("Openend device\n");

 	int ret;
	ChangePoint* points[NUM_CHANGEPOINTS];

	unsigned char buffer[1000000];
	memset( &buffer[0], 0xcc, 100000);

	for( int i = 0 ; i < NUM_CHANGEPOINTS ; i++)
	{
		points[i] = new ChangePoint( random() % VIDEO_SIZE_X,
			random() % VIDEO_SIZE_Y, 
			CHANGE_THRESHOLD, 
			&buffer[0] );
	}

	for (;;)
	{
		ret = read( dev, &buffer[0], 1000000);
		//printf("read %d bytes\n", ret );	
		int x_total = 0;
		int num_x_changes = 0;
		int y_total = 0;
		for( int i = 0 ; i < NUM_CHANGEPOINTS ; i++)
		{
			if( points[i]->Changed()  > 0 )
			{
				x_total += points[i]->posx;
				y_total += points[i]->posy;
				num_x_changes ++;
			//printf("Trig at %d\n", points[i]->posx);
			}	
		}
		if( num_x_changes > NUM_CHANGES_THRESHOLD )
		{
			int xavg = x_total / num_x_changes;
			int yavg = y_total / num_x_changes;
			printf( "Changes, num=%d, xavg=%d yavg=%d\n", num_x_changes
				, xavg, yavg );

			int midx = xavg - (VIDEO_SIZE_X / 2);
			int midy = yavg - (VIDEO_SIZE_Y / 2);
			SendView( sock, midx, midy * -1 );	
		}
	}
}
