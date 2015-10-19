#!/usr/bin/env python
import itertools
import time
import subprocess
from optparse import OptionParser

NUMACTL = ["No"]
PLACES = ["Contiguous", "Spread"]
SIZES = ["4096"]
BLOCKSIZES = ["256", "512"]
PROGS = ["dpotrf_taskdep"]
NTHREADS = ["1", "2", "3", "4"]
ITERATIONS = 5

#NUMACTL = ["No", "All"]
#PLACES = ["Spread", "Contiguous"]
#SIZES = ["8192", "16384", "32768"]
#BLOCKSIZES = ["256", "512"]
#PROGS = ["dpotrf_taskdep"]
#NTHREADS = ["4", "8", "16", "32", "48", "72", "96", "120", "144", "168", "192"]
#ITERATIONS = 10

BASELOGFILE = "log_plasma."

if __name__ == "__main__":
    logFilename = BASELOGFILE + str(int(time.time()))
    parser = OptionParser()
    parser.add_option('-r', '--runtime', dest='runtime',
                      help='Runtime used for this experiment')
    (options, args) = parser.parse_args()
    if not options.runtime:
        parser.error("You need to specify a runtime for this experiment")
    for n, pl, s, bs, th, pr in itertools.product(NUMACTL, PLACES, SIZES,
                                                  BLOCKSIZES, NTHREADS, PROGS):
        outputLogFilename = logFilename
        #TODO make this an enum
        outputLogFilename += "." + options.runtime
        cmdLine = ""
        cmdLine += "OMP_NUM_THREADS=" + th + " OMP_PLACES="
        if pl == "Spread":
            outputLogFilename += ".spread"
            #TODO: this part is idchire-specific
            threads = int(th)
            remains = threads - 96
            if remains > 0:
                threads = 96
            cmdLine += "\"{0}:" + str(threads) + ":2"
            if remains > 0:
                cmdLine += ",{1}:" + str(remains) + ":2"
            cmdLine += "\" "
        elif pl == "Contiguous":
            outputLogFilename += ".contiguous"
            cmdLine += "\"{0}:" + th + ":1\" "

        if n == "All":
            outputLogFilename += ".all"
            cmdLine += "numactl --interleave=all "
        else:
            outputLogFilename += ".none"
        outputLogFilename += ".log"
        cmdLine += "./" + pr + " -i 1 -n " + s + " -b " + bs
        cmdLine += " >> " + outputLogFilename
        #NOTE: This is BAD
        for i in xrange(ITERATIONS):
            print "Executing (" + str(i) + ") :\n"
            print cmdLine + "\n"
            subprocess.call(cmdLine, shell=True)

