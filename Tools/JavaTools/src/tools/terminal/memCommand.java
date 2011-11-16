/*
The following is the license of LiteOS.

This file is part of LiteOS.
Copyright Qing Cao, 2007-2008, University of Illinois , qcao2@uiuc.edu

LiteOS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LiteOS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LiteOS.  If not, see <http://www.gnu.org/licenses/>.
 */

package tools.terminal;

import java.util.ArrayList;

/**
 * The du command class that allows the current directory information to be
 * displayed.
 */
public class memCommand {

	private byte[] reply = new byte[64];

	public int setNewCommand(String[] options, int optioncount,
			String[] parameters, int parametercount, fileDirectory fdir) {

		fileNode currentnode = fdir.getCurrentNode();
		int currentAddress = currentnode.getNodeAddress();

		reply[0] = 3;
		reply[1] = (byte) 181;
		reply[2] = (new Integer(currentAddress)).byteValue();

		return 1; // To change body of implemented methods use File | Settings |
					// File Templates.
	}

	// Return the delay in milliseconds
	public int getDelay() {
		return 1000; // To change body of implemented methods use File |
						// Settings | File Templates.
	}

	// return the command will be used
	public byte[] getNewCommand(int index) {
		return reply;
	}

	public void handleresponse(String[] options, int optioncount,
			String[] parameters, int parametercount, ArrayList reply,
			fileDirectory fdir) {

		fileNode cnode = fdir.getCurrentNode();
		String nodeName = cnode.getNodeName();
		byte[] targetData = new byte[3000];
		int targetDataindex;

		int start = 5;
		int gotmsg = 0;

		int kernelromuse = 0;
		int kernelramuse = 0;

		int kernelstacksp = 0;
		int appromstart;
		int appromsize = 0;
		int appramstart = 0;
		int appromend = 0;
		int appramend = 0;

		int threadsp;
		int threadbsssize;
		// System.out.println("There are " + length +" bytes received");

		int lenoffilename;
		int appromzero = 0;
		int threadbssstart, threadbssend;

		byte[] response = null;

		targetDataindex = 0;
		// for (int start = 0; start + 32 <= length; start += 32) {
		while (reply.size() > 0) {
			response = (byte[]) reply.remove(0);

			if ((0x000000FF & ((int) response[start + 1])) != 181)
				continue;

			gotmsg++;

			if (gotmsg == 1) {
				// Now first message.
				// Print memory info and kernel memory usage info
				colorOutput.print(colorOutput.COLOR_BRIGHT_GREEN,
						"Following is the memory information\n");
				colorOutput.print(colorOutput.COLOR_BRIGHT_GREEN,
						"The total flash 0--0x1FFFF, ram 0x100--0x10FF\n");
				colorOutput
						.print(colorOutput.COLOR_BRIGHT_GREEN,
								"The kernel bootloader and system calls use 0x1D400 and above for flash\n");
				kernelromuse = (0x000000FF & ((int) response[start + 3])) * 256
						+ (0x000000FF & ((int) response[start + 4]));
				kernelramuse = (0x000000FF & ((int) response[start + 5])) * 256
						+ (0x000000FF & ((int) response[start + 6]));
				kernelromuse = kernelromuse * 2;
				kernelramuse = kernelramuse + 255;
				colorOutput.print(colorOutput.COLOR_BRIGHT_GREEN,
						"The kernel consumes 0x00 to 0x"
								+ Integer.toHexString(kernelromuse)
										.toUpperCase() + " bytes for flash\n");
				colorOutput.print(colorOutput.COLOR_BRIGHT_GREEN,
						"The mapped registers take 0x00 to 0xff for ram\n");
				colorOutput.print(colorOutput.COLOR_BRIGHT_GREEN,
						"The kernel consumes 0x100 to 0x"
								+ Integer.toHexString(kernelramuse)
										.toUpperCase() + " for ram\n");

			}
			// totalsize = response[start + 4];

			lenoffilename = (0x000000FF & ((int) response[start + 7]));
			System.arraycopy(response, start + 8, targetData, 0, lenoffilename);
			targetData[lenoffilename] = '\0';

			appromstart = (0x000000FF & ((int) response[start + lenoffilename
					+ 9]))
					* 256
					+ (0x000000FF & ((int) response[start + lenoffilename + 10]));
			appromstart = appromstart * 2;
			appromsize = (0x000000FF & ((int) response[start + lenoffilename
					+ 11]))
					* 256
					+ (0x000000FF & ((int) response[start + lenoffilename + 12]));
			appramstart = (0x000000FF & ((int) response[start + lenoffilename
					+ 13]))
					* 256
					+ (0x000000FF & ((int) response[start + lenoffilename + 14]));
			appramend = (0x000000FF & ((int) response[start + lenoffilename
					+ 15]))
					* 256
					+ (0x000000FF & ((int) response[start + lenoffilename + 16]));
			appromend = appromstart + appromsize;

			threadsp = (0x000000FF & ((int) response[start + lenoffilename + 17]))
					* 256
					+ (0x000000FF & ((int) response[start + lenoffilename + 18]));
			threadbsssize = (0x000000FF & ((int) response[start + lenoffilename
					+ 19]))
					* 256
					+ (0x000000FF & ((int) response[start + lenoffilename + 20]));

			threadbssstart = appramstart;
			threadbssend = appramstart + threadbsssize;

			String nameoffile = new String(targetData, 0, lenoffilename);
			if (((appromstart != 0) && (appromzero == 0)) || (appromzero == 1)) {
				colorOutput.print(colorOutput.COLOR_BRIGHT_GREEN, "The thread "
						+ nameoffile + " consumes 0x"
						+ Integer.toHexString(appromstart).toUpperCase()
						+ " to 0x"
						+ Integer.toHexString(appromend).toUpperCase()
						+ " for flash\n");
				colorOutput.print(colorOutput.COLOR_BRIGHT_GREEN, "The thread "
						+ nameoffile + " consumes 0x"
						+ Integer.toHexString(appramstart).toUpperCase()
						+ " to 0x"
						+ Integer.toHexString(appramend).toUpperCase()
						+ " for ram\n");
				colorOutput.print(colorOutput.COLOR_BRIGHT_GREEN, "The thread "
						+ nameoffile + " static variables consume 0x"
						+ Integer.toHexString(threadbssstart).toUpperCase()
						+ " to 0x"
						+ Integer.toHexString(threadbssend).toUpperCase()
						+ " for ram\n");
				colorOutput.print(colorOutput.COLOR_BRIGHT_GREEN, "The thread "
						+ nameoffile + " stack space consumes 0x"
						+ Integer.toHexString(threadsp).toUpperCase()
						+ " to 0x"
						+ Integer.toHexString(appramend).toUpperCase()
						+ " for ram\n");

			}

			else {
				appromzero = 1;
			}

			if (gotmsg == 1) {
				kernelstacksp = (0x000000FF & ((int) response[start
						+ lenoffilename + 21]))
						* 256
						+ (0x000000FF & ((int) response[start + lenoffilename
								+ 22]));

			}

		}
		// colorOutput.print(colorOutput.COLOR_BRIGHT_GREEN, "0x" +
		// Integer.toString(targetData[i] & 0xFF, 16) + " ,");

		colorOutput.print(colorOutput.COLOR_BRIGHT_GREEN,
				"The kernel stack consumes 0x"
						+ Integer.toHexString(kernelstacksp).toUpperCase()
						+ " to 0x10FF for ram\n");

	}

}
