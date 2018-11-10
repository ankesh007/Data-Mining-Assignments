git clone https://github.com/ankesh007/Data-Mining-Assignments.git

pushd .

install_path="$HOME/.local/lib"
bashrc_path="$HOME/.bashrc"

echo $install_path
mkdir -p $install_path
cd $install_path


wget https://dl.bintray.com/boostorg/release/1.68.0/source/boost_1_68_0.tar.bz2
# sudo apt install dtrx
tar xvjf boost_1_68_0.tar.bz2

cd boost_1_68_0
./bootstrap.sh
./b2

echo "" >> $bashrc_path
echo "LD_LIBRARY_PATH=\"$install_path/boost_1_68_0/stage/lib\":$LD_LIBRARY_PATH" >> $bashrc_path
echo "CPLUS_INCLUDE_PATH=\"$install_path/boost_1_68_0\":$CPLUS_INCLUDE_PATH" >> $bashrc_path
echo "export CPLUS_INCLUDE_PATH" >> $bashrc_path
echo "export LD_LIBRARY_PATH" >> $bashrc_path

popd