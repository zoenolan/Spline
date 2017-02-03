/*

   Computuational Geometry ACW

   Zoe Nolan

   20/4/1997

                                                                         */

/*
   Spline Class cpp

                                                                         */

#include "spline.h"
#include <string.h>
#include <fstream.h>
#include <iostream.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include "aux.h"

// constructor
TSpline::TSpline(char *FileName)
{
 char buffer[7] ;  // for names
 double X,Y,T ;
 int counter ;

 ifstream fin(FileName) ;

 // get file type
 fin>>buffer ;
 cout<<"Spline Type: " <<buffer<<endl ;
 if (strcmp(buffer,"sprial")==0)
   {
    _Type=1 ;
   }
 else
   {
    _Type=0 ;
   }

 // get number of points
 fin>>buffer ;

 fin>>_NumberOfPoints ;
 cout<<"Number of Points: "<<_NumberOfPoints<<endl ;

 // now set all th point arrays to this size
 _X=new double [_NumberOfPoints] ;
 _Y=new double [_NumberOfPoints] ;
 _T=new double [_NumberOfPoints] ;
 _M=new double [_NumberOfPoints] ;
 _N=new double [_NumberOfPoints] ;

 // read in the X,Y and T values
 for (counter=0;counter<_NumberOfPoints;counter++)
   {
    // read the data
    fin>>X ;
    fin>>Y ;
    fin>>T ;

    // put it in the right place
    _X[counter]=X ;
    _Y[counter]=Y ;
    _T[counter]=T ;

    // output some data
    cout<<"Point "<<counter<<": "<<X<<","<<Y<<","<<T<<endl ;
   }

 // now work out the max and min for each
 _MaxX=_X[0] ;
 _MinX=_X[0] ;
 _MaxY=_Y[0] ;
 _MinY=_Y[0] ;

 for (counter=1;counter<_NumberOfPoints;counter++)
   {
    // read the data
    X=_X[counter] ;
    Y=_Y[counter] ;

    // test it
    if (X<_MinX)
      {
       _MinX=X ;
      }

    if (X>_MaxX)
      {
       _MaxX=X ;
      }

    if (Y<_MinY)
      {
       _MinY=Y ;
      }

    if (Y>_MaxY)
      {
       _MaxY=Y ;
      }

   }

 if (_Type==0)
   {
    // work out second derivatives
    SecondDiff(_M,_X,_Y) ;
   }
 else
   {
    SecondDiff(_M,_T,_Y) ;
    SecondDiff(_N,_T,_X) ;
   }

}

// clean up data
TSpline::~TSpline()
{
 delete _X ;
 delete _Y ;
 delete _T ;
 delete _M ;
 delete _N ;
}

// set all the M's
TSpline::SecondDiff(double *Ms,double *X,double *Y)
{
 int counter ;
 double *Ts,*Vs,*Qs,*Ps,*Rs,*Ss,*Hs ;

 Hs= new double [_NumberOfPoints] ;
 Ts= new double [_NumberOfPoints] ;
 Vs= new double [_NumberOfPoints] ;
 Qs= new double [_NumberOfPoints] ;
 Ps= new double [_NumberOfPoints] ;
 Rs= new double [_NumberOfPoints] ;
 Ss= new double [_NumberOfPoints] ;

 for (counter=1;counter<_NumberOfPoints;counter++)
   {
    Hs[counter]=H(counter,X) ;
   }

 for (counter=1;counter<_NumberOfPoints-1;counter++)
   {
    Qs[counter]=Q(counter,Hs) ;
   }

 for (counter=2;counter<_NumberOfPoints-1;counter++)
   {
    Ps[counter]=P(counter,Hs) ;
   }

 for (counter=1;counter<_NumberOfPoints-1;counter++)
   {
    Rs[counter]=R(counter,Hs) ;
   }

 for (counter=1;counter<_NumberOfPoints-1;counter++)
   {
    Ss[counter]=S(counter,Y,Hs) ;
   }

 for (counter=1;counter<_NumberOfPoints-1;counter++)
   {
    Ts[counter]=T(counter,Ps,Qs,Rs,Ts) ;
   }

 for (counter=1;counter<_NumberOfPoints-1;counter++)
   {
    Vs[counter]=V(counter,Ss,Ps,Ts) ;
   }

 for (counter=_NumberOfPoints-2;counter>0;counter--)
   {
    Ms[counter]=M(counter,Vs,Rs,Ts,Ms) ;
   }

 // set the ends
 Ms[0]=Ms[1] ;
 Ms[_NumberOfPoints-1]=Ms[_NumberOfPoints-2] ;

 delete Ts ;
 delete Vs ;
 delete Qs ;
 delete Ps ;
 delete Rs ;
 delete Ss ;
 delete Hs ;

}

// functions to  work out some of the intermidate values

double TSpline::H(int place,double *X)
{
 double temp=1 ;

 temp=X[place]-X[place-1] ;

 return temp ;
}

double TSpline::Q(int place,double *H)
{
 double temp=1 ;

 if (place==1)
   {
    temp=(1.0/6.0)*((3.0*H[1])+(2*H[2])) ;
   }
 else if (place==_NumberOfPoints-2)
   {
    temp=(1.0/6.0)*((2.0*H[_NumberOfPoints-2])+(3.0*H[_NumberOfPoints-1])) ;
   }
 else
   {
    temp=(1.0/3.0)*(H[place]+H[place+1]) ;
   }

 return temp ;
}

double TSpline::P(int place,double *H)
{
 double temp=1 ;

 temp=(1.0/6.0)*H[place] ;

 return temp ;
}

double TSpline::R(int place,double *H)
{
 double temp=1 ;

 temp=(1.0/6.0)*H[place+1] ;

 return temp ;
}

double TSpline::S(int place,double *Y,double *H)
{
 double temp=1 ;

 temp=((Y[place+1]-Y[place])/H[place+1])-((Y[place]-Y[place-1])/H[place]) ;

 return temp ;
}

double TSpline::T(int place,double *P,double *Q,double *R,double *T)
{
 double temp=1 ;

 if (place==1)
   {
    temp=Q[1] ;
   }
 else
   {
    temp=Q[place]-(R[place-1]*(P[place]/T[place-1])) ;
   }

 return temp ;
}

double TSpline::V(int place,double *S,double *P,double *T)
{
 double temp=1 ;

 if (place==1)
   {
    temp=S[1] ;
   }
 else
   {
    temp=S[place]-(S[place-1]*(P[place]/T[place-1])) ;
   }

 return temp ;
}

double TSpline::M(int place,double *V,double *R,double *T,double *M)
{
 double temp=1 ;

 if (place==_NumberOfPoints-2)
   {
    temp=V[_NumberOfPoints-2]/T[_NumberOfPoints-2] ;
   }
 else
   {
    temp=(V[place]-(R[place]*M[place+1]))/T[place] ;
   }

 return temp ;
}

// select which type of object to draw
TSpline::Draw()
{
 if (_Type==0)
   {
    DrawSpline() ;
   }
 else
   {
    DrawSprial() ;
   }
}

TSpline::DrawSprial()
{
 int counter ;
 int PointCounter ;
 double X,Y ;
 double *Hs ;
 double A,B ;
 double OldX,OldY ;

 // Join the dots
 glColor3f(0.25,0.25,0.25) ;
 glBegin(GL_LINES) ;
   for (counter=0;counter<_NumberOfPoints-1;counter++)
     {
      glVertex2f(_X[counter],_Y[counter]) ;
      glVertex2f(_X[counter+1],_Y[counter+1]) ;
     }
 glEnd() ;


 // Draw the spline

 // work out the Hs ;
 Hs= new double [_NumberOfPoints] ;

 for (counter=1;counter<_NumberOfPoints;counter++)
   {
    Hs[counter]=H(counter,_T) ;
   }

 OldX=_X[0] ;
 OldY=_Y[0] ;

 glColor3f(1,1,1) ;
 glBegin(GL_LINES) ;
   for (counter=0;counter<_NumberOfPoints-1;counter++)
     {
      // Draw the segment between the points
      for (PointCounter=0;PointCounter<100;PointCounter++)
        {
         A=PointCounter/100.0 ;
         B=1-A ;

         X=A*_X[counter+1]+
           ((A*((A*A)-1)*Hs[counter+1]*Hs[counter+1]*_N[counter+1])/6.0)+
           B*_X[counter]+
           ((B*((B*B)-1)*Hs[counter+1]*Hs[counter+1]*_N[counter])/6.0) ;

         Y=A*_Y[counter+1]+
           ((A*((A*A)-1)*Hs[counter+1]*Hs[counter+1]*_M[counter+1])/6.0)+
           B*_Y[counter]+
           ((B*((B*B)-1)*Hs[counter+1]*Hs[counter+1]*_M[counter])/6.0) ;

         // draw section
         glVertex2f(OldX,OldY) ;
         glVertex2f(X,Y) ;

         // update old position
         OldX=X ;
         OldY=Y ;
        }
     }
 glEnd() ;

 delete Hs ;

 // Show the Data Points
 glColor3f(1,0,0) ;
 glPointSize(6) ;
 glBegin(GL_POINTS) ;
   for (counter=0;counter<_NumberOfPoints;counter++)
     {
      glVertex2f(_X[counter],_Y[counter]) ;
     }
 glEnd() ;
}

// draw a spline
TSpline::DrawSpline()
{
 int counter ;
 int PointCounter ;
 double X,Y ;
 double *Hs ;
 double A,B ;
 double OldX,OldY ;

 // Join the dots
 glColor3f(0.25,0.25,0.25) ;
 glBegin(GL_LINES) ;
   for (counter=0;counter<_NumberOfPoints-1;counter++)
     {
      glVertex2f(_X[counter],_Y[counter]) ;
      glVertex2f(_X[counter+1],_Y[counter+1]) ;
     }
 glEnd() ;

 // Draw the spline

 // work out the Hs ;
 Hs= new double [_NumberOfPoints] ;

 for (counter=1;counter<_NumberOfPoints;counter++)
   {
    Hs[counter]=H(counter,_X) ;
   }

 OldX=_X[0] ;
 OldY=_Y[0] ;

 glColor3f(1,1,1) ;
 glBegin(GL_LINES) ;
   for (counter=0;counter<_NumberOfPoints-1;counter++)
     {
      // Draw the segment between the points
      for (PointCounter=0;PointCounter<100;PointCounter++)
        {
         A=PointCounter/100.0 ;
         B=1-A ;

         X=_X[counter]+(A*Hs[counter+1]) ;

         Y=A*_Y[counter+1]+
           ((A*((A*A)-1)*Hs[counter+1]*Hs[counter+1]*_M[counter+1])/6.0)+
           B*_Y[counter]+
           ((B*((B*B)-1)*Hs[counter+1]*Hs[counter+1]*_M[counter])/6.0) ;

         // draw section
         glVertex2f(OldX,OldY) ;
         glVertex2f(X,Y) ;

         // update old position
         OldX=X ;
         OldY=Y ;
        }
     }
 glEnd() ;

 delete Hs ;

 // Show the Data Points
 glColor3f(1,0,0) ;
 glPointSize(6) ;
 glBegin(GL_POINTS) ;
   for (counter=0;counter<_NumberOfPoints;counter++)
     {
      glVertex2f(_X[counter],_Y[counter]) ;
     }
 glEnd() ;
}

TSpline::Resize(GLsizei w, GLsizei h )
{
 glViewport(0, 0, w, h);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity() ;
 gluOrtho2D(-0.5,0.5,-0.5,0.5);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity ();


 // scale to right size
 glScalef(0.75,0.75,0.75) ;


 // scale world to unit cube
 glScalef(1.0/(_MaxX-_MinX),1.0/(_MaxY-_MinY),1) ;

 // translate to orgin
 glTranslatef(-(_MinX+((_MaxX-_MinX)/2.0)),-(_MinY+((_MaxY-_MinY)/2.0)),0) ;

}
