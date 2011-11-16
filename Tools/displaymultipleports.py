#! /usr/bin/env python
import threading
import os
from  subprocess import Popen, PIPE
import shutil

#20 and 29 removed

commtable = [6, 10, 12, 16, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 50]
test = "exp1"

class MyThread ( threading.Thread ):

    def __init__ ( self, port):
        self.port = port
        threading.Thread.__init__ ( self )

    def run ( self ):
        print 'this is comm' + self.port
        cmdtemp ='java tools.tools.ListenRawTranslator ' + test+' '+self.port
        #p = Popen(['java', 'tools.tools.ListenRawTranslator', self.port],stdout= PIPE)

        fout = os.popen(cmdtemp)
        while True:
            line = fout.readline()
            if not line: break
            print line,


#total 9 threads
for i in range(1,6):
    if os.path.exists(test):
        shutil.rmtree(test)
    if not os.path.exists(test):
        os.makedirs(test)
    commport = 'COM'+str(commtable[i-1]+1)
    MyThread(commport).start()