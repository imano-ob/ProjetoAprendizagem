#!/usr/bin/env python

import sys, os

classifiers = ["BAYES", "KNN", "DTREE"]
descriptors = ["FAST", "STAR", "SURF"]
extractors = ["SIFT", "SURF"]

def Execute(argList):
    if len(argList) > 0:
        possibilities = [(argList[0], d, e) for d in descriptors for e in extractors]
    else:
        possibilities = [(c, d, e) for c in classifiers for d in descriptors for e in extractors]
        
    for classifierType, descriptorType, extractorType in possibilities:
        logfileBaseName = "logs/%s_%s_%s" % (classifierType, descriptorType, extractorType)
        logfileFinalName = logfileBaseName + '_log'
        logfileTmpName = logfileFinalName + '_tmp'
        cmdLine = "./LandmarkDetector %s %s %s sets/training/ sets/test/ > %s" % (classifierType, descriptorType, extractorType, logfileTmpName)
        if not os.access(logfileFinalName, os.F_OK):
            print "RUNNING ", cmdLine
            if os.system(cmdLine) == 0:
                print "RENAMING ", logfileTmpName, " -> ", logfileFinalName
                os.rename(logfileTmpName, logfileFinalName)
            else:
                print "RUN FAILED"
                os.remove(logfileTmpName)
        else:
            print "SKIPPING ", cmdLine
        print "======================================================================================="
    #possibilities = [(c, d, e) for c in classifiers for d in descriptors for e in extractors]
    #
    #for classifierType, descriptorType, extractorType in possibilities:
    #    cmdLine = "./LandmarkDetector %s %s %s sets/training/ sets/test/" % (classifierType, descriptorType, extractorType)
    #    print "RUNNING ", cmdLine
    #    os.system(cmdLine)
    #    print "======================================================================================="

if __name__ == '__main__':
    Execute(sys.argv[1:])
