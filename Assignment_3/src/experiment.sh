positive=$1
negative=$2
test_sample=$3
dir=$4
libsvm_dir='../libsvm-3.22'

rm -rf node_file.txt
rm -rf edge_file.txt
rm -rf training.matrix
python3 data_format.py $dir/aids.txt $dir/ca.txt $dir/ci.txt $positive $negative
# exit(0)
./gaia $positive 2 100 $test_sample
rm -rf $libsvm_dir/test.txt
rm -rf $libsvm_dir/train.txt
rm -rf $libsvm_dir/pred.txt
rm -rf $libsvm_dir/script.svm
# exit(0)
tail -$test_sample training.matrix >> $libsvm_dir/test.txt
head -n`expr $positive + $negative` training.matrix >> $libsvm_dir/train.txt

cd $libsvm_dir
./svm-train -t 0 train.txt script.svm
./svm-predict test.txt script.svm pred.txt
python3 get_Fscore.py $test_sample
