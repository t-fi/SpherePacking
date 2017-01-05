#!/bin/bash
mkdir -p "data/progress"
for pCount in {3..10}
do
	for seed in {0..2}
	do
		nohup ./packSphere ${pCount} 100000 50 0.02 ${seed} > /dev/null 2>> data/progress/Sphere${pCount}_${seed}.dat < /dev/null &
		nohup ./packTorus ${pCount} 100000 50 0.02 ${seed} > /dev/null 2>> data/progress/Torus${pCount}_${seed}.dat < /dev/null &
	done
done
exit
