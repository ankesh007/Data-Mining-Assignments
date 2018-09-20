
epsilon_collection=(1 10 30 100 300 1000)
minpts_collection=(3 5 10 15 30)
files=(1 2 3 4 5)
make clean
make DBSCAN
mkdir -p ../ignore/output

for eps in ${epsilon_collection[@]}
do
	for minpts in ${minpts_collection[@]}
	do
		for file in ${files[@]}
		do
			filename="data_$file.txt"
			temp="_eps$eps""_minpts$minpts""_filenum$file"
			# echo $filename
			# echo $temp
			echo $temp
			time ./DBSCAN $minpts $eps ../data/$filename ../ignore/output/$temp".txt" 2>&1 | tee -a ../ignore/log.txt 
		done
	done
done