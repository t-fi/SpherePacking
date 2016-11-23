reset

unset key

set xlabel ""

set ylabel "radius"

#set logscale y

set logscale x

#set xrange [1:]

plot for [i=0:7] sprintf("data/%d.dat",i) u 2:(1.414214-$3) w lines
