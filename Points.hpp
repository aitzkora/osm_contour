#ifndef POINTS_H
#define POINTS_H

struct SVector
{
   double dx,dy;
};

struct SPoint
{
   SPoint(double x,double y){this->x=x;this->y=y;}
   SPoint(){}
   double x,y;
};

struct SPair
{
   SPair(SPoint p1, SPoint p2){this->p1=p1;this->p2=p2;}
   SPair reverse(){return(SPair(p2,p1));}
   SPoint p1,p2;
};

bool operator <(SPoint p1, SPoint p2);
bool operator <(SPair p1,SPair p2);
bool operator ==(SPoint p1,SPoint p2);
bool operator !=(SPoint p1,SPoint p2);
SPoint operator +=(SPoint p, SVector v);

#endif
