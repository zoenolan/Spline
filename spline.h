#ifndef spline_h
#define spline_h

/*

   Computuational Geometry ACW

   Zoe Nolan 

   20/4/1997

                                                                         */

/*
   Spline Class header

                                                                         */

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include "aux.h"


class TSpline
{
 private:
   // number of points
   int _NumberOfPoints ;

   // data points
   double *_X ;
   double *_Y ;
   double *_T ;

   // second difs
   double *_M ;
   double *_N ;

   // max and mins
   double _MaxX ;
   double _MinX ;
   double _MaxY ;
   double _MinY ;

   // flags
   int _Type ;

   // work out the second derivative
   SecondDiff(double *Ms,double *X,double *Y) ;

   // functions to  work out the intermediates
   double H(int place,double *X) ;
   double Q(int place,double *H) ;
   double P(int place,double *H) ;
   double R(int place,double *H) ;
   double S(int place,double *Y,double *H) ;
   double T(int place,double *P,double *Q,double *R,double *T) ;
   double V(int place,double *S,double *P,double *T) ;
   double M(int place,double *V,double *R,double *T,double *M) ;

   DrawSpline() ;
   DrawSprial() ;


 public:
   TSpline(char *FileName) ;
   ~TSpline() ;

   Draw() ;
   Resize(GLsizei w, GLsizei h) ;
 } ;

#endif
