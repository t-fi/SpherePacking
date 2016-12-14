#!/usr/bin/env python

from argparse import ArgumentParser
import os.path

import subprocess as sp

def is_valid_file(parser, arg):
    if not os.path.exists(arg):
        parser.error("The file %s does not exist!" % arg)
    else:
        return open(arg, 'r')  # return an open file handle

def createOGlFile(numParticles, coordinates, radius):
    file = open('outfile.dat', 'w+')
    file.write("%d%s" % (numParticles,"\n"))
    file.write("-2 2\n")
    file.write("-2 2\n")
    file.write("-2 2\n")
    for coord in coordinates:
        file.write("%s %f %s\n" % (coord[0], radius, coord[1]))


def colorNeighbours(file):
    p = sp.Popen(["../../qhull/bin/qvoronoi","o"],stdin=file, stdout=sp.PIPE, stderr=sp.PIPE)
    out, err = p.communicate()
    print "=====STDOUT====="
    print out
    print "=====STDERR====="
    print err
    print "================"
    file.seek(0)
    lines = file.readlines()
    radius = float(lines[0].strip('\n').split("Diameter=")[1])
    print "radius=%f" % radius
    numParticles = int(lines[1])-1 # first particle is artificial
    coordinates = []
    voronoiData = out.split('\n')
    for count in range(numParticles):
        #last line is empty, hence the -1
        coordinates.append((lines[len(lines)-count-1].strip('\n'),voronoiData[-2-count][0]))
    file.close
    createOGlFile(numParticles,coordinates,radius)

if __name__ == "__main__":
    parser = ArgumentParser(description="read qhull data format, output openGl file with colors")
    parser.add_argument("-i", dest="filename", required=True,
                    help="input file", metavar="FILE",
                    type=lambda x: is_valid_file(parser, x))
    args = parser.parse_args()
    colorNeighbours(args.filename)
