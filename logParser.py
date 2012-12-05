#!/usr/bin/env python

import sys, os

classifiers = ["BAYES", "KNN", "DTREE"]
descriptors = ["FAST", "STAR", "SURF"]
extractors = ["SIFT", "SURF"]

def parse(filename):
    resultFilename = filename + '_analysis'
    resultFile = open(resultFilename, 'w')
    if not os.access(filename, os.R_OK):
        resultFile.write('Failed')
        successRate = 0
    else:
        resultFile.write('Exists!')
        successRate = 1
    resultFile.close()
    return successRate

def Execute(argList):
    possibilities = [(c, d, e) for c in classifiers for d in descriptors for e in extractors]
    bestRate = 0
    for classifierType, descriptorType, extractorType in possibilities:
        logfileName = classifierType + '_' + descriptorType + '_' + extractorType + '_log'
        successRate = parse(logFileName)
        if successRate > bestRate:
            bestRate = successRate
            bestDescriptor = descriptorType
            bestExtractor = extractorType
            bestClassifier = classifierType

if __name__ -- '__main__':
    Execute(sys.argv[1:])
