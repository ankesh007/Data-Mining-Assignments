import random

for d in range(1,6):
	filename = "data_" + str(d)
	f = open(filename, 'w')
	for _ in range(1000000):
		point = ""
		for _ in range(0,d):
			point += str(random.uniform(1,10001))
			point += " "
		point += "\n"
		f.write(point)
	f.close()