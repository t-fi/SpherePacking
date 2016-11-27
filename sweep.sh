#!/bin/bash

for j in {3..100}
do
		nohup ./packSphere ${j} 100000000 50 0.02 $1 > data/sphere_${j}_$1.dat 2>> data/resultsSphere_$1.dat &
		nohup ./packTorus ${j} 100000000 50 0.02 $1 > data/torus_${j}_$1.dat 2>> data/resultsTorus_$1.dat &
done

exit
