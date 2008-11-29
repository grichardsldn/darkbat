#include <stdio.h>

#include "window.h"

class myWindow : public GWindow
{
	public:	
		
	void Draw( );
	void Key( int key );
};

void myWindow::Draw()
{
	SetColour("linen");
	DrawText( 50,50,"Hello World");
	int i;
	for (i = 0 ; i < 500; i += 10 )
	{	
	DrawLine( 0,i, 500, i );
	//DrawLine( 0,i+1, 500, i+1 );
	}
		
}

void myWindow::Key( int key )
{
	if (key == 'q')
	{
		Quit();
	}
	else
	{
		ClearWindow();
	}
}

int main( int argc, char **argv )
{
	myWindow mywindow;

	mywindow.Run( 200, 100, argc, argv );
}
