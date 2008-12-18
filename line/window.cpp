#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <unistd.h>
#include "window.h"

void GWindow::Tick()
{
}

GWindow::GWindow( )
{
	quit_posted = false;
}

void GWindow::Press( int x, int y )
{
	printf("GWindow::Press(x=#%d, y=#%d)\n", x, y );
}

void GWindow::MouseMove( int x, int y )
{
	printf("GWindow::MouseMove(x=#%d, y=#%d)\n", x, y );
}

void GWindow::Release( int x, int y )
{
	printf("GWindow::Release(x=#%d, y=#%d)\n", x, y );
}

void GWindow::Draw()
{
	printf("GWindow::Draw()\n");
}

void GWindow::Key( int key )
{
	printf("GWindow::Key( key=\'%c\')\n", key);
}

void GWindow::Run( int x, int y, int argc, char **argv)
{
	if ( !(display = XOpenDisplay(NULL)) ) {
          fprintf(stderr,"Cannot open display: check DISPLAY variable\n");
	}

	screen = DefaultScreen( display );

	cmap = DefaultColormap( display, screen );

	XColor approx;
	XColor exact;

	XAllocNamedColor( display, cmap, "white", &exact, &approx );

	colour = approx.pixel;

	XSizeHints hint;
	hint.x = 100;
	hint.y = 150;
	hint.width = y;	
	hint.height = x;
	hint.flags = PPosition;

	unsigned long fg = WhitePixel( display, screen );
	unsigned long bg = BlackPixel( display, screen );

	xwin = XCreateSimpleWindow(
		display,
		DefaultRootWindow( display ),
		hint.x, hint.y,
		hint.width, hint.height,
		7, // border width
		fg, //Foreground colour
		bg ); // Background colour

	XSetStandardProperties(
		display,
		xwin,
		"WindowName",
		"IconName",
		None,  // pixmap for icon
		argv, argc, 
		&hint );

	XMapRaised( display, xwin );

	// XXX Event Loop goes here


      XEvent evt;             /* X Event */
      char kbuf[8];           /* Key conv buffer */
      KeySym key;             /* Key symbol */
      int kcount;             /* Key count */
      int b = 0;              /* Buttons Pressed */
      int star = False;       /* Draw stars when True */

      /*
       * Choose the XEvents that we want to process :
       */
      XSelectInput(display,xwin,
          KeyPressMask | ExposureMask |
          ButtonPressMask | ButtonReleaseMask |
          Button1MotionMask | Button2MotionMask | Button3MotionMask);

      /*
       * Create a Graphics Context :
       */
      gc = XCreateGC(display,xwin,0,0);
      XSetBackground(display,gc,bg); /* Set background color of gc */
      XSetForeground(display,gc,fg); /* Set foreground color of gc */

      /*
       * Process X Events :
       */
      while ( quit_posted != True )
	 {

	while( XPending(display) )
{

          /*
           * Fetch an X Event :
           */
          XNextEvent(display,&evt);

          /*
           * Process the X Event :
           */
          switch ( evt.type ) 
	{

          case Expose :
              /*
               * Window has been exposed :
               */
		Draw( );
              break;

          case ButtonPress :
              /*
               * A button has been pressed:
               *
               * Set the bit corresponding to the mouse button that
               * is pressed :
               */
#if 0
              switch ( evt.xbutton.button ) {
              case Button1 :
                  b |= B1;
                  break;
              case Button2 :
                  b |= B2;
                  break;
              default :
                  b |= B3;
              }
#endif

		Press( evt.xbutton.x, evt.xbutton.y );

             break;

         case ButtonRelease :
#if 0
             /*
              * A button has been released :
              *
              * Unset the bit corresponding to the released color :
              */
             switch ( evt.xbutton.button ) {
             case Button1 :
                 b &= ~B1;
                 break;
             case Button2 :
                 b &= ~B2;
                 break;
             default :
                 b &= ~B3;
             }

             /*
              * Set the color based upon the leftmost mouse button :
              */
             if ( b & B1 )
                 fg = red;
             else if ( b & B2 )
                 fg = green;
             else
                 fg = blue;
             XSetForeground(disp,gc,fg); /* Set foreground color of gc */
#endif
             break;

         case MotionNotify :
#if 0
             /*
              * Motion with a button down :
              *
              * Draw a line from the last know position, to the current :
              */
             XDrawLine(disp,xwin,gc,x0,y0,evt.xmotion.x,evt.xmotion.y);

             /*
              * When drawing lines, we must save the last position that
              * we have drawn a line segment to :
              */
             if ( star == False ) {
                 x0 = evt.xmotion.x;     /* Save x for next line segment */
                 y0 = evt.xmotion.y;     /* Save y for next line segment */
             }
#endif
             break;

         case MappingNotify :
             XRefreshKeyboardMapping(&evt.xmapping);
             break;

         case KeyPress :
             /*
              * A key was pressed; check for 'q'to quit :
              */
             kcount = XLookupString(&evt.xkey,kbuf,sizeof kbuf,&key,0);

             if ( kcount == 1 )
		{
			Key( kbuf[0] );
		}

		break;
         }
	}
	usleep(10000);
	Tick();
	}

	// end of event loop

     XFreeGC(display,gc);                   /* Release graphics context */

	XDestroyWindow( display, xwin );
	XCloseDisplay( display );

}

void GWindow::Quit()
{
	quit_posted = true;
}

void GWindow::SetColour( char *name )
{
	XColor approx;
	XColor exact;

	XAllocNamedColor( display, cmap, name, &exact, &approx );

	colour = approx.pixel;

    	XSetForeground(display,gc,colour); /* Set foreground color of gc */
}

void GWindow::DrawLine( int x1, int y1, int x2, int y2 )
{
	XDrawLine(display,xwin,gc,x1,y1,x2,y2);
	
}

void GWindow::ClearWindow()
{
	XClearWindow( display, xwin );
}

void GWindow::DrawText( int x, int y, char *text )
{
                  XDrawImageString(display,
                      xwin,
                      gc,
                      x, y,
                      text, strlen( text ) );
}
