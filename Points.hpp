#ifndef POINTS_H
#define POINTS_H

struct SVector
{
   double dx,dy;
};

struct SPoint
{
   SPoint(double a,double b){ x=a; y=b; }
   SPoint(){}
   double x,y;
};

struct SPair
{
   SPair(SPoint _p1, SPoint _p2){p1=_p1; p2=_p2;}
   SPair reverse(){return SPair(p2,p1) ;}
   SPoint p1,p2;
};

#define DIFFERENCE 0.0005
#define EQ(_x_,_y_) (((_x_-_y_<DIFFERENCE)&&(_y_-_x_<DIFFERENCE))?1:0)


inline bool operator < (SPoint p1, SPoint p2){
     return(((p1.x*(unsigned int)0xFFFFFFFF)+p1.y)<((p2.x*(unsigned int)0xFFFFFFFF)+p2.y));
}
inline bool operator <(SPair p1, SPair p2){
      return(p1.p1 < p2.p1);
}
inline bool operator ==(SPoint p1, SPoint p2){ 
      return((EQ(p1.x,p2.x))&&(EQ(p1.y,p2.y)));
}
inline bool operator !=(SPoint p1, SPoint p2){ 
      return !(EQ(p1.x,p2.x)&&!(EQ(p1.y,p2.y)));
}
inline SPoint operator +=(SPoint p, SVector v){ 
       return SPoint(p.x += v.dx, p.y += v.dy);
}
#endif
