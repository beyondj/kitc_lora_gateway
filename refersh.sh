cp vimrc /home/pi/.vimrc
rm -rf build
mkdir build
cd build
cmake ..
make -j
