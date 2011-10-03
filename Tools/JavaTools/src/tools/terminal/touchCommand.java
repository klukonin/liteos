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
 * The command for the touch command.
 */
public class touchCommand implements cmdcontrol {

	private byte[] reply = new byte[64];

	// Return the total number of commands will be used
	public int setNewCommand(String[] options, int optioncount,
			String[] parameters, int parametercount, fileDirectory fdir) {

		fileNode currentnode = fdir.getCurrentNode();
		int currentAddress = currentnode.getNodeAddress();
		int currentBlock = currentnode.getBlock();

		byte[] dirname = parameters[0].getBytes();

		reply[0] = (new Integer(dirname.length + 4)).byteValue();
		reply[1] = (byte) 151;
		reply[2] = (new Integer(currentAddress)).byteValue();
		reply[3] = (new Integer(currentBlock)).byteValue();

		System.arraycopy(dirname, 0, reply, 4, dirname.length);

		// if (currentnode.duplicateChild(parameters[0]) == false)
		return 1; // To change body of implemented methods use File | Settings |
					// File Templates.
		// else {
		// System.out.println("Duplicated file or directory");
		// colorOutput.println(colorOutput.COLOR_YELLOW,
		// "Duplicated file or directory");
		// return 0;
		// }
	}

	// Return the delay in milliseconds
	public int getDelay() {
		return 3200; // To change body of implemented methods use File |
						// Settings | File Templates.
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

		int start = 5;
		fileNode cnode = fdir.getCurrentNode();
		String filenodeName = parameters[0];

		while (reply.size() > 0) {
			byte[] response = (byte[]) reply.remove(0);
			int msgLength = (0x000000FF & ((int) response[start]));
			int blockaddress = (0x000000FF & ((int) response[start + 3]));

			if (blockaddress == 0) {
				colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
						"Duplicate file name, touch fails.");
				continue;
			}

			if (blockaddress == 255) {
				colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
						"The current directory is full. Touch fails. ");
				continue;
			}

			if (cnode.duplicateChild(filenodeName) == false) {
				{
					cnode.addChild(new fileNode(filenodeName, "file", cnode
							.getNodeAddress(), blockaddress));
					colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
							"creating file complete");
				}
			}
		}
		// System.out.println("make file complete");

		return;
	}
}
