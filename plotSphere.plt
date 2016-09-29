set term pngcairo size 1400,1400
set view equal xyz
set hidden3d
set parametric
set urange [0:2*pi]
set vrange [0:pi]
set isosamples 20,20
unset key

set xrange [-2:2]
set yrange [-2:2]
set zrange [-2:2]

set print "-"
print "reading files!"

files = system('ls data/*.dat')
i=0

system('rm images/*')

do for [file in files] {
	set output sprintf("images/img%05d.png",i)
	i=i+1
	splot 0.99*sin(v)*cos(u), 0.99*sin(v)*sin(u), 0.99*cos(v), file u 3:4:5 pt 7 ps 1
	#splot file u 3:4:5 pt 7
}

