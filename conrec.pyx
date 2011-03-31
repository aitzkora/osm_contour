#
#   Derivation from the fortran version of CONREC by Paul Bourke
#   d               ! matrix of data to contour
#   ilb,iub,jlb,jub ! index bounds of data matrix
#   x               ! data matrix column coordinates
#   y               ! data matrix row coordinates
#   nc              ! number of contour levels
#   z               ! contour levels in increasing order
#

cdef inline double xsect(p1,p2):
    return (h[p2]*xh[p1]-h[p1]*xh[p2])/(h[p2]-h[p1])
cdef inline double ysect(p1,p2):
    return (h[p2]*yh[p1]-h[p1]*yh[p2])/(h[p2]-h[p1])
cdef inline double MIN(double a, double b) : return a if a <= b  else b
cdef inline double MAX(double a, double b) : return b if a <= b  else a 


import numpy as np
cimport numpy as nc

def Contour(nc.ndarray[double,ndim=2] d,int ilb,int iub,int jlb,int jub,
            nc.ndarray[double,ndim = 1] x,
            nc.ndarray[double,ndim = 1] y,
            int nc, 
            nc.ndarray[double,ndim = 1] z):

   cdef int m1,m2,m3,case_value
   cdef double dmin,dmax,x1=0,x2=0,y1=0,y2=0
   cdef int i,j,k,m
   cdef double h[5]
   cdef int sh[5]
   cdef double xh[5], yh[5]
   cdef int *im = [0, 1, 1, 0]
   cdef int *jm = [0, 0, 1, 1]
   cdef nc.ndarray[double,ndim=3] castab = np.array(
       [[[0,0,8], [0,2,5], [7,6,9]],
       [[0,3,4], [1,3,1], [4,3,0]],
       [[9,6,7], [5,2,0], [8,0,0]]])
   
   cdef double temp1, temp2

   for j from (jub-1) >= j >=jlb:
      for i from ilb <= i <=(iub-1):
         temp1 = MIN(d[i][j],d[i][j+1])
         temp2 = MIN(d[i+1][j],d[i+1][j+1])
         dmin  = MIN(temp1,temp2)
         temp1 = MAX(d[i][j],d[i][j+1])
         temp2 = MAX(d[i+1][j],d[i+1][j+1])
         dmax  = MAX(temp1,temp2);
         if (dmax < z[0]) or ( dmin > z[nc-1]):
            continue
         for k from 0<= k <nc:
            if (z[k] < dmin ) or ( z[k] > dmax):
               continue
            for m from 4>= m >=0: 
               if (m > 0): 
                  h[m]  = d[i+im[m-1]][j+jm[m-1]]-z[k]
                  xh[m] = x[i+im[m-1]]
                  yh[m] = y[j+jm[m-1]]
               else: 
                  h[0]  = 0.25 * (h[1]+h[2]+h[3]+h[4])
                  xh[0] = 0.50 * (x[i]+x[i+1])
                  yh[0] = 0.50 * (y[j]+y[j+1])
               if (h[m] > 0.0):
                  sh[m] = 1
               elif (h[m] < 0.0):
                  sh[m] = -1
               else:
                  sh[m] = 0
            # Scan each triangle in the box 
            for m from 1 <= m <= 4:
               m1 = m
               m2 = 0
               if (m != 4):
                  m3 = m + 1
               else:
                  m3 = 1
               #if ((case_value = castab[sh[m1]+1][sh[m2]+1][sh[m3]+1]) == 0):
               #   continue
               switch (case_value) 
               if case_value=1: # Line between vertices 1 and 2 
                   x1 = xh[m1]
                   y1 = yh[m1]
                   x2 = xh[m2]
                   y2 = yh[m2]
                   break
                elif case_value=2: # Line between vertices 2 and 3 
                   x1 = xh[m2];
                   y1 = yh[m2];
                   x2 = xh[m3];
                   y2 = yh[m3];
                   break;
               elif case_value= 3: # Line between vertices 3 and 1 
                   x1 = xh[m3];
                   y1 = yh[m3];
                   x2 = xh[m1];
                   y2 = yh[m1];
                   break;
               elif case_value= 4: # Line between vertex 1 and side 2-3 
                   x1 = xh[m1];
                   y1 = yh[m1];
                   x2 = xsect(m2,m3);
                   y2 = ysect(m2,m3);
                   break;
               elif case_value= 5: # Line between vertex 2 and side 3-1 
                   x1 = xh[m2];
                   y1 = yh[m2];
                   x2 = xsect(m3,m1);
                   y2 = ysect(m3,m1);
               elif case_value= 6: # Line between vertex 3 and side 1-2 */
                   x1 = xh[m3];
                   y1 = yh[m3];
                   x2 = xsect(m1,m2);
                   y2 = ysect(m1,m2);
               elif case_value= 7: # Line between sides 1-2 and 2-3 */
                   x1 = xsect(m1,m2);
                   y1 = ysect(m1,m2);
                   x2 = xsect(m2,m3);
                   y2 = ysect(m2,m3);
               elif case_value= 8: # Line between sides 2-3 and 3-1 */
                   x1 = xsect(m2,m3);
                   y1 = ysect(m2,m3);
                   x2 = xsect(m3,m1);
                   y2 = ysect(m3,m1);
               elif case_value= 9: # Line between sides 3-1 and 1-2 */
                   x1 = xsect(m3,m1);
                   y1 = ysect(m3,m1);
                   x2 = xsect(m1,m2);
                   y2 = ysect(m1,m2);
               end 

               # Finally draw the line */
               ConrecLine(x1,y1,x2,y2,z[k]);
