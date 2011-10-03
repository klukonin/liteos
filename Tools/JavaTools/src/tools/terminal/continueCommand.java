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
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

/**
 * The du command class that allows the current directory information to be
 * displayed.
 */
public class continueCommand {

	private byte[] reply = new byte[64];

	private String getInstructionsFromFile(File lssfile, String instructionnum)
			throws Exception {

		String pathname = lssfile.getAbsolutePath();

		// Now read the file content into a string
		FileInputStream fis = new FileInputStream(pathname);
		int x = fis.available();

		byte b[] = new byte[x];
		fis.read(b);
		String content = new String(b);

		int pagenum = new Integer(content.substring(0, 3)).intValue();
		int startbinaryposition = pagenum * 256;
		int offset = Integer.parseInt(instructionnum, 16) - startbinaryposition;
		// 16 is the first byte menaingful
		offset = offset * 2 + 16;
		String bytescontent = content.substring(offset, offset + 12);
		return bytescontent;

	}

	public int setNewCommand(String[] options, int optioncount,
			String[] parameters, int parametercount, fileDirectory fdir,
			breakpointCommand breakpointhandle, debugCommand debughandle) {

		fileNode currentnode = fdir.getCurrentNode();
		int currentAddress = currentnode.getNodeAddress();
		int numOfBreaks = breakpointhandle.getNumOfBreakpoint();
		int i;
		int temp = 0;
		String patchinstructions = null;
		boolean patchinitilized = false;
		int patchedthroughfile = 0;

		byte[] patchinstructionsbinary = new byte[6];

		String numparameter;

		if (parameters[0].startsWith("0x") == true)
			numparameter = parameters[0].substring(2);
		else
			numparameter = parameters[0];

		for (i = 0; i < numOfBreaks; i++) {
			temp = breakpointhandle.getBreakPointAddr(i);
			if (temp == Integer.parseInt(numparameter, 16))
				break;
		}

		// if (i==numOfBreaks)
		// { System.out.println("The breakpoint is not found!");
		// return 0;
		// }

		if (i < numOfBreaks) {
			System.arraycopy(breakpointhandle.getInstructions(i), 0,
					patchinstructionsbinary, 0, 6);
			patchinitilized = true;
		}

		if (patchinitilized == false) {
			// Now read from the file to continue. Requires the debugging
			// environment to be set up

			if (debughandle.isSetUp() == false) {
				System.out
						.println("The debugging environment has not been set up properly. Please set up the correct debugging environment and proceed.\n ");
				return 0;
			}
			String directoryname = debughandle.getLocalDirectory();

			// Now open the file ending with lss and read its contents

			File lssfile = new File(directoryname + "\\build\\App.hex");

			if ((lssfile.exists() == false)) {
				lssfile = new File(directoryname + "\\bin\\LiteOS.hex");
				if ((lssfile.exists() == false)) {
					System.out
							.println("Could not find the correct files. Please ensure that the environment is correct.\n ");
					return 0;
				}
			}

			// patchinstructions get the results in the explict form. For
			// example, for the
			// code, then it is like E4E0FAEE0995
			// IT should be e4 as one byte, and so on

			try {
				patchinstructions = getInstructionsFromFile(lssfile,
						numparameter);
				patchinitilized = true;
				patchedthroughfile = 1;
			} catch (java.lang.Exception e) {
				patchinitilized = false;
			}

			int j;
			for (j = 0; j < 6; j++) {
				String temp3 = null;
				try {

					temp3 = patchinstructions.substring(j * 2, j * 2 + 2);
				} catch (java.lang.NullPointerException e) {
					System.out.println("Null pointer catched");
					System.out.println(patchinstructions.toString());

				}

				patchinstructionsbinary[j] = (byte) Integer.parseInt(temp3, 16);
			}
		}
		if (patchinitilized == false) {
			// Should not reach here.
			System.out
					.println("Could not find the breakpoint information. Either the debugging environment is not set up "
							+ "correctly or because the shell has been restarted, hence the debugging info lost. Please set up the debuging info with debug command.");

			return 0;
		}
		int addr = Integer.parseInt(numparameter, 16);

		addr = addr / 2;

		int addrhigh = addr / 256;
		int addrlow = addr % 256;

		reply[0] = (byte) 12;
		reply[1] = (byte) 93;
		reply[2] = (new Integer(currentAddress)).byteValue();
		reply[3] = (new Integer(addrhigh)).byteValue();
		reply[4] = (new Integer(addrlow)).byteValue();
		System.arraycopy(patchinstructionsbinary, 0, reply, 5, 6);

		if (patchedthroughfile == 0)
			breakpointhandle.removeBreakPointAddr(addr * 2);

		// System.arraycopy(filename, 0, reply, 3, filename.length);

		// reply[3+filename.length] = (byte)0;

		return 1; // To change body of implemented methods use File | Settings |
					// File Templates.
	}

	// Return the delay in milliseconds
	public int getDelay() {
		return 600; // To change body of implemented methods use File | Settings
					// | File Templates.
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
		int addr;
		byte[] response = null;
		int start = 5;
		int replied = 0;

		while (reply.size() > 0) {

			response = (byte[]) reply.remove(0);
			if (response[start + 1] != 93)
				continue;

			replied = 1;
			if (response[start + 3] == 0)
				System.out.print("The specified breakpoint is not found.");
			else
				System.out
						.print("The specified breakpoint is found. Continue command is successful.");

		}
		if (replied == 0)
			System.out
					.print("No reply received. Probably because the link is very weak. Use ps to see if the breakpoint has been removed");

		System.out.print("\n");

	}
}
