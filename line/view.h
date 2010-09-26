
// requires window.h, line.h, model.h

class View : public GWindow
{
	public:

	// GWindow stuff:
	void Draw();
	void Press( int x, int y);
	void Tick();
	void Key( int key );	

	// View stuff

	View( Model *model );
	Point viewpoint;
	Point screenpoint;
	int view; // 0 = normal, 1 = left, 2 = right
	char white_colour[1024];
	char highlight_colour[1024];
	Model *model;

	private:
	void SetWhiteColour();
	void DrawPoint( Point &p );
	void DrawLine( Point &a, Point &b, bool highlight = false );
	void DrawTriangle( Point &a, Point &b, Point &c, bool highlight = false );
	bool MapPoint( Point p, int *x, int *y);
};
