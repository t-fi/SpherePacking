set term pngcairo
unset key

set xrange [-5:15]
set yrange [-5:15]

set print "-"
print "reading files!"

files = system('ls data/*.dat')
i=0

system('rm images/*')

do for [file in files] {
	set output sprintf("images/img%05d.png",i)
	i=i+1
	plot file u 1:2 pt 7 ps 1
}
