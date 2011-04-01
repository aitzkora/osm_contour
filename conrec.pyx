cimport numpy as nc
import numpy as np
import cython
#   Derivation from the fortran version of CONREC by Paul Bourke
#   d               ! matrix of data to contour
#   x               ! data matrix column coordinates
#   y               ! data matrix row coordinates
#   nc              ! number of contour levels
#   z               ! contour levels in increasing order
#

@cython.cdivision(True)
cdef inline double xsect(double xh[5], double h[5], int p1, int p2): return (h[p2]*xh[p1]-h[p1]*xh[p2])/(h[p2]-h[p1]) 
@cython.cdivision(True)
cdef inline double ysect(double yh[5], double h[5], int p1, int p2): return (h[p2]*yh[p1]-h[p1]*yh[p2])/(h[p2]-h[p1])

cdef inline double MIN(double a, double b) : return a if a <= b  else b
cdef inline double MAX(double a, double b) : return b if a <= b  else a 


@cython.boundscheck(False)
@cython.wraparound(False)
@cython.cdivision(True)

def contour(nc.ndarray[double,ndim=2] d,
            nc.ndarray[double, ndim = 1] x,
            nc.ndarray[double, ndim = 1] y,
            nc.ndarray[double, ndim = 1] z):

   cdef int m1,m2,m3,case_value
   cdef double dmin,dmax,x1=0,x2=0,y1=0,y2=0
   cdef int i,j,k,m
   cdef double h[5]
   cdef int sh[5]
   cdef double xh[5], yh[5]
   cdef int *im = [0, 1, 1, 0]
   cdef int *jm = [0, 0, 1, 1]
   cdef nc.ndarray[long,ndim=3] castab = np.array( [[[0,0,8], [0,2,5], [7,6,9]],
                                                   [[0,3,4], [1,3,1], [4,3,0]],
                                                   [[9,6,7], [5,2,0], [8,0,0]]],dtype='int64')
   
   cdef double temp1, temp2
   cdef dict l = {} 
   for i from 0 <= i <z.shape[0]:
       l[z[i]]=[]
   for j from (d.shape[1]-1) >= j >=0:
      for i from 0 <= i <=(d.shape[0]-1):
         temp1 = MIN(d[i,j], d[i,j+1])
         temp2 = MIN(d[i+1,j], d[i+1,j+1])
         dmin  = MIN(temp1, temp2)
         temp1 = MAX(d[i,j], d[i,j+1])
         temp2 = MAX(d[i+1,j], d[i+1,j+1])
         dmax  = MAX(temp1, temp2);
         if (dmax < z[0]) or (dmin > z[-1]):
            continue
         for k from  0 <= k <= (z.shape[0]-1):
            if (z[k] < dmin ) or ( z[k] > dmax):
               continue
            for m from 4>= m >=0: 
               if (m > 0): 
                  h[m]  = d[i+im[m-1],j + jm[m-1]]-z[k]
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
               case_value = castab[sh[m1]+1,sh[m2]+1,sh[m3]+1]
               
               if (case_value != 0):
                  [x1, y1, x2, y2] =\
                  {1: [xh[m1], yh[m1], xh[m2], yh[m2]], # 1 and 2  
                   2: [xh[m2], yh[m2], xh[m3], yh[m3]], # 2 and 3 
                   3: [xh[m3], yh[m3], xh[m1], yh[m1]], # 3 and 1 
                   4: [xh[m1], yh[m1], xsect(xh, h, m2, m3), ysect(yh, h, m2, m3)], # vertex 1 and side 2-3 
                   5: [xh[m2], yh[m2], xsect(xh, h, m3, m1), ysect(yh, h, m3, m1)], # vertex 2 and side 3-1 
                   6: [xh[m3], yh[m3], xsect(xh, h, m1, m2), ysect(yh, h, m1, m2)], # vertex 3 and side 1-2 
                   7: [xsect(xh, h, m1, m2), ysect(yh, h, m1, m2), xsect(xh, h, m2, m3), ysect(yh, h, m2,m3)], # sides 1-2 and 2-3 
                   8: [xsect(xh, h, m2, m3), ysect(yh, h, m2, m3), xsect(xh, h, m3, m1), ysect(yh, h, m3,m1)], # sides 2-3 and 3-1 
                   9: [xsect(xh, h, m3, m1), ysect(yh, h, m3, m1), xsect(xh, h, m1, m2), ysect(yh, h, m1,m2)]  # sides 3-1 and 1-2 
                  }[case_value]
               
                  # Finally draw the line */
                  #print "line (%f,%f,%f,%f) z = %f" %(x1,y1,x2,y2,z[k])
                  l[z[k]].append([x1,y1,x2,y2])
		  #ConrecLine(x1,y1,x2,y2,z[k]);
   return l		  
