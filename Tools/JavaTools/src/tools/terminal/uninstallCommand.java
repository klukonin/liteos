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
public class uninstallCommand implements cmdcontrol {

	private byte[] reply = new byte[64];

	public int setNewCommand(String[] options, int optioncount,
			String[] parameters, int parametercount, fileDirectory fdir) {

		fileNode currentnode = fdir.getCurrentNode();
		int currentAddress = currentnode.getNodeAddress();
		int currentBlock = currentnode.getBlock();

		byte[] filename = parameters[0].getBytes();

		reply[0] = (byte) (3 + filename.length);
		reply[1] = (byte) 232;
		reply[2] = (new Integer(currentAddress)).byteValue();

		System.arraycopy(filename, 0, reply, 3, filename.length);

		reply[3 + filename.length] = (byte) 0;

		return 1; // To change body of implemented methods use File | Settings |
					// File Templates.
	}

	// Return the delay in milliseconds
	public int getDelay() {
		return 800; // To change body of implemented methods use File | Settings
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

		String filenodeName = parameters[0];

		int length = reply.size();

		if (length == 0) {
			colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
					"No response info received");
			return;
		}

		int start = 5;
		// for (int start = 0; start + 32 <= length; start += 32) {
		while (reply.size() > 0) {
			byte[] response = (byte[]) reply.remove(0);

			int success = (0x000000FF & ((int) response[start + 3]));
			if (success == 0) {
				colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
						"The request probably fails.");
			} else {
				colorOutput.println(colorOutput.COLOR_GREEN,
						"The request succeeds.");
			}
		}
		return;
	}
}
