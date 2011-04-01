import contour
import make_contours
import numpy as np
[nc,nr,xl,yl,cs,mat]=make_contours.extract_data()
m=mat.shape[0]
n=mat.shape[1]
x=np.mgrid[0.:1.:m*1.j]
y=np.mgrid[0.:1.:n*1.j]
z=np.mgrid[mat.min():mat.max():100*1.j]
l=contour.contour(mat,x,y,z)
clefs=l.keys()
# on trie les chemins
for i in range(clefs.__len__()):
   l[clefs[i]].sort()

	
