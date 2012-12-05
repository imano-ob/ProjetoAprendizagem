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
        logFile = open(filename, 'r')
        for line in logFile:
            cleanLine = line.rstrip()
            lineWords = cleanLine.split()
            if lineWords[0] == 'ERROR':
                resultFile.write('Failed')
                successRate = 0
                resultFile.close()
                logFile.close()
                return successRate
            elif lineWords[0] == 'REGISTER':
                doStuff()
            elif lineWords[0] == 'TEST':
                doStuff()
            resultFile.write('Stuff')
            resultFile.close()
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
