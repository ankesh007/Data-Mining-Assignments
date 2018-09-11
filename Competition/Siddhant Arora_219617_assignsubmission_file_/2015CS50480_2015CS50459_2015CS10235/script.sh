./fp retail.txt 1 output1.txt >> output_result.txt
./fp retail.txt 5 output1.txt >> output_result.txt
./fp retail.txt 10 output1.txt >> output_result.txt
./fp retail.txt 25 output1.txt >> output_result.txt
./fp retail.txt 50 output1.txt >> output_result.txt
./fp retail.txt 90 output1.txt >> output_result.txt
./apriori retail.txt 1 output2.txt >> output_result2.txt
./apriori retail.txt 5 output2.txt >> output_result2.txt
./apriori retail.txt 10 output2.txt >> output_result2.txt
./apriori retail.txt 25 output2.txt >> output_result2.txt
./apriori retail.txt 50 output2.txt >> output_result2.txt
./apriori retail.txt 90 output2.txt >> output_result2.txt
python3 plot.py
