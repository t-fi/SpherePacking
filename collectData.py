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
    f_torus.write("#type\tseed\tnumParticles\tRadius\tdiscPackingDensity\texactPackingDensity\tMCPackingDensity\n")
    f_sphere.write("#type\tseed\tnumParticles\tRadius\tdiscPackingDensity\texactPackingDensity\tMCPackingDensity\n")
    for element in data:
        if element[0] == "torus":
            dummy = f_torus
        else:
            dummy = f_sphere
        for datapoint in element:
            dummy.write("%s\t" % (datapoint))
        dummy.write("\n")

def save_only_best_configs(data):
    f_torus = open("data/bestResults_torus.dat", "w")
    f_sphere = open("data/bestResults_sphere.dat", "w")
    f_torus.write("#type\tseed\tnumParticles\tRadius\tdiscPackingDensity\texactPackingDensity\tMCPackingDensity\n")
    f_sphere.write("#type\tseed\tnumParticles\tRadius\tdiscPackingDensity\texactPackingDensity\tMCPackingDensity\n")
    bestTorus = {}
    bestSphere = {}
    for element in data:
        if element[0] == "torus":
            if len(element)<7: continue
            if bestTorus.get(element[2],0) < element[6]:
                bestTorus[element[2]] = element
        else:
            if bestSphere.get(element[2],0) < element[5]:
                bestSphere[element[2]] = element
    for v in bestTorus.values():
        for datapoint in v:
            f_torus.write("%s\t" % (datapoint))
        f_torus.write("\n")
    for v in bestSphere.values():
        for datapoint in v:
            f_sphere.write("%s\t" % (datapoint))
        f_sphere.write("\n")

data = []

for subdir, dirs, files in os.walk("./data"):
    for filename in files:
        fileData = {}
        filepath = os.path.join(subdir, filename)
        file = open(filepath, "r")
        lines = [x.strip() for x in file.readlines()]
        if filename == "Observables.dat":
            data.append(extract_data(lines))

save_data(data)
save_only_best_configs(data)
