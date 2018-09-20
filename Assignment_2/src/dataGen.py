x0 = 0
y0 = 0
x1 = 0
y1 = 1.8
d0 = d1 = 0.01

for i in range(0,100):
	for j in range(0,100):
		x = x0 + d0*i
		y = y0 + d0*j
		print(str(x) + " "+ str(y))
		x = x1 + d1*i
		y = y1 + d1*j
		print(str(x) + " "+ str(y))
		

x2 = 6
y2 = 0
d2 = 1
for i in range(0,400):
	for j in range(0,200):
		x = x2 + d2*i
		y = y2 + d2*j
		print(str(x) + " "+ str(y))
