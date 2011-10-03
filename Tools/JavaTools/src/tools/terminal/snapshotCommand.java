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
public class snapshotCommand implements cmdcontrol {

	private byte[] reply = new byte[64];

	public int setNewCommand(String[] options, int optioncount,
			String[] parameters, int parametercount, fileDirectory fdir) {

		fileNode currentnode = fdir.getCurrentNode();
		int currentAddress = currentnode.getNodeAddress();
		int currentBlock = currentnode.getBlock();

		byte[] filename = parameters[0].getBytes();
		byte[] targetfilename = parameters[1].getBytes();

		int lengthoffilename = filename.length;
		int lengthoffilenametarget = targetfilename.length;

		reply[0] = (byte) (new Integer(lengthoffilename).byteValue() + (byte) 6 + new Integer(
				lengthoffilenametarget).byteValue());
		reply[1] = (byte) 94;
		reply[2] = (new Integer(currentAddress)).byteValue();
		reply[3] = (byte) lengthoffilename;
		reply[4] = (byte) lengthoffilenametarget;

		System.arraycopy(filename, 0, reply, 5, lengthoffilename);
		reply[5 + lengthoffilename] = 0;
		System.arraycopy(targetfilename, 0, reply, 5 + lengthoffilename + 1,
				lengthoffilenametarget);

		return 1; // To change body of implemented methods use File | Settings |
					// File Templates.
	}

	// Return the delay in milliseconds
	public int getDelay() {
		return 200; // To change body of implemented methods use File | Settings
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
		int testtrue = 0;

		int start = 5;
		// for (int start = 0; start + 32 <= length; start += 32) {
		while (reply.size() > 0) {
			byte[] response = (byte[]) reply.remove(0);
			testtrue = (0x000000FF & ((int) response[start + 3]));
		}

		if (testtrue == 0)
			colorOutput
					.println(colorOutput.COLOR_BRIGHT_RED,
							"The thread is not found. The snapshot operation probably fails.");

		return;
	}
}
