import os
import time
import subprocess
import sys
import matplotlib.pyplot as plt



X = [1,5,10,25,50,90]
Y_ap = []
Y_fp = []
inp_file = sys.argv[1]

for x in X:
	start = time.time()
	subprocess.call(["./apriori", inp_file, str(x), "temp_data_ap" + str(x)])
	elapsed_time_fl = (time.time() - start)
	Y_ap.append(elapsed_time_fl)

	start = time.time()
	subprocess.call(["./fptree", inp_file, str(x), "temp_data_fp" + str(x)])	
	elapsed_time_fl = (time.time() - start)
	Y_fp.append(elapsed_time_fl)

plt.plot(X, Y_ap, 'r--', X, Y_fp, 'b--')
plt.gca().legend(('apriori','fptree'))
plt.savefig('comp.jpeg')
plt.clf()
plt.plot(X, Y_ap, 'r--')
plt.savefig('apriori.jpeg')
plt.clf()
plt.plot(X, Y_fp, 'r--')
plt.savefig('fptree.jpeg')
