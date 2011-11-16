// $Id: BrokenPacketizer.java,v 1.9 2004/05/31 10:30:28 szewczyk Exp $

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
package tools.packet;

import tools.util.Crc;
import tools.message.*;
import java.io.*;
import java.lang.reflect.Method;

/**
 * A packet source for the old, broken mote-PC protocol. Requires a ByteSource
 * for low-level I/O
 */
public class BrokenPacketizer extends AbstractSource {
	final static int SYNC_BYTE = 0x7e;
	private int packetSize;
	private MessageFactory messageFactory;
	private TOSMsg template;
	private int msg_size_crc;
	ByteSource io;

	/**
	 * Packetizers are built using the makeXXX methods in BuildSource
	 */
	public BrokenPacketizer(String name, int packetSize, ByteSource io) {
		super(name + "," + packetSize);
		this.packetSize = packetSize;
		this.io = io;
		messageFactory = new MessageFactory();
		template = messageFactory.createTOSMsg();
		try {
			Class tcls = template.getClass();
			Method m = tcls.getMethod("size_crc", null);
			Integer i = (Integer) m.invoke(tcls, null);
			msg_size_crc = i.intValue();
		} catch (Exception e) {
			msg_size_crc = 2;
		}
	}

	protected void openSource() throws IOException {
		io.open();
		messageFactory = new MessageFactory(this);
		template = messageFactory.createTOSMsg(packetSize);
		try {
			Class tcls = template.getClass();
			Method m = tcls.getMethod("size_crc", null);
			Integer i = (Integer) m.invoke(tcls, null);
			msg_size_crc = i.intValue();
		} catch (Exception e) {
			msg_size_crc = 2;
		}
	}

	protected void closeSource() {
		io.close();
	}

	protected byte[] readSourcePacket() throws IOException {
		byte[] packet = new byte[packetSize];
		int count = 0;

		do {
			while (count < packetSize) {
				byte b = io.readByte();

				// Rather broken form of sync
				if (count == 0 && b != SYNC_BYTE)
					continue;
				packet[count++] = b;
			}
			// Drop packets where the crc field is not 1
		} while (packet[packetSize - msg_size_crc] != 1);

		return collapsePacket(packet);
	}

	protected boolean writeSourcePacket(byte[] packet) throws IOException {
		io.writeBytes(expandPacket(packet, packetSize));
		return true;
	}

	public byte[] collapsePacket(byte[] oldPacket) throws IOException {
		// Make a PacketSource-format byte array from the full packet
		int size = (oldPacket[lengthOffset] & 0xff) + dataOffset;
		if (size > oldPacket.length)
			throw new IOException("TOS_Msg length is too long for packet");
		byte[] psPacket = new byte[size];
		System.arraycopy(oldPacket, 0, psPacket, 0, size);

		return psPacket;
	}

	public byte[] expandPacket(byte[] psPacket, int packetSize) {
		// Make a full TOS_Msg from the PacketSource-format byte array
		TOSMsg tmsg = messageFactory.createTOSMsg(packetSize);
		tmsg.dataSet(psPacket, 0, 0, psPacket.length);
		tmsg.set_crc(Crc.calc(tmsg.dataGet(), tmsg.get_length() + dataOffset));
		return tmsg.dataGet();
	}
}
