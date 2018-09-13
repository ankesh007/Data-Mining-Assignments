import matplotlib.pyplot as plt
y=[]
with open("output_result.txt") as file:
	for line in file:
		y.append(float(line.rstrip()))
x=[1,5,10,25,50,90]
plt.xlabel('support threshold')
plt.ylabel('corresponding running time')
plt.plot(x,y)
plt.savefig("result.jpg")

y=[]
with open("output_result2.txt") as file:
	for line in file:
		y.append(float(line.rstrip()))
x=[1,5,10,25,50,90]
plt.xlabel('support threshold')
plt.ylabel('corresponding running time')
plt.plot(x,y)
plt.savefig("result2.jpg")