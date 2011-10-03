/*
The following is the license of LiteOS.

This file is part of LiteOS.
Copyright Qing Cao, Hossein Ahmadi 2007-2008, University of Illinois , qcao2@uiuc.edu

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

import tools.message.Dump;

import java.io.*;
import java.util.ArrayList;

/**
 * This is the class for implementing the copy command. Copy command is one of
 * the most complicated command in terms of implementation. Here are detailed
 * explanations of how it is implemented <p/> cp supports three models of copy
 * operation <p/> TYPE I cp from the PC to the mote <p/> TYPE II <p/> cp from
 * the mote to the PC <p/> TYPE III cp between two folders on the SAME mote <p/>
 * The support for cp between different motes is not implemented yet. <p/> <p/>
 * cp from the PC to the mote the module issues one command, and waits for one
 * single response. Then it starts to send out multiple commadns in the handle
 * section, where it waits for one more reply and ends <p/> * cp from the mote
 * to PC the module issues one command, and waits for a whole amount of feedback
 * from the mote. This is the most simple <p/> <p/> cp between folders on the
 * same mote the PC sends out two commands, and then waits for one reply <p/>
 * <p/> Here are some more design chocies <p/> Type I gerneate one message, send
 * one message, reply handle, skip one input, generate more messages, send more,
 * handle reply <p/> Type II gerenate one message, send one message, reply
 * handle, skip one input, generate zero message, receive more, handle reply
 * <p/> Type III gerenate two ...
 */

public class cpCommand implements cmdcontrol {

	private byte[] reply = new byte[64];

	final int SIZEOFDATA = 20;

	private int copyType = 0;

	private byte[] reply2 = new byte[64];
	private byte[] replysync = new byte[64];

	private int SkipContinue = 0;
	private int newnodeid;
	private int receivenodeid;

	private int receivesize;

	private String localFile;

	private byte[][] dataCommands = new byte[10240][100];

	private String[] incomingDataArray = new String[10240];

	private boolean endofFile;

	private int endisnear;

	private int IncomingDataFileSize = 0;

	private boolean morepatch;

	private int MyCurrentWait;

	private int addressType(String addr) {
		if ((addr.charAt(0) == '/') && (addr.charAt(2) == '/'))
			// if ((addr.charAt(0) == '/'))
			return 1;
		else
			return 0;
	}

	/*
	 * Usage byte[] bytes = new byte[10000];
	 * 
	 * int length = cphandle.readFile("C:\\Temp\\TestADC.hex", bytes);
	 * cphandle.writeFile("C:\\Temp\\TestADCCopy.hex", bytes, length);
	 */

	public int readFile(String filename, byte[] bytes) {
		ByteArrayOutputStream out = null;
		try {
			InputStream in = new FileInputStream(filename);
			File f = new File(filename); // only used to be able to init the
											// ByteArrayoutputStream
			out = new ByteArrayOutputStream((int) f.length());
			int k;
			while ((k = in.read()) != -1) {
				out.write(k);
			}
			in.close();
		}

		catch (FileNotFoundException fnfe) {
			// System.out.println("File not found.");
			colorOutput
					.println(colorOutput.COLOR_BRIGHT_RED, "File not found!");
			return -1;
		}

		catch (IOException ioe) {
			// System.out.println("Unable to read from file");
			colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
					"Unable to read from file");
		}

		System.arraycopy(out.toByteArray(), 0, bytes, 0,
				out.toByteArray().length);

		return out.toByteArray().length;
	}

	public void writeFile(String filename, byte[] bytes, int length) {
		try {

			File newfile = new File(filename);
			newfile.delete();
			newfile.createNewFile();
			ByteArrayInputStream ba = new ByteArrayInputStream(bytes);
			FileOutputStream fos = new FileOutputStream(newfile);

			int n = 0;

			for (int i = 0; i < length; i++) {
				n = ba.read();
				fos.write(n);
			}

			fos.close();
			ba.close();
		} catch (FileNotFoundException fnfe) {
			// System.out.println("File not found.");
			colorOutput
					.println(colorOutput.COLOR_BRIGHT_RED, "File not found.");
		}

		catch (IOException ioe) {
			// System.out.println("Unable to read from file");
			colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
					"Unable to read from file.");

		}

	}

	static public String stdFileName(String file) {
		char[] temp = new char[2000];
		temp = file.toCharArray();
		temp[0] = temp[1];
		temp[1] = ':';
		String newstring = new String(temp);
		newstring = newstring.replaceAll("/", "\\\\");
		return newstring;
	}

	private String getLastFileName(String localfilename) {

		int index = localfilename.lastIndexOf('/');
		return localfilename.substring(index + 1);

	}

	public int fileExist(String parameter) {
		String localFile = stdFileName(parameter);
		File file = new File(localFile);
		if (file.exists() == false)
			return 0;
		else
			return 1;
	}

	public int setNewCommand(String[] options, int optioncount,
			String[] parameters, int parametercount, fileDirectory fdir) {

		fileNode currentnode = fdir.getCurrentNode();

		if ((currentnode.getType().compareTo("network") == 0)
				|| (currentnode.getType().compareTo("root") == 0))
			return -3;

		int currentAddress = currentnode.getNodeAddress();

		// skipcontinue is set as 0 when the application starts
		if (SkipContinue == 0)
			receivenodeid = currentAddress;

		int currentBlock = currentnode.getBlock();

		byte[] filename;
		byte[] filename2;

		// set up the copy type

		if (addressType(parameters[0]) == 1)
			copyType = 1;
		else if (addressType(parameters[1]) == 1)
			copyType = 2;
		else
			copyType = 3;

		// pc to node
		if ((copyType == 1) && (fileExist(parameters[0]) == 0))
			return -1;
		// node to pc
		if ((copyType == 2) && (fileExist(parameters[1]) == 1))
			return -2;

		if (SkipContinue == 0) {

			if (copyType == 1) {

				// check if it is like cp A . type copy command

				if (parameters[1].compareTo(".") != 0)
					filename = parameters[1].getBytes();
				else
					filename = getLastFileName(parameters[0]).getBytes();

				reply[0] = (new Integer(filename.length + 4)).byteValue();
				reply[1] = (byte) 121;
				reply[2] = (new Integer(currentAddress)).byteValue();

				reply[3] = (new Integer(currentBlock)).byteValue();

				System.arraycopy(filename, 0, reply, 4, filename.length);

				// set up local file name and first send out one command
				localFile = stdFileName(parameters[0]);
				return 1;
			} else if (copyType == 2) {

				filename = parameters[0].getBytes();
				reply[0] = (new Integer(filename.length + 4)).byteValue();
				reply[1] = (byte) 124;
				reply[2] = (new Integer(currentAddress)).byteValue();
				reply[3] = (new Integer(currentBlock)).byteValue();
				System.arraycopy(filename, 0, reply, 4, filename.length);
				localFile = stdFileName(parameters[1]);

				for (int i = 0; i < 10240; i++)
					incomingDataArray[i] = "";

				endofFile = false;
				endisnear = 0;
				// set up lcoal file name and first send out a command
				return 1;
			} else if (copyType == 3) {
				// to be implemented
				filename = parameters[0].getBytes();
				filename2 = parameters[1].getBytes();
				reply[0] = (new Integer(filename.length + 4)).byteValue();
				reply[1] = (byte) 127;
				reply[2] = (new Integer(currentAddress)).byteValue();
				reply[3] = (new Integer(currentBlock)).byteValue();
				System.arraycopy(filename, 0, reply, 4, filename.length);

				reply2[0] = (new Integer(filename.length + 4)).byteValue();
				reply2[1] = (byte) 127;
				reply2[2] = (new Integer(currentAddress)).byteValue();
				reply2[3] = (new Integer(currentBlock)).byteValue();
				System.arraycopy(filename2, 0, reply, 4, filename2.length);
				return 2;
			}

		}

		if (SkipContinue == 1) {
			if (copyType == 1) {

				// open the file in binary mode and write itsretur contents into
				// the dataCommands as specificied and wait
				// prepare data commands and fill in the info with newnodeid and
				// localFile

				byte[] filecontent = new byte[200000];
				int length;
				int totalseq;
				int remain;
				int remainstart;
				length = readFile(localFile, filecontent);

				// System.out.println("The length is "+length);
				totalseq = length / SIZEOFDATA;
				remain = length - totalseq * SIZEOFDATA;
				remainstart = totalseq * SIZEOFDATA;

				for (int i = 1; i <= totalseq; i++) {
					dataCommands[i - 1][0] = SIZEOFDATA + 7;
					dataCommands[i - 1][1] = 122;
					dataCommands[i - 1][2] = (byte) receivenodeid;
					dataCommands[i - 1][3] = (byte) newnodeid;
					dataCommands[i - 1][4] = (byte) (i / 256);
					dataCommands[i - 1][5] = (byte) (i % 256);
					dataCommands[i - 1][6] = (byte) (SIZEOFDATA);
					for (int j = 0; j < SIZEOFDATA; j++)
						dataCommands[i - 1][7 + j] = filecontent[(i - 1)
								* SIZEOFDATA + j];
				}
				if (remain > 0) {
					int i = totalseq + 1;
					dataCommands[i - 1][0] = (byte) (7 + remain);
					dataCommands[i - 1][1] = 122;
					dataCommands[i - 1][2] = (byte) receivenodeid;
					dataCommands[i - 1][3] = (byte) newnodeid;
					dataCommands[i - 1][4] = (byte) (i / 256);
					dataCommands[i - 1][5] = (byte) (i % 256);
					dataCommands[i - 1][6] = (byte) (remain);
					for (int j = 0; j < remain; j++)
						dataCommands[i - 1][7 + j] = filecontent[remainstart
								+ j];
				}
				int i;
				if (remain > 0)
					i = totalseq + 2;
				else
					i = totalseq + 1;
				dataCommands[i - 1][0] = 6;
				dataCommands[i - 1][1] = 123;
				dataCommands[i - 1][2] = (byte) receivenodeid;
				dataCommands[i - 1][3] = (byte) newnodeid;
				dataCommands[i - 1][4] = (byte) ((i - 1) / 256);
				dataCommands[i - 1][5] = (byte) ((i - 1) % 256);

				return i;

			} else if (copyType == 2) {
				return 0;
			} else {
				// assert(false);
			}

		}
		// assert(false);
		return 0;

	}

	// Return the delay in milliseconds
	public int getDelay() {
		if (copyType == 2)
			return 5000;
		else
			return 10000;

	}

	public int checkreply(int count, byte[] response) {
		int start = 5;
		if (count == 0) {
			return -1;
		}
		if (!((response[start + 3] == 15) && (response[start + 4] == 15)))
			return -1;
		else
			return (0x000000FF & ((int) response[start + 5])) * 256
					+ (0x000000FF & ((int) response[start + 6]));
	}

	public byte[] getSync(int lastIndexOfCommand) {
		replysync[0] = 6;
		replysync[1] = 122;
		replysync[2] = (byte) receivenodeid;
		replysync[3] = (byte) 255;
		replysync[4] = (byte) (lastIndexOfCommand / 256);
		replysync[5] = (byte) (lastIndexOfCommand % 256);

		return replysync;

	}

	public byte[] getEndPacket() {
		replysync[0] = 4;
		replysync[1] = 123;
		replysync[2] = (byte) receivenodeid;
		replysync[3] = (byte) 255;

		return replysync;

	}

	public int getSize() {
		return receivesize;
	}

	// return the command will be used
	public byte[] getNewCommand(int index) {

		if (SkipContinue == 1)
			return dataCommands[index];
		else if (index == 0)
			return reply;
		else if (index == 1)
			return reply2;
		return null;
	}

	// This function must handle multiple cases, for type I, II, and III
	// respectively.

	public void handleresponse(String[] options, int optioncount,
			String[] parameters, int parametercount, int length,
			byte[] response, fileDirectory fdir) {

		fileNode cnode = fdir.getCurrentNode();
		byte[] filecontent = new byte[2000000];
		int currentIndex = 0;

		// this is the case that the first type and the first round
		if ((copyType == 1) && (SkipContinue == 0)) {
			// System.out.println("the total length " + length);
			colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
					"The total length is " + length);
			newnodeid = (0x000000FF & ((int) response[3]));
			SkipContinue = 1;
			return;
		}

		if ((copyType == 2) && (SkipContinue == 0)) {

			int size1, size2;
			SkipContinue = 1;
			// size1 = (0x000000FF & ((int)response[3]));
			// size2 = (0x000000FF & ((int)response[4]));
			// receivesize = size1*256+size2;
			return;

		}

		if ((copyType == 1) && (SkipContinue == 1)) {
			SkipContinue = 0;
			colorOutput
					.println(colorOutput.COLOR_BRIGHT_GREEN, "Copy finished");
			return;
		}

		if ((copyType == 2) && (SkipContinue == 1)) {

			// use localFile to write the feedback into this file in binary
			// mode, after parsing the data

			// the repsonse is supposed to be patched, complete response
			// set skipcontinue as 0

			{
				SkipContinue = 0;
				/*
				 * for (int start=0;start+32<=length;start+=32) { int msgLength
				 * = (0x000000FF & ((int)response[start])); int dataLength =
				 * msgLength - 4; for (int j=0;j<dataLength;j++) {
				 * filecontent[currentIndex++] = response[start+j+4]; } }
				 */
				writeFile(localFile, response, length);
				// System.out.println("Copy is finished from mote to PC");
				colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
						"Copy finished");
				return;
			}

		}

		if (copyType == 3) {
			SkipContinue = 0;
			return;
		}
	}

	public void handleresponse(String[] options, int optioncount,
			String[] parameters, int parametercount, ArrayList reply,
			fileDirectory fdir) {

		fileNode cnode = fdir.getCurrentNode();
		byte[] filecontent = new byte[2000000];
		int currentIndex = 0;
		int length = reply.size();
		int start = 5;

		// this is the case that the first type and the first round
		if ((copyType == 1) && (SkipContinue == 0)) {
			// System.out.println("the total length " + length);
			colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
					"The reply has " + length + " packets.");
			if (reply.size() == 1) {
				byte[] response = (byte[]) reply.remove(0);
				newnodeid = (0x000000FF & ((int) response[start + 3]));
				SkipContinue = 1;
			} else {
				SkipContinue = 0;
				colorOutput
						.println(colorOutput.COLOR_BRIGHT_RED,
								"The copy command appears to have failed. Please try again.");
			}

			return;
		}

		if ((copyType == 2) && (SkipContinue == 0)) {

			int size1, size2;
			SkipContinue = 1;
			// size1 = (0x000000FF & ((int)response[3]));
			// size2 = (0x000000FF & ((int)response[4]));
			// receivesize = size1*256+size2;
			return;

		}

		if ((copyType == 1) && (SkipContinue == 1)) {
			SkipContinue = 0;
			colorOutput
					.println(colorOutput.COLOR_BRIGHT_GREEN, "Copy finished");
			return;
		}

		if ((copyType == 2) && (SkipContinue == 1)) {

			// use localFile to write the feedback into this file in binary
			// mode, after parsing the data

			// the repsonse is supposed to be patched, complete response
			// set skipcontinue as 0

			{
				SkipContinue = 0;
				/*
				 * for (int start=0;start+32<=length;start+=32) { int msgLength
				 * = (0x000000FF & ((int)response[start])); int dataLength =
				 * msgLength - 4; for (int j=0;j<dataLength;j++) {
				 * filecontent[currentIndex++] = response[start+j+4]; } }
				 */
				// added as a hack
				byte[] response = null;
				response = (byte[]) reply.remove(0);
				length = response.length;

				writeFile(localFile, response, length);
				// System.out.println("Copy is finished from mote to PC");
				colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
						"Copy finished");
				return;
			}

		}

		if (copyType == 3) {
			SkipContinue = 0;
			return;
		}
	}

	public int skipReading() {
		return SkipContinue;

	}

	// get the incoming from the port, maybe incomplete
	public void getIncomingData(ArrayList reply) {
		int roundsync;

		if (reply.size() == 0)
			endisnear++;
		byte[] response = null;
		int responsecount;
		int start = 5;
		responsecount = reply.size();

		// System.out.println("Now get " + responsecount + " bytes ");
		/*
		 * int j; for (j=0;j<responsecount;j++) {
		 * colorOutput.print(colorOutput.COLOR_YELLOW, ""+new
		 * Byte(response[j]).toString()+" "); if ((j+1)%32 == 0)
		 * System.out.println();
		 * 
		 * } System.out.println();
		 */
		if (responsecount == 0)
			return;

		while (reply.size() > 0) {

			// roundsync = ((int)response[start+4])*256+(int)response[start+5];

			response = (byte[]) reply.remove(0);
			roundsync = (0x000000FF & ((int) response[start + 4])) * 256
					+ (0x000000FF & ((int) response[start + 5]));

			if ((((byte) response[start + 3]) == (byte) 239)
					&& (((byte) response[start + 4] == (byte) 239)))
				break;

			byte[] subbytes = new byte[32];

			// System.out.println("Current round is " + roundsync);
			if ((roundsync % 30 == 0) && (roundsync > 0))
				colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
						"Copy round is " + (roundsync - 29) + " to "
								+ roundsync);

			System.arraycopy(response, start, subbytes, 0, 32);
			try {
				incomingDataArray[roundsync] = new String(subbytes,
						"ISO-8859-1");

			} catch (java.io.UnsupportedEncodingException e) {
			}
		}

		if ((response[start + 3] == (byte) 239)
				&& (response[start + 4] == (byte) 239)) {
			IncomingDataFileSize = (0x000000FF & (int) response[start + 5])
					* 256 + ((0x000000FF & (int) response[start + 6]));
			endofFile = true;

		}

	}

	// does the previous incoming data has terminate signal?

	public boolean terminateSignal() {

		if (endisnear > 8)
			endofFile = true;
		return endofFile;

	}

	// is there any more patches needed?

	public boolean morepatchingdata() {
		int i;
		for (i = 0; i < IncomingDataFileSize; i++) {
			if (incomingDataArray[i] == "")

			{
				morepatch = true;
				return true;
			}
		}
		morepatch = false;
		return false;

	}

	// the next patch command
	// the patch command asks the start position and asks the 20 bytes
	public byte[] getpatchcommand(fileDirectory fdir) {

		int i;

		fileNode currentnode = fdir.getCurrentNode();
		int currentAddress = currentnode.getNodeAddress();
		int currentBlock = currentnode.getBlock();

		for (i = 0; i < IncomingDataFileSize; i++) {
			if (incomingDataArray[i] == "") {
				reply[0] = 6;
				reply[1] = (byte) 125;
				reply[2] = (new Integer(currentAddress)).byteValue();
				reply[3] = (new Integer(currentBlock)).byteValue();
				reply[4] = (byte) (i * 20 / 256);
				reply[5] = (byte) (i * 20 % 256);
				MyCurrentWait = i;
				break;
			}
		}

		return reply;
	}

	// the patch is supposedly to be here. patch it
	public void handlePatch(ArrayList reply) {

		int responsecount = reply.size();
		int start = 5;
		byte[] response = null;

		if (responsecount == 0)
			return;

		System.out.println("Patch data received");

		while (reply.size() > 0) {

			response = (byte[]) reply.remove(0);
			if (response[start + 1] != 125) {
				colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
						"Irrelvant reply received. Corrupted Packet.");
				continue;
			}

			if (((0x000000FF & ((int) response[start + 3])) * 256 + (0x000000FF & ((int) response[start + 4]))) != MyCurrentWait * 20)
			// if
			// (((int)response[start+3])*256+(int)response[start+4]!=MyCurrentWait
			// )
			{
				// System.out.println("Incorrect Matching for cp patch");
				colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
						"Incorrect Matching for cp patch.");
				Dump.dump(System.out, "data", response);
				continue;

			}
			// System.out.println("Patch successful for block " +
			// MyCurrentWait);
			colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
					"Patch successful for block " + MyCurrentWait);
			byte[] subbytes = new byte[32];

			System.arraycopy(response, start, subbytes, 0, 32);
			try {
				incomingDataArray[MyCurrentWait] = new String(subbytes,
						"ISO-8859-1");
			} catch (java.io.UnsupportedEncodingException e) {

			}

			break;
		}
	}

	// generate the patched file length
	public int patchresponsecount() {
		int i;
		for (i = 0; i < IncomingDataFileSize; i++) {
			if (incomingDataArray[i] == "") {
				colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
						"Not fully patched error.");
			}
		}

		int sum = 0;

		for (i = 0; i < IncomingDataFileSize; i++) {

			try {
				sum += incomingDataArray[i].getBytes("ISO-8859-1")[0] - 6;
			} catch (java.io.UnsupportedEncodingException e) {

				colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
						"Unsupported Encodings Error.");
			}

		}

		return sum;
	}

	// generate the patched file data
	public ArrayList patchedresponse() {
		byte[] temp = new byte[100000];
		int index = 0;
		int i;
		for (i = 0; i < IncomingDataFileSize; i++) {

			try {
				System.arraycopy(incomingDataArray[i].getBytes("ISO-8859-1"),
						6, temp, index, incomingDataArray[i]
								.getBytes("ISO-8859-1")[0] - 6);
				index += incomingDataArray[i].getBytes("ISO-8859-1")[0] - 6;
			} catch (java.io.UnsupportedEncodingException e) {
				colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
						"Unsupported Encodings Error.");
			}

		}

		byte[] element = new byte[index];

		System.arraycopy(temp, 0, element, 0, index);

		ArrayList templist = new ArrayList();

		templist.add(0, element);

		return templist;
	}

}
