#include <stdio.h>

#include "window.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <linux/videodev.h>
#include <linux/videodev2.h>
#include <unistd.h>
#include <string.h>

int dev;

class myWindow : public GWindow
{
	public:	
		
	void Draw( );
	void Key( int key );
};

void myWindow::Draw()
{
	ClearWindow();
	SetColour("linen");
	/*
	DrawText( 50,50,"Hello World");
	int i;
	for (i = 0 ; i < 500; i += 10 )
	{	
	DrawLine( 0,i, 500, i );
	//DrawLine( 0,i+1, 500, i+1 );
	}*/
		int ret;	
		unsigned char buffer[1000000];
		memset( &buffer[0], 0xcc, 100000);
		ret = read( dev, &buffer[0], 1000000);
		printf("read return #%d\n", ret );
	int x = 0;
	int y = 0;
	for( int i = 0; i < 230000 ; i+=3 )
	{
		if( buffer[i] > 50 )
		{
			DrawLine( x,y,x,y );
		}
		x ++;
		// was 319
		if( x > 351 )
		{
			x =0;
			y++;
		}
	}
	
}


void myWindow::Key( int key )
{
	if (key == 'q')
	{
		Quit();
	}
}

int main( int argc, char **argv )
{
	myWindow mywindow;

	dev = open("/dev/video0", O_RDONLY );

	mywindow.Run( 200, 100, argc, argv );
}




		
