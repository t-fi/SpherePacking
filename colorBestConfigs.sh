for i in {3..200}
do
  python colorNeighbours.py -i data/sphere/numParticles\:${i}/seed\:0/iteration:0000000000/coordinatesQhull.dat
done
exit
