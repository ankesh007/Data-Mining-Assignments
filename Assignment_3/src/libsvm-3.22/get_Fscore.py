import numpy as np
import sys
tot=int(sys.argv[1])
a=np.genfromtxt('./pred.txt')
b=np.ones((tot))
b[tot//2:]*=-1

from sklearn.metrics import f1_score
print(f1_score(b, a, average='macro'))
print(f1_score(b, a, average=None))
