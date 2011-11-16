// $Id: SerialPortStub.java,v 1.9 2003/10/07 21:46:09 idgay Exp $

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

//==============================================================================
//===   SerialPortStub.java   ==============================================

package tools.util;

import java.util.*;
import java.io.*;
import javax.comm.*;

/**
 * 
 * Init the serial port and reads data from it.
 * 
 * @author <A HREF="http://www.cs.berkeley.edu/~mikechen/">Mike Chen</A> (<A
 *         HREF="mailto:mikechen@cs.berkeley.edu">mikechen@cs.berkeley.edu</A>)
 * @since 1.1.6
 * @deprecated Use tools.packet.BuildSource instead
 */

public class SerialPortStub implements SerialStub {

	// =========================================================================
	// = CONSTANTS =========================================================

	private static final String CLASS_NAME = "SerialPortStub";

	// size of a message, in bytes
	private int msgSize = 36;

	// =========================================================================
	// = PRIVATE VARIABLES =================================================

	CommPortIdentifier portId;
	SerialPort port;
	String portName;
	int baudrate = 19200;
	InputStream in;
	OutputStream out;
	public static int debug = 0;
	private Vector listeners = new Vector();

	// = CONSTRUCTORS ======================================================
	// =========================================================================

	public SerialPortStub(String portName) {
		this.portName = portName;
	}

	public SerialPortStub(String portName, int packetSize) {
		this.portName = portName;
		this.msgSize = packetSize;
	}

	public SerialPortStub(String portName, int packetSize, int baudrate) {
		this.portName = portName;
		this.msgSize = packetSize;
		this.baudrate = baudrate;
	}

	// =========================================================================

	public void Open() throws NoSuchPortException, PortInUseException,
			IOException, UnsupportedCommOperationException {
		portId = CommPortIdentifier.getPortIdentifier(portName);
		port = (SerialPort) portId.open(CLASS_NAME, 0);
		in = port.getInputStream();
		out = port.getOutputStream();
		port.setFlowControlMode(SerialPort.FLOWCONTROL_NONE);

		port.setSerialPortParams(baudrate, SerialPort.DATABITS_8,
				SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
		printPortStatus();
	}

	public void Close() throws Exception {
		in.close();
		out.close();
		port.close();
	}

	private void printPortStatus() {
		System.err.println("baud rate: " + port.getBaudRate());
		System.err.println("data bits: " + port.getDataBits());
		System.err.println("stop bits: " + port.getStopBits());
		System.err.println("parity:    " + port.getParity());
	}

	// =========================================================================

	/*
	 * Get an enumeration of all of the comm ports on the machine
	 */

	public void printAllPorts() {
		Enumeration ports = CommPortIdentifier.getPortIdentifiers();

		if (ports == null) {
			System.out.println("No comm ports found!");
			return;
		}

		// print out all ports
		System.out.println("printing all ports...");
		while (ports.hasMoreElements()) {
			System.out.println("-  "
					+ ((CommPortIdentifier) ports.nextElement()).getName());
		}
		System.out.println("done.");
	}

	// =======================================================================

	public void registerPacketListener(PacketListenerIF listener) {
		if (debug > 0)
			System.err.println("SPS: Adding listener: " + listener);
		listeners.add(listener);
	}

	// =======================================================================

	public synchronized void Read() throws IOException {
		int i;
		int count = 0;
		byte[] packet = new byte[msgSize];
		if (debug > 0)
			System.out.print("!");

		while ((i = in.read()) != -1) {
			if (debug > 0)
				System.out.print("!");
			packet[count] = (byte) i;
			count++;
			if (count == msgSize) {
				count = 0;
				Enumeration e = listeners.elements();
				while (e.hasMoreElements()) {
					PacketListenerIF listener = (PacketListenerIF) e
							.nextElement();
					listener.packetReceived(packet);
				}
			} else if (count == 1 && i != 0x7e) {
				count = 0;
				if (debug > 0)
					System.out.print("?");
			}
		}
	}

	public void Write(byte[] packet) throws IOException {
		if (debug > 0)
			System.out.print("-");
		out.write(packet);
		out.flush();
	}
}
