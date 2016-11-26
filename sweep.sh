#!/bin/bash

for j in {1..10}
do
	for i in {1..3}
	do
		nohup ./packSphere ${j} 10000000 1 1 $i > data/${i}_${j}.dat 2> data/results.dat &
	done
done

exit
