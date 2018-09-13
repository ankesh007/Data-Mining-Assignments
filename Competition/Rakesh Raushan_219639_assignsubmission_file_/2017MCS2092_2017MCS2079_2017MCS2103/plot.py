import sys
import matplotlib.pyplot as plt
import subprocess

apriori = "./Apriori/apriori_plot.cpp"

fp_time = []
apriori_time = []
filename = str(sys.argv[1])
threshold = ["1","5","10","25","50","90"]

subprocess.call(["g++","-O3","-std=c++14","./F_Plot/fp_tree.cpp", "./F_Plot/Build_Cond_FP_Tree.cpp", "./F_Plot/build_FP_Tree.cpp", "./F_Plot/Build_Tree.cpp", "./F_Plot/fp_tree.h", "./F_Plot/freq_sort.cpp", "./F_Plot/trans_freq_count.cpp"])

for i in threshold:
    subprocess.call(["./a.out",filename,i,"out.txt"])


subprocess.call(["g++","-O3","-std=c++14",apriori])
for i in threshold:
    subprocess.call(["./a.out",filename,i,"out.txt"])

fp_file = open("fp_time.txt","r")
for line in fp_file:
    fp_time.append(float(line.strip()))

apriori_file = open("a_plot_time.txt","r")
for line in apriori_file:
    apriori_time.append(float(line.strip()))

thresh = [1,5,10,25,50,90]


#print apriori_time
plt.plot(thresh, fp_time, 'r--',marker='+', label='FP-Tree')
plt.plot(thresh, apriori_time, 'g--', marker='*', label='Apriori' )
plt.ylabel('Time (in sec)')
plt.xlabel('Threshold (in %)')
plt.title('FP Tree Vs Apriori')
plt.legend()
plt.show()


