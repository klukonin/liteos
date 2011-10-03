// $Id: Sender.java,v 1.8 2004/10/11 04:04:23 szewczyk Exp $

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
/* Authors:  David Gay  <dgay@intel-research.net>
 *           Intel Research Berkeley Lab
 *
 */

/**
 * @author David Gay <dgay@intel-research.net>
 * @author Intel Research Berkeley Lab
 */

package tools.message;

import tools.util.*;
import tools.packet.*;
import java.io.*;

/**
 * Sender class (send tinyos messages).
 * <p>
 * 
 * A sender class provides a simple interface built on Message for sending
 * tinyos messages to a SerialForwarder
 * 
 * @version 2, 24 Jul 2003
 * @author David Gay
 */
public class Sender {
	// If true, dump packet contents that are sent
	private static final boolean VERBOSE = false;

	byte groupId;
	PhoenixSource sender;
	MessageFactory messageFactory;
	TOSMsg template;

	private void init(int gid) {
		if (gid < MoteIF.ANY_GROUP_ID || gid > 0xff) {
			throw new IllegalArgumentException(
					"Cannot send messages to invalid group ID: " + gid);
		}
		// Use "default" group id when none specified (a few people will be
		// temporarily less confused)
		if (gid == MoteIF.ANY_GROUP_ID)
			groupId = 0x7d;
		else
			groupId = (byte) gid;
	}

	/**
	 * Create a sender for group id gid talking to PhoenixSource forwarder
	 * 
	 * @param forwarder
	 *            PhoenixSource with which we wish to send packets
	 * @param gid
	 *            group id to be placed in sent packets
	 */
	public Sender(PhoenixSource forwarder, int gid) {
		sender = forwarder;
		packetSize = -1;
		init(gid);
		// System.err.println("We're connected to " +
		// Platform.getPlatformName(forwarder.getPacketSource().getPlatform()));
		messageFactory = new MessageFactory(forwarder);
		template = messageFactory.createTOSMsg(10);
	}

	/**
	 * Create a sender talking to PhoenixSource forwarder. The group id of sent
	 * packets will be arbitrary (for use with new TinyosBase)
	 * 
	 * @param forwarder
	 *            PhoenixSource with which we wish to send packets
	 */
	public Sender(PhoenixSource forwarder) {
		this(forwarder, MoteIF.ANY_GROUP_ID);
	}

	/**
	 * Send m to moteId via this Sender's SerialForwarder
	 * 
	 * @param moteId
	 *            message destination
	 * @param m
	 *            message
	 * @exception IOException
	 *                thrown if message could not be sent
	 */
	synchronized public void send(int moteId, Message m) throws IOException {
		int amType = m.amType();
		byte[] data = m.dataGet();

		if (amType < 0) {
			throw new IOException("unknown AM type for message "
					+ m.getClass().getName());
		}

		TOSMsg packet;
		if (packetSize == -1) {
			// normal case, a PhoenixSource
			// XXX: hack: we don't leave any space for the crc, so
			// numElements_data() will be wrong. But we access the
			// data area via dataSet/dataGet, so we're ok.
			packet = messageFactory.createTOSMsg(template.offset_data(0)
					+ data.length);
			// new TOSMsg(TOSMsg.offset_data(0) + data.length);
		} else {
			// legacy support
			packet = messageFactory.createTOSMsg(packetSize);// new
																// TOSMsg(packetSize
																// );
			// Set CRC to 1 in case we're going directly to a UART
			packet.set_crc(1);
		}

		// message header: destination, group id, and message type
		packet.set_addr(moteId);
		packet.set_group(groupId);
		packet.set_type((short) amType);
		packet.set_length((short) data.length);
		if (data.length >= (1L << 8) && //
				// XXX:hack -- assume that length field
				// is less than 1 byte long
				(packetSize != -1 && data.length > packet.numElements_data())) // only
																				// possible
																				// in
																				// legacy
																				// case
																				// ...
			throw new IOException("message too big (" + data.length + " bytes)");
		packet.dataSet(data, 0, packet.offset_data(0), data.length);

		byte[] packetData = packet.dataGet();
		if (sender != null) {
			sender.writePacket(packetData);
		} else {
			sfw.Write(packetData);
		}
		if (VERBOSE)
			Dump.dump("sent", packetData);
	}

	// ////////////// DEPRECATED ROUTINES /////////////////

	SerialStub sfw;
	int packetSize;

	/**
	 * Create a sender for group id gid talking to SerialForwarder forwarder
	 * 
	 * @param forwarder
	 *            SerialForwarder with which we wish to send packets
	 * @param gid
	 *            group id to be placed in sent packets
	 * @deprecated Use the version which takes a PhoenixSource instead
	 */
	public Sender(SerialStub forwarder, int gid) {
		this(forwarder, gid, SerialForwarderStub.PACKET_SIZE);
	}

	/**
	 * Create a sender for group id gid talking to SerialForwarder forwarder
	 * 
	 * @param forwarder
	 *            SerialForwarder with which we wish to send packets
	 * @param gid
	 *            group id to be placed in sent packets
	 * @param packet_size
	 *            size of packets to send
	 * @deprecated Use the version which takes a PhoenixSource instead
	 */
	public Sender(SerialStub stub, int gid, int packet_size) {
		init(gid);
		sfw = stub;
		packetSize = packet_size;
	}
}
