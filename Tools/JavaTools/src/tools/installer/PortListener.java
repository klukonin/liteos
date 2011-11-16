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

package tools.installer;

import java.io.*;

import javax.comm.*;

/**
 * The class for listening to the serial port. Bsaically it allows two
 * paraemters, the timeout, and the number of bytes to be listened to, as the
 * criteria for return from the listening mode. Once one of these two criteria
 * are met, the code will return and continue. It is interesting to note that
 * here we use a large enough serial buffer, 2M bytes, which is large enough for
 * most purposes of the applciation running.
 */
public class PortListener {

	private static String CLASS_NAME = "PortListener";
	// private static final int MAX_MSG_SIZE = 36;
	// private static final int PORT_SPEED_MICAZ = 57600;
	// private static final int PORT_SPEED_MICAZ = 57600;
	// private static final int PORT_SPEED_RENE = 19200;
	// private static final int LENGTH_OFFSET = 4;
	// private int packetLength;
	private int portSpeed;

	private CommPortIdentifier portId;
	private SerialPort port;
	private String portName;
	private InputStream in;
	private int receiveCount;
	private OutputStream outstream;
	private int WaitTime;

	private byte[] serBuf = new byte[2000000];

	public PortListener(String portName, int portSpeed) {
		this.portName = portName;
		this.portSpeed = portSpeed;
	}

	public void open() throws NoSuchPortException, PortInUseException,
			IOException, UnsupportedCommOperationException {
		

		// System.out.println("Now listening to bytes from " + portName);
		 

		try {
			portId = CommPortIdentifier.getPortIdentifier(portName);
			port = (SerialPort) portId.open(CLASS_NAME, 0);
		}

		catch (javax.comm.NoSuchPortException e) {
			// System.out.println("No such port");
			colorOutput.println(colorOutput.COLOR_BRIGHT_RED, "No such port");
			System.exit(1);
		} catch (javax.comm.PortInUseException e) {
			// System.out.println("Port in use");
			colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
					"Port in use IN OPEN");
			System.exit(1);

		}

		catch (Exception e) {
			// System.out.println("error!");
			colorOutput.println(colorOutput.COLOR_BRIGHT_RED, "General error!");
			System.exit(1);
		}

		port.setFlowControlMode(SerialPort.FLOWCONTROL_NONE);
		port.disableReceiveFraming();

		port.setSerialPortParams(portSpeed, SerialPort.DATABITS_8,
				SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);

		port.enableReceiveThreshold(1024000);
		port.setInputBufferSize(2048);
		port.enableReceiveTimeout(30000);

		try {
			in = port.getInputStream();
			outstream = port.getOutputStream();

		} catch (Exception e) {
			System.out.println("error!");
			System.exit(1);
		}

		System.out.println();
	}

	public void setWait(int wait) {
		this.WaitTime = wait;
		try {
			port.enableReceiveTimeout(this.WaitTime);
		} catch (javax.comm.UnsupportedCommOperationException e) {
		}
	}

	public void read() throws IOException {

		int count = 0;

		count = in.read(serBuf, 0, serBuf.length);

		receiveCount = count;

	}

	public void setThreshold(int bytes) {
		try {
			port.enableReceiveThreshold(bytes);
		} catch (javax.comm.UnsupportedCommOperationException e) {
			// System.out.println("Port Operation Unsupported");
			colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
					"Port Operation Unsupported!");
		}
	}

	public void write(byte[] data) {

		byte[] padding = new byte[32];

		byte[] sync = new byte[2];

		sync[0] = 'z';

		for (int i = 0; i < 32; i++) {
			padding[i] = (byte) (i + 30);

		}

		try {

			outstream.write(sync, 0, 1);

			outstream.write(data, 0, 33);

		} catch (IOException e) {
		}
	}

	public int getCount() {
		return receiveCount;
	}

	public byte[] getData() {
		return serBuf;
	}
}
