import sys
import re
from struct import *

#This function returns the content of a file as a byte array
def get_bytes_from_file(filename):
    return open(filename, 'rb').read()

dict = {}

if __name__ == "__main__":
    args = sys.argv[1:]
    num = 0
    if len(args)<1:
        print "The number of parameters must not be zero."
        sys.exit(1)

    rulesfile = open(args[0], 'r')
    rules = rulesfile.readlines()
    rulematch = re.compile('\s*#define\s+(\w+)\s+(\d+)\s*')
    for line in rules:
        if rulematch.match(line):
            result = rulematch.match(line)
            #turns out must use 1 and 2 instead of 0, which is the total string
            name = result.group(1)
            value = result.group(2)
            value1 = int(value)
            dict[value1] =  name



    datafile = get_bytes_from_file(args[1])
    index = 0
    debugk = len(datafile)
    output = open('testfile2', 'w')
    while True:
        if index%128 >=120:
            pad = 128 - index%128
            index = pad + index
        if index >=len(datafile):
            break

        type = datafile[index]
        id = ord(datafile[index+1])*256 + ord(datafile[index+2])
        typereal = ord(type)
        idreal = id

        if typereal == 1:
            indexofthread = ord(datafile[index+3])
            nameofevent = dict[idreal]
            if indexofthread != 100:

                temp = datafile[index+4]+datafile[index+5]+datafile[index+6]+datafile[index+7]
                timestamp = unpack('<L', temp)
                output.write('The thread index '+ str(indexofthread)+ ' has event: '+ str(nameofevent).rjust(40)+ ' at timestamp '+str(timestamp[0]) +'\n')
            elif idreal!=4:
                    output.write('The kernel'+ ' has event: '+ nameofevent+ 'at timestamp '+ timestamp)

            index = index+8
        elif typereal == 2:
            indexofthread = ord(datafile[index+2])
            nameofevent = dict[idreal]
            output.write('The app has a event SEND \n')
            output.write('The counter value is : '+ str(indexofthread)+ '\n')
            index = index+3
        elif typereal == 3:
            nameofevent = dict[idreal]
            lengthreal = ord(datafile[index+2])
            memvalues = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
            for i in range(1, lengthreal+1):
                memvalues[i] = ord(datafile[index+2+i])
            output.write('The memory for '+ nameofevent + ' is ')
            for i in range(1, lengthreal+1):
                output.write(str(memvalues[i])+' ')
            output.write('\n')

            index = index+2+lengthreal+1
        elif typereal >3:
            output.write("failure input!")
            break

        if index >= len(datafile):
            break
    output.close()


