from matplotlib import pyplot as plt
import timeit
import sys
import subprocess
# sh <RollNo>.sh <inputfile> -plot

inputfile = sys.argv[1]
thresholds = [1, 5, 10, 25, 50, 90]
# assuming fptree takes arguments X inputfile outfilename
execution_times_apriori = [timeit.timeit("subprocess.run(\"./apriori " + str(x) + " " + inputfile + " output\", shell=True)", setup = "import subprocess", number=1) for x in thresholds]
execution_times_fptree = [timeit.timeit("subprocess.run(\"./fptree " + inputfile + " " + str(x) + " output\", shell=True)", setup = "import subprocess", number=1) for x in thresholds]

plt.figure()
plt.plot(thresholds, execution_times_apriori, label='apriori')
plt.plot(thresholds, execution_times_fptree, label='fptree')
plt.title('Execution time comparison')
plt.xlabel('Support threshold')
plt.ylabel('Execution Time (s)')
plt.legend()
plt.show()
