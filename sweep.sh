#!/bin/bash
mkdir -p "data/progress"
for pCount in {2..50}
do
	for seed in {1204..1204}
	do
		nohup ./packSphere ${pCount} 10000000 50 0.02 ${seed} > /dev/null 2> data/progress/Sphere${pCount}_${seed}.dat < /dev/null &
		nohup ./packTorus ${pCount} 10000000 50 0.02 ${seed} > /dev/null 2> data/progress/Torus${pCount}_${seed}.dat < /dev/null &
	done
done
exit
