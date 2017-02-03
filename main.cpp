/*

   Computuational Geometry ACW

   Zoe Nolan

   20/4/1997

                                                                         */

/*
   Main Program

                                                                         */

#include "spline.h"
#include <iostream.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include "aux.h"

TSpline *Displayer ;

void display()
{
    glClear (GL_COLOR_BUFFER_BIT);
    Displayer->Draw() ;
    glFlush() ;
}

void myReshape(GLsizei w, GLsizei h)
{
 Displayer->Resize(w,h) ;
}

main()
{
 char Name[256] ;
 // show start up message
 cout<<"Computational Geometry"<<endl<<endl ;
 cout<<"ACW - Cubic Splines"<<endl ;
 cout<<endl<<endl<<"By Iain Nolan"<<endl<<endl ;

 cout<<"Please Enter the Filename to load: " ;
 cin>>Name ;

 // load data
 TSpline Spline(Name) ;
 cout<<endl ;

 Displayer=&Spline ;
 // draw spline

 auxInitDisplayMode (AUX_SINGLE | AUX_RGBA);
 auxInitPosition (0, 0, 600, 600);
 auxInitWindow ("Splines");
 glClearColor (0.0, 0.0, 0.0, 1.0);
 glShadeModel (GL_FLAT);
 auxReshapeFunc (myReshape);
 auxMainLoop(display);
}
