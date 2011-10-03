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
public class listCommand implements cmdcontrol {
	private byte[] reply = new byte[64];

	public listCommand() {
	}

	public int setNewCommand(String[] options, int optioncount,
			String[] parameters, int parametercount, fileDirectory fdir) {
		fileNode currentNode = fdir.getCurrentNode();

		/*
		 * removed because of -u is no longer supported if
		 * ((currentNode.getListed() == true) && (!((optioncount == 2) &&
		 * (options[1].compareTo("-u") == 0)))) { if ((optioncount > 0) &&
		 * (options[0].compareTo("-l") == 0)) currentNode.printChildren("long");
		 * 
		 * else currentNode.printChildren("short");
		 * 
		 * return 0;
		 * 
		 * }
		 */
		if (currentNode.getType().compareTo("root") == 0) {
			reply[0] = 3;
			reply[1] = 101;
			reply[2] = 0;

			return 1;
		} else if (currentNode.getType().compareTo("network") == 0) {
			reply[0] = (byte) (currentNode.getName().getBytes().length + 3);
			reply[1] = 102;
			reply[2] = 0;
			System.arraycopy(currentNode.getName().getBytes(), 0, reply, 3,
					currentNode.getName().getBytes().length);

			return 1;
		} else if ((currentNode.getType().compareTo("noderoot") == 0)
				&& (optioncount == 0)) {
			reply[0] = 4;
			reply[1] = 103;
			reply[2] = (new Integer(currentNode.getNodeAddress())).byteValue();
			reply[3] = (new Integer(currentNode.getBlock())).byteValue();

			if (reply[3] != 0) {

				colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
						"Something is wrong\n");
			}

			return 1;
		} else if ((currentNode.getType().compareTo("directory") == 0)
				&& (optioncount == 0)) {
			reply[0] = 4;
			reply[1] = 103;
			reply[2] = (new Integer(currentNode.getNodeAddress())).byteValue();
			reply[3] = (new Integer(currentNode.getBlock())).byteValue();

			return 1;
		} else if ((currentNode.getType().compareTo("noderoot") == 0)
				&& (optioncount > 0)) {
			reply[0] = 4;
			reply[1] = 104;
			reply[2] = (new Integer(currentNode.getNodeAddress())).byteValue();
			reply[3] = (new Integer(currentNode.getBlock())).byteValue();

			if (reply[3] != 0) {
				colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
						"Something is wrong\n");
			}

			return 1;
		} else if ((currentNode.getType().compareTo("directory") == 0)
				&& (optioncount > 0)) {
			reply[0] = 4;
			reply[1] = 104;
			reply[2] = (new Integer(currentNode.getNodeAddress())).byteValue();
			reply[3] = (new Integer(currentNode.getBlock())).byteValue();

			return 1;
		}

		return 1;
	}

	// Return the delay in milliseconds
	public int getDelay() {
		return 500;
	}

	// return the command will be used
	public byte[] getNewCommand(int index) {
		if (index >= 1) {
			colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
					"System state problem\n");
		}

		return reply;
	}

	public void handleresponse(String[] options, int optioncount,
			String[] parameters, int parametercount, int length,
			byte[] response, fileDirectory fdir) {
		fileNode cnode = fdir.getCurrentNode();
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, "The returned has "
				+ length + " packets.");

		int j;

		for (j = 0; j < length; j++) {
			/*
			 * colorOutput.print(colorOutput.COLOR_YELLOW, ""+new
			 * Byte(response[j]).toString()+" "); if ((j+1)%32 == 0)
			 * System.out.println();
			 */
		}

		System.out.println();

		// no cache hack
		cnode.cleanAll();

		// To change body of implemented methods use File | Settings | Filels
		// lks Templates.
		if (cnode.getType().compareTo("root") == 0) {
			for (int start = 0; (start + 32) <= length; start += 32) {
				int msgLength = (0x000000FF & ((int) response[start]));
				String networkName = (new String(response)).substring(
						3 + start, (3 + start + msgLength) - 3);

				if (cnode.duplicateChild(networkName) == false) {
					cnode.addChild(new fileNode(networkName, "network"));
				}

				cnode.setListed();
			}

			if ((optioncount > 0) && (options[0].compareTo("-l") == 0)) {
				cnode.printChildren("long");
			} else {
				cnode.printChildren("short");
			}
		} else if (cnode.getType().compareTo("network") == 0) {
			for (int start = 0; (start + 32) <= length; start += 32) {
				int msgLength = (0x000000FF & ((int) response[start]));
				String fileNodeName = (new String(response)).substring(
						3 + start, (3 + start + msgLength) - 3);
				int nodeaddress = (0x000000FF & ((int) response[start + 2]));

				if (cnode.duplicateChild(fileNodeName) == false) {
					cnode.addChild(new fileNode(fileNodeName, "noderoot",
							nodeaddress, 0));
				}

				cnode.setListed();
			}

			if ((optioncount > 0) && (options[0].compareTo("-l") == 0)) {
				cnode.printChildren("long");
			} else {
				cnode.printChildren("short");
			}
		} else if (cnode.getType().compareTo("noderoot") == 0) {
			for (int start = 0; (start + 32) <= length; start += 32) {
				int msgLength = (0x000000FF & ((int) response[start]));
				String fileNodeName2 = new String(response, 5 + start,
						5 + start + 11);
				int nodeaddress = (0x000000FF & ((int) response[start + 2]));
				int blockaddress = (0x000000FF & ((int) response[start + 3]));
				int type = (0x000000FF & ((int) response[start + 17]));
				int size = (0x000000FF & ((int) response[start + 18]))
						+ ((0x000000FF & ((int) response[start + 19])) * 256);
				String fileNodeName = "";

				for (int i = 0; i < fileNodeName2.length(); i++) {
					if (fileNodeName2.charAt(i) != '\0') {
						fileNodeName += fileNodeName2.charAt(i);
					} else {
						break;
					}
				}

				if (cnode.duplicateChild(fileNodeName) == false) {
					if (type == 1) {
						cnode.addChild(new fileNode(fileNodeName, "directory",
								nodeaddress, blockaddress));
					}

					if (type == 2) {
						cnode.addChild(new fileNode(fileNodeName, "file",
								nodeaddress, blockaddress, size));
					}

					if (type == 3) {
						cnode.addChild(new fileNode(fileNodeName, "file_dev",
								nodeaddress, blockaddress));
					}

					if (type == 4) {
						cnode.addChild(new fileNode(fileNodeName, "file_app",
								nodeaddress, blockaddress));
					}
				}
			}

			cnode.setListed();

			if ((optioncount > 0) && (options[0].compareTo("-l") == 0)) {
				cnode.printChildren("long");
			} else {
				cnode.printChildren("short");
			}
		} else if (cnode.getType().compareTo("directory") == 0) {
			for (int start = 0; (start + 32) <= length; start += 32) {
				int msgLength = (0x000000FF & ((int) response[start]));
				String fileNodeName2 = new String(response, 5 + start,
						5 + start + 11);
				int nodeaddress = (0x000000FF & ((int) response[start + 2]));
				int blockaddress = (0x000000FF & ((int) response[start + 3]));
				int type = (0x000000FF & ((int) response[start + 17]));
				int size = (0x000000FF & ((int) response[start + 18]))
						+ ((0x000000FF & ((int) response[start + 19])) * 256);

				String fileNodeName = "";

				for (int i = 0; i < fileNodeName2.length(); i++) {
					if (fileNodeName2.charAt(i) != '\0') {
						fileNodeName += fileNodeName2.charAt(i);
					} else {
						break;
					}
				}

				if (cnode.duplicateChild(fileNodeName) == false) {
					if (type == 1) {
						cnode.addChild(new fileNode(fileNodeName, "directory",
								nodeaddress, blockaddress));
					}

					if (type == 2) {
						cnode.addChild(new fileNode(fileNodeName, "file",
								nodeaddress, blockaddress, size));
					}

					if (type == 3) {
						cnode.addChild(new fileNode(fileNodeName, "file_dev",
								nodeaddress, blockaddress));
					}

					if (type == 4) {
						cnode.addChild(new fileNode(fileNodeName, "file_app",
								nodeaddress, blockaddress));
					}
				}
			}

			cnode.setListed();

			if ((optioncount > 0) && (options[0].compareTo("-l") == 0)) {
				cnode.printChildren("long");
			} else {
				cnode.printChildren("short");
			}
		}
	}

	// The real handle reponse command is here.
	public void handleresponse(String[] options, int optioncount,
			String[] parameters, int parametercount, ArrayList reply,
			fileDirectory fdir) {
		fileNode cnode = fdir.getCurrentNode();
		int length = reply.size();
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, "The returned has "
				+ reply.size() + " packets.");
		System.out.println();

		// no cache hack. set the node childnum = 0;
		cnode.cleanAll();
		int start = 5;

		// To change body of implemented methods use File | Settings | Filels
		// lks Templates.
		if (cnode.getType().compareTo("root") == 0) {
			while (reply.size() > 0)

			{
				byte[] response = (byte[]) reply.remove(0);

				int msgLength = (0x000000FF & ((int) response[start]));
				String networkName = null;
				try {
					networkName = (new String(response, "ISO-8859-1"));
				} catch (java.io.UnsupportedEncodingException e) {
				}

				networkName = networkName.substring(3 + start,
						(3 + start + msgLength) - 3);

				if (cnode.duplicateChild(networkName) == false) {
					cnode.addChild(new fileNode(networkName, "network"));
				}

				cnode.setListed();
			}

			if ((optioncount > 0) && (options[0].compareTo("-l") == 0)) {
				cnode.printChildren("long");
			} else {
				cnode.printChildren("short");
			}
		} else if (cnode.getType().compareTo("network") == 0) {
			while (reply.size() > 0)

			{
				byte[] response = (byte[]) reply.remove(0);
				int msgLength = (0x000000FF & ((int) response[start]));

				String fileNodeName = null;
				try {
					fileNodeName = (new String(response, "ISO-8859-1"))
							.substring(3 + start, (3 + start + msgLength) - 3);
				} catch (java.io.UnsupportedEncodingException e) {
				}
				int nodeaddress = (0x000000FF & ((int) response[start + 2]));

				if (cnode.duplicateChild(fileNodeName) == false) {
					cnode.addChild(new fileNode(fileNodeName, "noderoot",
							nodeaddress, 0));
				}

				cnode.setListed();
			}

			if ((optioncount > 0) && (options[0].compareTo("-l") == 0)) {
				cnode.printChildren("long");
			} else {
				cnode.printChildren("short");
			}
		} else if (cnode.getType().compareTo("noderoot") == 0) {
			while (reply.size() > 0)

			{
				byte[] response = (byte[]) reply.remove(0);
				int msgLength = (0x000000FF & ((int) response[start]));

				String fileNodeName2 = null;
				try {
					fileNodeName2 = new String(response, 5 + start,
							5 + start + 11, "ISO-8859-1");
				} catch (java.io.UnsupportedEncodingException e) {
				}
				int nodeaddress = (0x000000FF & ((int) response[start + 2]));
				int blockaddress = (0x000000FF & ((int) response[start + 3]));
				int type = (0x000000FF & ((int) response[start + 17]));
				int size = (0x000000FF & ((int) response[start + 18]))
						+ ((0x000000FF & ((int) response[start + 19])) * 256);
				String fileNodeName = "";

				for (int i = 0; i < fileNodeName2.length(); i++) {
					if (fileNodeName2.charAt(i) != '\0') {
						fileNodeName += fileNodeName2.charAt(i);
					} else {
						break;
					}
				}

				if (cnode.duplicateChild(fileNodeName) == false) {
					if (type == 1) {
						cnode.addChild(new fileNode(fileNodeName, "directory",
								nodeaddress, blockaddress));
					}

					if (type == 2) {
						cnode.addChild(new fileNode(fileNodeName, "file",
								nodeaddress, blockaddress, size));
					}

					if (type == 3) {
						cnode.addChild(new fileNode(fileNodeName, "file_dev",
								nodeaddress, blockaddress));
					}

					if (type == 4) {
						cnode.addChild(new fileNode(fileNodeName, "file_app",
								nodeaddress, blockaddress));
					}
				}
			}

			cnode.setListed();

			if ((optioncount > 0) && (options[0].compareTo("-l") == 0)) {
				cnode.printChildren("long");
			} else {
				cnode.printChildren("short");
			}
		} else if (cnode.getType().compareTo("directory") == 0) {
			while (reply.size() > 0)

			{
				byte[] response = (byte[]) reply.remove(0);
				int msgLength = (0x000000FF & ((int) response[start]));
				String fileNodeName2 = null;
				try {
					fileNodeName2 = new String(response, 5 + start,
							5 + start + 11, "ISO-8859-1");
				} catch (java.io.UnsupportedEncodingException e) {
				}
				int nodeaddress = (0x000000FF & ((int) response[start + 2]));
				int blockaddress = (0x000000FF & ((int) response[start + 3]));
				int type = (0x000000FF & ((int) response[start + 17]));
				int size = (0x000000FF & ((int) response[start + 18]))
						+ ((0x000000FF & ((int) response[start + 19])) * 256);

				String fileNodeName = "";

				for (int i = 0; i < fileNodeName2.length(); i++) {
					if (fileNodeName2.charAt(i) != '\0') {
						fileNodeName += fileNodeName2.charAt(i);
					} else {
						break;
					}
				}

				if (cnode.duplicateChild(fileNodeName) == false) {
					if (type == 1) {
						cnode.addChild(new fileNode(fileNodeName, "directory",
								nodeaddress, blockaddress));
					}

					if (type == 2) {
						cnode.addChild(new fileNode(fileNodeName, "file",
								nodeaddress, blockaddress, size));
					}

					if (type == 3) {
						cnode.addChild(new fileNode(fileNodeName, "file_dev",
								nodeaddress, blockaddress));
					}

					if (type == 4) {
						cnode.addChild(new fileNode(fileNodeName, "file_app",
								nodeaddress, blockaddress));
					}
				}
			}

			cnode.setListed();

			if ((optioncount > 0) && (options[0].compareTo("-l") == 0)) {
				cnode.printChildren("long");
			} else {
				cnode.printChildren("short");
			}
		}
	}
}
