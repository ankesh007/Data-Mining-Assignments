import matplotlib.pyplot as plt
import sys

outFile = open(sys.argv[1])
lines = outFile.readlines()
pointX = []
pointY = []
for i in range(0, len(lines)):
	values = lines[i].strip().split(" ")
	pointX.append(float(values[0]))
	pointY.append(float(values[1]))

plt.xlim(-20,550)
plt.ylim(-20,220)
plt.plot(pointX, pointY, 'go')
# plt.axis([-10,45,-10,45])
plt.show()
