
// requires line.h

#define NUM_OBJECTS (120)

#define OBJECT_POINT (1)
#define OBJECT_LINE (2)
#define OBJECT_TRIANGLE (3)

class Object
{
	public:
	bool allocated;
	int type;
	Point point_a;
	Point point_b;
	Point point_c;
	int ref;
	int connection_ref;
	int from_id;
	int expires;
	int clickref;
};

class Model
{
	public:

	Model();

	// return a pointer to the object rendered nearest to the given
  	// points.
	Object *Locate( int x, int y);
	void Press( int key );

	void AddPoint( Point point, int ref,int connection_ref, int lifetime );
	void AddPoint( float x, float y, float z, int ref,int connection_ref, int lifetime );
	void AddLine( Line line, int ref,int connection_ref, int lifetime );
	void AddLine( float x1, float y1, float z1, float x2, float y2, float z2, int ref,int connection_ref, int lifetime);
	void AddClickTri( float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int ref,int connection_ref, int from_id, int lifetime, int clickref);
	void AddTriangle( Point a, Point b, Point c, int ref,int connection_ref, int from_id, int lifetime, int clickref = 0 );
	void Remove( int ref );

	Object objects[NUM_OBJECTS];
	void Clock();

	Object *selected;

	private:
	int frame;
};
	
	
