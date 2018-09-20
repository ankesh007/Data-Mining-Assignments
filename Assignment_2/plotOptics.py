import matplotlib.pyplot as plt

outFile = open("out.txt")
outContent = outFile.readlines()
outContent = list(map(float, outContent))
maxVal = max(outContent)
for i in range(0, len(outContent)):
	if (outContent[i]== -1):
		outContent[i] = 2*maxVal

plt.plot(outContent)
plt.ylabel('Reachability Distance')
plt.xlabel('Data Points')
plt.show()