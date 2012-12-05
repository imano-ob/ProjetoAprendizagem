#!/usr/bin/env python

import sys, os

classifiers = ["BAYES", "KNN", "DTREE"]
descriptors = ["FAST", "STAR", "SURF"]
extractors = ["SIFT", "SURF"]

def parse():
    return 1

def Execute(argList):
    possibilities = [(c, d, e) for c in classifiers for d in descriptors for e in extractors]
    for classifierType, descriptorType, extractorType in possibilities:
        logfileName = classifierType + '_' + descriptorType + '_' + extractorType + '_log'
        successRate = parse(logFileName)

if __name__ -- '__main__':
    Execute(sys.argv[1:])
