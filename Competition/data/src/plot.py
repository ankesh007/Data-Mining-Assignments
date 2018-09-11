import sys
import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt(sys.argv[1])
minsup = [1,5,10,25,50,90]
plt.figure()
plt.plot(minsup,data[:,0],"b-", label='Apriori')
plt.plot(minsup,data[:,1],"g-", label='FPTree')
plt.legend()
plt.show()
plt.savefig("plt.png")