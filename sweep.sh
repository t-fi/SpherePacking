#!/bin/bash

rm data/*.dat
for j in {1..10}
do
	for i in {1..10}
	do
		nohup nice -n 1 ./packSphere 6 10000000 $((j * 0.2)) $((2 / j)) $i > data/${i}_${j}.dat 2> /dev/null &
	done
done

time nice -n 2 ./packSphere 6 10000000 1 1 0 > data/0_0.dat

exit
