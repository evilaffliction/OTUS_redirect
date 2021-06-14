sudo apt-get install -y libmicrohttpd-dev

git clone https://github.com/etr/libhttpserver.git && \
cd libhttpserver && \
./bootstrap && ./bootstrap && \
mkdir build && \
cd build && \
../configure && \
make && \
sudo make install 