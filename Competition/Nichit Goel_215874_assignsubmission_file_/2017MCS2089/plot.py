import matplotlib.pyplot as plt
import numpy as np

with open('time.csv','r') as timefile:
	lines = []
	for line in timefile:
		lines.append(line.rstrip())
	x = [int(i) for i in lines[0].split(',')]
	apriori = [float(i) for i in lines[1].split(',')]
	fptree = [float(i) for i in lines[2].split(',')]

fig = plt.figure()
plt.plot(x,apriori,'b^-',label='Apriori')
plt.plot(x,fptree,'r^-',label='FP Tree')
plt.xlabel('%')
plt.ylabel('Running time (s)')
plt.title('Performance comparison')
plt.legend()
plt.show()