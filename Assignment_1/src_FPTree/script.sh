time ./FP_Tree ../Dataset/retail.dat 5 new_output5.txt
time ./FP_Tree ../Dataset/retail.dat 1 new_output1.txt
time ./FP_Tree ../Dataset/retail.dat 0.1 new_output01.txt
time ./FP_Tree ../Dataset/retail.dat 0.08 new_output008.txt

colordiff new_output008.txt output008.txt
colordiff new_output01.txt output01.txt
colordiff new_output1.txt output1.txt
colordiff new_output5.txt output5.txt
