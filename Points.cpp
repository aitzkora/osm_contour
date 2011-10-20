#include "Points.hpp"

#define DIFFERENCE 0.0005
#define EQ(_x_,_y_) (((_x_-_y_<DIFFERENCE)&&(_y_-_x_<DIFFERENCE))?1:0)


bool operator <(SPoint p1, SPoint p2){return(((p1.x*(unsigned int)0xFFFFFFFF)+p1.y)<((p2.x*(unsigned int)0xFFFFFFFF)+p2.y));}
bool operator <(SPair p1,
                SPair p2){return(p1.p1<p2.p1);}
bool operator ==(SPoint p1,
                 SPoint p2){return((EQ(p1.x,p2.x))&&(EQ(p1.y,p2.y)));}
bool operator !=(SPoint p1,
                 SPoint p2){return(!(EQ(p1.x,p2.x)&&!(EQ(p1.y,p2.y))));}
SPoint operator +=(SPoint p, 
                   SVector v){return(SPoint(p.x+=v.dx,p.y+=v.dy));}


