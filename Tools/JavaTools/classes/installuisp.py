#! /usr/bin/env python
import threading
import os
import time
import sys
#20 and 29 removed


commtable = [ 8, 10, 12, 14, 16, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40]
startid = 1
commonchannel = 21

class MyThread ( threading.Thread ):

    def __init__ ( self, portnum, nodeid, addr):
        self.portnum = portnum
        self.nodeid = nodeid
        self.addr = addr
        threading.Thread.__init__ ( self )

    def run ( self ):
        cmdcompile = 'reAddr ihex LiteOS.hex _temp_LiteOS'+ self.nodeid + '.hex ' + self.addr + ' ' + self.nodeid
        cmdtemp = 'uisp -dprog=mib510 -dpart=atmega128 -dserial=/dev/ttyS' + self.portnum +' --wr_fuse_h=0x19 --wr_fuse_e=ff  --erase --upload if=_temp_LiteOS'+self.nodeid+'.hex'
        cmddelete = 'rm _temp_LiteOS'+self.nodeid+'.hex'
        
        print 'node: '+ self.nodeid + ' The string is '+ cmdcompile
        print 'node: '+ self.nodeid + ' The string is '+ cmdtemp
        
        fout = os.popen(cmdcompile)
        resultoutput = fout.read()
        print 'node: ' + self.nodeid + ' '+resultoutput
        fout = os.popen(cmdtemp)
        resultoutput = fout.read()
        print 'node: ' + self.nodeid+ ' '+resultoutput

        fout = os.popen(cmddelete)
        resultoutput = fout.read()
        print 'node: ' + self.nodeid+ ' '+resultoutput
        
#total 9 threads
for i in range(1, 5):
    addr = sys.argv[1]
    commport = str(commtable[i-1]-1)
    MyThread(commport, str(startid+i-1), addr).start()
    
time.sleep(3)
for i in range(5, 9):
    addr = sys.argv[1]
    commport = str(commtable[i-1]-1)
    MyThread(commport, str(startid+i-1), addr).start()
    
time.sleep(45)

for i in range(9, 13):
    addr = sys.argv[1]
    commport = str(commtable[i-1]-1)
    MyThread(commport, str(startid+i-1), addr).start()
    
time.sleep(3)

for i in range(13, 17):
    addr = sys.argv[1]
    commport = str(commtable[i-1]-1)
    MyThread(commport, str(startid+i-1), addr).start()
