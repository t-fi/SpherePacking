#!/bin/bash

rm data/*.dat

for i in {1..7}
do
   nohup nice -n 1 ./packSphere 6 1000000000 1 1 $i > data/$i.dat 2> /dev/null &
done

time nice -n 2 ./packSphere 6 1000000000 1 1 0 > data/0.dat

exit
