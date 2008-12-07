#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "line.h"
#include "window.h"
#include "model.h"
#include "view.h"




int main(int argc, char **argv)
{
	Model model;
	View view( &model);

	Point a;

	//model.AddPoint( 5.0, 0.0, 1.0, 0);
	//model.AddPoint( 6.0, 0.0, 1.0, 0);

	model.AddLine( 5.0, 0.0, 1.0, 6.0, 0.0, 1.0, 0);

	model.AddPoint( 6.0, 0.0, 2.0, 0);
	model.AddPoint( 7.0, 0.0, 2.0, 0);
	model.AddPoint( 7.0, 1.0, 2.0, 0);
	model.AddPoint( 5.0, 0.0, -0.5, 0);
	model.AddPoint( 7.0, 1.0, 2.2, 0);
	model.AddPoint( 7.0, 1.0, 2.4, 0);
	model.AddPoint( 7.0, 1.0, 2.6, 0);

	view.Run( 400,400, argc, argv);
}

	
	
