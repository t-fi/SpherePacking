#!/bin/bash

for j in {3..100}
do
		nohup ./packSphere ${j} 100000000 1 1 $1 > data/sphere_${i}_$1.dat 2>> data/resultsSphere.dat &
		nohup ./packTorus ${j} 100000000 1 1 $1 > data/torus_${i}_$1.dat 2>> data/resultsTorus.dat &
done

exit
