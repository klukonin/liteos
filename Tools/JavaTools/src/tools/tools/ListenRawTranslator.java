// $Id: ListenRaw.java,v 1.5 2003/10/07 21:46:08 idgay Exp $

/*									tab:4
 * "Copyright (c) 2000-2003 The Regents of the University  of California.
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the author appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Copyright (c) 2002-2003 Intel Corporation
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached INTEL-LICENSE
 * file. If you do not find these files, copies can be found by writing to
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300, Berkeley, CA,
 * 94704.  Attention:  Intel License Inquiry.
 */
/* Authors: Mike Chen, Philip Levis
 * Last Modified: 7/1/02 (transition to nesC)
 *
 */

/**
 * @author Mike Chen
 * @author Philip Levis
 */

//This file iscurrently modified to be used for the purpose of the energy experiment 
package tools.tools;

import java.util.*;
import java.util.Date; 
import java.io.*;
import java.text.SimpleDateFormat;
import java.math.BigInteger;
import javax.comm.*;
import tools.util.*;
import tools.message.*;

public class ListenRawTranslator {
	private static String CLASS_NAME = "tools.tools.ListenRaw";
	private static final int MAX_MSG_SIZE = 36;
	private static final int PORT_SPEED_MICA2 = 57600;
	private static final int PORT_SPEED_MICA2DOT = 19200;
	private static final int PORT_SPEED_MICA = 19200;
	private static final int PORT_SPEED_RENE = 19200;
	private static final int LENGTH_OFFSET = 4;
	private int packetLength;
	private static int countintegers;
	
	private long lastvalue = 0;
	private long lastvalue2 = 0;
	private int portSpeed;
	private long counter = -1;

	
 
	private long starttimestamp, nowtimestamp; 
	private static FileOutputStream outputstream; // declare a file output
													// object
	private static PrintStream pstream; // declare a print stream object

	private CommPortIdentifier portId;
	private SerialPort port;
	private String portName;
	private InputStream in;
	private OutputStream out;

	public ListenRawTranslator(String portName, int portSpeed) {
		this.portName = portName;
		this.portSpeed = portSpeed;
		countintegers = 0; 
		 
		
	}

	public void open() throws NoSuchPortException, PortInUseException,
			IOException, UnsupportedCommOperationException {
		System.out.println("Opening port " + portName);
		portId = CommPortIdentifier.getPortIdentifier(portName);
		port = (SerialPort) portId.open(CLASS_NAME, 0);
		in = port.getInputStream();
		out = port.getOutputStream();

		port.setFlowControlMode(SerialPort.FLOWCONTROL_NONE);
		port.disableReceiveFraming();
		// printPortStatus();
		// These are the mote UART parameters
		port.setSerialPortParams(portSpeed, SerialPort.DATABITS_8,
				SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
		printPortStatus();
		System.out.println();
	}

	private void printPortStatus() {
		System.out.println(" baud rate: " + port.getBaudRate());
		System.out.println(" data bits: " + port.getDataBits());
		System.out.println(" stop bits: " + port.getStopBits());
		System.out.println(" parity:    " + port.getParity());
	}

	private static void printAllPorts() {
		Enumeration ports = CommPortIdentifier.getPortIdentifiers();

		if (ports == null) {
			System.out.println("No comm ports found!");
			return;
		}

		// print out all ports
		System.out.println("printing all ports...");
		while (ports.hasMoreElements()) {
			System.out.println("  "
					+ ((CommPortIdentifier) ports.nextElement()).getName());
		}
	}

	int[] packet = new int[MAX_MSG_SIZE];
	int index = 0;

	int getAscii(int value) {
		if ((value >= 48) && (value <= 57))
			return value - 48;
		else
			return value - 97 + 10;

	}

	public static final String DATE_FORMAT_NOW = "yyyy-MM-dd HH:mm:ss";

	long previoustime, nowtime;

	byte[] receivedbyte = new byte[100];

	public static long arr2long(byte[] arr, int start) {
		int i = 0;
		int len = 4;
		int cnt = 0;
		byte[] tmp = new byte[len];
		for (i = start; i < (start + len); i++) {
			tmp[cnt] = arr[i];
			cnt++;
		}
		long accum = 0;
		i = 0;
		for (int shiftBy = 0; shiftBy < 32; shiftBy += 8) {
			accum |= ((long) (tmp[i] & 0xff)) << shiftBy;
			i++;
		}
		return accum;
	}

	public static long unsignedByteToInt(byte b) {
		return (long) b & 0xFF;
	}

	public void read() throws IOException {

		int i;
		int mode;
		
		mode = 0;
		int[] receivedstring = new int[100];
		int counter = 0;
	
		nowtimestamp = new Date().getTime();

		while ((i = in.read()) != -1) {
			if (i == 0xFC) {

				// Now try to reset
				if (counter > 60) {
					mode = 0;
				}

				if ((counter == 0) && (mode == 1)) {
					mode = 0;
					continue;
				}
				if (mode == 0) {
					mode = 1;
					counter = 0;
					continue;
				}
				if (mode == 1) {
					mode = 0;
					// print the string
					int j;
					if ((receivedstring[0]== 'c')&&(receivedstring[1] == 'o')&&(receivedstring[4]== '1'))
					{ System.out.print(" end \n");
					 // pstream.print(" end \n");
					}
					
					if ((receivedstring[0]== 0x6D)&&(receivedstring[1] == 0x6D)&&(receivedstring[2]== 0x6D))
					{ System.out.print("\n");
					 // pstream.print("\n");
					 // System.out.print(nowtimestamp+ " - ");
					 // pstream.print(nowtimestamp + " ");
					 
					}
					
					else
					{
					for (j = 0; j < counter; j++) {
						System.out.print((char) receivedstring[j]);
					//	pstream.print((char) receivedstring[j]);
					}
					// System.out.print("\ncounter is "+counter+ "\n");

					if (receivedstring[counter - 1] != '\n') {
						System.out.print(" ");
					//	pstream.print(" ");
				
					}
					
					else
					{
						Calendar cal = Calendar.getInstance();
						SimpleDateFormat sdf = new SimpleDateFormat(DATE_FORMAT_NOW);

						// System.out.println("Now:  "+ sdf.format(cal.getTime()) +
						// " counter: "+String.valueOf(total) + " diff "+
						// String.valueOf(total-lastvalue));
						Date now = new Date();
						nowtime = now.getTime();
						//System.out.print((nowtime)+ " " + (nowtime-previoustime)+ ": ");

					 
						previoustime = nowtime;
					//	 pstream.print(nowtimestamp + " ");
						
					}
					   
					}
					counter = 0;
					continue;
				}

			}

			if (i == 0xFD) {
				int j;
				countintegers++;
				mode = 0;
				for (j = 0; j < 5; j++) {
					i = in.read();
					receivedbyte[counter++] = (byte) i;
				}
				byte[] newinteger = new byte[4];
				newinteger[0] = receivedbyte[0];
				newinteger[1] = receivedbyte[1];
				newinteger[2] = receivedbyte[2];
				newinteger[3] = receivedbyte[3];
				long realvalue = new BigInteger(newinteger).longValue();
				// System.out.print("integer ");
				// System.out.print(" " +realvalue + " "+ "(0x"+
				// Long.toHexString(Math.abs(realvalue))+") ");
				{
					System.out.print(" " + realvalue + " ");
					//pstream.print(" " +  realvalue + " ");
				}
				if (countintegers%3== 0){
				//	System.out.println();
					//pstream.print("\n");
				}
				counter = 0;
				continue;

			}

			if (i == 0xFE) {
				int j;
				mode = 0;
				for (j = 0; j < 5; j++) {
					i = in.read();
					receivedbyte[counter++] = (byte) i;
				}
				byte[] newinteger = new byte[4];
				newinteger[0] = receivedbyte[0];
				newinteger[1] = receivedbyte[1];
				newinteger[2] = receivedbyte[2];
				newinteger[3] = receivedbyte[3];
				// long realvalue = new BigInteger(newinteger).longValue();
				long realvalue = ((unsignedByteToInt(newinteger[0]) * 256 + unsignedByteToInt(newinteger[1])) * 256 + unsignedByteToInt(newinteger[2]))
						* 256 + unsignedByteToInt(newinteger[3]);
				// System.out.print(" "+realvalue + " "+ "(0x"+
				// Long.toHexString(Math.abs(realvalue))+") ");
				// System.out.print("unsinged integer ");
				{
					System.out.print(" "+realvalue+ " ");
					
				//	pstream.print(" "+realvalue + " ");
				}
				counter = 0;
				continue;

			}

			if (mode == 1) {
				receivedstring[counter++] = i;

			}

		}

	}

	public void read2() throws IOException {
		int i;

		previoustime = 0;

		while ((i = in.read()) != -1) {
			// if (i == 0x7e) {
			// System.out.println();
			// }
			// Dump.printByte(System.out, i);
			counter++;
			packet[index++] = i;
			if (i == 20) {
				index = 0;
				int val1 = getAscii(packet[2]) * 16 + getAscii(packet[3]);
				int val2 = getAscii(packet[4]) * 16 + getAscii(packet[5]);
				int val3 = getAscii(packet[6]) * 16 + getAscii(packet[7]);
				int val4 = getAscii(packet[8]) * 16 + getAscii(packet[9]);
				long total = ((val1 * 256 + val2) * 256 + val3) * 256 + val4;

				val1 = getAscii(packet[12]) * 16 + getAscii(packet[13]);
				val2 = getAscii(packet[14]) * 16 + getAscii(packet[15]);
				val3 = getAscii(packet[16]) * 16 + getAscii(packet[17]);
				val4 = getAscii(packet[18]) * 16 + getAscii(packet[19]);
				long total2 = ((val1 * 256 + val2) * 256 + val3) * 256 + val4;

				Calendar cal = Calendar.getInstance();
				SimpleDateFormat sdf = new SimpleDateFormat(DATE_FORMAT_NOW);

				// System.out.println("Now:  "+ sdf.format(cal.getTime()) +
				// " counter: "+String.valueOf(total) + " diff "+
				// String.valueOf(total-lastvalue));
				Date now = new Date();
				nowtime = now.getTime();
				System.out.println("The time elpased is "
						+ (nowtime - previoustime) + " counter: "
						+ String.valueOf(total) + " diff "
						+ String.valueOf(total - lastvalue) + " with task   "
						+ (total2 - lastvalue2) + "\n");

				lastvalue = total;
				lastvalue2 = total2;
				previoustime = nowtime;

			}

		}
	}

	private static void printUsage() {
		System.err
				.println("usage: java tools.tools.ListenRaw [options] <port>");
		System.err.println("options are:");
		System.err.println("  -h, --help:    usage help");
		System.err.println("  -p:            print available ports");
		System.err.println("  -mica2:        Mica2 (" + PORT_SPEED_MICA2
				+ " bps) [default]");
		System.err.println("  -mica2dot:        Mica2Dot ("
				+ PORT_SPEED_MICA2DOT + " bps)");
		System.err.println("  -mica:         Mica (" + PORT_SPEED_MICA
				+ " bps)");
		System.err.println("  -rene:         Rene (" + PORT_SPEED_RENE
				+ " bps)");
		System.exit(-1);
	}

	public static void main(String args[]) {
		int speed = PORT_SPEED_MICA2;

		if ((args.length < 1) || (args.length > 3)) {
			printUsage();
		}

		File fileoutput = new File("trace");
		boolean exist = fileoutput.exists();
		if (exist == true)
			fileoutput.delete();

		try {
			// Create a new file output stream
			// connected to "myfile.txt"
			outputstream = new FileOutputStream("trace");
			// Connect print stream to the output stream
			pstream = new PrintStream(outputstream);
			

		}

		catch (Exception e) {
			System.err.println("Error writing to file");
		}

		for (int i = 0; i < args.length; i++) {
			if (args[i].equals("-h") || args[i].equals("--help")) {
				printUsage();
			}
			if (args[i].equals("-p")) {
				printAllPorts();
			}
			if (args[i].equals("-mica2")) {
				speed = PORT_SPEED_MICA2;
			}
			if (args[i].equals("-mica2dot")) {
				speed = PORT_SPEED_MICA2DOT;
			}
			if (args[i].equals("-mica")) {
				speed = PORT_SPEED_MICA;
			}
			if (args[i].equals("-rene")) {
				speed = PORT_SPEED_RENE;
			}
		}

		if (args[args.length - 1].charAt(0) == '-') {
			return; // No port specified
		}

		ListenRawTranslator reader = new ListenRawTranslator(
				args[args.length - 1], speed);
		try {
			reader.open();
		} catch (Exception e) {
			e.printStackTrace();
		}

		try {
			reader.read();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
