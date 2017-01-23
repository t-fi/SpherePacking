reset

set logscale x

set grid

set key bottom right

set term svg enhanced size 400,300

set xrange [2.8 :2200]

set xlabel "Number of particles"
set ylabel "MC packing density"

set output "Sphere_vs_Torus.svg"

plot "data/bestResults_torus.dat" u 3:7 t "Torus" ps 0.5, "data/bestResults_sphere.dat" u 3:7 t "Sphere" ps 0.5, 0.9069 t "2D flat"


unset logscale x
set xrange [2.8 :85]
set yrange [0.73:0.91]

set ylabel "Exact packing density"


set output "Sphere_vs_Literature.svg"
plot "data/literature_sphere.dat" u 1:3 t "Literature" pt 1 ps 0.5, "data/literature_sphere.dat" u 1:5 t "Literature (energy)" pt 1 ps 0.5,"data/bestResults_sphere.dat" u 3:6 t "this work" pt 2 ps 0.5, 0.9069 t "2D flat"
set output ".dummy"
