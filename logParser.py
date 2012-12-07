#!/usr/bin/env python

import sys, os

classifiers = ["BAYES", "KNN", "DTREE"]
descriptors = ["FAST", "STAR", "SURF"]
extractors = ["SIFT", "SURF"]

def parse(filename):
    resultFilename = filename + '_results'
    resultFile = open(resultFilename, 'w')
    classNames = {}
    confusion = {}
    successes = 0
    if not os.access(filename, os.R_OK):
        print "Nao foi possivel ler ", filename
        resultFile.write('Failed')
    else:
        logFile = open(filename, 'r')
        for line in logFile:
            cleanLine = line.rstrip()
            lineWords = cleanLine.split()
            if lineWords[0] == 'ERROR':
                print "Erro em ", filename
                resultFile.write('Failed')
                successes = 0
                resultFile.close()
                logFile.close()
                return successRate
            elif lineWords[0] == 'REGISTER':
                classNames [int(float(lineWords[2]))] = lineWords[1] #Logs tem 1.0 no REGISTER, mas usam 1 nos TESTS. E nao queremos gerar os logs de novo.
            elif lineWords[0] == 'TEST':
                predicted = int(lineWords[1])
                expected = int(lineWords[3])
                if not confusion.has_key(expected):
                    confusion[expected] = {}
                if not confusion[expected].has_key(predicted):
                    confusion[expected][predicted] = 0
                confusion[expected][predicted] += 1
                if expected == predicted:
                    successes += 1
        for nameKey, name in classNames.items():
            resultFile.write('Resultados de ' + name + '\n\n')
            for confKey, count in confusion[nameKey].items():
                resultFile.write(classNames[confKey] + ' : ' + str(count) + '\n')
            resultFile.write("\n=======================================================================================\n\n")
        resultFile.write("Sucessos: " + str(successes) + "\n")
        resultFile.close()
    resultFile.close()
    return successes

def Execute(argList):
    possibilities = [(c, d, e) for c in classifiers for d in descriptors for e in extractors]
    bestRate = -1
    for classifierType, descriptorType, extractorType in possibilities:
        logfileName = 'logs/' + classifierType + '_' + descriptorType + '_' + extractorType + '_log'
        successRate = parse(logfileName)
        if successRate > bestRate:
            bestRate = successRate
            bestDescriptor = descriptorType
            bestExtractor = extractorType
            bestClassifier = classifierType
    print bestClassifier, ' ', bestDescriptor, ' ', bestExtractor, ' foi a melhor combinacao com ', bestRate
if __name__ == '__main__':
    Execute(sys.argv[1:])
