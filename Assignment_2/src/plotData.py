import matplotlib.pyplot as plt

outFile = open("data.txt")
lines = outFile.readlines()
pointX = []
pointY = []
for i in range(0, len(lines)):
	values = lines[i].strip().split(" ")
	pointX = pointX + [float(values[0])]
	pointY = pointY + [float(values[1])]

plt.xlim(0,0.1)
plt.ylim(0,1.81)
plt.plot(pointX, pointY, 'ro')
# plt.axis([-10,45,-10,45])
plt.show()
