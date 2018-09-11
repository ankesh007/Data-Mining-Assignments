import matplotlib.pyplot as pt
import sys
import subprocess
s1 = float(subprocess.check_output([sys.executable, "fpt.py",sys.argv[1], "1", "ptp","out.txt"])[0:-1])

s2 = float(subprocess.check_output([sys.executable, "fpt.py",sys.argv[1], "5", "ptp","out.txt"])[0:-1])
s3 = float(subprocess.check_output([sys.executable, "fpt.py",sys.argv[1], "10", "ptp","out.txt"])[0:-1])
s4 = float(subprocess.check_output([sys.executable, "fpt.py",sys.argv[1], "25", "ptp","out.txt"])[0:-1])
s5 = float(subprocess.check_output([sys.executable, "fpt.py",sys.argv[1], "50", "ptp","out.txt"])[0:-1])
s6 = float(subprocess.check_output([sys.executable, "fpt.py",sys.argv[1], "90", "ptp","out.txt"])[0:-1])

a1 = float(subprocess.check_output([sys.executable, "Apriori.py",sys.argv[1], "1", "ptp","out.txt"])[0:-1])

a2 = float(subprocess.check_output([sys.executable, "Apriori.py",sys.argv[1], "5", "ptp","out.txt"])[0:-1])
a3 = float(subprocess.check_output([sys.executable, "Apriori.py",sys.argv[1], "10", "ptp","out.txt"])[0:-1])
a4 = float(subprocess.check_output([sys.executable, "Apriori.py",sys.argv[1], "25", "ptp","out.txt"])[0:-1])
a5 = float(subprocess.check_output([sys.executable, "Apriori.py",sys.argv[1], "50", "ptp","out.txt"])[0:-1])
a6 = float(subprocess.check_output([sys.executable, "Apriori.py",sys.argv[1], "90", "ptp","out.txt"])[0:-1])
# fp= [s1, s2, s3, s4, s5, s6]
# ap= [a1, a2, a3, a4, a5, a6]

# x=[1,5, 10, 25, 50, 90]
# print (fp)
# print(ap)
# pt.plot(x,fp,"-",lable= "FP-Tree")

# pt.plot(x,ap,"-",lable= "Apriori")
# pt.legend(loc='best')
# pt.show()
import matplotlib.pyplot as plt
x = [1,5,10,25,50,90]
y = [[ s1,s2, s3, s4, s5, s6],[ a1,a2, a3, a4, a5, a6]]
plt.xlabel("Minimun Support")
plt.ylabel("Time in seconds")
plt.title("Compariosn Between FP-Tree and Apriori")
s=["FP-Tree", "Apriori"]
for i in range(len(y)):
    plt.plot(x,y[i],label = s[i])
plt.legend()
plt.show()
# import Apriori.py sys.argv[1] 1 "apriori" "oout.txt" 
# import fpt.py as fpt
# import os
# str2="fpt.py"
# str2+=" "+sys.argv[1]+"fpt"

# fpttime={os.system(str2+" "+str(1)+"out.txt"),os.system(str2+" "+str(5)+"out.txt"),os.system(str2+" "+str(10)+"out.txt"),os.system(str2+" "+str(25)+"out.txt"),os.system(str2+" "+str(50)+"out.txt"),os.system(str2+" "+str(90)+"out.txt")}
# print(fpttime)
# str1="Apriori.py"
# str1+=" "+sys.argv[1]
# aptime={os.system(str1+" "+str(1)+"out.txt").t,os.system(str1+" "+str(5)+"out.txt").t,os.system(str1+" "+str(10)+"out.txt").t,os.system(str1+" "+str(25)+"out.txt").t,os.system(str1+" "+str(50)+"out.txt").t,os.system(str1+" "+str(90)+"out.txt").t}

# print(fpttime)
# print(aptime)


