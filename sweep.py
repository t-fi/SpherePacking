#!/usr/bin/python
import time
from subprocess import Popen, PIPE

def stopWatch(startTime):
    '''From seconds to Days;Hours:Minutes;Seconds'''

    value = time.time()-startTime

    valueD = (((value/365)/24)/60)
    Days = int (valueD)

    valueH = (valueD-Days)*365
    Hours = int(valueH)

    valueM = (valueH - Hours)*24
    Minutes = int(valueM)

    valueS = (valueM - Minutes)*60
    Seconds = int(valueS)


    return "%3d;%2d;%2d;%2d" % (Days, Hours, Minutes, Seconds)

start = time.time()

totalParticles = 2000
steps = 100000
lmbda = 1
sigma = 1

data = "#particles\tpacking density\n"

totalRuns = 8*totalParticles
run = 0

print "  d  h  m  s"

for particles in range(2,totalParticles+2):
	runs = []
	for seed in range(8): #run 8 processes in parallel
		argColl = ["./packSphere",str(particles), str(steps), str(lmbda), str(sigma), str(seed)]
		p = Popen(argColl, stdout=PIPE, stderr=PIPE)
		runs.append((p, argColl))
	for p, argColl in runs:
		data += "%s %s" % (argColl[1], p.communicate()[1])
		run+=1
		print "\r%s finished run %6d of %6d [%3d%%]" % (stopWatch(start), run,totalRuns,100*run/totalRuns),
		


with open('somefile.txt', 'w') as f:
	print "\nsaving data to %s" % (f.name)
	f.write(data)


