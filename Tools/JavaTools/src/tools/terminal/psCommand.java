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
 * The class for the list command ls
 */
public class psCommand implements cmdcontrol {

	private byte[] reply = new byte[64];

	public psCommand() {
	}

	public int setNewCommand(String[] options, int optioncount,
			String[] parameters, int parametercount, fileDirectory fdir) {
		fileNode currentnode = fdir.getCurrentNode();
		int currentAddress = currentnode.getNodeAddress();

		reply[0] = 3;
		reply[1] = (byte) 171;
		reply[2] = (new Integer(currentAddress)).byteValue();

		return 1; // To change body of implemented methods use File | Settings |
					// File Templates.
	}

	// Return the delay in milliseconds
	public int getDelay() {
		return 300;
	}

	// return the command will be used
	public byte[] getNewCommand(int index) {
		if (index >= 1)
			// System.out.println("System state problem\n");
			colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
					"System state problem\n");
		return reply;

	}

	private String StatusToString(int num) {
		if (num == 1) {
			return new String("Ready");
		}
		if (num == 2) {
			return new String("Active");
		}
		if (num == 3) {
			return new String("Blocked");
		}
		if (num == 4) {
			return new String("Presleep");
		}
		if (num == 5) {
			return new String("Sleep");
		}
		if (num == 6) {
			return new String("IO_Blocked");
		}
		if (num == 7) {
			return new String("File_Blocked");
		}

		if (num == 8) {
			return new String("Breakpoint_Reached");
		}

		if (num == 9) {
			return new String("Memory_Corrupted");

		}
		return new String("Unknown");
	}

	public void handleresponse(String[] options, int optioncount,
			String[] parameters, int parametercount, ArrayList reply,
			fileDirectory fdir) {

		fileNode cnode = fdir.getCurrentNode();
		String processName = null;
		// System.out.println("The returned has " + length + "bytes");

		// colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
		// "The returned has " + length + "bytes");

		int status;
		byte temparray[] = new byte[32];
		int start = 5;
		int packetnum = reply.size();

		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, "The reply has "
				+ packetnum + " packets.");
		// for (int start = 0; start + 32 <= length; start += 32) {

		while (reply.size() > 0) {
			byte[] response = (byte[]) reply.remove(0);
			int msgLength = (0x000000FF & ((int) response[start]));
			System.arraycopy(response, start, temparray, 0, 32);
			try {
				processName = (new String(temparray, "ISO-8859-1")).substring(
						4, 4 + msgLength - 4);
			} catch (java.io.UnsupportedEncodingException e) {
			}
			status = (0x000000FF & ((int) response[start + 3]));
			// System.out.println(processName + "   " + StatusToString(status));
			if (status != 8)
				colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, processName
						+ "   " + StatusToString(status));
			else {
				try {
					processName = (new String(temparray, "ISO-8859-1"))
							.substring(6, 6 + msgLength - 6);
				} catch (java.io.UnsupportedEncodingException e) {
				}
				int addr = (0x000000FF & ((int) response[start + 4])) * 256
						+ (0x000000FF & ((int) response[start + 5]));
				addr = addr * 2;
				colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, processName
						+ "   "
						+ StatusToString(status)
						+ " at 0x"
						+ Integer.toHexString(0x1000000 | addr).substring(1)
								.toUpperCase());

			}

		}
	}

}