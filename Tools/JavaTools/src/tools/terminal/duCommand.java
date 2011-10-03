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
public class duCommand implements cmdcontrol {

	private byte[] reply = new byte[64];

	public int setNewCommand(String[] options, int optioncount,
			String[] parameters, int parametercount, fileDirectory fdir) {

		fileNode currentnode = fdir.getCurrentNode();
		int currentAddress = currentnode.getNodeAddress();
		int currentBlock = currentnode.getBlock();

		byte[] filename = parameters[0].getBytes();

		reply[0] = 3;
		reply[1] = (byte) 211;
		reply[2] = (new Integer(currentAddress)).byteValue();

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
		int eepromusage = 0;
		int flashusage1 = 0;
		int flashusage2 = 0;
		int start = 5;

		int packetnum = reply.size();

		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, "The reply has "
				+ packetnum + " packets.");

		// for (int start = 0; start + 32 <= length; start += 32) {
		while (reply.size() > 0) {
			byte[] response = (byte[]) reply.remove(0);
			eepromusage = (0x000000FF & ((int) response[start + 3]));
			flashusage1 = (0x000000FF & ((int) response[start + 4]));
			flashusage2 = (0x000000FF & ((int) response[start + 5]));

		}

		double v1 = (double) eepromusage / 96;
		double v2 = (double) (flashusage1 + flashusage2) / 256;

		int v11 = (int) (v1 * 100);
		int v21 = (int) (v2 * 100);

		// System.out.println("Node " + nodeName + " remains " + v11 +
		// "% of EEPROM and " + v21 + "% of Flash");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, "Node " + nodeName
				+ " remains " + v11 + "% of EEPROM and " + v21 + "% of Flash");
		return;
	}
}
