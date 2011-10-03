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
public class breakpointCommand {

	private byte[] reply = new byte[64];
	private int breakpointNum;

	public int getNumOfBreakpoint() {
		return breakpointNum;
	}

	public int getBreakPointAddr(int index) {
		return InternalBreakpoints[index].getAddr();
	}

	public byte[] getInstructions(int index) {
		return InternalBreakpoints[index].getinstructions();
	}

	public boolean getBreakPointStatus(int index) {
		return InternalBreakpoints[index].getStatus();
	}

	public void removeBreakPointAddr(int addr) {
		int i;
		int toBeRemoved;
		toBeRemoved = -1;

		for (i = 0; i < breakpointNum; i++) {
			if (InternalBreakpoints[i].getAddr() == addr) {
				toBeRemoved = i;
				break;
			}
		}

		if (toBeRemoved == -1)
			return;

		for (i = toBeRemoved; i < breakpointNum - 1; i++)
			InternalBreakpoints[i] = InternalBreakpoints[i + 1];

		breakpointNum--;

	}

	public breakpointCommand() {
		breakpointNum = 0;
	}

	private class breakpointinfo {
		private int addr;
		private boolean activated;
		private byte[] internalinstructions = new byte[6];

		public breakpointinfo(int addr, boolean activated) {
			this.addr = addr;
			this.activated = activated;
		}

		public boolean getStatus() {
			return activated;
		}

		public void setEnable() {
			activated = true;
		}

		public void disable() {
			activated = false;
		}

		public int getAddr() {
			return addr;
		}

		public void printInfo() {
			System.out.println("Breakpoint at 0x"
					+ Integer.toHexString(0x1000000 | addr).substring(1)
							.toUpperCase());
		}

		public void handleinstructions(byte[] response) {
			int i;
			for (i = 0; i < 6; i++)
				internalinstructions[i] = response[6 + i + 5];
		}

		public byte[] getinstructions() {
			return internalinstructions;
		}
	}

	breakpointinfo[] InternalBreakpoints = new breakpointinfo[128];

	public int setNewCommand(String[] options, int optioncount,
			String[] parameters, int parametercount, fileDirectory fdir) {

		fileNode currentnode = fdir.getCurrentNode();
		int currentAddress = currentnode.getNodeAddress();

		int addr;

		if (parameters[0].startsWith("0x") == true)
			addr = Integer.parseInt(parameters[0].substring(2), 16) / 2;
		else
			addr = Integer.parseInt(parameters[0], 16) / 2;

		int breakpointpage = addr / 128;
		int breakpointpageoffset = addr * 2 % 256;

		int breakpointpagehigh = breakpointpage / 256;
		int breakpointpagelow = breakpointpage % 256;

		if (breakpointpageoffset > 240) {
			System.out
					.println("Please select a different breakpoing. Cross-page breakpoint is currently disabled.");
			return 0;
		}

		reply[0] = (byte) 6;
		reply[1] = (byte) 92;
		reply[2] = (new Integer(currentAddress)).byteValue();
		reply[3] = (new Integer(breakpointpagehigh)).byteValue();
		reply[4] = (new Integer(breakpointpagelow)).byteValue();
		reply[5] = (new Integer(breakpointpageoffset)).byteValue();

		// System.arraycopy(filename, 0, reply, 3, filename.length);

		// reply[3+filename.length] = (byte)0;

		return 1; // To change body of implemented methods use File | Settings |
					// File Templates.
	}

	void updateInternalBreakpoint(int addr, boolean status, byte[] response) {
		int i;

		for (i = 0; i < breakpointNum; i++) {
			if (InternalBreakpoints[i].getAddr() == addr) {
				if (status)
					InternalBreakpoints[i].setEnable();
				else
					InternalBreakpoints[i].disable();

				InternalBreakpoints[i].handleinstructions(response);
				break;
			}
		}
	}

	void printBreakpoints() {
		System.out
				.println("Note that there may be additional breakpoints on the thread that are not listed.");
		System.out.println("There are " + breakpointNum + " breakpoints.");
		int i;
		for (i = 0; i < breakpointNum; i++) {
			InternalBreakpoints[i].printInfo();
		}

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

	private static int unsignedByteToInt(byte b) {
		return (int) b & 0xFF;
	}

	public void handleresponse(String[] options, int optioncount,
			String[] parameters, int parametercount, ArrayList reply,
			fileDirectory fdir) {

		fileNode cnode = fdir.getCurrentNode();
		String nodeName = cnode.getNodeName();
		int addr;
		int breakpointreceived = 0;
		byte[] targetData = new byte[100];
		// System.out.println("There are " + length +" bytes received");
		byte[] response = null;
		int start = 5;

		while (reply.size() > 0) {
			// for (int start = 0; start + 32 <= length; start += 32) {
			response = (byte[]) reply.remove(0);

			if (response[start + 1] != 92)
				continue;

			addr = (unsignedByteToInt(response[start + 3]) * 256 + unsignedByteToInt(response[start + 4]))
					* 256 + unsignedByteToInt(response[start + 5]);

			InternalBreakpoints[breakpointNum++] = new breakpointinfo(addr,
					false);
			updateInternalBreakpoint(addr, true, response);
			breakpointreceived = 1;
			break;

		}

		if (breakpointreceived == 0)
			System.out
					.println("The shell has not received any meaningful breakpoint information. The information is probably lost due to extremely bad link quality or interference. To use the coninue command, you must first set up the debugging environment using the debug command.\n");
	}

}
