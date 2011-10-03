#! /usr/bin/env python
import threading
import os
import time
import sys
#20 and 29 removed


commtable = [8,  12, 16, 22]
startid = 1
commonchannel = 21

class MyThread ( threading.Thread ):

    def __init__ ( self, portnum, nodeid, addr):
        self.portnum = portnum
        self.nodeid = nodeid
        self.addr = addr
        threading.Thread.__init__ ( self )

    def run ( self ):
        cmdtemp = 'uisp -dprog=mib510 -dpart=atmega128 -dserial=/dev/ttyS' + self.portnum +' --erase'
        #cmddelete = 'rm _temp_LiteOS'+self.nodeid+'.hex'
        

        print 'node: '+ self.nodeid + ' The string is '+ cmdtemp + '\n'
        

        fout = os.popen(cmdtemp)
        resultoutput = fout.read()
        print 'node: ' + self.nodeid+ ' '+resultoutput

        
#total 9 threads
for i in range(1, 5):
    addr = sys.argv[1]
    commport = str(commtable[i-1]-1)
    MyThread(commport, str(startid+i-1), addr).start()
