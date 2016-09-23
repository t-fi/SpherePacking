set term pngcairo 
set view equal xyz
set hidden3d
set parametric
set urange [0:2*pi]
set vrange [0:2*pi]
set isosamples 20,20
unset key

set xrange [-4:4]
set yrange [-4:4]
set zrange [-4:4]

set print "-"
print "reading files!"

files = system('ls data/*.dat')
i=0

system('rm images/*')

do for [file in files] {
	set output sprintf("images/img%05d.png",i)
	i=i+1
	splot (2+0.99*cos(v))*cos(u), (2+0.99*cos(v))*sin(u), 0.99*sin(v), file u 3:4:5 pt 7
	#splot file u 3:4:5 pt 7
}

