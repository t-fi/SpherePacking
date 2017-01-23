#!/bin/bash
mkdir -p "data/progress"
for pCount in {81..99}
do
	for seed in {0..0}
	do
		nohup ./packSphere ${pCount} 1000000000 50 0.02 ${seed} > /dev/null 2> data/progress/Sphere${pCount}_${seed}.dat < /dev/null &
		nohup ./packTorus ${pCount} 1000000000 50 0.02 ${seed} > /dev/null 2> data/progress/Torus${pCount}_${seed}.dat < /dev/null &
	done
done
exit
