make clean
rm -rf "node_file.txt"
rm -rf "edge_file.txt"
rm -rf "train_test.svm"
make GAIA
cd libsvm-3.22/
make clean
make