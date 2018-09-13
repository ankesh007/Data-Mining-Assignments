# import matplotlib
# matplotlib.use("Agg")
import matplotlib.pyplot as plt

y1 = []
with open ('apriori_runtime.txt') as f:
	y1 = [float(x) for x in f]

y2 = []
with open ('fptree_runtime.txt') as f:
	y2 = [float(x) for x in f]

x= [1,5,10,25,50,90]

plt.plot(x, y1, label='Apriori')
plt.plot(x, y2, label='Fptree')
plt.ylabel('Time (in sec)')
plt.xlabel('Threshold (in percentage)')
plt.legend()
plt.show()