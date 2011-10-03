// $Id: SFProtocol.java,v 1.7 2004/11/18 19:02:19 idgay Exp $

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

import tools.message.Dump;
import java.io.*;

abstract public class SFProtocol extends AbstractSource {
	// Protocol version, written at connection-open time
	// 2 bytes: first byte is always 'T', second byte is
	// protocol version
	// The actual protocol used will be min(my-version, other-version)
	// current protocols:
	// ' ': initial protocol, no further connection data, packets are
	// 1-byte length followed by n-bytes data
	// If platform is unspecified by constructor, platform is
	// the default platform.
	// '!': add platform exchange at connection time, packets are
	// unchanged
	// If platform is unspecified by constructor, platform is
	// the platform returned at connection time (protocol error
	// if that is the unknown platform)
	final static byte VERSION[] = { 'T', '!' };
	byte version; // The protocol version we're running (negotiated)

	protected InputStream is;
	protected OutputStream os;

	protected SFProtocol(String name) {
		this(name, Platform.unknown);
	}

	protected SFProtocol(String name, int plat) {
		super(name);
		platform = plat;
	}

	protected void openSource() throws IOException {
		// Assumes streams are open
		os.write(VERSION);
		byte[] partner = readN(2);

		// Check that it's a valid header (min version is ' ')
		if (!(partner[0] == VERSION[0] && (partner[1] & 0xff) >= ' '))
			throw new IOException("protocol error");
		// Actual version is min received vs our version
		version = partner[1];
		if (VERSION[1] < version)
			version = VERSION[1];

		// Any connection-time data-exchange goes here
		switch (version) {
		case ' ':
			if (platform == Platform.unknown)
				platform = Platform.defaultPlatform;
			break;
		case '!':
			byte f[] = new byte[4];
			f[0] = (byte) (platform & 0xff);
			f[1] = (byte) (platform >> 8 & 0xff);
			f[2] = (byte) (platform >> 16 & 0xff);
			f[3] = (byte) (platform >> 24 & 0xff);
			os.write(f);
			byte[] received = readN(4);
			if (platform == Platform.unknown) {
				platform = received[0] & 0xff | (received[1] & 0xff) << 8
						| (received[2] & 0xff) << 16
						| (received[3] & 0xff) << 24;
				if (platform == Platform.unknown) {
					throw new IOException(
							"connecting to unknown platform from " + this);
				}
			}
			break;
		}
	}

	protected byte[] readSourcePacket() throws IOException {
		// Protocol is straightforward: 1 size byte, <n> data bytes
		byte[] size = readN(1);
		byte[] read = readN(size[0] & 0xff);
		// Dump.dump("reading", read);
		return read;
	}

	protected byte[] readN(int n) throws IOException {
		byte[] data = new byte[n];
		int offset = 0;

		// A timeout would be nice, but there's no obvious way to
		// write it before java 1.4 (probably some trickery with
		// a thread and closing the stream would do the trick, but...)
		while (offset < n) {
			int count = is.read(data, offset, n - offset);

			if (count == -1)
				throw new IOException("end-of-stream");
			offset += count;
		}
		return data;
	}

	public boolean writeSourcePacketSF(byte[] packet) throws IOException {
		if (packet.length > 255)
			throw new IOException("packet too long");

		// Dump.dump(System.out, "writing", packet);
		// System.out.println();

		os.write((byte) packet.length);
		os.write(packet);
		os.flush();
		return true;
	}

	// This function is re-implemented to be used for LiteOS packet formats

	protected boolean writeSourcePacket(byte[] packet) throws IOException {
		if (packet.length > 255)
			throw new IOException("packet too long");

		// Dump.dump(System.out, "writing", packet);
		// System.out.println();
		int length = (int) packet[0];
		byte[] sync = new byte[2];
		byte[] padding = new byte[100];

		for (int i = 0; i < 100; i++) {
			padding[i] = (byte) (66);

		}

		if (length <= 32)
			os.write((byte) 33);
		else
			os.write((byte) 65);

		if (length <= 32)
			sync[0] = 'a';
		else
			sync[0] = 'l';

		try {
			os.write(sync, 0, 1);

			os.write(packet, 0, length);

			if (length <= 32)
				os.write(padding, 0, 32 - length);
			else
				os.write(padding, 0, 64 - length);

			// os.write((byte)0);
			os.flush();

		} catch (IOException e) {
		}
		// os.write((byte)packet.length);
		// os.write(packet);
		// os.flush();
		return true;
	}
}
