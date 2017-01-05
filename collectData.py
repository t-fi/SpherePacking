#!/usr/bin/python

import os

def extract_data(lines):
    data = []
    slines = [x.split("=") for x in lines]
    for sline in slines:
        data.append(sline[1])
    return data

def save_data(data):
    f_torus = open("data/results_torus.dat", "w")
    f_sphere = open("data/results_sphere.dat", "w")
    f_torus.write("#type\tseed\tnumParticles\tRadius\tdiscPackingDensity\texactPackingDensity\tMCPackingDensity")
    f_sphere.write("#type\tseed\tnumParticles\tRadius\tdiscPackingDensity\texactPackingDensity\tMCPackingDensity")
    for element in data:
        if element[0] == "torus":
            dummy = f_torus
        else:
            dummy = f_sphere
        for datapoint in element:
            dummy.write("%s\t" % (datapoint))
        dummy.write("\n")

data = []

for subdir, dirs, files in os.walk("./data"):
    for filename in files:
        fileData = {}
        filepath = os.path.join(subdir, filename)
        file = open(filepath, "r")
        lines = [x.strip() for x in file.readlines()]
        if filename == "Observables.dat":
            print("the file %s matches! first line is %s" % (filepath, lines[0]))
            data.append(extract_data(lines))

print(data)
save_data(data)
