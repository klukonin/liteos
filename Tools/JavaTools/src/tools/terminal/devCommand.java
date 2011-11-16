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
public class devCommand implements cmdcontrol {

	private int waithold = 32;
	private byte[] reply = new byte[64];
	private int skipreading;
	private int radioget;
	private int ledset;

	public int setNewCommand(String[] options, int optioncount,
			String[] parameters, int parametercount, fileDirectory fdir) {

		skipreading = 0;

		radioget = 0;
		fileNode currentnode = fdir.getCurrentNode();
		int currentAddress = currentnode.getNodeAddress();
		int currentBlock = currentnode.getBlock();

		byte[] filename = options[0].getBytes();
		if (options[0].startsWith("radio") == true)
			radioget = 1;
		else
			radioget = 0;

		if (options[0].startsWith("led") == true)
			ledset = 1;
		else
			ledset = 0;

		reply[0] = 3;

		if (ledset == 1)
			reply[1] = (byte) 12;
		else
			reply[1] = (byte) 11;

		reply[2] = (new Integer(currentAddress)).byteValue();

		reply[3] = (new Integer(currentBlock)).byteValue();

		if (ledset == 1) {
			reply[0] = 7;
			if (((new Integer(parameters[0]).intValue()) > 2)
					|| ((new Integer(parameters[1]).intValue()) > 2)
					|| ((new Integer(parameters[2]).intValue()) > 2)) {
				ledset = 0;
				colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
						"Invalid syntax or parameters.");
				return 0;
			}
			reply[4] = (new Integer(parameters[0])).byteValue();
			reply[5] = (new Integer(parameters[1])).byteValue();
			reply[6] = (new Integer(parameters[2])).byteValue();
			return 1;
		}

		// reply4 should be the length of the filename

		reply[4] = (new Integer(options[0].length())).byteValue();

		System.arraycopy(options[0].getBytes(), 0, reply, 5, reply[4]);

		int currentindex = 5 + reply[4];

		currentindex++;
		boolean inquote = false;

		for (int i = 0; i < parametercount; i++) {
			if ((parameters[i].startsWith("\"") == true) || (inquote == true)) {
				try {
					if (inquote == true) {
						reply[currentindex++] = 32;
						if (parameters[i].endsWith("\"") == true) {
							int k = parameters[i].length();

							System.arraycopy(parameters[i].getBytes("ASCII"),
									0, reply, currentindex, parameters[i]
											.length());
						} else
							System.arraycopy(parameters[i].getBytes("ASCII"),
									0, reply, currentindex, parameters[i]
											.length());

					} else {
						int k = parameters[i].length();
						System.arraycopy(parameters[i].getBytes("ASCII"), 0,
								reply, currentindex, parameters[i].length());
						inquote = true;
					}
				} catch (java.io.UnsupportedEncodingException e) {
				}

				if (inquote == false) {
					currentindex += parameters[i].length() - 1;
					assert (false);
				} else if (parameters[i].startsWith("\"") == true)
					currentindex += parameters[i].length();
				else
					currentindex += parameters[i].length();
			}

			else {
				reply[currentindex] = Integer.decode(parameters[i]).byteValue();

				currentindex++;

			}

		}

		reply[5 + reply[4]] = (new Integer(currentindex - 6 - reply[4]))
				.byteValue();
		reply[0] = (new Integer(currentindex)).byteValue();

		waithold = Integer.decode(parameters[0]).intValue() * 32;
		return 1; // To change body of implemented methods use File | Settings |
					// File Templates.
	}

	// Return the delay in milliseconds
	public int getDelay() {
		if (ledset == 0)
			return 2000000;
		else
			return 30; // To change body of implemented methods use File |
						// Settings | File Templates.
	}

	// this function should not be used now.

	public int getThreshold() {

		int temp = waithold - 64;
		if (temp < 0)
			temp = 32;
		// waithold = 32;
		return temp;

	}

	// return the command will be used
	public byte[] getNewCommand(int index) {
		return reply;
	}

	public int getSkipReading() {
		return skipreading;
	}

	public void handleresponse(String[] options, int optioncount,
			String[] parameters, int parametercount, ArrayList reply,
			fileDirectory fdir) {

		int endofloopindicator;
		boolean lastreading = false;
		int reading;

		// System.out.println("The packet received is "+ length + " bytes");

		if (ledset == 1) {
			ledset = 0;
			return;

		}
		int start = 5;
		// for (int start = 0; start + 32 <= length; start += 32) {
		while (reply.size() > 0) {
			byte[] response = (byte[]) reply.remove(0);
			endofloopindicator = (0x000000FF & ((int) response[start + 1]));
			if (endofloopindicator == 11) {
				lastreading = true;

			} else if (endofloopindicator == 12) {
				lastreading = false;

			}

			else if (endofloopindicator == 10) {
				lastreading = true;
				skipreading = 0;
				colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
						"The specified device is not found.");
				return;

			} else {
				continue;
			}
			reading = (0x000000FF & ((int) response[start + 3])) * 256
					+ (0x000000FF & ((int) response[start + 4]));
			// System.out.println("Get reading of " + reading);
			if (radioget == 0)
				colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
						"Get reading of " + reading);
			else
				colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
						"Radio message sent once");
			if (lastreading == true) {
				skipreading = 0;
				break;
			} else {
				skipreading = 1;
			}

		}

		return;
	}
}
