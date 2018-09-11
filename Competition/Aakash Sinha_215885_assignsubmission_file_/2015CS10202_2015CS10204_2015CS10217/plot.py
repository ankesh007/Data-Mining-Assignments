import matplotlib.pyplot as plt
import commands,sys
import numpy as np
import time
in_file=sys.argv[1]
xlist=[1,5,10,25,50,90]
out_list1=[]
out_list2=[]
for x in xlist:
	start = time.time()
	comm = "./2015CS10202.sh "+ str(in_file) + " " + str(x) + " -apriori temp"
	status, output = commands.getstatusoutput(comm)
	end = time.time()
	ou = end-start
	out_list1.append(ou)

for x in xlist:
	start = time.time()
	comm = "./2015CS10202.sh "+ str(in_file) + " " + str(x) + " -fptree temp"
	status, output = commands.getstatusoutput(comm)
	end = time.time()
	ou = end-start
	out_list2.append(ou)

fig = plt.figure()
plt.title('Apriori vs Fptree')
plt.xlabel('Threshold')
plt.ylabel('Time')
plt.plot(xlist,out_list1,color='r',marker='x',label="apriori")
plt.plot(xlist,out_list2,color='b',marker='o',label="fptree")
plt.legend()
plt.show()