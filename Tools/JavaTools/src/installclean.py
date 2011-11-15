#! /usr/bin/env python
import threading
import os
import time
import sys
#20 and 29 removed


commtable = [6, 10, 12, 16, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 50]
startid = 1
commonchannel = 21

class MyThread ( threading.Thread ):

    def __init__ ( self, portnum, nodeid):
        self.portnum = portnum
        self.nodeid = nodeid
        threading.Thread.__init__ ( self )

    def run ( self ):
        cmdtemp = 'avrdude -p m1281 -c mib510 -PCOM' + self.portnum +'  -V -e'


        fout = os.popen(cmdtemp)
        resultoutput = fout.read()
        print 'node: ' + self.nodeid+ ' '+resultoutput


#total 9 threads
for i in range(1, 18):
    commport = str(commtable[i-1])
    MyThread(commport, str(startid+i-1)).start()
