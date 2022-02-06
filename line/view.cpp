
#include <stdio.h>
#include "window.h"
#include "line.h"
#include "model.h"
#include "view.h"
#include <string.h>

#define DISPLAY_X ( 400 )
#define DISPLAY_Y ( 400)
#define MID_DISPLAY_X ( 200 )
#define MID_DISPLAY_Y ( 200 )
// think the below means the size from the centre to the edge
#define SCREEN_X (2.0)
#define SCREEN_Y (2.0)

// #define STEREOSCOPIC (1)

View::View( Model *a_model )
{
	screen_offset_x = 0;
	screen_offset_y = 0;
	viewpoint.x = -10.0;
	viewpoint.y = 1.0;
	viewpoint.z = 2.0;

	screenpoint.x = 2.0;
	screenpoint.y = 1.0;
	screenpoint.z = 2.0;

#ifdef STEREOSCOPIC
	view = 1;
#else
	view = 0;
#endif

	SetWhiteColour();

	model = a_model;
}

void View::SetScreenOffset( int x, int y)
{
	printf("View::SetScreenOffset: x=%d\n", x);
	screen_offset_x = x;
	screen_offset_y = y;
}

void View::SetWhiteColour()
{
	switch (view)
	{
		case 0:
			strcpy( white_colour, "white");
			strcpy( highlight_colour, "green");
		break;
		case 1: 
			strcpy( white_colour, "magenta");
			strcpy( highlight_colour, "green");
		break;
		case 2: 
			strcpy( white_colour, "green");
			strcpy( highlight_colour, "black");
		break;
	}
}
void View::Key( int key )
{
	switch (key)
	{
		case 'j':
		viewpoint.x -= 10.0;
		screenpoint.x -= 10.0;
		break;
		case 'k':
		viewpoint.x += 10.0;
		screenpoint.x += 10.0;
		break;
		case 'h':
		viewpoint.z += 10.0;
		screenpoint.z += 10.0;
		break;
		case ',':
		viewpoint.y -= 10.0;
		screenpoint.y -= 10.0;
		break;
		case 'u':
		viewpoint.y += 10.0;
		screenpoint.y += 10.0;
		break;
		case 'l':
		viewpoint.z -= 10.0;
		screenpoint.z -= 10.0;
		break;


		case 'z':
		viewpoint.z -= 1.0;
		break;
		case 'x':
		viewpoint.z += 1.0;
		break;

		case '-':
		screenpoint.x += 1.0;
		break;

		case '=':
		screenpoint.x -= 1.0;
		break;

		default:
			model->Press( key );
	}
}

void View::Tick()
{
	double f_xoff = (double)screen_offset_x / 50.0;
	double f_yoff = (double)screen_offset_y / 50.0;
	viewpoint.z += f_xoff;
	screenpoint.z += f_xoff;

	viewpoint.y += f_yoff;
	screenpoint.y += f_yoff;

	float eye_amount = 0.20;
	//viewpoint.x -= 0.01;
	//screenpoint.x -= 0.01;
	if( view > 0 )
	{	
	view = 1;
	SetWhiteColour();
	Draw();
	viewpoint.z += eye_amount;
	screenpoint.z += eye_amount;
	view = 2;
	SetWhiteColour();
	Draw();
	viewpoint.z -= eye_amount;
	screenpoint.z -= eye_amount;
	}
	else
	{
		Draw();
	}
	model->Clock();

	// put it back
	viewpoint.z -= f_xoff;
	screenpoint.z -= f_xoff;
	viewpoint.y -= f_yoff;
	screenpoint.y -= f_yoff;
}
	

void View::Press( int x, int y)
{
	printf("Moving viewpoint\n");

	//viewpoint.z = (float)x / 100.0;
	//viewpoint.y = (float)y/100.0;

	if( y <MID_DISPLAY_Y)
	{
	viewpoint.x += 1.0;
	screenpoint.x += 1.0;
	}
	else
	{
	viewpoint.x -= 1.0;
	screenpoint.x -= 1.0;
	}


	model->Locate( x, y );	
	//Draw();
}

bool View::MapPoint( Point p, int *x_out, int *y_out )
{
        //printf("MapPoint: %f,%f,%f\n", p.x,p.y,p.z);

        float screen_x; // the final x position on the display
        float screen_y; // the final y position on the display

        //printf("X plane:\n");

        // start with the x display plane (looking down)
        Point2d a;
        Point2d b;

        Line2d los;  // the line between the viewpoint and the object
        a.x = viewpoint.x;
        a.y = viewpoint.z;
        b.x = p.x;
        b.y = p.z;
        los.Set( a, b );

        Line2d screen;  // the display screen
        a.x = screenpoint.x;
        a.y = screenpoint.z + SCREEN_X;
        b.x = screenpoint.x;
        b.y = screenpoint.z - SCREEN_Y;
        screen.Set( a,b );

        //printf("Line of sight:\n");
        los.Tell();
        //printf("Screen:\n");
        screen.Tell();

        Point2d ipoint;
        bool intersects;
        intersects = los.Intersects( &screen, &ipoint );

        if( !intersects )
        {
                //printf("Doesnt intersect\n");
                // Nothing to see
                return false;
        }
        //printf("intersects at %f %f\n", ipoint.x, ipoint.y );

        screen_x = ipoint.y - screenpoint.z;

        //printf("screen_x=%f\n", screen_x );

        // now do the y display plane
        //printf("Now doing y plane:\n");
        // using los and screen
        a.x = viewpoint.x;
        a.y = viewpoint.y;
        b.x = p.x;
        b.y = p.y;
        los.Set( a, b );

        a.x = screenpoint.x;
        a.y = screenpoint.y + SCREEN_X;
        b.x = screenpoint.x;
        b.y = screenpoint.y - SCREEN_Y;

        screen.Set( a,b );

        //printf("Line of sight:\n");
        los.Tell();
        ////printf("Screen:\n");
        screen.Tell();
        intersects = los.Intersects( &screen, &ipoint );

        if( !intersects )
        {
                ////printf("Lines dont intersect in y.\n");
                // Nothing to see
                return false;
        }
        ////printf("Lines intersect in y: %f,%f\n", ipoint.x, ipoint.y);

        screen_y = ipoint.y - screenpoint.y;

        //printf("screen_y =%f\n", screen_y);
        int x = (int) (screen_x * 100.0 );
        // x is now +-200 and backwards
        x = x+MID_DISPLAY_X;
        x = DISPLAY_X-x;

        int y = (int) (screen_y * 100.0);
        y = y +MID_DISPLAY_Y;
        y = DISPLAY_Y -y;

	*x_out = x;
	*y_out = y;
	
	return true;
}

void View::DrawPoint( Point &p )
{
/*
	printf("DrawPoint: %f,%f,%f\n", p.x,p.y,p.z);

	float screen_x; // the final x position on the display
	float screen_y; // the final y position on the display

	printf("X plane:\n");

	// start with the x display plane (looking down)
	Point2d a;
	Point2d b;

	Line2d los;  // the line between the viewpoint and the object
	a.x = viewpoint.x;
	a.y = viewpoint.z;
	b.x = p.x;
	b.y = p.z;
	los.Set( a, b );

	Line2d screen;  // the display screen
	a.x = screenpoint.x;
	a.y = screenpoint.z + SCREEN_X;
	b.x = screenpoint.x;
	b.y = screenpoint.z - SCREEN_Y;
	screen.Set( a,b );

	//printf("Line of sight:\n");		
	los.Tell();
	//printf("Screen:\n");
	screen.Tell();

	Point2d ipoint;
	bool intersects;
	intersects = los.Intersects( &screen, &ipoint );	

	if( !intersects )
	{
		//printf("Doesnt intersect\n");
		// Nothing to see
		return;
	}
	//printf("intersects at %f %f\n", ipoint.x, ipoint.y );
	
	screen_x = ipoint.y - screenpoint.z;

	//printf("screen_x=%f\n", screen_x );	

	// now do the y display plane 
	//printf("Now doing y plane:\n");

	// using los and screen
	a.x = viewpoint.x;
	a.y = viewpoint.y;
	b.x = p.x;
	b.y = p.y;
	los.Set( a, b );

	a.x = screenpoint.x;
	a.y = screenpoint.y + SCREEN_X;
	b.x = screenpoint.x;
	b.y = screenpoint.y - SCREEN_Y; 

	screen.Set( a,b );

	//printf("Line of sight:\n");		
	los.Tell();
	//printf("Screen:\n");
	screen.Tell();
	intersects = los.Intersects( &screen, &ipoint );	

	if( !intersects )
	{
		//printf("Lines dont intersect in y.\n");
		// Nothing to see
		return;
	}
	//printf("Lines intersect in y: %f,%f\n", ipoint.x, ipoint.y);

	screen_y = ipoint.y - screenpoint.y;

	//printf("screen_y =%f\n", screen_y);
	int x = (int) (screen_x * 100.0 );
	// x is now +-200 and backwards
	x = x+MID_DISPLAY_X;
	x = DISPLAY_X-x;

	int y = (int) (screen_y * 100.0);
	y = y + MID_DISPLAY_Y;
	y = DISPLAY_Y -y;
*/
	int x;
	int y;

	MapPoint( p, &x, &y);

	SetColour(white_colour);	
	GWindow::DrawLine( x,y,x,y );

	// record where it was rendered
	p.rendered_x = x;	
	p.rendered_y = y;
}

void View::DrawTriangle( Point &a, Point &b, Point &c,bool highlight )
{
	int x1,y1, x2,y2, x3,y3;

	bool offscreen = false;	
	if( MapPoint( a, &x1, &y1) == false )
	{
		offscreen = true;
	}
	if( MapPoint( b, &x2, &y2) == false )
	{
		offscreen = true;
	}
	if( MapPoint( c, &x3, &y3) == false )
	{
		offscreen = true;
	}

	if( offscreen == true ) 
	{
		return ;
	}
	a.rendered_x = x1;
	a.rendered_y = y1;
	b.rendered_x = x2;
	b.rendered_y = y2;
	c.rendered_x = x3;
	c.rendered_y = y3;


	if( highlight )
	{
		GWindow::SetColour(highlight_colour);
	}
	else
	{
		GWindow::SetColour(white_colour);
	}

	GWindow::DrawLine( x1,y1,x2,y2);
	GWindow::DrawLine( x2,y2,x3,y3);
	GWindow::DrawLine( x3,y3,x1,y1);
}
	

void View::DrawLine( Point &a, Point &b, bool highlight )
{
	int x1,y1, x2,y2;
	
	bool offscreen = false;	
	if( MapPoint( a, &x1, &y1) ==false)
	{
		offscreen=true;
	}
	if( MapPoint( b, &x2, &y2) == false )
	{
		offscreen=true;
	}

	if( offscreen == true )
	{
		return;
	}

	a.rendered_x = x1;
	a.rendered_y = y1;
	b.rendered_x = x2;
	b.rendered_y = y2;

	if( highlight )
	{
		GWindow::SetColour(highlight_colour);
	}
	else
	{
		GWindow::SetColour(white_colour);
	}

	GWindow::DrawLine( x1,y1,x2,y2);
}
	

void View::Draw()
{
	if( view != 2 )ClearWindow();
	int i;
	
	for( i = 0 ; i < NUM_OBJECTS; i++)
	{
		if( model->objects[i].allocated )
		{
			if( model->objects[i].type == OBJECT_POINT )
			{
				DrawPoint( model->objects[i].point_a );
			}
			else if( model->objects[i].type == OBJECT_TRIANGLE )
			{
				bool highlight = false;
				if (model->selected == &model->objects[i] )
				{
				
					highlight = true;
				}
				DrawTriangle( model->objects[i].point_a, model->objects[i].point_b,  model->objects[i].point_c, highlight );
			}
			else if( model->objects[i].type == OBJECT_LINE )
			{
				bool highlight = false;
				if (model->selected == &model->objects[i] )
				{
				
					highlight = true;
				}
				DrawLine( model->objects[i].point_a, model->objects[i].point_b, highlight );
			}
		}
	}
}
