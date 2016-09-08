all:
	g++ -O3 -std=c++11 main.cpp simulator.cpp point.cpp -Dsphere -o packSphere
	g++ -O3 -std=c++11 main.cpp simulator.cpp point.cpp -Dtorus -o packTorus
