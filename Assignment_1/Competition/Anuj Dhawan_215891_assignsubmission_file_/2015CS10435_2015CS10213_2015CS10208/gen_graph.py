import sys
import os
import matplotlib.pyplot as plt

input_dir = sys.argv[1]
command1 = "./apriori "+ input_dir + " out " 
command2 = "./FP_Tree "+ input_dir + " "
thresholds = [1,5,10,25,50,90]
for threshold in thresholds:
	if threshold==1:
		command_apriori = command1 + str(threshold) +" 1 1"  
		os.system(command_apriori)
		command_fptree = command2 + str(threshold) + " out 1 1"
		os.system(command_fptree)
	else:
		command_apriori = command1 + str(threshold) +" 1 0"  
		os.system(command_apriori)
		command_fptree = command2 + str(threshold) + " out 1 0"
		os.system(command_fptree)

time_apriori = []
time_fptree = []

with open("time_apriori") as f1:
	content1 = f1.readlines()

for content in content1:
	if content!="":
		time_apriori.append(float(content))


with open("time_fptree") as f2:
	content2 = f2.readlines()

for content in content2:
	if content!="":
		time_fptree.append(float(content))


plt.xlabel("Thresholds (%)")
plt.ylabel("Running Time (seconds)")
plt.title("Comparison of Running Times of Apriori and Fptree")



lines1 = plt.plot(thresholds,time_apriori)
lines2 = plt.plot(thresholds,time_fptree)
plt.setp(lines1, color='r', linewidth=1.5, label="Apriori")
plt.setp(lines2, color='b', linewidth=1.5, label="FpTree")
plt.legend()
plt.show()
