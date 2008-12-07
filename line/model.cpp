#include <stdio.h>
#include <math.h>

#include "line.h"
#include "model.h"


Model::Model()
{
	int i;
	for( i = 0 ; i < NUM_OBJECTS ; i++)
	{
		objects[i].allocated = false;
	}
}

void Model::AddPoint( float x, float y, float z, int ref )
{
	Point a;
	a.x = x;
	a.y = y;
	a.z = z;
	AddPoint( a, ref );
}


void Model::AddLine( float x1, float y1, float z1, float x2, float y2, float z2, int ref )
{
	Line *a = new Line( x1,y1,z1,  x2, y2, z2 );

	AddLine( *a, ref );
}

void Model::AddLine( Line line, int ref )
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
			break;
		}
	}
}

void Model::AddPoint( Point point, int ref )
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
			break;
		}
	}
}

