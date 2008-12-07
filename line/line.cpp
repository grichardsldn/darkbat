#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "line.h"

Line::Line()
{
	Line( 0,0,0, 0,0,0);
}

Line::Line( float x1,float y1, float z1, float x2, float y2, float z2)
{
	start.x = x1;
	start.y = y1;
	start.z = z1;
	end.x = x2;
	end.y = y2;
	end.z = z2;
}
extern bool Within( float val, float bound1, float bound2)
{
	float lower;
	float upper;
	if( bound2 > bound1)
	{
		lower = bound1;
		upper = bound2;
	}
	else
	{
		lower = bound2;
		upper = bound1;
	}

	if( (val < lower) || (val > upper ) )
	{
		return false;
	}
	return true;
}
	


Line2d::Line2d()
{
	start.x = 0;
	start.y = 0;
	finish.x = 0;
	finish.y = 0;
}

void Line2d::Tell()
{
	printf("Line2d: &%08x\n", (unsigned int) this);
	printf(" start.x=%f start.y=%f finish.x=%f finish.y=%f\n", 
		start.x, start.y, finish.x, finish.y );
}

void Line2d::Set( Point2d a, Point2d b )
{
	// put them in x order
	if( a.x < b.x )
	{
		start.x = a.x;
		start.y = a.y;
		finish.x = b.x;
		finish.y = b.y;
	}
	else
	{
		start.x = b.x;
		start.y = b.y;
		finish.x = a.x;
		finish.y = a.y;
	}
}

bool Line2d::Intersects( Line2d *other, Point2d *crosses )
{
	assert( other );
	assert( crosses );

	bool intersects = false;

	Slope2d myslope;
	myslope.Set( start, finish );
	Slope2d otherslope;
	otherslope.Set( other->start, other->finish );

	intersects = myslope.Intersects( &otherslope, crosses );

	printf("slopes intersect at %f %f\n", crosses->x, crosses->y);

	if( intersects == true )
	{
		printf("Slopes intersect\n");
		float x = crosses->x;

		if (	(!Within( x, start.x, finish.x)) || 
			(!Within( x, other->start.x, other->finish.x)) )
		{
			// it meets out of x bounds
			printf("Out of x bounds\n");
			intersects = false;
		}

		float y = crosses->y;

		if( (!Within( y, start.y, finish.y)) ||
			(!Within( y, other->start.y, other->finish.y)) )
		{
			// it meets out of y bounds
			printf("Out of y bounds\n");
			intersects = false;
		}
	}
		
	return intersects;
}



Slope2d::Slope2d()
{
	by_y = -1;
	grad = 0.0;
	crosses_origin=0.0;
}

void Slope2d::Set( Point2d start, Point2d finish )
{
	float dx1 = finish.x - start.x;
	float dy1 = finish.y - start.y;

	printf("dx1=%f dy1=%f\n", dx1, dy1);

	if( fabsf(dx1) >= fabsf(dy1) )
	{
		by_y = 1;
	}
	else
	{
		by_y = 0;
	}


	if( by_y )
	{
		grad = dy1 / dx1;
	
		crosses_origin = start.y - (grad * start.x );	
	}
	else
	{
		if( dx1 == 0.0)
		{
			grad = 0.0;
			crosses_origin = start.x;
		}
		else
		{
			printf("dx1=%f:\n", dx1 );
			assert( 0 );
		}
	}
}

bool Slope2d::Intersects( Slope2d *other, Point2d *crosses )
{
	assert( other );
	assert( crosses );	
	assert( other->by_y != -1 ); // unitialised

	bool intersected = false;

	if( by_y == other->by_y)
	{
		if ( grad == other->grad )
		{
			// the same slope
			intersected = false;
		}
		else
		{
			float bottom = grad - other->grad;
			float top = other->crosses_origin - crosses_origin;

			float the_x = top / bottom;
			
			printf("Points cross at x=%f\n", the_x );

			crosses->x = the_x;
			crosses->y = (grad * the_x) + crosses_origin;

			intersected = true;
		}
	}
	else
	{
		if( other->grad == 0.0 )
		{
			// we can do this:
			float the_x = other->crosses_origin;
			crosses->x = the_x;
			crosses->y = (grad * the_x) + crosses_origin;

			intersected = true;
		}	
		else if ( grad == 0.0 )
		{
			// we can also do this:
			float the_x = crosses_origin;
			crosses->x = the_x;
			crosses->y = (other->grad * the_x) + other->crosses_origin;
			intersected = true;
		}
		else
		{
			assert( 0 );
		}

	}

	return intersected;
}


void Slope2d::Tell()
{
	printf("Slope2d:&%08x\n", (unsigned int)this);
	printf("  grad=%f crosses_origin=%f by_y=%d\n", grad, crosses_origin, 
		by_y );
}

void Point2d::Tell()
{
	printf("Point2d:&%08x: x=%f y=%f\n", (unsigned int)this, x, y );
}



	
