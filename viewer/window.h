
#include <strings.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

class Object;

class GWindow
{
	public:
	virtual void 
Press( 
	int x, 
int y );
	virtual void MouseMove( int x, int y );
	virtual void Release( int x, int y );

	GWindow();
	virtual void Draw();
	virtual void Key( int key );

	void Run( int x, int y, int argc, char **argv);
	void Quit();
	virtual void Tick();

	void SetColour( char *name );
	void DrawLine( int x1, int y1, int x2, int y2 );
	void DrawText( int x, int y, char *text );
	void ClearWindow( );

	private:
	bool quit_posted;
	unsigned long colour;
	Display *display;
	int screen;
	GC gc;
	Window xwin;
	Colormap cmap;
};


