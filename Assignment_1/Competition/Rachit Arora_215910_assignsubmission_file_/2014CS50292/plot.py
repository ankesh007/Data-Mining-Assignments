import matplotlib.pyplot as plt
import sys

def getSeconds( s ):
	[m,s] = (s.split(":"))
	return (int(m) * 60 + float(s))

with open(sys.argv[1]) as time:
	read = time.readlines()

read = [getSeconds(i) for i in read]

x_val = ["1%", "5%","10%", "25%", "50%", "90%"]
x_l = [0,1,2,3,4,5]
aprior = read[0:6]
fp = read[6:]

fig, ax1 = plt.subplots(2, figsize=(15,15))
plt.subplot(2,1,1)
plt.grid()
plt.title("Running Time of Apriori with varying support")
plt.plot(x_l, aprior, marker='o')
plt.xticks(range(len(x_val)), x_val)
plt.subplot(2,1,2)
plt.grid()
plt.title("Running Time of FPTree with varying support")
plt.plot(x_l, fp, marker='o')
plt.xticks(range(len(x_val)), x_val)
plt.savefig('plot.png')
plt.close()