import contour
import make_contours
import numpy as np
import matplotlib
import matplotlib.pyplot as plt

[nc,nr,xl,yl,cs,mat]=make_contours.extract_data()
m=mat.shape[0]
n=mat.shape[1]
x=np.array(range(n),dtype='float')
y=np.array(range(m),dtype='float')
z=np.mgrid[mat.min():mat.max():10*1.j]
plt.figure()
CS = plt.contour(x, y, mat)
plt.show()
l=contour.contour(mat,x,y,z)
clefs=l.keys()
# on trie les chemins
for i in range(clefs.__len__()):
   l[clefs[i]].sort()
