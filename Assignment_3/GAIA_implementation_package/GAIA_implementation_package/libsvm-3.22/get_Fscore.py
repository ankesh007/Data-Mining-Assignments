import numpy as np

tot=144
a=np.genfromtxt('./pred.txt')
b=np.ones((tot))
b[tot//2:]*=-1

from sklearn.metrics import f1_score
print(f1_score(b, a, average='macro'))
print(f1_score(b, a, average=None))
