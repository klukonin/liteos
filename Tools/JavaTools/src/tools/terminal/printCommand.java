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
public class printCommand {

	private byte[] reply = new byte[64];

	public int setNewCommand(String[] options, int optioncount,
			String[] parameters, int parametercount, fileDirectory fdir,
			debugCommand debughandle) {

		fileNode currentnode = fdir.getCurrentNode();
		int currentAddress = currentnode.getNodeAddress();

		int numOfVar = debughandle.getNumOfVariables();
		int i;
		variable temp;
		temp = null;

		for (i = 0; i < numOfVar; i++) {
			temp = debughandle.getVariable(i);
			if (temp.getName().compareTo(parameters[0]) == 0)
				break;
		}

		if (i == numOfVar) {
			System.out.println("The variable is not found!");
			return 0;
		}

		if (temp == null) {
			System.out.println("The variable is not found!");
			return 0;
		}
		int addr = temp.getaddr();
		int sizeofvar = temp.getSize();

		int addrhigh = addr / 256;
		int addrlow = addr % 256;
		int sizehigh = sizeofvar / 256;
		int sizelow = sizeofvar % 256;

		reply[0] = (byte) 7;
		reply[1] = (byte) 91;
		reply[2] = (new Integer(currentAddress)).byteValue();
		reply[3] = (new Integer(addrhigh)).byteValue();
		reply[4] = (new Integer(addrlow)).byteValue();
		reply[5] = (new Integer(sizehigh)).byteValue();
		reply[6] = (new Integer(sizelow)).byteValue();

		// System.arraycopy(filename, 0, reply, 3, filename.length);

		// reply[3+filename.length] = (byte)0;

		return 1; // To change body of implemented methods use File | Settings |
					// File Templates.
	}

	// Return the delay in milliseconds
	public int getDelay() {
		return 500; // To change body of implemented methods use File | Settings
					// | File Templates.
	}

	// return the command will be used
	public byte[] getNewCommand(int index) {
		return reply;
	}

	public void handleresponse(String[] options, int optioncount,
			String[] parameters, int parametercount, ArrayList reply,
			fileDirectory fdir, debugCommand debughandle) {

		fileNode cnode = fdir.getCurrentNode();
		String nodeName = cnode.getNodeName();
		byte[] targetData = new byte[3000];
		int targetDataindex;

		int start = 5;
		int gotmsg = 0;
		int chunkindex;
		int totalsizelow = 0;
		int totalsizehigh = 0;
		int totalsize = 0;
		// System.out.println("There are " + length +" bytes received");

		byte[] response = null;

		targetDataindex = 0;
		// for (int start = 0; start + 32 <= length; start += 32) {
		while (reply.size() > 0) {
			response = (byte[]) reply.remove(0);

			if (response[start + 1] != 91)
				continue;

			gotmsg++;
			// totalsize = response[start + 4];
			totalsizehigh = (0x000000FF & ((int) response[start + 4]));
			totalsizelow = (0x000000FF & ((int) response[start + 5]));
			totalsize = totalsizelow + totalsizehigh * 256;

			chunkindex = response[start + 3];

			System.arraycopy(response, start + 6, targetData, targetDataindex,
					24);
			targetDataindex += 24;
			// break;
		}

		// int lengthdata = response[start + 0] - 3;

		if ((gotmsg * 24 < totalsize) || (totalsize == 0)) {
			colorOutput.print(colorOutput.COLOR_BRIGHT_GREEN,
					"Print failure due to message loss\n");
			return;
		}

		colorOutput.print(colorOutput.COLOR_BRIGHT_GREEN, "Variable length "
				+ totalsize + " : ");
		for (int i = 0; i < totalsize; i++) {
			colorOutput.print(colorOutput.COLOR_BRIGHT_GREEN, "0x"
					+ Integer.toString(targetData[i] & 0xFF, 16) + " ,");

		}
		colorOutput.print(colorOutput.COLOR_BRIGHT_GREEN, "\n");

		return;
	}
}
