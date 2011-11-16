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
 * The class for the search command or the find command.
 */

public class searchCommand implements cmdcontrol {

	private byte[] reply = new byte[64];

	// Return the total number of commands will be used
	public int setNewCommand(String[] options, int optioncount,
			String[] parameters, int parametercount, fileDirectory fdir) {

		fileNode currentnode = fdir.getCurrentNode();
		int currentAddress = currentnode.getNodeAddress();
		int currentBlock = currentnode.getBlock();

		byte[] filename = parameters[0].getBytes();

		reply[0] = (new Integer(filename.length + 3)).byteValue();
		reply[1] = (byte) 221;
		reply[2] = (new Integer(currentAddress)).byteValue();

		System.arraycopy(filename, 0, reply, 3, filename.length);

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
			fileDirectory fdir) {
		// To change body of implemented methods use File | Settings | File
		// Templates.
		fileNode cnode = fdir.getCurrentNode();
		String filenodeName = parameters[0];

		int start = 5;

		while (reply.size() > 0) {
			byte[] response = (byte[]) reply.remove(0);
			int msgLength = (0x000000FF & ((int) response[start]));
			int namelength = msgLength - 3;
			String nameofthefile = (new String(response)).substring(3 + start,
					3 + start + namelength);

			fileNode[] temp = new fileNode[24];
			int tempcount = 0;
			fileNode current = fdir.getCurrentNode();
			String commonprefix = "";
			while (current.getType().compareTo("noderoot") != 0) {
				current = current.getParent();
			}

			while (current.getName().compareTo("root") != 0) {

				temp[tempcount++] = current;
				current = current.getParent();
			}

			for (int i = tempcount - 1; i >= 0; i--) {
				commonprefix += "/";
				commonprefix += temp[i].getName();
			}
			commonprefix += "/";
			// System.out.print(commonprefix);
			colorOutput.print(colorOutput.COLOR_BRIGHT_GREEN, commonprefix);
			// System.out.println(nameofthefile);
			colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, nameofthefile);
		}
		System.out.println();
		return;
	}
}
