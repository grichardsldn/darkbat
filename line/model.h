
// requires line.h

#define NUM_OBJECTS (100)

#define OBJECT_POINT (1)
#define OBJECT_LINE (2)

class Object
{
	public:
	bool allocated;
	int type;
	Point point_a;
	Point point_b;
	Point point_c;
	int ref;
};

class Model
{
	public:

	Model();

	void AddPoint( Point point, int ref );
	void AddPoint( float x, float y, float z, int ref );
	void AddLine( Line line, int ref );
	void AddLine( float x1, float y1, float z1, float x2, float y2, float z2, int ref);

	Object objects[NUM_OBJECTS];
};
	
	
