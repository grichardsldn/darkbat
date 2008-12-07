
// requires window.h, line.h, model.h

class View : public GWindow
{
	public:

	// GWindow stuff:
	void Draw();
	void Press( int x, int y);
	

	// View stuff

	View( Model *model );
	Point viewpoint;
	Point screenpoint;

	Model *model;

	private:
	void DrawPoint( Point p );
	void DrawLine( Point a, Point b );
	void MapPoint( Point p, int *x, int *y);
};
