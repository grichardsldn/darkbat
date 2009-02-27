
class Point
{
	public:
	float x;
	float y;
	float z;

	int rendered_x;
	int rendered_y;
};

class Line
{
	public:
	Point start;
	Point end;
	
	Line();
	Line( float x1, float y1, float z1, float x2, float y2, float z2 );
};



class Point2d
{
	public:
	float x;
	float y;

	void Tell();
	float DistanceFrom( Point2d *other );
};

class Line2d
{
	public:

	public:
	Point2d start;
	Point2d finish;

	Line2d();
	void Set( Point2d a, Point2d b );

	bool Intersects( Line2d *other, Point2d *crosses);
	void Tell();
};

extern bool Within( float val, float bound1, float bound2);

class Slope2d
{
	public:
	int by_y;

	float grad;
	float crosses_origin;

	Slope2d();
	void Set( Point2d start, Point2d finish );
	
	bool Intersects( Slope2d *other, Point2d *crosses );
	void Tell();
};


	
	
