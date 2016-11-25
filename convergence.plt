reset

#unset key

set xlabel ""

set ylabel "radius"

#set logscale y

set logscale x

#set xrange [1:]

plot for [j=1:10] for [i=1:10] sprintf("data/%d_%d.dat",i,j) u 2:(1.414214-$3) w l lc 1 t sprintf("params %d",j)
