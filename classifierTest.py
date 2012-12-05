#!/usr/bin/env python

import sys, os

classifiers = ["BAYES", "KNN", "DTREE"]
descriptors = ["FAST", "STAR", "SURF"]
extractors = ["SIFT", "SURF"]

def Execute(argList):
    possibilities = [(d, e) for d in descriptors for e in extractors]
    classifierType = argList[0]
    for descriptorType, extractorType in possibilities:
        cmdLine = "./LandmarkDetector %s %s %s sets/training/ sets/test/ > %s_%s_%s_tmp_log" % (classifierType, descriptorType, extractorType, classifierType, descriptorType, extractorType)
        print "RUNNING ", cmdLine
        os.system(cmdLine)
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
