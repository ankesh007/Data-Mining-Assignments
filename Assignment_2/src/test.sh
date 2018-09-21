output_dir='../ignore/output2'
log_path='../ignore/log2.txt'

epsilon_collection=(1 10 30 100 300 500)
minpts_collection=(3 5 10 15 30)
files=(1 2 3 4 5)
# make clean
make DBSCAN
mkdir -p $output_dir

for eps in ${epsilon_collection[@]}
do
	for minpts in ${minpts_collection[@]}
	do
		for file in ${files[@]}
		do
			filename="data_$file.txt"
			temp="_eps$eps""_minpts$minpts""_$filename"
			# echo $filename
			# echo $temp
			echo $temp | tee -a $log_path 
			{ time ./DBSCAN $minpts $eps ../data/$filename $output_dir/$temp; } 2>&1 | tee -a $log_path 
		done
	done
done