
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
	int expires;
};

class Model
{
	public:

	Model();

	void AddPoint( Point point, int ref, int lifetime );
	void AddPoint( float x, float y, float z, int ref, int lifetime );
	void AddLine( Line line, int ref, int lifetime );
	void AddLine( float x1, float y1, float z1, float x2, float y2, float z2, int ref, int lifetime);
	void AddClickTri( float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int ref, int lifetime);
	void Remove( int ref );

	Object objects[NUM_OBJECTS];
	void Clock();

	private:
	int frame;
};
	
	
