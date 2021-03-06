import matplotlib.pyplot as plt
import sys

outFile = open(sys.argv[1])
lines = outFile.readlines()
xpoints = []
ypoints = []
for line in lines:
	values = line.strip().split(" ")
	xpoints.append(float(values[0]))
	ypoints.append(float(values[1]))

outFile.close()

print("done data")

outFile = open(sys.argv[2])
num_clusters = int(sys.argv[3])
lines = outFile.readlines()

print("done cluster")
pointX = []
pointY = []

for j in range(0,num_clusters):
	pointX.append([])
	pointY.append([])


cluster_number = -1
for line in lines:
	if(line[0]=='#'):
		cluster_number = cluster_number + 1
		continue

	index = int(line)
	pointX[cluster_number].append(xpoints[index])
	pointY[cluster_number].append(ypoints[index])


print("points")

plt.xlim(-60,550)
plt.ylim(-60,220)
plt.plot(pointX[0], pointY[0], 'ro')
plt.plot(pointX[1], pointY[1], 'bs')
plt.plot(pointX[2], pointY[2], 'go')
plt.plot(pointX[3], pointY[3], 'ks')
plt.plot(pointX[4], pointY[4], 'ms')
# plt.axis([-10,45,-10,45])
plt.show()
