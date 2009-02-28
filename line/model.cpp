#include <stdio.h>
#include <math.h>

#include "line.h"
#include "model.h"

void Model::Press(int key )
{
	if (selected != NULL )
	{
		printf("Signal clickref %d key %c\n",
			selected->clickref, key );
	}
}


// set 'selected' to point to the object that is nearest to
// the specified on-screen co-ordinates
Object *Model::Locate( int x, int y )
{
	float shortest_dist = 10000.0;
	Object *shortest_obj = NULL;

	Point2d select_point;
	select_point.x = (float)x;
	select_point.y = (float)y;

	Point2d test_point;
	
	for( int i = 0 ; i < NUM_OBJECTS ; i ++)
	{
		if(objects[i].allocated )
		{
			if( objects[i].type != OBJECT_TRIANGLE)
			{
				continue;
			}
			test_point.x = (float)objects[i].point_a.rendered_x;
			test_point.y = (float)objects[i].point_a.rendered_y;

			float dist = test_point.DistanceFrom( &select_point );

			test_point.x = (float)objects[i].point_b.rendered_x;
			test_point.y = (float)objects[i].point_b.rendered_y;

			dist += test_point.DistanceFrom( &select_point );
			test_point.x = (float)objects[i].point_c.rendered_x;
			test_point.y = (float)objects[i].point_c.rendered_y;

			dist += test_point.DistanceFrom( &select_point );

			dist /= 3.0;

			if( dist < shortest_dist )
			{	
				shortest_dist = dist;
				shortest_obj = &objects[i];
			}
		}
	}
	selected = shortest_obj;
	printf("Locate: shortest_dist=%f\n", shortest_dist );
}

void Model::Clock()
{
	frame ++;
	int i;
	for( i = 0 ; i < NUM_OBJECTS ; i++)
	{	
		if( objects[i].allocated )
		{
			if( objects[i].expires < frame )
			{
				objects[i].allocated = false;
			}
		}
	}
}

Model::Model()
{
	int i;
	for( i = 0 ; i < NUM_OBJECTS ; i++)
	{
		objects[i].allocated = false;
	}
}

void Model::Remove( int ref )
{
	int i;
	for( i = 0 ; i < NUM_OBJECTS ; i++)
	{
		if( objects[i].allocated )
		{
			if( objects[i].ref == ref )
			{
				objects[i].allocated = false;
			}
		}
	}
}


void Model::AddPoint( float x, float y, float z, int ref, int lifetime )
{
	Point a;
	a.x = x;
	a.y = y;
	a.z = z;
	AddPoint( a, ref, lifetime );
}



void Model::AddClickTri( float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int ref, int lifetime, int clickref )
{
	Point a;
	a.x = x1;
	a.y = y1;
	a.z = z1;
	Point b;
	b.x = x2;
	b.y = y2;
	b.z = z2;
	Point c;
	c.x = x3;
	c.y = y3;
	c.z = z3;
	AddTriangle( a,b,c, ref, lifetime, clickref );
}

void Model::AddLine( float x1, float y1, float z1, float x2, float y2, float z2, int ref, int lifetime )


{
	Line *a = new Line( x1,y1,z1,  x2, y2, z2 );

	AddLine( *a, ref, lifetime );
}

void Model::AddTriangle( Point a, Point b, Point c, int ref, int lifetime, int clickref )
{
	for( int i = 0 ;i < NUM_OBJECTS; i++)
		{
		if (objects[i].allocated == false)
		{
			objects[i].type = OBJECT_TRIANGLE;
			objects[i].point_a = a;
			objects[i].point_b = b;
			objects[i].point_c = c;
			objects[i].ref = ref;
			objects[i].allocated = true;
			objects[i].expires = lifetime + frame;
			objects[i].clickref = clickref;
			break;
		}
	}
}

void Model::AddLine( Line line, int ref, int lifetime )
{
	int i;
	for( int i = 0 ;i < NUM_OBJECTS; i++)
		{
		if (objects[i].allocated == false)
		{
			objects[i].type = OBJECT_LINE;
			objects[i].point_a = line.start;
			objects[i].point_b = line.end;
			objects[i].ref = ref;
			objects[i].allocated = true;
			objects[i].expires = lifetime + frame;
			break;
		}
	}
}

void Model::AddPoint( Point point, int ref, int lifetime )
{
	int i;
	for( int i = 0 ;i < NUM_OBJECTS; i++)
		{
		if (objects[i].allocated == false)
		{
			objects[i].type = OBJECT_POINT;
			objects[i].point_a = point;
			objects[i].ref = ref;
			objects[i].allocated = true;
			objects[i].expires = lifetime + frame;
			break;
		}
	}
}

