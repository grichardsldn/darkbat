
#include <stdio.h>
#include "window.h"
#include "line.h"
#include "model.h"
#include "view.h"

View::View( Model *a_model )
{
	viewpoint.x = -10.0;
	viewpoint.y = 1.0;
	viewpoint.z = 2.0;

	screenpoint.x = 2.0;
	screenpoint.y = 1.0;
	screenpoint.z = 2.0;

	model = a_model;
}

void View::Key( int key )
{
	switch (key)
	{
		case 'j':
		viewpoint.x -= 1.0;
		screenpoint.x -= 1.0;
		break;
		case 'k':
		viewpoint.x += 1.0;
		screenpoint.x += 1.0;
		break;
		case 'h':
		viewpoint.z += 1.0;
		screenpoint.z += 1.0;
		break;
		case ',':
		viewpoint.y -= 1.0;
		screenpoint.y -= 1.0;
		break;
		case 'u':
		viewpoint.y += 1.0;
		screenpoint.y += 1.0;
		break;
		case 'l':
		viewpoint.z -= 1.0;
		screenpoint.z -= 1.0;
		break;

		default:
			model->Press( key );
	}
}

void View::Tick()
{
	viewpoint.x -= 0.05;
	screenpoint.x -= 0.05;
	Draw();
	model->Clock();
	
}

void View::Press( int x, int y)
{
	printf("Moving viewpoint\n");

	//viewpoint.z = (float)x / 100.0;
	//viewpoint.y = (float)y/100.0;

	if( y <200)
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
        a.y = screenpoint.z + 2.0;
        b.x = screenpoint.x;
        b.y = screenpoint.z - 2.0;
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
        a.y = screenpoint.y + 2.0;
        b.x = screenpoint.x;
        b.y = screenpoint.y - 2.0;

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
        x = x+200;
        x = 400-x;

        int y = (int) (screen_y * 100.0);
        y = y +200;
        y = 400 -y;

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
	a.y = screenpoint.z + 2.0;
	b.x = screenpoint.x;
	b.y = screenpoint.z - 2.0;
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
	a.y = screenpoint.y + 2.0;
	b.x = screenpoint.x;
	b.y = screenpoint.y - 2.0; 

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
	x = x+200;
	x = 400-x;

	int y = (int) (screen_y * 100.0);
	y = y +200;
	y = 400 -y;
*/
	int x;
	int y;

	MapPoint( p, &x, &y);

	SetColour("linen");	
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
		GWindow::SetColour("red");
	}
	else
	{
		GWindow::SetColour("white");
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
		GWindow::SetColour("red");
	}
	else
	{
		GWindow::SetColour("white");
	}

	GWindow::DrawLine( x1,y1,x2,y2);
}
	

void View::Draw()
{
	ClearWindow();
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
