
positive=$1
negative=$2
dir=CNS
test_sample=$3

python3 data_format.py $dir/$dir.txt $dir/ca.txt $dir/ci.txt $positive $negative
./gaia -t $test_sample

rm libsvm-3.22/test.txt
rm libsvm-3.22/train.txt

tail -$test_sample svmResult.txt >> libsvm-3.22/test.txt
head -n`expr $positive + $negative` svmResult.txt >> libsvm-3.22/train.txt

cd libsvm-3.22
./svm-train -t 0 train.txt script.svm
./svm-predict test.txt script.svm pred.txt
python3 get_Fscore.py $test_sample