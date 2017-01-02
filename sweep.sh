#!/bin/bash
for seed in {0..100}
do
	for pCount in {3..100}
	do
		nohup ./packSphere ${pCount} 100000 50 0.02 ${seed} > /dev/null 2>> data/resultsSphere.dat < /dev/null &
		nohup ./packTorus ${pCount} 100000 50 0.02 ${seed} > /dev/null 2>> data/resultsTorus.dat < /dev/null &
	done
done
exit
