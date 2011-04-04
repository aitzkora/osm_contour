#!/usr/bin/python
import string
import numpy as np
def extract_data(fichier_asc="srtm_38_03.asc", 
                 lat_min=48.2299, 
		 long_min=6.8489,
		 lat_max=48.3343,
		 long_max=7.0514):
    """
    
    filter which extract the sub area defined by 
    [long_min,long_max]x[lat_min,lat_max] in the srtm tile
    contained in the file fichier_asc
    """
    fid=open(fichier_asc,'r')
    
    # header reading
    head=6*[None]
    for i in range(6):
       head[i]=fid.readline().split()[1];
    
    ncols = string.atoi(head[0])
    nrows = string.atoi(head[1])
    xllcorner = string.atoi(head[2])
    yllcorner = string.atoi(head[3])
    cellsize = string.atof(head[4])
    NODATA =  string.atoi(head[5])
    # print header data
    print ('cols = %d\nrows = %d\nx_cor = %d\n'+\
           'y_cor = %d\ncell = %f\nNODATA= %d') \
	   % (ncols, nrows, xllcorner, yllcorner , cellsize, NODATA)
    # computing the window
    first_col = int((long_min - xllcorner) / cellsize)
    last_col = int((long_max - xllcorner) / cellsize)
    first_row = nrows - int((lat_max - yllcorner) / cellsize)
    last_row = nrows - int((lat_min - yllcorner) / cellsize)

    new_ncols = last_col - first_col
    new_nrows = last_row - first_row

    new_xllcorner = xllcorner + first_col * cellsize
    new_yllcorner = yllcorner + (nrows-last_row) * cellsize
  
    print 'Extracting columns %d-%d from rows %d-%d' %\
          (first_col, last_col, first_row, last_row)

    # construct the submatrix of elevation 
    elevation = np.zeros((last_row-first_row+1,last_col-first_col+1),dtype='float')
    for i in range(last_row+1):
        line = fid.readline().split()
	if i >= first_row:
	    for j in range(first_col,last_col):
	         elevation[i - first_row ,j-first_col]=line[j]

    fid.close()

    return [new_ncols, new_nrows, 
            new_xllcorner, new_yllcorner,
	    cellsize , elevation]

def convert(fichier_osm="data.osm",
             fichier_asc="srtm_38_03.asc", 
             lat_min=48.2299, 
	     long_min=6.8489,
     	     lat_max=48.3343,
  	     long_max=7.0514):
     	     
    [nc, nr, xc, yc, cs, elev] =\
        extract_data( fichier_asc, lat_min, long_min, lat_max, long_max)
    
    m=elev.shape[0]
    n=elev.shape[1]

    import matplotlib
    import matplotlib.pyplot as plt

    x  = np.arange(n * 1.) * cs + xc;
    y  = (m - np.arange(m * 1.)) * cs + yc;
    v  = np.mgrid[-500:9000:25.]

    plt.figure()
    CS = plt.contour(x, y, elev, v)
    plt.show()
   
    import contour
    list_levels = contour.contour(elev, x , y, v)
    
if __name__ == '__main__':
     convert() 
