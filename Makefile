CXX=/home/ddavis/Software/gcc/9.2.0/bin/c++

all:
	$(CXX) -static-libstdc++ -march=x86-64 -m64 -O3 -Wall -shared -std=c++17 -fPIC `python3 -m pybind11 --includes` stdfs.cpp -o stdfs`python3-config --extension-suffix`; strip stdfs*so
